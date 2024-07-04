import serial
import json
import time
import socket
import requests

class arduinoToServerClass:
    def __init__(self):
        self.ser = serial.Serial('COM9', 9600)
        self.data_sent = False
        self.backend_url = "http://127.0.0.1:3333/test"  # Reemplaza con la URL de tu backend

    def save_to_json(self, obj):
        try:
            with open('data.json', 'r', encoding='utf-8') as file:
                existing_data = json.load(file)
        except (FileNotFoundError, json.JSONDecodeError):
            existing_data = []
        existing_data.append(obj)
        with open('data.json', 'w', encoding='utf-8') as file:
            json.dump(existing_data, file, ensure_ascii=False, indent=2)

    def send_json_to_server(self):
        if not self.data_sent:
            try:
                with open('data.json', 'r', encoding='utf-8') as file:
                    data = json.load(file)
                if not data:
                    return
                response = requests.post(self.backend_url, json=data)
                if response.status_code == 200:
                    print('JSON enviado al servidor backend')
                    with open('data.json', 'w') as file:
                        pass  # Limpiar el archivo JSON
                    self.data_sent = True
                else:
                    print(f'Error al enviar datos al servidor: {response.status_code}')
            except Exception as e:
                print('No se pudo enviar los datos al servidor:', e)

    def run(self):
        while True:
            # Leer datos Arduino
            data = self.ser.readline().decode().strip()
            if not data:
                print("No se recibieron datos del puerto serial")
                continue
            
            sensor_data = data.split(":")
            if len(sensor_data) != 4:
                print("Datos recibidos en formato incorrecto")
                continue
            
            sensor_name, sensor_value, sensor_unit, sensor_description = [s.strip() for s in sensor_data]
            obj = {
                'identificador': sensor_name,
                'valor': sensor_value,
                'unidad': sensor_unit,
                'descripcion': sensor_description,
                'cunaID': 1
            }
            
            # Verificar conexión a internet
            remote_server = 'www.google.com'
            try:
                host = socket.gethostbyname(remote_server)
                socket.create_connection((host, 80), 2)
                response = requests.post(self.backend_url, json=obj)
                if response.status_code == 200:
                    print('Datos enviados al servidor backend')
                else:
                    print(f'Error al enviar datos al servidor: {response.status_code}')
                self.send_json_to_server()  # Intenta enviar datos guardados previamente
            except:
                try:
                    self.save_to_json(obj)
                    print('Datos guardados en archivo JSON')
                except Exception as e:
                    print('No se pudo guardar los datos en el archivo JSON:', e)     
            
            time.sleep(3)

if __name__ == "__main__":
    arduino_to_server = arduinoToServerClass()
    arduino_to_server.run()