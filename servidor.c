#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN 100

void generarNombreUsuario(int longitud, char *nombreUsuario);
void generarContrasena(int longitud, char *contrasena);

int main(int argc, char **argv) {
    WSADATA wsa;
    SOCKET skt, skt2;
    int longitud_cliente, puerto = 55227, recv_size;
    struct sockaddr_in server, cliente;
    char mensaje[2000];

    // Inicialización de Winsock
    printf("Inicializando Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al inicializar Winsock\n");
        exit(-1);
    }
    printf("Winsock inicializado\n");

    // Inicializar el generador de números aleatorios
    srand(time(NULL));

    // Creación del socket
    printf("Creando socket...");
    if ((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error creando socket\n");
        WSACleanup();
        exit(-1);
    }
    printf("Socket creado\n");

    // Configuración de sockaddr_in
    server.sin_family = AF_INET; // Familia TCP/IP
    server.sin_port = htons(puerto); // Puerto
    server.sin_addr.s_addr = INADDR_ANY; // Cualquier cliente puede conectarse

    // Realizar el bind
    if (bind(skt, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error al realizar el bind\n");
        closesocket(skt);
        WSACleanup();
        exit(-1);
    }
    printf("Bind realizado\n");

    listen(skt, 5);
    printf("Esperando conexiones entrantes...\n");

    longitud_cliente = sizeof(struct sockaddr_in);
    skt2 = accept(skt, (struct sockaddr*)&cliente, &longitud_cliente);
    if (skt2 == INVALID_SOCKET) {
        printf("Fallo al aceptar la conexion\n");
        closesocket(skt);
        WSACleanup();
        exit(-1);
    }

    printf("Cliente %s conectado exitosamente\n", inet_ntoa(cliente.sin_addr));

    while ((recv_size = recv(skt2, mensaje, sizeof(mensaje) - 1, 0)) > 0) {
        mensaje[recv_size] = '\0';

        int longitud;
        char respuesta[MAX_LEN];

        // Debugging: Imprime el mensaje recibido
        printf("Mensaje recibido: %s\n", mensaje);

        if (strncmp(mensaje, "USUARIO", 7) == 0) {
            sscanf(mensaje, "USUARIO %d", &longitud);
            if (longitud < 5 || longitud > 15) {
                strcpy(respuesta, "Error: La longitud del nombre de usuario debe estar entre 5 y 15 caracteres.");
            } else {
                generarNombreUsuario(longitud, respuesta);
            }
        } else if (strncmp(mensaje, "CONTRASENA", 10) == 0) {
            sscanf(mensaje, "CONTRASENA %d", &longitud);
            if (longitud < 8 || longitud > 50) {
                strcpy(respuesta, "Error: La longitud de la contraseña debe estar entre 8 y 50 caracteres.");
            } else {
                generarContrasena(longitud, respuesta);
            }
        } else {
            strcpy(respuesta, "Error: Comando desconocido.");
        }

        if (send(skt2, respuesta, strlen(respuesta), 0) == SOCKET_ERROR) {
            printf("Error al enviar respuesta\n");
        }
    }

    if (recv_size == SOCKET_ERROR) {
        printf("Error en recv\n");
    }

    closesocket(skt2);
    closesocket(skt);
    WSACleanup();

    return 0;
}

void generarNombreUsuario(int longitud, char *nombreUsuario) {
    const char vocales[] = "aeiou";
    const char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int longitud_vocales = strlen(vocales);
    int longitud_consonantes = strlen(consonantes);

    int i;
    int esVocal = rand() % 2;

    for (i = 0; i < longitud; i++) {
        if (esVocal) {
            nombreUsuario[i] = vocales[rand() % longitud_vocales];
        } else {
            nombreUsuario[i] = consonantes[rand() % longitud_consonantes];
        }
        esVocal = !esVocal;
    }

    nombreUsuario[longitud] = '\0';
}

void generarContrasena(int longitud, char *contrasena) {
    const char caracteres[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int longitud_caracteres = strlen(caracteres);
    int i;

    for (i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % longitud_caracteres];
    }

    contrasena[longitud] = '\0';
}
