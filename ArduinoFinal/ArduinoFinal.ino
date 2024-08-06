#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//Peso
#include "HX711.h"
HX711 celda;
#define DT 6		
#define SCK 5
float peso = 0;

//Comprar resistencia de 1k ohmio a tierra
const int ldrPin = A0;

//Ultrasonico
const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;

const int mic = A1;

const int gas = A2;

//motor
const int pinIN1 = 7;
bool estadoMotor = false;

//leds
const int pinLeds = 11;
bool estadoLeds = false;
bool controlManual = false;

//bpm
const int LOPlus = 4;
const int LOMinus = 3;
const int BPM = A4;

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
  delay(2000);
}

void getData(String sensor) {
  if (sensor == "1") {
    sensorDHT11();
  } else if (sensor == "2") {
    sensorLDR();
  } else if (sensor == "3") {
    controlManual = !controlManual;
    cambiarLeds();
  } else if (sensor == "4") {
    sensorSonido();
  } else if (sensor == "5") {
    sensorMQ2();
  } else if (sensor == "6") {
    sensorPeso();
  } else if (sensor == "7") {
    sensorFrecuencia();
  } else if (sensor == "8") {
    sensorDHT11();
    sensorLDR();
    sensorSonido();
    sensorMQ2();
    sensorPeso();
    sensorFrecuencia();
  } else if (sensor == "9") {
    cambiarMotor();
  } else {
  }
}

void sensorPeso() {
  peso = celda.get_units(10);
  celda.power_down();
  celda.power_up();	
  Serial.print("PE : "); Serial.print(peso); Serial.print(" : gr"); Serial.println(": Bascula");
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

void sensorFrecuencia() {
  int frecValue = analogRead(BPM);
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
  Serial.print("SRC : "); Serial.print(nivel); Serial.print(" : bpm "); Serial.println(": Ritmo Card");
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

void cambiarEstado(int pin, bool &estado) {
  estado = !estado;
  digitalWrite(pin, estado ? HIGH : LOW);
}

void cambiarMotor() {
  cambiarEstado(pinIN1, estadoMotor);
  Serial.print("Motor : "); Serial.print(estadoMotor); Serial.print(" : Status"); Serial.println(": Motor");
}

void cambiarLeds() {
  cambiarEstado(pinLeds, estadoLeds);
  Serial.print("Leds : "); Serial.print(controlManual); Serial.print(" : Status"); Serial.println(": Leds");
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
