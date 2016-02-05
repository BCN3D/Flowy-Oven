
/*
 -------------------------------------- 
  LCD 16x4 - Menu
 --------------------------------------
 

  El circuito:
 * LCD RS pin va conectado al digital pin 8
 * LCD Enable pin conectado al digital pin 9
 * LCD D4 pin conectado al digital pin 10
 * LCD D5 pin conectado al digital pin 11
 * LCD D6 pin conectado al digital pin 12
 * LCD D7 pin conectado al digital pin 13

 
*/
  #define salida2 4
  #define salida 6
//  #define SOS 7
  int reflow=0;
  int p=0;
  int menu;              
  int fotocelda;                      //Conexion Fotocelda
  String AnuncioMenu;                 //Anuncio de impresion LCD 
  //Funciones
  void Estado();                      //Funcion Estado de las salidas
  void ma6675();
#include <Encoder.h>                  //libreria encoder
#include <LiquidCrystal.h>            //incluimos la libreria LCD

LiquidCrystal lcd(34,32, 30, 36, 28, 38);//inicializamos los pines 
Encoder knob(15, 16); 
int buton2= digitalRead(A15);
//------------------------------------
//Funcion principal
//------------------------------------
 
void setup() {
//  pinMode   (SOS,INPUT);              //Buzzer como salida
  pinMode    (salida,OUTPUT);      
  pinMode    (salida2,OUTPUT);         
  lcd.begin     (16, 4);              //Configuramos el tamaño de la LCD
  digitalWrite(1,HIGH);              //Resistancia de PullUp A1 pulsador
  digitalWrite(A15,HIGH);              //Resistancia de PullUp A1 pulsador
    pinMode(20, OUTPUT);//fan
  digitalWrite(20, LOW);
   menu = 0;
}
 
//------------------------------------
//Funcion ciclicla
//------------------------------------
  long oldPosition=0;
 
  void loop() {
  long newPosition = knob.read()/2;

  if (salida,LOW){

  lcd.clear(); 
  lcd.setCursor(5, 1);                //Asignamos la posicion de impresion
  lcd.print("EMERGENCY");
  digitalWrite(salida2,HIGH);
  }
  
  if (newPosition == oldPosition) { p=0; }
  if (newPosition > oldPosition ){ p=1;  }
  if (newPosition < oldPosition) { p=2;  }
  if (newPosition != oldPosition)
   {oldPosition = newPosition;}
  switch(p)
  {
   case 0:
   
   
   break;
   
   case 1:
    menu++;
    if (menu>3) menu=3;
   break;
   
   case 2:
     menu--;
     if (menu<0) menu=0;
   break;
  }
  
  
  switch (menu)  {                    //Valoramos el estadp del Menu
 
    case 0 :                          //si el valor de escala es igual a 0:
      lcd.setCursor(6, 1);                
      lcd.print("BCN3D");
     

    break;                            //Retorno
 
    case 1 :                          //si el valor de escala es igual a 1:
      lcd.setCursor(5, 0);                
      lcd.print("Horno SMD");
      Estado ();                      //Vamos al la funicion Menu  
      
    break;                            //Retorno
 
    case 2 :                          //si el valor de escala es igual a 2:
      lcd.setCursor(1, 0);                
      lcd.print("Preparar Pizza");
      Estado ();                      //Vamos al la funicion Estado
    break;                            //Retorno
 
    case 3 :                          //si el valor de escala es igual a 3:
        
       lcd.setCursor(1, 0);                
       lcd.print("Calentar bocadillo"); //cargamos la impresion
       Estado ();                      //Vamos al la funicion Estado
                                      //Vamos al la funicion Estado
      break;                          //Retorno
      
     
      
  }
 
  delay(300);                         //Retardo entre impresiones
  lcd.clear();                        //Borramos todo de la pantalla
}
 
void Estado()  {                      //Funcion de valoracion ON/OFF salidas
  
  int buton = digitalRead(A15);        //15 como entada digital del pulsador 
  int SOS = digitalRead(1);
 
  if (buton == HIGH) {                //Valoracion estado del pulsador
 
  digitalWrite(salida,LOW);          //Encendemos la salida
  lcd.setCursor(5, 1);                //Asignamos la posicion de impresion
  lcd.print("OFF");                    //Impresion del estado de la salida
 
 
  }
 
  else {
 
  digitalWrite(salida,HIGH);           //Apagamos la salida  
  lcd.setCursor(0, 1);                //Asignamos la posicion de impresion
  lcd.print("ON");                   //Impresion del estado de la salida
  reflow=1;
  }

 if (SOS == HIGH) {                //Valoracion estado del pulsador
  digitalWrite(salida2,LOW);           //Apagamos la salida  

  
  }
 
  else {
 digitalWrite(salida2,HIGH);          //Encendemos la salida
  lcd.clear(); 
  lcd.setCursor(6, 1);                //Asignamos la posicion de impresion
  lcd.print("SOS");                   //Impresion del estado de la salida
 
 
 
  } 
}



  

//}
 

   
//Fin programa
