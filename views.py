from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import serial
import json
from .mongo import arduinoToMongoClass
'''
from pymongo import MongoClient
import time
import socket'''
'''
class arduinoToMongoClass:
    def __init__(self):
        self.mongo_client = MongoClient('mongodb://utt:password!@ec2-3-128-121-154.us-east-2.compute.amazonaws.com,ec2-3-14-171-23.us-east-2.compute.amazonaws.com,ec2-3-140-126-105.us-east-2.compute.amazonaws.com/?replicaSet=integradora')
        self.db = self.mongo_client['cunas']
        self.collection = self.db['data']
        self.data_sent = False
        self.ser = serial.Serial('COM8', 9600) 
    def read_json(self):
        try:
            with open('data.json', 'r', encoding='utf-8') as file:
                existing_data = json.load(file)
            return existing_data
        except FileNotFoundError:
            pass
	
    def save_to_json(self, obj):
        try:
            with open('data.json', 'r', encoding='utf-8') as file:
                existing_data = json.load(file)
        except (FileNotFoundError, json.JSONDecodeError):
            existing_data = []
        existing_data.append(obj)
        with open('data.json', 'w', encoding='utf-8') as file:
            json.dump(existing_data, file, ensure_ascii=False, indent=2)
    def send_json_to_mongo(self, collection):
        if not self.data_sent:
            try:
                data = self.read_json()
            except Exception as e:
                pass
        if not data:
            return
        try:
            collection.insert_many(data)
            print('Json enviado a MongoDB')
            with open('data.json', 'w') as file:
                pass
        except Exception as e:
            print('No se pudo enviar los datos a MongoDB:', e)
    def run(self):
# Leer datos  Arduino
        print('Si se metio')
        time.sleep(1)
        data = self.ser.readline().decode().strip()
        print(data)
        if not data:
            print("No se recibieron datos del puerto serial")
        sensor_data = data.split(":")
        sensor_name = sensor_data[0].strip()
        sensor_value = sensor_data[1].strip()
        sensor_unit = sensor_data[2].strip()
        sensor_description = sensor_data[3].strip()
        obj = {
            'identificador': sensor_name,
            'valor': sensor_value,
            'unidad': sensor_unit,
            'descripcion': sensor_description,
            'cunaID': 1
            }
        print(obj)
        # Verificar conexión a internet
        remote_server = 'www.google.com'
        try:
            host = socket.gethostbyname(remote_server)
            socket.create_connection((host, 80), 2)
            self.send_json_to_mongo(self.collection)
            self.collection.insert_one(obj)
            print('Datos guardados en MongoDB')
        except:
            try:
                self.save_to_json(obj)
                print('Datos guardados en archivo JSON')
            except Exception as e:
                print('No se pudo guardar los datos en el archivo JSON:', e)
                
    def close_serial(self):
        if self.ser.is_open:
            self.ser.close()
        print('Conexión serial cerrada')
'''

Mongo = arduinoToMongoClass()


@csrf_exempt
def send_number(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Método no permitido'}, status=405)

    try:
        data = json.loads(request.body)
        number = data.get('number')
        print(number)

        if number is None or not isinstance(number, int):
            return JsonResponse({'error': 'Número inválido'}, status=400)

        # Enviar el número por serial
        try:
            ser = Mongo.ser
            ser.write(str(number).encode())
            print('Si mando el numero')
            # Leer datos  Arduino
            if 'sesion' in data:
                Mongo.runSesion(data['sesion'])
                print("Sesion")
            else:
                Mongo.runSingle()
                print("Single")
            return JsonResponse({'message': 'Número enviado con éxito'}, status=200)
        except Exception as e:
            return JsonResponse({'error': f'Error al enviar el número: {e}'}, status=500)
    
    except json.JSONDecodeError:
        return JsonResponse({'error': 'JSON inválido'}, status=400)

@csrf_exempt
def recibir_datos(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            return JsonResponse({
                "mensaje": "Datos recibidos correctamente",
                "datos_recibidos": data
            })
        except json.JSONDecodeError:
            return JsonResponse({
                "error": "Los datos enviados no son JSON válido"
            }, status=400)
    else:
        return JsonResponse({
            "error": "Solo se aceptan solicitudes POST"
        }, status=405)