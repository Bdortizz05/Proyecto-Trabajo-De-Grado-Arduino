//******************************************************************//
//                                                              // 
//                                                              //
//                           version 1                                   //
//                                                              //
//                                                              //
//                                                              //
//******************************************************************//

   const int BotonInicio = 2;  // Botón de inicio
       ///////////////////////// MOTORES DC ////////////////////////                                                              
    const int MotorDC1 = 9;  // Motor de rallado (RF1) - PWM
    const int MotorDC2 = 10;  // Motor de la cinta transportadora - PWM
    const int MotorDC3 = 11; // Motor DC para triturado de masa (RF3) - PWM
    const int MotorDC4 = 12; // Motor DC para cernido de masa (RF4) - PWM
    //////////////////// INFRARROJOS ///////////////////
    const int INF1 = 3;  // Pin para el sensor infrarrojo INF1 extraccion liquido
    const int INF2 = 4;  // Pin para el sensor infrarrojo INF2 triturado
    const int INF3 = 5;  // Pin para el tercer sensor infrarrojo cernido 
    const int INF4 = 6;  // Pin para el cuarto sensor infrarrojo final 
// Pines DE SENSTIDO para los motores DC
    const int IN1_DCRF1 = 32; // Sentido de giro 1 para Motor de rallado (RF1)
    const int IN2_DCRF1 = 33; // Sentido de giro 2 para Motor de rallado (RF1)
    const int IN1_DCRF2 = 30; // Sentido de giro 1 para Motor de la cinta transportadora
    const int IN2_DCRF2 = 31; // Sentido de giro 2 para Motor de la cinta transportadora
    const int IN1_DCRF3 = 26; // Sentido de giro 1 para Motor DC para triturado de masa (RF3)
    const int IN2_DCRF3 = 27; // Sentido de giro 2 para Motor DC para triturado de masa (RF3)
    const int IN1_DCRF4 = 28; // Sentido de giro 1 para Motor DC para cernido de masa (RF4)
    const int IN2_DCRF4 = 29; // Sentido de giro 2 para Motor DC para cernido de masa (RF4)
//////// Definición de pines para el motor paso a paso, etc. ////////
    byte IN1_PTOPRF2 = 40;
    byte IN2_PTOPRF2 = 41;
    byte IN3_PTOPRF2 = 42;
    byte IN4_PTOPRF2 = 43;
    byte IN1_PTOPRF3 = 44;
    byte IN2_PTOPRF3 = 45;
    byte IN3_PTOPRF3 = 46;
    byte IN4_PTOPRF3 = 47;
    byte IN1_PTOPRF4 = 48;
    byte IN2_PTOPRF4 = 49;
    byte IN3_PTOPRF4 = 50;
    byte IN4_PTOPRF4 = 51;
 /////////////////////// VARIABLES PARA FUNCIONES //////////////////////////////
    bool ralladoCompleto = false;
    bool extraccionCompleta = false;  // Añadida esta variable
    bool trituradoCompleto = false;
    bool cernidoCompleto = false;
    bool motorRalladoActivado = false;
    bool cintaTransportadoraActivada = false;
    bool cintaTransportadoraIniciada = false;
    bool INF1Detectado = false;
    bool INF2Detectado = false;
    bool INF3Detectado = false;  
    bool INF4Detectado = false;

///////// /// ///   declaraciones de motor paso a paso nueva /// /// /// /// /// 

//definicion variables
int velocidadMotor = 1000;  // variable para determinar la velocidad 
// 800 maxima - minima 1000 o mas
int contadorPasos = 0;      // contador para los pasos
int pasosPorVuelta = 4076;  // pasos para una vuelta completa

// Tablas con la secuencia de encendido 
// quitar signo de comentar a la necesaria)

//secuencia 1-fase usar velocidadMotor = 2000
//const int cantidadPasos = 4;
//const int tablaPasos[4] = { B1000, B0100, B0010, B0001 };

//secuencia 2-fases usar velocidadMotor = 2000
//const int cantidadPasos = 4;
//const int tablaPasos[4] = { B1001, B1100, B0110, B0011 };

//secuencia media fase usar velocidadMotor = 1000
const int cantidadPasos = 8;
const int tablaPasos[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };

