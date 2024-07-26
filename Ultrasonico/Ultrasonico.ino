// Definimos los pines del Trigger y Echo
const int trigPin = 9;
const int echoPin = 8;
// leds
const int pinLeds = 11;
bool estadoLeds = false;
bool controlManual = false;

// Definimos una variable para la distancia
long duration;
int distance;

void setup() {
  // Inicializamos el puerto serie para la comunicación
  Serial.begin(9600);
  // Definimos los pines del Trigger y Echo como salida y entrada respectivamente
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // configurar leds
  pinMode(pinLeds, OUTPUT);
  digitalWrite(pinLeds, LOW);
}

void loop() {
  String lectura = Serial.readStringUntil('\n'); // Leer hasta un salto de línea
  getData(lectura);
  ultrasonico();
  delay(1000);
}

void getData(String sensor) {
  if (sensor == "1") {
    controlManual = !controlManual;
    cambiarLeds();
  } else {
  }
}

void ultrasonico() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (!controlManual) {
    if (distance < 15) {
      digitalWrite(pinLeds, HIGH);
    } else {
      digitalWrite(pinLeds, LOW);
    }
  }
}

void cambiarLeds() {
  cambiarEstado(pinLeds, estadoLeds);
}

void cambiarEstado(int pin, bool &estado) {
  estado = !estado;
  digitalWrite(pin, estado ? HIGH : LOW);
}
