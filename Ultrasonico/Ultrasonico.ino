// Definimos los pines del Trigger y Echo
const int trigPin = 9;
const int echoPin = 8;
//leds
const int pinLeds = 11;
bool estadoLeds = false;

// Definimos una variable para la distancia
long duration;
int distance;

void setup() {
  // Inicializamos el puerto serie para la comunicación
  Serial.begin(9600);
  // Definimos los pines del Trigger y Echo como salida y entrada respectivamente
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //configurar leds
  pinMode(pinLeds, OUTPUT);
  digitalWrite(pinLeds, LOW);
}

void loop() {
  // Limpiamos el pin del Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Enviamos un pulso de 10 microsegundos en el Trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leemos el tiempo que tarda en llegar el pulso al Echo
  duration = pulseIn(echoPin, HIGH);

  // Calculamos la distancia en centímetros
  distance = duration * 0.034 / 2;

  // Imprimimos la distancia en el monitor serie
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < 15){
    digitalWrite(pinLeds, HIGH);
  }
  else{
    digitalWrite(pinLeds, LOW);
  }

  // Esperamos un poco antes de la siguiente medición
  delay(1000);
}
void cambiarLeds() {
  cambiarEstado(pinLeds, estadoLeds);
}

void cambiarEstado(int pin, bool &estado) {
  estado = !estado;
  digitalWrite(pin, estado ? HIGH : LOW);
}