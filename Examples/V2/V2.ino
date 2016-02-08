#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);//LCD pins
#include "Adafruit_MAX31855.h"

int first, second;//temperature variables

String reflow_status[4] = {"press to start","preheat","critical","rampdwn"};
double temperature, old_temperature;
unsigned long start_time=0, old_time;
int setpoint = 0, reflow_stage=0, rate;
int i;
int thermoCLK = 52;
int thermoCS = 53;
int thermoDO = 50;
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Arduino Reflow");
  digitalWrite(5,HIGH);              //Resistancia de PullUp A1 pulsador
  digitalWrite(6,HIGH);
  pinMode(7, OUTPUT);//solidstate switch
  pinMode(A14, INPUT);//switch input
  pinMode(15, OUTPUT);//fan
  digitalWrite(2, HIGH);//disable max6675
  delay(500);
}//Setup


void loop(){
  get_temp();
  reflow_update();
  lcd_update();
  heat_control();
  switch_check();
  fan_control();
  Serial.println(int(temperature));
  
}//loop

//get temp   get temp   get temp   get temp   get temp   get temp
void get_temp(){
temperature = thermocouple.readCelsius();
}//get temp
//reflow_update  reflow_update  reflow_update  reflow_update  
void reflow_update(){
  
  if(reflow_stage==0){//idle   idle   idle
    setpoint=0;//don't do anything
  }//0
  
  if(reflow_stage==1){//preheat   preheat   preheat
    if(start_time == 0 && temperature>100)//start timer for preheat soak
    start_time=millis();
    
    setpoint=120;//regulate here during preheat
   /* rate = (temperature-old_temperature)/((millis()-old_time)/1000);//get rate of rise
    if(rate>3)//if rising too fast, kill the setpoint so it slows down
    setpoint=0;
    */
    old_time = millis();//used to find rate
    old_temperature=temperature;
    
    if((millis()-start_time)>60000 && start_time !=0){//this is where it checks to see if it needs to move on to the next stage
      reflow_stage++;
      start_time=0;}
  }//1 
  if(reflow_stage==2){//critical  critical  critical
    if(start_time ==0 && temperature>150)//start timer during this stage
    start_time=millis();
    
    setpoint=170;//regulate here
    rate = (temperature-old_temperature)/((millis()-old_time)/1000);//eh, you can regulate the rate here as well if needed
    if(rate>3)
    setpoint=0;
    
    old_time = millis();//rate stuff again
    old_temperature=temperature;
    
    if((millis()-start_time)>20000 && start_time !=0){//how long to stay in
      reflow_stage++;
      start_time=0;}
  }//2
  
   if(reflow_stage==3){//rampdown   rampdown   rampdown
    if(start_time ==0)
    start_time=millis();
    
    setpoint=0;
    rate = (temperature-old_temperature)/((millis()-old_time)/1000);
    if(rate<-20)//if rate is too fast you can turn heater back on
    setpoint=200;
    old_time = millis();
    old_temperature=temperature;
    if((millis()-start_time)>60000 && start_time !=0){
      reflow_stage=0;
      start_time=0;}
  }//3
 
  
}//reflow update
//lcd_update   lcd_update   lcd_update   lcd_update   lcd_update
void lcd_update(){
lcd.clear();
lcd.print("PV=");
lcd.print(temperature);
lcd.print("  ");
lcd.print(rate);//show the rate as well
lcd.setCursor(0,1);
lcd.print(reflow_status[reflow_stage]);
}//lcd_update
// heat_control   heat_control   heat_control   heat_control   
void heat_control(){//starigt forward here
  if(temperature<setpoint)
  digitalWrite(7, HIGH);
  else
  digitalWrite(7,LOW);
}//heat
//switch check   switch check   switch check  switch check
void switch_check(){//could be better, but it works
  if(digitalRead(5)==LOW){
   lcd.clear();
    lcd.print("Starting...");
    reflow_stage=1;
    delay(200);
    }
   if(digitalRead(6)==LOW){
   lcd.clear();
    lcd.print("STOP.");
    reflow_stage=0;
    delay(300);
    }
   
 // }
  
}//switch check
//fan control   fan control   fan control   fan control
void fan_control(){
  if(reflow_stage ==0 || reflow_stage ==3)//keep on continuously if idle or ramping down
  digitalWrite(15, HIGH);
  else
  digitalWrite(15, LOW);
  
  if(reflow_stage >0  && reflow_stage <3){//give a little 'love tap' if the temp is higher than the setpoint or if it's ramping quickly
    if(temperature>setpoint || rate>1){
    digitalWrite(15, HIGH);
    delay(500);
    digitalWrite(15,LOW);}
    
    
  }

}//fan
