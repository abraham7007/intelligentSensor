int TRIG = 10;
int ECO = 9;
//led//
int ledRed = 5;
int ledGreen = 6;
//buzzer//
#define BUZZER_ACTIVO 8
//MODO DE OPERACION//
#define MOD_1 2
#define MOD_2 3
#define MOD_3 4
//OTRAS VARIABLES PARA CORREGIR ERRORES
int d1 = 0;
int limit = 0;
int error = 0;
int flag = 0;
int t = 0;
int t1 = 0;
int t2 = 0;
int flagRed = 0;
//ESTADO DE BATERIA
float maximo = 4.2;
float minimo = 2.75;
void setup()
{
  pinMode(TRIG, OUTPUT);  // trigger como salida
  pinMode(ECO, INPUT);    // echo como entrada
  pinMode(MOD_1, INPUT_PULLUP);  // MODO_1 0.5m ENTRADA EN ALTO DEL DESLIZADOR
  pinMode(MOD_2, INPUT_PULLUP);  // MODO_2 1m ENTRADA EN ALTO DEL DESLIZADOR
  pinMode(MOD_3, INPUT_PULLUP);  // MODO_3 1.5m ENTRADA EN ALTO DEL DESLIZADOR
  pinMode(BUZZER_ACTIVO, OUTPUT);  // BUZZER COMO SALIDA
  pinMode(ledRed, OUTPUT); //LED ROJO COMO SALIDA
  pinMode(ledGreen, OUTPUT); //LED VERDE COMO SALIDA
  Serial.begin(9600);     // inicializacion de comunicacion serial a 9600 bps
}

void loop()
{
  //midiendo estado de bateria
  int sensorValue = analogRead(A0);
  float voltaje = sensorValue*(5/1023.00)*0.93;
  long DISTANCIA;
  long DURACION;
  digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG, LOW);    // del sensor
  DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
  DISTANCIA = DURACION / 58.2;    // distancia medida en centimetros
  Serial.print(DISTANCIA);
  Serial.println("cm");  
  error = abs(DISTANCIA - d1);
  d1 = DISTANCIA;
  
  if(digitalRead(MOD_1) == LOW && digitalRead(MOD_2) == HIGH && digitalRead(MOD_3) == HIGH){
    limit = 50;
    Serial.println("Modo : 0.5 metro");
  }
  else if(digitalRead(MOD_1) == HIGH && digitalRead(MOD_2) == LOW && digitalRead(MOD_3) == HIGH){
    limit = 100; 
    Serial.println("Modo : 1 metros"); 
  }
  else if(digitalRead(MOD_1) == HIGH && digitalRead(MOD_2) == HIGH && digitalRead(MOD_3) == LOW){
    limit = 150; 
    Serial.println("Modo : 1.5 metros"); 
  }
  else{
    limit = 25;
    Serial.println("Modo: 0.25 metros");
  }

  if (voltaje <= minimo ){
      for (int i=0; i<3; i++){
        digitalWrite(BUZZER_ACTIVO,HIGH);
        delay(750);
        digitalWrite(BUZZER_ACTIVO,LOW);
        delay(750); 
      }  
  }
  else {
      if ((DISTANCIA>=2 && DISTANCIA<=limit) && (error<10 || error>200)){ 
        flagRed = 1;
        digitalWrite(ledRed,HIGH);
        digitalWrite(ledGreen,LOW);
        if (t!=1){
          for (int i=0; i<3; i++){
            digitalWrite(BUZZER_ACTIVO,HIGH);
            delay(250);
            digitalWrite(BUZZER_ACTIVO,LOW);
            delay(250); 
          }
          t=1;
        }
        flag = 1;
        t1 = 0;
        t2 = 0;
      }
      else if ((flag==1 && DISTANCIA>limit) && (DISTANCIA<300 && error<10)){
        digitalWrite(ledRed,LOW);
        digitalWrite(BUZZER_ACTIVO,LOW);
        if (t1!=1){
          digitalWrite(ledGreen,HIGH);
          delay(4000);
          digitalWrite(ledGreen,LOW);
          t1=1;
        }
        flag = 0;
        flagRed = 0;
        t = 0;
        t2 = 0;
      }
      else{
        digitalWrite(ledRed,LOW);
        digitalWrite(ledGreen,LOW);
        digitalWrite(BUZZER_ACTIVO,LOW);
        flag = 0;
        if (flagRed==1){
              digitalWrite(ledRed,HIGH);
              digitalWrite(ledGreen,LOW);
              t2++;
              if (t2>25){
                digitalWrite(ledRed,LOW);
                digitalWrite(ledRed,LOW);
                t = 0;
              }
        }
        if (DISTANCIA>limit && DISTANCIA<2000){
          flag=1;
        }
      }
      delay(100);    
  }

}
