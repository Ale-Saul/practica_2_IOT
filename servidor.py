import socket

# Configuración del server
HOST = '192.168.31.149'  
PORT = 12345  # Puerto del servidor 
# Valor inicial 
actuators_value = [0,0,0]

def convert_data_to_array(sensor_value):
    if 2 < sensor_value <= 43:
        return [1, 0, 0]
    elif 43 < sensor_value <= 71:
        return [0, 1, 0]
    elif 71 < sensor_value <= 100:
        return [0, 0, 1]
    else:
        return [0, 0, 0]

# Función para procesar los comandos de los clientes
def process_command(command):
    global actuators_value 
    command_parts = command.split()
    # Comando set
    if command_parts[0].lower() == "set":
        sensor_value = command_parts[1]  # Obtener el valor después de "set"
        try:
            actuators_value  = convert_data_to_array(float(sensor_value))
            return "Dato actualizado correctamente a " + str(actuators_value) # Se actualiza el array de los actuadores
        except ValueError:
            return "Error: Se esperaba un valor numérico."
    # Comando get
    elif command_parts[0].lower() == "get":
        return actuators_value # Se devuelve el array de los actuadores
    else:
        return "Comando no válido. Use 'set <valor>' para establecer un nuevo valor o 'get' para obtener el valor actual."

# Creacion de un socket TCP
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    # Colocamos al socket la dirección y puerto del servidor
    server_socket.bind((HOST, PORT))
    # Esperamos las conexiones entrantes al servidor (ESP del sensor y del actuador)
    server_socket.listen(2)
    print("Esperando conexiones entrantes...")
    while True:
        client_socket, client_address = server_socket.accept()
        print('Conexión establecida desde', client_address)
        # Recibir comando del cliente y procesarlo
        command = client_socket.recv(1024).decode().strip()
        if command:
            response = process_command(command)
            client_socket.sendall(str(response).encode())
            print(str(response))
       