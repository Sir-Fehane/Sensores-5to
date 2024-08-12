#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11 
const int sizeDHT = 1;
DHT dht(DHTPIN, DHTTYPE);
DHT arrayDht[sizeDHT] = {dht}; //cambiar tamaño del arreglo según la cantidad de sensores que haya

//Peso
#include "HX711.h"
HX711 celda;
#define DT 6		
#define SCK 5
const int sizeHX = 1;
HX711 arrayHX[sizeHX] = {celda};

//Comprar resistencia de 1k ohmio a tierra
const int ldrPin = A0;
const int sizeLdr = 1;
int arrayLdr[sizeLdr] = {ldrPin};

//Ultrasonico  Realmente este no manda informacion, no se si ponerlo escalable
//Este se hace con un array multidimensional
const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;

//SD
const int mic = A1;
const int sizeSD = 1;
int arraySD[sizeSD] = {mic};

//MQ
const int gas = A2;
const int sizeMQ = 1;
int arrayMQ[sizeMQ] = {gas};

//motor
const int pinIN1 = 7;
bool estadoMotor = false;
const int sizeMotor = 1;
const int sizeEstadoMotor = 1;
int arrayMotor[sizeMotor] = {pinIN1};
bool arrayEstadoMotor[sizeEstadoMotor] = {estadoMotor};

//leds
const int pinLeds = 11;
bool estadoLeds = false;
const int sizeLeds = 1;
int arrayLeds[sizeLeds] = {pinLeds};
bool controlManual = false;
const int sizeEstadoLeds = 1;
bool arrayEstadoLeds[sizeEstadoLeds] = {controlManual};

//bpm
const int LOPlus = 4;
const int LOMinus = 3;
const int BPM = A4;
const int sizeFC = 1;
int arrayFrecuenciaCard[sizeFC] = {BPM};

void setup() {
  dht.begin();
    // Configurar Motor
  pinMode(pinIN1, OUTPUT);
  digitalWrite(pinIN1, LOW);
  //configurar leds
  pinMode(pinLeds, OUTPUT);
  digitalWrite(pinLeds, LOW);
  // BPM
  pinMode(LOPlus, INPUT);
  pinMode(LOMinus, INPUT);
  //Ultrasonico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Peso
  celda.begin(DT, SCK);
  celda.set_scale(113.f);	// establece el factor de escala obtenido del primer programa
  celda.tare();			
  Serial.begin(9600);
}

void loop() {
  String lectura = Serial.readStringUntil('\n'); // Leer hasta un salto de línea
  getData(lectura);
  
  ultrasonico();
}

void getData(String sensor) {
  int separatorIndex = sensor.indexOf(':');
  String identificador = sensor.substring(0, separatorIndex);
  String numSensorStr = sensor.substring(separatorIndex + 1);
  int numSensor = numSensorStr.toInt(); // Convertir la parte numérica a int
  if (identificador == "HM") {
    sensorHM(numSensor);
  } else if (identificador == "TP") {
    sensorTP(numSensor);
  } else if (identificador == "FR") {
    sensorLDR(numSensor);
  } else if (identificador == "SD") {
    sensorSonido(numSensor);
  } else if (identificador == "MQ") {
    sensorMQ2(numSensor);
  } else if (identificador == "PE") {
    sensorPeso(numSensor);
  } else if (identificador == "SRC") {
    sensorFrecuencia(numSensor);
  } else if (identificador == "Leds") {
    controlManual = !controlManual;
    cambiarLeds(numSensor);
  } else if (identificador == "Motor") {
    cambiarMotor(numSensor);
  } else if (identificador == "All"){ //Debe llegar como un All:999
    sensorHM(numSensor);
    sensorTP(numSensor);
    sensorLDR(numSensor);
    sensorSonido(numSensor);
    sensorMQ2(numSensor);
    sensorPeso(numSensor);
    sensorFrecuencia(numSensor);
  }
  else{
  }
}

bool checkIndexArray(int index, int size){
  if (index >= 0 && index+1 <= size) {
    return true;
  } else {
    return false;
  }
}

void sensorHM(int numSensor) {
  if (numSensor == 999) {
    for (int i=0; i < sizeDHT; i++){
      float h = arrayDht[i].readHumidity();
      Serial.print("HM"); Serial.print(i); Serial.print(" : "); Serial.print(h); Serial.print(" : % "); Serial.println(": Humedad");
    }
    return;
  }
  if (checkIndexArray(numSensor, sizeDHT) == true) {
    float h = arrayDht[numSensor].readHumidity();
    Serial.print("HM"); Serial.print(numSensor); Serial.print(" : "); Serial.print(h); Serial.print(" : % "); Serial.println(": Humedad");
  }
  else{
    Serial.println("ERROR");
  }
}

