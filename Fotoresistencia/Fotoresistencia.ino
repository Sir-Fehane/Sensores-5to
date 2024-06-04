const int ldrPin = A0; // Pin donde está conectado el LDR
int valorLDR = 0;
String nivel = "";

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial
}
//Comprar resistencia de 1k ohmio a tierra
void loop() {
  valorLDR = analogRead(ldrPin); // Lee el valor del LDR
  
  // Usa un if-else case para determinar si la luz es alta o baja

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
  Serial.print("FR : "); Serial.print(nivel); Serial.print(" "); Serial.println(": Luz");
  delay(500); // Espera un segundo antes de la próxima lectura
}