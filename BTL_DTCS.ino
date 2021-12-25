#include <DHT.h>
#include <LiquidCrystal.h> //LCD Library

#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
char auth[] = "EQhoT2sIr8wUUfPAIhpN7uiA8Thc9Kic";
WidgetLED led1(V1);
WidgetLED led2(V2);
WidgetLED led3(V3);
WidgetLED led4(V4);

const int DHTPIN = A1;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
int humid;
int temp;
int Moisture;

const int M_Sensor = A0;



const int PWM = 9;
const int interrupt_run = 2;
const int interrupt_auto = 3;
volatile byte Pump = LOW;
volatile byte A_M = LOW;
LiquidCrystal lcd(12, 11, 5, 4, 7, 6);

const int led_run = 10;
const int led_stop = 8;
const int led_manual = 13;
const int led_auto = A5;
void setup()
{
  Serial.begin(9600);
  Blynk.begin(Serial, auth);  
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear(); 

  pinMode(M_Sensor, INPUT);
  pinMode(13,OUTPUT);
  pinMode(PWM,OUTPUT);
  
  pinMode(led_run,OUTPUT);
  pinMode(led_stop,OUTPUT);
  pinMode(led_manual,OUTPUT);
  pinMode(led_auto,OUTPUT);
  

   
  lcd.setCursor(0,0);
  lcd.print("Project By");
  lcd.setCursor(0,1);
  lcd.print("Nhom xxx");
  delay(1000);




  pinMode(interrupt_run, INPUT_PULLUP);
  pinMode(interrupt_auto, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_run), change_state, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupt_auto), change_mode, FALLING);

}

void change_state() 
{
  delay(30);
  Pump = !Pump;
  
}
void change_mode() 
{
  delay(30);
  A_M = !A_M;
}



void loop()
{
  Blynk.run();
  lcd.begin(16, 2);
  lcd.clear(); 
    

  humid = dht.readHumidity();
  temp  = dht.readTemperature();

  Moisture = map(analogRead(M_Sensor), 0, 1023, 0, 100);
  lcd.setCursor(0,0);
  lcd.print("HUMID:");
  lcd.setCursor(6,0);
  lcd.print(humid);
  lcd.setCursor(9,0);
  lcd.print("TEMP:");
  lcd.print(temp);
  if(Pump == HIGH)
  {
    lcd.setCursor(0,1);
    lcd.print("PUMP:ON");
    digitalWrite(led_run,HIGH);  
    digitalWrite(led_stop,LOW);
    led1.on();
    led2.off();  
  }
  else
  { 
    lcd.setCursor(0,1);
    lcd.print("PUMP:OFF");
    digitalWrite(led_run,LOW);  
    digitalWrite(led_stop,HIGH); 
    led1.off();
    led2.on();  
  }
  if (A_M == HIGH) 
  {
    analogWrite(led_auto,255);  
    digitalWrite(led_manual,LOW);
    led4.off();
    led3.on();  
  }
  else
  {
    analogWrite(led_auto,0);  
    digitalWrite(led_manual,HIGH);
    led3.off();
    led4.on();
  }
  
  if(Moisture>70)   
  {         
      lcd.setCursor(9,1);
      lcd.print("UOT");
      if (A_M == HIGH) 
      {
        Pump = LOW;
      }  
      
  }
  else if (Moisture>= 30 && Moisture<=70) 
  { 
     lcd.setCursor(9,1);
     lcd.print("AM");
     if (A_M == HIGH) 
     {
       analogWrite(PWM,125);
       Pump = HIGH;       
     }
  }
  else  
  { 
     lcd.setCursor(9,1);
     lcd.print("KHO");
     if (A_M==HIGH) 
     {
       analogWrite(PWM,255);
       Pump = HIGH;  
     }
  }
 delay(100);    
}  