void sensorTP(int numSensor) {
  if (numSensor == 999) {
    for (int i=0; i < sizeDHT; i++){
      float t = arrayDht[i].readTemperature();
      Serial.print("TP"); Serial.print(i); Serial.print(" : "); Serial.print(t); Serial.print(" : *C "); Serial.println(": Temperatura");
    }
    return;
  }
  if (checkIndexArray(numSensor, sizeDHT) == true) {
    float t = arrayDht[numSensor].readTemperature();
    Serial.print("TP"); Serial.print(numSensor); Serial.print(" : "); Serial.print(t); Serial.print(" : *C "); Serial.println(": Temperatura");
  }
  else{
    Serial.println("ERROR");
  }
}

void sensorPeso(int numSensor) {
  if (numSensor == 999) {
    for (int i=0; i < sizeHX; i++){
    float peso = arrayHX[i].get_units(10);
    arrayHX[i].power_down();
    arrayHX[i].power_up();	
    Serial.print("PE"); Serial.print(i); Serial.print(" : "); Serial.print(peso); Serial.print(" : gr"); Serial.println(": Bascula");
    }
    return;
  }
  if (checkIndexArray(numSensor, sizeDHT) == true) {
    float peso = arrayHX[numSensor].get_units(10);
    arrayHX[numSensor].power_down();
    arrayHX[numSensor].power_up();	
    Serial.print("PE"); Serial.print(numSensor); Serial.print(" : "); Serial.print(peso); Serial.print(" : gr"); Serial.println(": Bascula");
  }
  else {
    Serial.println("ERROR");
  }

}

void sensorLDR(int numSensor) {
  int valorLDR = analogRead(arrayLdr[numSensor]);
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
  Serial.print("FR"); Serial.print(numSensor); Serial.print(" : "); Serial.print(nivel); Serial.print(" : lux "); Serial.println(": Luz");
}

void sensorFrecuencia(int numSensor) {
  int frecValue = analogRead(arrayFrecuenciaCard[numSensor]);
  String nivel = "";
  if(frecValue >= 0 && frecValue < 50)
  {
    nivel ="Muy bajo";
  }
  else if(frecValue >= 50 && frecValue < 100)
  {
    nivel ="Normal";
  }
  else if(frecValue >= 100 && frecValue < 150)
  {
    nivel = "Poco alto";
  }
  else if(frecValue >= 150 && frecValue < 200)
  {
    nivel ="Alto";
  }
  else
  {
    nivel = "Muy alto";
  }
  Serial.print("SRC"); Serial.print(numSensor); Serial.print(" : "); Serial.print(nivel); Serial.print(" : bpm "); Serial.println(": Ritmo Card");
}

void leerSensor(int pin, int umbral, const String& etiqueta, const String& unidad, int numSensor) {
  int valor = analogRead(pin);
  String res = (valor < umbral) ? "Bajo" : "Alto";
  Serial.print(etiqueta); Serial.print(numSensor); Serial.print(" : "); Serial.print(res); Serial.print(" : "); Serial.println(unidad);
}

void sensorSonido(int numSensor) {
  leerSensor(arraySD[numSensor], 105, "SD", "dB : Sonido", numSensor);
}

void sensorMQ2(int numSensor) {
  leerSensor(arrayMQ[numSensor], 210, "MQ", "ppm : Gas", numSensor);
}

void cambiarMotor(int numSensor) {
  cambiarEstado(arrayMotor[numSensor], arrayEstadoMotor[numSensor]);
  Serial.print("Motor"); Serial.print(numSensor); Serial.print(" : "); Serial.print(estadoMotor); Serial.print(" : Status"); Serial.println(": Motor");
}

void cambiarLeds(int numSensor) {
  cambiarEstado(arrayLeds[numSensor], arrayEstadoLeds[numSensor]);
  Serial.print("Leds"); Serial.print(numSensor); Serial.print(" : "); Serial.print(controlManual); Serial.print(" : Status"); Serial.println(": Leds");
}

void cambiarEstado(int pin, bool &estado) {
  estado = !estado;
  digitalWrite(pin, estado ? HIGH : LOW);
}
void ultrasonico() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (!controlManual) {
    if (distance < 15) {
      digitalWrite(pinLeds, HIGH);
    } else {
      digitalWrite(pinLeds, LOW);
    }
  }
}


  if (numSensor == 999) {
    for (int i=0; i < sizeDHT; i++){

    }
    return;
  }
  if (checkIndexArray(numSensor, sizeDHT) == true) {

  }
  else {
    Serial.println("ERROR");
  }