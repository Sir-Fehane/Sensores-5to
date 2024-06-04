/*Esta curioso el sensor, realmente no sirve para lo que lo necesitamos,
pero puedo ajustarlo para hacerlo funcionar. Que se mande una alerta al iphone si es que llego 
a un umbral de ruido muy alto y que vayan a checar al bebe*/

const int MIC = 0;
int valor;				// variable para almacenar valor leio de A0

void setup(){
// entradas analogicas no requieren inicializacion
  Serial.begin(9600);
}


void loop(){
  valor = analogRead(MIC);		// lee valor de entrada analogica
  Serial.println(valor); // escribe valor en monitor serial
  delay(500);

}
