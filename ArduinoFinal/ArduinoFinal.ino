#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#include "HX711.h"
HX711 celda;

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

//motor
const int pinIN1 = 8;
bool estadoMotor = false;

//leds
const int pinLeds = 11;
bool estadoLeds = false;

//bpm
const int LOPlus = 4;
const int LOMinus = 3;
const int BPM = A4;

void setup() {
  dht.begin();
  pinMode(dPIR, INPUT);
    // Configurar Motor
  pinMode(pinIN1, OUTPUT);
  digitalWrite(pinIN1, LOW);
  //configurar leds
  pinMode(pinLeds, OUTPUT);
  digitalWrite(pinLeds, LOW);
  // BPM
  pinMode(LOPlus, INPUT);
  pinMode(LOMinus, INPUT);

  Serial.begin(9600);
}

void loop() {
  String lectura = Serial.readStringUntil('\n'); // Leer hasta un salto de línea
  getData(lectura);
  delay(2000);
}

void getData(String sensor) {
  if (sensor == "1") {
    sensorDHT11();
  } else if (sensor == "2") {
    sensorLDR();
  } else if (sensor == "3") {
    sensorPIR();
  } else if (sensor == "4") {
    sensorSonido();
  } else if (sensor == "5") {
    sensorMQ2();
  } else if (sensor == "6") {
    //sensorPeso();
  } else if (sensor == "7") {
    sensorFrecuencia();
  } else if (sensor == "8") {
    sensorDHT11();
    sensorLDR();
    sensorPIR();
    sensorSonido();
    sensorMQ2();
    //sensorPeso();
  } else if (sensor == "9") {
    cambiarMotor();
  } else if (sensor == "10") {
    cambiarLeds();
  } else {
  }
}

void sensorPeso() {
  float peso = 0.0;
  peso = celda.get_units(10);
  celda.power_down();
  celda.power_up();	
  celda.tare();
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

void cambiarEstado(int pin, bool &estado) {
  estado = !estado;
  digitalWrite(pin, estado ? HIGH : LOW);
}

void cambiarMotor() {
  cambiarEstado(pinIN1, estadoMotor);
}

void cambiarLeds() {
  cambiarEstado(pinLeds, estadoLeds);
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
