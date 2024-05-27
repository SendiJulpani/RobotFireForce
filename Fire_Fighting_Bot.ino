#include <Servo.h>            //include servo.h library
#include <SoftwareSerial.h>  //include SoftwareSerial.h library

Servo myservo;
 
int pos = 0;    
boolean api = false;


const String PHONE = "+6285864389701"; //menggunakan nomer handpone untuk SIM800L

#define rxPin 2   // TX dari sim800l
#define txPin 3   // RX dari sim800l
SoftwareSerial sim800L(rxPin,txPin);
#define Kiri 4          // flame sensor kiri
#define Kanan 5         // flame sensor kanan
#define Depan 6       // flame sensor depan
#define GAS_SENSOR 7    // gas mq2 sensor 
#define KM1 8           // kiri motor wheel
#define KM2 9           // kiri motor wheel
#define NM1 10          // kanan motor wheel
#define NM2 11          // kanan motor wheel
#define pump 12         // waterpumb



void setup()
{
   
  Serial.begin(115200);
 
  sim800L.begin(9600);

  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
  
  pinMode(Kiri, INPUT);
  pinMode(Kanan, INPUT);
  pinMode(Depan, INPUT);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(KM1, OUTPUT);
  pinMode(KM2, OUTPUT);
  pinMode(NM1, OUTPUT);
  pinMode(NM2, OUTPUT);
  pinMode(pump, OUTPUT);
 
  myservo.attach(13);
  myservo.write(90); 
  
   while(sim800L.available()){
  Serial.println(sim800L.readString());
  }
  
}
 
void mematikan_api()
{
  
 
 
    digitalWrite(KM1, HIGH);
    digitalWrite(KM2, HIGH);
    digitalWrite(NM1, HIGH);
    digitalWrite(NM2, HIGH);
    digitalWrite(pump,HIGH);
    delay(500);
 
 
    for (pos = 50; pos <= 110; pos += 1) { 
    myservo.write(pos); 
    delay(10);  
  }
  for (pos = 110; pos >= 50; pos -= 1) { 
    myservo.write(pos); 
    delay(10);
  }
  digitalWrite(pump,LOW);
  myservo.write(90); 
  api=false;
}
 
void loop()
{

  
  myservo.write(90); //gerak servo();  
  
  if (digitalRead(Kiri) ==1 && digitalRead(Kanan)==1 && digitalRead(Depan) ==1) 
    {
    delay(500);
    digitalWrite(KM1, HIGH);
    digitalWrite(KM2, HIGH);
    digitalWrite(NM1, HIGH);
    digitalWrite(NM2, HIGH);
    }
    else if (digitalRead(Depan) ==0) 
    {
    digitalWrite(KM1, HIGH);
    digitalWrite(KM2, LOW);
    digitalWrite(NM1, HIGH);
    digitalWrite(NM2, LOW);
    api = true;
    
    }
    
    else if (digitalRead(Kiri) ==0)
    {
    digitalWrite(KM1, HIGH);
    digitalWrite(KM2, LOW);
    digitalWrite(NM1, HIGH);
    digitalWrite(NM2, HIGH);
    }
    
    else if (digitalRead(Kanan) ==0) 
    {
    digitalWrite(KM1, HIGH);
    digitalWrite(KM2, HIGH);
    digitalWrite(NM1, HIGH);
    digitalWrite(NM2, LOW);
    }
    delay(400); //ubah nilai untuk merubah jarak
    
     if(digitalRead(GAS_SENSOR)== 0)
    {
     Serial.println("Gas Terdeteksi");
      mengirim_sms(); 
    }
   
     while (api == true)
     {
      mematikan_api();
      Serial.println("Api Terdeteksi");
      menelphone();
     }}
     void menelphone()
{
    Serial.println("telpon.....");
    sim800L.println("ATD"+PHONE+";");
    delay(20000); //20 detik delay
    sim800L.println("ATH");
    delay(1000); //1 detik delay
}

void mengirim_sms()
{
    Serial.println("mengirim sms....");
    delay(50);
    sim800L.print("AT+CMGF=1\r");
    delay(1000);
    sim800L.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800L.print("Gas Terdeteksi");
    delay(100);
    sim800L.write(0x1A);
    delay(5000);
}
