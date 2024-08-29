import socket

def main():
    puerto = 55227
    server_ip = "127.0.0.1"

    # Crear el socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as cliente_socket:
        # Conectar al servidor
        try:
            cliente_socket.connect((server_ip, puerto))
        except socket.error as e:
            print(f"Error de conexión: {e}")
            return

        while True:
            print("Seleccione una opción:")
            print("1. Generar nombre de usuario")
            print("2. Generar contraseña")
            print("3. Salir")
            opcion = input("Opción: ")

            if opcion == "1":
                longitud = int(input("Ingrese la longitud del nombre de usuario (5-15): "))
                if longitud < 5 or longitud > 15:
                    print("Longitud inválida")
                    continue
                mensaje = f"USUARIO {longitud}"
            elif opcion == "2":
                longitud = int(input("Ingrese la longitud de la contraseña (8-50): "))
                if longitud < 8 or longitud > 50:
                    print("Longitud inválida")
                    continue
                mensaje = f"CONTRASENA {longitud}"
            elif opcion == "3":
                mensaje = "SALIR"
            else:
                print("Opción inválida")
                continue

            # Enviar el mensaje al servidor
            try:
                cliente_socket.sendall(mensaje.encode())
            except socket.error as e:
                print(f"Error al enviar mensaje: {e}")
                break

            if opcion == "3":
                break

            # Recibir la respuesta del servidor
            try:
                respuesta = cliente_socket.recv(2000).decode()
                print("Respuesta del servidor:", respuesta)
            except socket.error as e:
                print(f"Error al recibir respuesta: {e}")
                break

if __name__ == "__main__":
    main()
