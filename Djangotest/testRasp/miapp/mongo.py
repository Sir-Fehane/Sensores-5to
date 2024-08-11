import json
import socket
import time
import datetime
import serial
from pymongo import MongoClient

class arduinoToMongoClass:
    def __init__(self):
        self.mongo_client = MongoClient('mongodb://utt:password!@ec2-3-128-121-154.us-east-2.compute.amazonaws.com,ec2-3-14-171-23.us-east-2.compute.amazonaws.com,ec2-3-140-126-105.us-east-2.compute.amazonaws.com/?replicaSet=integradora')
        self.db = self.mongo_client['cunas']
        self.devices_collection = self.db['devices']
        self.data_sent = False
        self.ser = serial.Serial('COM10', 9600, timeout=1)
        
        self.collecion = self.db['Data']
        
        #ID del dispositivo
        self.device_id = "Cuna005"
        #cambiarlo cada vez que se reinicie el ngrok
        self.ngrokURL ="127.0.0.1:8000"
        
    def read_json(self):
        try:
            with open('data.json', 'r', encoding='utf-8') as file:
                existing_data = json.load(file)
            return existing_data
        except FileNotFoundError:
            return []
    
    def save_to_json(self, obj):
        try:
            with open('data.json', 'r', encoding='utf-8') as file:
                existing_data = json.load(file)
        except (FileNotFoundError, json.JSONDecodeError):
            existing_data = []
        existing_data.append(obj)
        with open('data.json', 'w', encoding='utf-8') as file:
            json.dump(existing_data, file, ensure_ascii=False, indent=2)
    
    def send_json_to_mongo(self):
        if not self.data_sent:
            try:
                data = self.read_json()
                self.data_sent = True
            except Exception as e:
                print(f'Error al leer el JSON: {e}')
                return
        
        if not data:
            return
        
        try:
            for entry in data:
                for device_id, sensors in entry['Dispositivos'].items():
                    # Actualizar la colección de dispositivos
                    self.devices_collection.update_one(
                        {"deviceID": device_id},
                        {"$set": {"deviceID": device_id, "url":self.ngrokURL}},
                        upsert=True
                    )
                    
                    for sensor_name, sensor_data in sensors.items():
                        collection_name = sensor_name
                        collection = self.db['Data']
                        
                        # Insertar los datos de los sensores en su colección
                        if "infoSensor" in sensor_data:
                            sensor_document = {
                                
                                    "infoSensor": {
                                        "IdSensor": sensor_name["infoSensor"].get("IdSensor", ""),
                                        "unidad": sensor_data["infoSensor"].get("unidad", ""),
                                        "descripcion": sensor_data["infoSensor"].get("descripcion", ""),
                                        "deviceID": device_id,
                                        "data": sensor_data["infoSensor"]["data"]
                                    }
                                }
                            collection.insert_one(sensor_document)
            
            # Limpiar el archivo JSON después de enviar los datos
            with open('data.json', 'w') as file:
                pass
            
            print('Json enviado a MongoDB')
        except Exception as e:
            print(f'No se pudo enviar los datos a MongoDB: {e}')
        
    def runSesion(self):
        
        print('Iniciando lectura de datos')
        time.sleep(1)
        
        obj = {"Dispositivos": {self.device_id: {}}}
        
        # Leer múltiples líneas
        for _ in range(7):
            data = self.ser.readline().decode().strip()
            if data:
                print(f"Datos recibidos: {data}")
                sensor_data = data.split(":")
                sensor_name = sensor_data[0].strip()
                sensor_value = sensor_data[1].strip()
                sensor_unit = sensor_data[2].strip()
                sensor_description = sensor_data[3].strip()
                
                obj["Dispositivos"][self.device_id][sensor_name] = {
                    "infoSensor": {
                        "IdSensor": sensor_name,
                        "unidad": sensor_unit,
                        "descripcion": sensor_description,
                        "deviceID": self.device_id,
                        "data": [
                            {
                                "data": sensor_value,
                                "datetime": datetime.datetime.now().strftime("%d/%m/%y %H:%M:%S")
                            }
                        ]
                    }
                }
            else:
                print(f"No se recibieron datos para la lectura {_+1}")
        
        if not obj["Dispositivos"][self.device_id]:
            print("No se recibieron datos del puerto serial")
            return
        
        print(obj)

        # Verificar conexión a internet
        remote_server = 'www.google.com'
        try:
            host = socket.gethostbyname(remote_server)
            socket.create_connection((host, 80), 2)
            
            # Insertar en la colección de dispositivos
            self.devices_collection.update_one(
                {"deviceID": self.device_id},
                {"$set": {"deviceID": self.device_id, "url":self.ngrokURL}},
                upsert=True
            )
            
            # Insertar cada sensor en su propia colección
            for sensor_name, sensor_data in obj["Dispositivos"][self.device_id].items():
                collection_name = sensor_name
                collection = self.db['Data']
                collection.insert_one(sensor_data)
            
            print('Datos guardados en MongoDB')
        except Exception as e:
            print(f'Error al guardar en MongoDB: {e}')
            try:
                self.save_to_json(obj)
                print('Datos guardados en archivo JSON')
            except Exception as e:
                print('No se pudo guardar los datos en el archivo JSON:', e)

    def runSingle(self):
            print('Iniciando lectura de datos')
            time.sleep(1)
            obj = {"Dispositivos": {self.device_id: {}}}
            
            data = self.ser.readline().decode().strip()
            if data:
                print(f"Datos recibidos: {data}")
                sensor_data = data.split(":")
                sensor_name = sensor_data[0].strip()
                sensor_value = sensor_data[1].strip()
                sensor_unit = sensor_data[2].strip()
                sensor_description = sensor_data[3].strip()
                    
                obj["Dispositivos"][self.device_id][sensor_name] = {
                        "infoSensor": {
                            "unidad": sensor_unit,
                            "descripcion": sensor_description,
                            "deviceID": self.device_id,
                            "data": [
                                {
                                    "data": sensor_value,
                                    "datetime": datetime.datetime.now().strftime("%d/%m/%y %H:%M:%S")
                                }
                            ]
                        }
                    }
            else:
                print("No se recibieron datos para la lectura")
            
            if not obj["Dispositivos"][self.device_id]:
                print("No se recibieron datos del puerto serial")
                return
            
            print(obj)
        
        # El resto de tu código para guardar en MongoDB o JSON...
            
            # Verificar conexión a internet
            remote_server = 'www.google.com'
            try:
                host = socket.gethostbyname(remote_server)
                socket.create_connection((host, 80), 2)
                self.send_json_to_mongo()
                
                # Insertar en la colección de dispositivos
                self.devices_collection.update_one(
                    {"deviceID": self.device_id},
                    {"$set": {"deviceID": self.device_id}},
                    upsert=True
                )
                
                # Insertar en la colección de sensores
                collection_name = sensor_name
                collection = self.db[collection_name]
                sensor_document = obj["Dispositivos"][self.device_id][sensor_name]
                collection.insert_one(sensor_document)
                
                print('Datos guardados en MongoDB')
            except Exception as e:
                try:
                    self.save_to_json(obj)
                    print('Datos guardados en archivo JSON')
                except Exception as e:
                    print('No se pudo guardar los datos en el archivo JSON:', e)

           
                
if __name__ == '__main__':
    a = arduinoToMongoClass()
    a.runSesion()
                
