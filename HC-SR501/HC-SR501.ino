 // Este sensor se utilizara como actuador para prender las luces led, por lo que el codigo
  // inconcluso hasta que se haga la tira led

int PIR=2;
int ESTADO = 0;
void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);
}

void loop() {
  ESTADO = digitalRead(PIR);
  Serial.print("PR : "); Serial.print(ESTADO); Serial.println(": Proximidad");
  delay(1000);

}
