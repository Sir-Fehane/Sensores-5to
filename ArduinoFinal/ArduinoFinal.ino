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
int dPIR=2;

void setup() {
  dht.begin();
  pinMode(dPIR, INPUT);
  Serial.begin(9600);
}

void loop() {
  String nivel = checkValueLDR(analogRead(ldrPin));

  int ESTADO = digitalRead(dPIR);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("HM : "); Serial.print(h); Serial.print(" : % "); Serial.println(": Humedad");
  Serial.print("TP : "); Serial.print(t); Serial.print(" : *C "); Serial.println(": Temperatura");
  //tengo duda si mandar el caracter string o el valor tal cual ↓
  Serial.print("FR : "); Serial.print(nivel); Serial.print(" : lux "); Serial.println(": Luz");
  //habrá pedo en iOS por estos: ^ ↓
  Serial.print("PIR : "); Serial.print(ESTADO); Serial.print(" : "); Serial.println(": Movimiento");
  delay(2000);
}

String checkValueLDR(int valorLDR){
  String nivel = " ";
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
  return nivel;
}
