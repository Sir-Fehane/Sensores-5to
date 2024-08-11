'''
from mongo import arduinoToMongoClass
import requests
import time

class Cicle:
    def __init__(self):
        self.cicle = False
    def setCicle(self, cicle):
        self.cicle = cicle
    def getCicle(self):
        return self.cicle
mongo = arduinoToMongoClass()
cicle = Cicle()

if __name__ == '__main__':
        response = requests.get('http://127.0.0.1:8000/api/send/')
        data = response.json()
        ser = mongo.ser
        ser.write(str(data['number']).encode())
        mongo.runSesion()
        time.sleep(5)
        '''