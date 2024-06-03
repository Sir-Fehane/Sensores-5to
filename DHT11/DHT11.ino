#include "DHT.h"

#define DHTPIN 12     // Pin en el que está conectado el Data del DHT11
#define DHTTYPE DHT11   // Definimos el tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // Esperamos 2 segundos entre lecturas
  // Leemos la humedad y la temperatura
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Mostramos los valores leídos
  Serial.print("HM : "); Serial.print(h); Serial.print("% "); Serial.println(": Humedad");
  Serial.print("TP : "); Serial.print(t); Serial.print("*C "); Serial.println(": Temperatura");
}