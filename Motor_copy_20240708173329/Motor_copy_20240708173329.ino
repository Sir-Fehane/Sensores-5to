// Definición de los pines
const int pinIN1 = 8;

// Variable de estado
bool estado = false;

void setup() {
  // Configurar los pines como salidas
  pinMode(pinIN1, OUTPUT);

  // Inicializar el motor apagado
  digitalWrite(pinIN1, LOW);

  Serial.begin(9600);
}

void loop() {
   // Alternar el estado cada 2 segundos (ejemplo)
  delay(2000);
  estado = !estado;

  if (estado) {
    // Encender el motor en una dirección
    digitalWrite(pinIN1, HIGH);
    Serial.print("Encendido");
  } else {
    // Apagar el motor
    digitalWrite(pinIN1, LOW);
    Serial.print("Apagado");
  }
}
