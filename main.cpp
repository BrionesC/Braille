//Incluimos las librerias
#include <ESP32Servo.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección I2C de la pantalla LCD 16x2
#define LCD_ADDRESS 0x27

// Número de columnas y filas de la pantalla LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Inicializa la pantalla LCD con la dirección I2C y el número de columnas y filas
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

//Instanciamos los servos
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;

#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

int led_1=26;
int led_2=14;
int led_3=13;
int led_4=25;
int led_5=27;
int led_6=12;

int modo_aprender=34;
int modo_leccion=35;
int boton_serie=33;
int incorrecto=32;
int correcto=5;
int buzzer = 4;
//Variables para el botón de selección de serie
int buttonState = HIGH;
int lastButtonState = HIGH;
int buttonCounter = 0;
unsigned long lastButtonPressTime = 0;
unsigned long buttonTimeout = 2000; // 2 segundos
int savedCounter = 0;
int calificacion=0;

//Variables de salida del PWM
int canal=0, frec=2000,resolucion=8;

// Notas de la melodía:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// Duración de las notas: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

//SERIE UNO A-J
String a_j[]={"a","b","c","d","e","f","g","h","i","j"};
boolean seleccionado_uno_dos[10];
int serie_uno [10][6]={ // ORDEN DE LOS MUELLES: 1-4-2-5-3-6
  {HIGH, LOW, LOW, LOW, LOW, LOW},//a
  {HIGH, LOW, HIGH, LOW, LOW, LOW},//b
  {HIGH, HIGH, LOW, LOW, LOW, LOW},//c
  {HIGH, HIGH, LOW, HIGH, LOW, LOW},//d
  {HIGH, LOW, LOW, HIGH, LOW, LOW},//e
  {HIGH, HIGH, HIGH, LOW, LOW, LOW},//f
  {HIGH, HIGH, HIGH, HIGH, LOW, LOW},//g
  {HIGH, LOW, HIGH, HIGH, LOW, LOW},//h
  {LOW, HIGH, HIGH, LOW, LOW, LOW},//i
  {LOW, HIGH, HIGH, HIGH, LOW, LOW}//j
};
//SERIE DOS K-T
String k_t[]={"k","l","m","n","o","p","q","r","s","t"};
int serie_dos[10][6]={
  {HIGH, LOW, LOW, LOW, HIGH, LOW}, //k
  {HIGH, LOW, HIGH, LOW, HIGH, LOW}, //l
  {HIGH, HIGH, LOW, LOW, HIGH, LOW}, //m
  {HIGH, HIGH, LOW, HIGH, HIGH, LOW}, //n
  {HIGH, LOW, LOW, HIGH, HIGH, LOW}, //o
  {HIGH, HIGH, HIGH, LOW, HIGH, LOW}, //p
  {HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, //q
  {HIGH, LOW, HIGH, HIGH, HIGH, LOW}, //r
  {LOW, HIGH, HIGH, LOW, HIGH, LOW}, //s
  {LOW, HIGH, HIGH, HIGH, HIGH, LOW} //t
};
//SERIE TRES U-Z
String u_z[]={"u","v","x","y","z"};
boolean seleccionado_tres[5];
int serie_tres[5][6]={
  {HIGH, LOW, LOW, LOW, HIGH, HIGH},//u
  {HIGH, LOW, HIGH, LOW, HIGH, HIGH},//v
  {HIGH, HIGH, LOW, LOW, HIGH, HIGH},//x
  {HIGH, HIGH, LOW, HIGH, HIGH, HIGH},//y
  {HIGH, LOW, LOW, HIGH, HIGH, HIGH}//z
};
//SERIE CUATRO Ñ-W
String enie_w[]={"ñ","w"};
boolean seleccionado_cuatro[2];
int serie_cuatro[2][6]={
  {HIGH, HIGH, HIGH, HIGH, LOW, HIGH},//ñ
  {LOW, HIGH, HIGH, HIGH, LOW, HIGH}//w
};

//FUNCIÓN PARA POSICIÓN DEL SERVOMOTOR
int pos_servo(boolean alto_bajo){
  if(alto_bajo==HIGH){
    return 180;
  }
  else{
    return 90;
  }
}

//LECTOR DEL ARREGLO UNIDIMENSIONAL PARA LOS SERVOMOTORES
void salida_servos(int lista_servo[]){
  servo_1.write(pos_servo(lista_servo[0]));
  digitalWrite(led_1,lista_servo[0]);
  //Serial.println(pos_servo(serie_uno[i][1]));
  servo_2.write(pos_servo(lista_servo[2]));
  digitalWrite(led_2,lista_servo[2]);
  //Serial.println(pos_servo(serie_uno[i][2]));
  servo_3.write(pos_servo(lista_servo[4]));
  digitalWrite(led_3,lista_servo[4]);
  //Serial.println(pos_servo(serie_uno[i][3]));
  servo_4.write(pos_servo(lista_servo[1]));
  digitalWrite(led_4,lista_servo[1]);
  //Serial.println(pos_servo(serie_uno[i][4]));
  servo_5.write(pos_servo(lista_servo[3]));
  digitalWrite(led_5,lista_servo[3]);
  //Serial.println(pos_servo(serie_uno[i][5]));
  servo_6.write(pos_servo(lista_servo[5]));
  digitalWrite(led_6,lista_servo[5]);

  delay(5000); //10 segundos CAMBIAR
      Serial.println("Cambio de letra");

      servo_1.write(90);
      servo_2.write(90);
      servo_3.write(90);
      servo_4.write(90);
      servo_5.write(90);
      servo_6.write(90);
      digitalWrite(led_1,LOW);
      digitalWrite(led_2,LOW);
      digitalWrite(led_3,LOW);
      digitalWrite(led_4,LOW);
      digitalWrite(led_5,LOW);
      digitalWrite(led_6,LOW);
      delay(5000);
      Serial.println("Servos abajo");
}

//TEXTO EN LA PANTALLA
void pantalla(String frase1, String frase2){
  // Borra el contenido de la pantalla LCD
  lcd.clear();

  // Establece la posición del cursor en la columna 0, fila 0
  lcd.setCursor(0, 0);

  // Muestra el mensaje en la pantalla LCD
  lcd.print(frase1);

  lcd.setCursor(0,1);
  lcd.print(frase2);

  // Espera 2 segundos
  delay(2000);
}

void IRAM_ATTR correctocorrecto(){
  //SONIDO DE ACIERTO
  calificacion++;
  String fr= "Evaluacion";
  String fr1="Puntaje: "+calificacion;
  pantalla(fr,fr1);
}

void IRAM_ATTR incorrectoincorrecto(){
  //SONIDO DE EQUIVOCACION
  String fr= "Evaluacion";
  String fr1="Puntaje: "+calificacion;
  pantalla(fr,fr1);
}


//FUNCIONES PARA CADA SERIE
//Serie uno:
void uno(){
  if(digitalRead(modo_aprender)==HIGH){//Trabajo con servomotores
    //ledcWriteTone(0, 0);
    for(int i=0;i<10;i++){
      Serial.println("Aprendiendo serie uno: "+a_j[i]);
      String frase1= "SERIE UNO";
      String frase2= "Letra: "+a_j[i];
      pantalla(frase1,frase2);
      salida_servos(serie_uno[i]);
    }

  }

  if(digitalRead(modo_leccion)==HIGH){

    for(int j=0;j<10;j++){
  
      // Verificar si todos los elementos han sido seleccionados
      bool allSelected = true;
      for (int i = 0; i < 10; i++) {
        if (!seleccionado_uno_dos[i]) {
          allSelected = false;
          break;
        }
      }

      if (allSelected) {
        // Todos los elementos han sido seleccionados
        Serial.println("Todos los elementos han sido seleccionados");
        while (1) {}  // Detener la ejecución
      }

      // Generar un índice aleatorio
      int randomIndex = random(0, 10);

      // Buscar un elemento no seleccionado
      while (seleccionado_uno_dos[randomIndex]) {
        randomIndex = random(0, 10);
      }

      // Marcar el elemento como seleccionado
      seleccionado_uno_dos[randomIndex] = true;

      // Imprimir el elemento seleccionado
      Serial.print("Elemento seleccionado: ");
      Serial.println(a_j[randomIndex]);

      String frase11="SERIE UNO EVALUACIÓN";
      String frase22="Letra: "+a_j[randomIndex];
      pantalla(frase11,frase22);
      salida_servos(serie_uno[randomIndex]);

      /*do{
        if(digitalRead(correcto)==HIGH){
        calificacion++;
        Serial.println("CORRECTO");
        pantalla("CALIFICACION:",calificacion+"/10");
      }
      else if(digitalRead(incorrecto)==HIGH){
        //SONIDO DE EQUIVOCACION
        Serial.println("INCORRECTO");
      }
      } while(digitalRead(correcto)==LOW && digitalRead(incorrecto)==LOW);*/

      delay(1000);
    }
    calificacion=0;
  }

}

//Serie dos:
void dos(){
  if(digitalRead(modo_aprender)==HIGH){//Trabajo con servomotores
    //ledcWriteTone(0, 0);
    for(int i=0;i<10;i++){
      Serial.println("Aprendiendo serie dos: "+k_t[i]);
      String frase1= "SERIE DOS";
      String frase2= "Letra: "+k_t[i];
      pantalla(frase1,frase2);
      salida_servos(serie_dos[i]);
    }

  }

  if(digitalRead(modo_leccion)==HIGH){
      
      for(int j=0;j<10;j++){
  
      // Verificar si todos los elementos han sido seleccionados
      bool allSelected = true;
      for (int i = 0; i < 10; i++) {
        if (!seleccionado_uno_dos[i]) {
          allSelected = false;
          break;
        }
      }

      if (allSelected) {
        // Todos los elementos han sido seleccionados
        Serial.println("Todos los elementos han sido seleccionados");
        while (1) {}  // Detener la ejecución
      }

      // Generar un índice aleatorio
      int randomIndex = random(0, 10);

      // Buscar un elemento no seleccionado
      while (seleccionado_uno_dos[randomIndex]) {
        randomIndex = random(0, 10);
      }

      // Marcar el elemento como seleccionado
      seleccionado_uno_dos[randomIndex] = true;

      // Imprimir el elemento seleccionado
      Serial.print("Elemento seleccionado: ");
      Serial.println(k_t[randomIndex]);
      
      String frase11="SERIE UNO EVALUACIÓN";
      String frase22="Letra: "+k_t[randomIndex];
      pantalla(frase11,frase22);
      salida_servos(serie_dos[randomIndex]);

      delay(1000);
    }
  }
}

//Serie tres:
void tres(){
  if(digitalRead(modo_aprender)==HIGH){//Trabajo con servomotores
    //ledcWriteTone(0, 0);
    for(int i=0;i<5;i++){
      Serial.println("Aprendiendo serie tres: "+u_z[i]);
      String frase1= "SERIE TRES";
      String frase2= "Letra: "+u_z[i];
      pantalla(frase1,frase2);
      salida_servos(serie_tres[i]);
    }
  }

    if(digitalRead(modo_leccion)==HIGH){
      
      for(int j=0;j<5;j++){
  
      // Verificar si todos los elementos han sido seleccionados
      bool allSelected = true;
      for (int i = 0; i < 5; i++) {
        if (!seleccionado_tres[i]) {
          allSelected = false;
          break;
        }
      }

      if (allSelected) {
        // Todos los elementos han sido seleccionados
        Serial.println("Todos los elementos han sido seleccionados");
        while (1) {}  // Detener la ejecución
      }

      // Generar un índice aleatorio
      int randomIndex = random(0, 5);

      // Buscar un elemento no seleccionado
      while (seleccionado_tres[randomIndex]) {
        randomIndex = random(0, 5);
      }

      // Marcar el elemento como seleccionado
      seleccionado_tres[randomIndex] = true;

      // Imprimir el elemento seleccionado
      Serial.print("Elemento seleccionado: ");
      Serial.println(u_z[randomIndex]);

      String frase11="SERIE DOS EVALUACIÓN";
      String frase22="Letra: "+u_z[randomIndex];
      pantalla(frase11,frase22);
      salida_servos(serie_tres[randomIndex]);

      delay(1000);
    }
  }
}

//Serie cuatro:
void cuatro(){
  if(digitalRead(modo_aprender)==HIGH){//Trabajo con servomotores
    //ledcWriteTone(0, 0);
    for(int i=0;i<2;i++){
      Serial.println("Aprendiendo serie cuatro: "+enie_w[i]);
      String frase1= "SERIE CUATRO";
      String frase2= "Letra: "+enie_w[i];
      pantalla(frase1,frase2);
      salida_servos(serie_cuatro[i]);
    }
  }

    if(digitalRead(modo_leccion)==HIGH){
      
      for(int j=0;j<2;j++){
  
      // Verificar si todos los elementos han sido seleccionados
      bool allSelected = true;
      for (int i = 0; i < 2; i++) {
        if (!seleccionado_cuatro[i]) {
          allSelected = false;
          break;
        }
      }

      if (allSelected) {
        // Todos los elementos han sido seleccionados
        Serial.println("Todos los elementos han sido seleccionados");
        while (1) {}  // Detener la ejecución
      }

      // Generar un índice aleatorio
      int randomIndex = random(0, 2);

      // Buscar un elemento no seleccionado
      while (seleccionado_cuatro[randomIndex]) {
        randomIndex = random(0, 2);
      }

      // Marcar el elemento como seleccionado
      seleccionado_cuatro[randomIndex] = true;

      // Imprimir el elemento seleccionado
      Serial.print("Elemento seleccionado: ");
      Serial.println(enie_w[randomIndex]);

      String frase11="SERIE TRES EVALUACIÓN";
      String frase22="Letra: "+enie_w[randomIndex];
      pantalla(frase11,frase22);

      salida_servos(serie_cuatro[randomIndex]);

      delay(1000);
    }
  }
}

//APAGA LEDS
void apagar_leds(){
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  digitalWrite(led_5, LOW);
  digitalWrite(led_6, LOW);
}

void setup() {
  Serial.begin(115200);
  //DECLARO ENTRADAS Y SALIDAS
  pinMode(modo_aprender, INPUT);
  pinMode(modo_leccion, INPUT);
  pinMode(boton_serie, INPUT);
  pinMode(correcto, INPUT);
  pinMode(incorrecto, INPUT);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  pinMode(led_6, OUTPUT);
  
  Serial.println(digitalRead(modo_aprender));
  Serial.println(digitalRead(modo_leccion));
  Serial.println(digitalRead(boton_serie));
  Serial.println(digitalRead(correcto));
  Serial.println(digitalRead(incorrecto));

  //INTERRUPCIONES
  attachInterrupt(digitalPinToInterrupt(correcto), correctocorrecto, FALLING);
  attachInterrupt(digitalPinToInterrupt(incorrecto), incorrectoincorrecto, FALLING);

  // Inicializa la comunicación I2C
  Wire.begin();

  // Inicializa la pantalla LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS);

   // Inicializar el arreglo auxiliar de elementos seleccionados
  randomSeed(analogRead(0)); //Semilla A0
  for (int i = 0; i < 10; i++) {
    seleccionado_uno_dos[i] = false;
  }
  for (int i = 0; i < 5; i++) {
    seleccionado_tres[i] = false;
  }
  for (int i = 0; i < 2; i++) {
    seleccionado_cuatro[i] = false;
  }


 /* //inicializo el PWM para el buzzer
  ledcSetup(canal,frec,resolucion);
  //declaramos el pin donde se conecta el buzzer
  ledcAttachPin(buzzer,0);*/

  //Configuramos los servos a sus respectivos pines y sus tiempos
  servo_1.attach(15, 500, 2500);
  servo_2.attach(2, 500, 2500);
  servo_3.attach(16, 500, 2500);
  servo_4.attach(19, 500, 2500);
  servo_5.attach(18, 500, 2500);
  servo_6.attach(23, 500, 2500);
  //Movemos los dos servos a 90 grados
  servo_1.write(90);
  servo_2.write(90);
  servo_3.write(90);
  servo_4.write(90);
  servo_5.write(90);
  servo_6.write(90);


  /*//MELODIA AL ENCENDER EL JUEGO
  	for (int thisNote = 0; thisNote < 8; thisNote++) {

    	int noteDuration = 1000 / noteDurations[thisNote];
      ledcWriteTone(0,melody[thisNote]);
    	int pauseBetweenNotes = noteDuration * 1.30;
    	delay(pauseBetweenNotes);
    }
    //Para la melodía:
    ledcWriteTone(0, 0);*/
}

void loop() {

  buttonState = digitalRead(boton_serie);

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      lastButtonPressTime = millis();
      buttonCounter++;
      if (buttonCounter > 4) {
        buttonCounter = 1; // Reiniciar el contador si se excede de 4 pulsaciones
      }
    }

    delay(50);
  }

  lastButtonState = buttonState;

  unsigned long elapsedTime = millis() - lastButtonPressTime;
  if (elapsedTime > buttonTimeout) {
    savedCounter = buttonCounter;
    //buttonCounter = 0;
    Serial.print("Contador guardado: ");
    Serial.println(savedCounter);

    switch (savedCounter){
      case 1:
        Serial.println("serie uno");
        //Apago todos los leds por si acaso
        apagar_leds();
        //Activo led correspondiente
        digitalWrite(led_1, HIGH);
        apagar_leds();
        uno();
        break;

      case 2:
        Serial.println("serie dos");
        //Apago todos los leds por si acaso
        apagar_leds();
        //Activo led correspondiente
        digitalWrite(led_2, HIGH);
        apagar_leds();
        dos();
        break;

      case 3:
        Serial.println("serie tres");
        //Apago todos los leds por si acaso
        apagar_leds();
        //Activo led correspondiente
        digitalWrite(led_3, HIGH);
        apagar_leds();
        tres();
        break;

      case 4:
        Serial.println("serie cuatro");
        //Apago todos los leds por si acaso
        apagar_leds();
        //Activo led correspondiente
        digitalWrite(led_4, HIGH);
        apagar_leds();
        cuatro();
        break;

      default:
        Serial.println("Default");
        break;
    }

  }

  if (buttonCounter > 0) {
    Serial.print("Contador: ");
    Serial.println(buttonCounter);
  }

}