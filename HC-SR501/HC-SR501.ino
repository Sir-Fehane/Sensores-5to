/*Este sensor se utilizara como actuador para prender las luces led, por lo que el codigo
inconcluso hasta que se haga la tira led
Se podría hacer con el servidor http en la rasp, que detecte el cambio y mande la peticion para prenderlo
*/
int dPIR=2;
void setup() {
  Serial.begin(9600);
  pinMode(dPIR, INPUT);
}

void loop() {
 int ESTADO = digitalRead(dPIR);
  Serial.print("PIR : "); Serial.print(ESTADO); Serial.println(": Movimiento");
  delay(1000);

}
