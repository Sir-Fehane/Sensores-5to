from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import serial
import json
import time
from .mongo import arduinoToMongoClass
import threading


class Number:
    def __init__(self):
        self.numberToSend = 8
        self.status = False
    def setStatus(self, status):
        self.status = status
    def getStatus(self):
        return self.status
    
numberToSend = Number()
Mongo = arduinoToMongoClass()

@csrf_exempt
def send_number(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Método no permitido'}, status=405)
    
   # if numberToSend.getStatus() != False:
    #    return JsonResponse({'error': 'Ya hay un ciclo en ejecución'}, status=400)

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
            Mongo.runSingle()
            return JsonResponse({'message': 'Número enviado con éxito'}, status=200)
        except Exception as e:
            return JsonResponse({'error': f'Error al enviar el número: {e}'}, status=500)
    
    except json.JSONDecodeError:
        return JsonResponse({'error': 'JSON inválido'}, status=400)
'''
@csrf_exempt
def recibir_datos(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        numberToSend.setNumber(data.get('number'))
        return JsonResponse({
            "mensaje": "Datos recibidos correctamente",
            "datos_recibidos": numberToSend.getNumber()
            })
    else:
        return JsonResponse({
            "error": "Solo se aceptan solicitudes POST"
        }, status=405)

@csrf_exempt        
def mandar_number(request):
    if request.method != 'GET':
        return JsonResponse({
            "error": "Método no permitido"
        }, status=405)
    return JsonResponse({
        "number": numberToSend.getNumber()
    })
'''
@csrf_exempt
def start_cicle(request):
    if request.method == 'POST':
        status=numberToSend.getStatus()
        start = not status
        numberToSend.setStatus(start)
        if start:
            thread = threading.Thread(target=ciclowhile)
            thread.start()
        return JsonResponse({
            "message": "Ciclo iniciado" if start else "Ciclo detenido"
        })
    else:
        return JsonResponse({
            "error": "Solo se aceptan solicitudes POST"
        }, status=405)

def ciclowhile():
    while numberToSend.getStatus() == True: 
        print('Inicia')
        data = numberToSend.numberToSend
        sendingdata = str(data).encode()
        ser = Mongo.ser
        ser.write(sendingdata)
        Mongo.runSesion()
        time.sleep(5)
    print('Termina')