const int ledPin = 9; // Pin digital donde están conectados los LEDs

void setup() {
  pinMode(ledPin, OUTPUT); // Configura el pin como salida
}

void loop() {
  digitalWrite(ledPin, HIGH); // Enciende los LEDs
  delay(1000);                // Espera 1 segundo
  digitalWrite(ledPin, LOW);  // Apaga los LEDs
  delay(1000);                // Espera 1 segundo
}