/////  TIEMPOS //// 
const int tiempoRallado = 3000;  // Tiempo de rallado en milisegundos
const int tiempoTriturado = 3000;  // Tiempo de triturado en milisegundos
const int tiempoCernido = 3000;  // Tiempo de cernido en milisegundos
const int tiempoExtraccion = 3000;  // Tiempo de cernido en milisegundos







void setup() {
  Serial.begin(9600);
  pinMode(BotonInicio, INPUT);
  pinMode(INF1, INPUT);
  pinMode(INF2, INPUT);
  pinMode(INF3, INPUT);
  pinMode(MotorDC1, OUTPUT);
  pinMode(MotorDC2, OUTPUT);
  pinMode(MotorDC3, OUTPUT);
  pinMode(MotorDC4, OUTPUT);
  pinMode(IN1_DCRF1, OUTPUT);
  pinMode(IN2_DCRF1, OUTPUT);
  pinMode(IN1_DCRF2, OUTPUT);
  pinMode(IN2_DCRF2, OUTPUT);
  pinMode(IN1_DCRF3, OUTPUT);
  pinMode(IN2_DCRF3, OUTPUT);
  pinMode(IN1_DCRF4, OUTPUT);
  pinMode(IN2_DCRF4, OUTPUT);
  for(int i = IN1_PTOPRF2; i <= IN4_PTOPRF2; i++) pinMode(i, OUTPUT);
  for(int i = IN1_PTOPRF3; i <= IN4_PTOPRF3; i++) pinMode(i, OUTPUT);
  for(int i = IN1_PTOPRF4; i <= IN4_PTOPRF4; i++) pinMode(i, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


esperarBoton();
// /////////////// ETAPA DE RALLADO // /////////////// 
  if (!motorRalladoActivado) {
    // Espera a que se presione el botón de inicio
    if (digitalRead(BotonInicio) == HIGH) {
      activarMotorRallado();
      motorRalladoActivado = true;
    }
  }

// /////////////// infrarrojo INF1 de banda transportadora // /////////////// 
if (INF1Detectado == false)  {
  digitalWrite(IN1_DCRF2, HIGH);  // Sentido de giro 1
  digitalWrite(IN2_DCRF2, LOW);   // Sentido de giro 2
  analogWrite(MotorDC2, 40);  // Máxima velocidad

  // Espera a que se detecte INF1
  while (digitalRead(INF1) == HIGH) {
    // Espera a que se detecte INF1
     // El botón está presionado
 //  delay(10);  // Esperar 10 ms 
     Serial.println("infrarrojo detectado ");
   INF1Detectado = true ; 
  }
  // La cinta transportadora se detiene después de detectar INF1
  analogWrite(MotorDC2, 0);  // Apaga la cinta transportadora
  Serial.println("MOTOR APAGADO");
     cintaTransportadoraActivada = true;
     cintaTransportadoraIniciada = true;  // Marca como iniciada
}

 // ///////////////// ETAPA  extracción solo después de que la cinta se detiene  // /////////////// 
  if (motorRalladoActivado && cintaTransportadoraActivada == true && extraccionCompleta == false) {
    extraccionLiquido();
    extraccionCompleta = true;
  }

// /////////////// infrarrojo INF2 de banda transportadora // /////////////// 
if (INF2Detectado == false && extraccionCompleta == true )  {
  digitalWrite(IN1_DCRF2, HIGH);  // Sentido de giro 1
  digitalWrite(IN2_DCRF2, LOW);   // Sentido de giro 2
  analogWrite(MotorDC2, 100);  // Máxima velocidad

  // Espera a que se detecte INF1
  while (digitalRead(INF2) == HIGH) {
  INF2Detectado = true ; 
  }
  // La cinta transportadora se detiene después de detectar INF1
  analogWrite(MotorDC2, 0);  // Apaga la cinta transportadora
  Serial.println("MOTOR APAGADO infrarrojo 2");

}
  

// /////////////// ETAPA TRITURADO DE MASA // /////////////// 
  if (INF2Detectado == true && trituradoCompleto == false) {
    trituradoMasa();
    trituradoCompleto = true;  // Marcar como completo
  }

  

}



//////////////////////////////// Actuadores
/////////////FASE DE RALLADO ////////////////
void activarMotorRallado() {
    digitalWrite(IN1_DCRF1, HIGH);  // Sentido de giro 1
  digitalWrite(IN2_DCRF1, LOW);   // Sentido de giro 2
  analogWrite(MotorDC1, 40);  // Máxima velocidad 0 a 2555 
  delay(tiempoRallado);  // Activa el motor durante 1 segundo (ajustable)
  analogWrite(MotorDC1, 0);  // Apaga el motor
}

/////////////FASE DE EXTRACCION DE LIQUIDO ////////////////
void extraccionLiquido() {

for (int i = 0; i < pasosPorVuelta * 2; i++)
  {
  sentidoHorario(IN1_PTOPRF2,IN2_PTOPRF2,IN3_PTOPRF2,IN4_PTOPRF2);
  delayMicroseconds(velocidadMotor);
  }
/// CAMBIO DE SENTIDO 
 Serial.println("GIRO  DE MOTOR PASO A PASO A DERECHA ");
      delay(tiempoExtraccion);

for (int i = 0; i < pasosPorVuelta * 2; i++)
  {
  sentidoAntihorario(IN1_PTOPRF2,IN2_PTOPRF2,IN3_PTOPRF2,IN4_PTOPRF2);
  delayMicroseconds(velocidadMotor);
  }
      delay(200);
      Serial.println("GIRO  DE MOTOR PASO A PASO A IZQUIERDA ");
    }

/////////////FASE DE TRITURADO DE MASA////////////////
    // Función principal para el triturado de masa
    void trituradoMasa() {
      
// Triturado paso a paso 
            for (int i = 0; i < pasosPorVuelta * 2; i++){
              sentidoHorario(IN1_PTOPRF2,IN2_PTOPRF2,IN3_PTOPRF2,IN4_PTOPRF2);
              delayMicroseconds(velocidadMotor);
              }
/// CAMBIO DE SENTIDO 
Serial.println("GIRO  DE MOTOR PASO A PASO A DERECHA ");
delay(tiempoTriturado);
      triturarMasaDC(tiempoTriturado);  // Triturado de masa con motor DC
// Triturado paso a paso a retornar a posicion inicial
            for (int i = 0; i < pasosPorVuelta * 2; i++){
              sentidoAntihorario(IN1_PTOPRF2,IN2_PTOPRF2,IN3_PTOPRF2,IN4_PTOPRF2);
              delayMicroseconds(velocidadMotor);
              }
Serial.println("GIRO  DE MOTOR PASO A PASO A IZQUIERDA ");

 }
    // Función para manejar el motor DC
    void triturarMasaDC(int tiempo) {
      // Configurar sentido de giro del motor DC
      digitalWrite(IN1_DCRF3, HIGH);
      digitalWrite(IN2_DCRF3, LOW);
      analogWrite(MotorDC3, 120);  // Encender motor DC a máxima velocidad
      delay(tiempoTriturado);  // Mantener encendido durante tiempo ajustable
      analogWrite(MotorDC3, 0);  // Apagar motor DC
     
    }
    


///////////////////////// ---------------------------------------//////////////////////////////////
/// Funciones paso a paso nuevas

void sentidoHorario(int pin1, int pin2, int pin3, int pin4) // en dirección de las agujas del reloj
{
contadorPasos++;
if (contadorPasos >= cantidadPasos) contadorPasos = 0;
escribirSalidas(contadorPasos,pin1,pin2,pin3,pin4);
}

void sentidoAntihorario(int pin1, int pin2, int pin3, int pin4)// en dirección contraria a las agujas del reloj
{
contadorPasos--;
if (contadorPasos < 0) contadorPasos = cantidadPasos - 1;
escribirSalidas(contadorPasos,pin1,pin2,pin3,pin4);
}

void escribirSalidas(int paso, int pin1, int pin2, int pin3, int pin4) {
  digitalWrite(pin1, bitRead(tablaPasos[paso], 0));
  digitalWrite(pin2, bitRead(tablaPasos[paso], 1));
  digitalWrite(pin3, bitRead(tablaPasos[paso], 2));
  digitalWrite(pin4, bitRead(tablaPasos[paso], 3));
}

/////////





//////////////////// boton //////////////////

void esperarBoton() {
  while (digitalRead(BotonInicio) == LOW) {
    delay(10);  // Esperar 10 ms
    // El botón está presionado
  }
  Serial.println("Botón  presionado");
}
