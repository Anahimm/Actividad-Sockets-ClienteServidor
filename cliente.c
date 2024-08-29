#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 2000


int main(int argc, char **argv) {
    WSADATA wsa;
    SOCKET skt;
    int puerto = 55227, recv_size;
    char mensaje[MAX_LEN];
    struct sockaddr_in server;

    // Inicialización de Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al inicializar Winsock: %d\n", WSAGetLastError());
        exit(-1);
    }

    // Creación del socket
    if ((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error creando socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(-1);
    }

    // Configuración de sockaddr_in
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor

    // Conexión al servidor
    if (connect(skt, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error de conexion: %d\n", WSAGetLastError());
        closesocket(skt);
        WSACleanup();
        exit(-1);
    }

    while (1) {
        int opcion, longitud;
        printf("Seleccione una opcion:\n");
        printf("1. Generar nombre de usuario\n");
        printf("2. Generar contrasenia\n");
        printf("3. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("Ingrese la longitud del nombre de usuario (5-15): ");
            scanf("%d", &longitud);
            if (longitud < 5 || longitud > 15) {
                printf("Longitud invalida\n");
                continue;
            }
            snprintf(mensaje, sizeof(mensaje), "USUARIO %d", longitud);
        } else if (opcion == 2) {
            printf("Ingrese la longitud de la contrasenia (8-50): ");
            scanf("%d", &longitud);
            if (longitud < 8 || longitud > 50) {
                printf("Longitud invalida\n");
                continue;
            }
            snprintf(mensaje, sizeof(mensaje), "CONTRASENA %d", longitud);
        } else if (opcion == 3) {
            // Enviar comando de salida al servidor
            snprintf(mensaje, sizeof(mensaje), "SALIR");
            if (send(skt, mensaje, strlen(mensaje), 0) == SOCKET_ERROR) {
                printf("Error al enviar mensaje de salida: %d\n", WSAGetLastError());
            }
            break;
        } else {
            printf("Opcion invalida\n");
            continue;
        }

        // Enviar el mensaje al servidor
        if (send(skt, mensaje, strlen(mensaje), 0) == SOCKET_ERROR) {
            printf("Error al enviar mensaje: %d\n", WSAGetLastError());
            continue;
        }

        // Recibir la respuesta del servidor
        recv_size = recv(skt, mensaje, sizeof(mensaje) - 1, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Error al recibir respuesta: %d\n", WSAGetLastError());
            break;
        } else if (recv_size == 0) {
            printf("La conexion ha sido cerrada por el servidor.\n");
            break;
        }

        mensaje[recv_size] = '\0';
        printf("Respuesta del servidor: %s\n", mensaje);
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(skt);
    WSACleanup();

    return 0;
}
