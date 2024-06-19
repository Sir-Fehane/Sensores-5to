#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//Comprar resistencia de 1k ohmio a tierra
const int ldrPin = A0; // Pin donde está conectado el LDR

/*Este sensor se utilizara como actuador para prender las luces led, por lo que el codigo
inconcluso hasta que se haga la tira led
Se podría hacer con el servidor http en la rasp, que detecte el cambio y mande la peticion para prenderlo
*/
const int dPIR=2;
int estadoPIR = 0;


const int mic = A1;

const int gas = A2;

void setup() {
  dht.begin();
  pinMode(dPIR, INPUT);
  Serial.begin(9600);
}

void loop() {

  int lectura = Serial.read();
  getData(lectura);
  delay(2000);
}

void getData(int sensor) {
  switch(sensor){
    case 1:
      sensorDHT11();
      break;
    case 2:
      sensorLDR();
      break;
    case 3:
      sensorPIR();
      break;
    case 4:
      sensorSonido();
      break;
    case 5:
      sensorMQ2();
      break;
    case 6:
    //Peso
      break;
    case 7:
    //frecuencia
      break; 
    case 8:
      sensorDHT11();
      sensorLDR();
      sensorPIR();
      sensorSonido();
      sensorMQ2();
      break; 
    default:
      break;
  }
}

void sensorLDR() {
  int valorLDR = analogRead(ldrPin);
  String nivel = "";
  if(valorLDR >= 0 && valorLDR < 150)
  {
    nivel ="Muy alta";
  }
  else if(valorLDR >= 150 && valorLDR < 300)
  {
    nivel ="Alta";
  }
  else if(valorLDR >= 300 && valorLDR < 499)
  {
    nivel = "Aceptable";
  }
  else if(valorLDR >= 500 && valorLDR < 699)
  {
    nivel ="Baja";
  }
  else
  {
    nivel = "Luz apagada";
  }
  //tengo duda si mandar el caracter string o el valor tal cual ↓
  Serial.print("FR : "); Serial.print(nivel); Serial.print(" : lux "); Serial.println(": Luz");
}

void sensorDHT11() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("HM : "); Serial.print(h); Serial.print(" : % "); Serial.println(": Humedad");
  Serial.print("TP : "); Serial.print(t); Serial.print(" : *C "); Serial.println(": Temperatura");
}

void sensorPIR() {
  int ESTADO = digitalRead(dPIR);
  //habrá pedo en iOS por esto: ↓
  Serial.print("PIR : "); Serial.print(ESTADO); Serial.print(" : "); Serial.println(": Movimiento");
}

void leerSensor(int pin, int umbral, const String& etiqueta, const String& unidad) {
  int valor = analogRead(pin);
  String res = (valor < umbral) ? "Bajo" : "Alto";
  Serial.print(etiqueta); Serial.print(" : "); Serial.print(res); Serial.print(" : "); Serial.println(unidad);
}

void sensorSonido() {
  leerSensor(mic, 105, "SD", ": Sonido");
  delay(250);
}

void sensorMQ2() {
  leerSensor(gas, 210, "MQ", "ppm : Gas");
}