#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial esp8266(3 , 2);
const int d0 = 13;
const int d1 = 12;
const int d2 = 11;
const int d3 = 10;
const int VT = 8;
const int buzzer = 6;
unsigned int x = 0;
unsigned int data_present;
int VT_status = 0;
int data0 = 0;
int data1 = 0;
int data2 = 0;
int data3 = 0;

#define ATE  "ATE0\r\n"
#define ok "OK"
#define pinstr "pin="
#define err "Error"
#define InvalidUserIdOrPassword 18
#define InvalidData 45
#define HitLimitExceed 104
#define VALID 24
#define error18 "18"
#define error45 "45"
#define error104 "104"
#define CIPSEND "AT+CIPSEND=4,"

#define CWJAP  "AT+CWJAP=\"iot\",\"project1234\"\r\n"

bool mode_status;
String dataOne = "SOME%20PROBLEM";
String dataTwo = "Call%20Attendant";
String dataThree = "I%20Need%20food";
String dataFour = "I%20Need%20water";
String dataFive = "No%20Input";
bool pump_value = 1;
unsigned int flame_value;
unsigned int flame_value1;
int err_index;
String finallinktoSenddata;
String data;
String linkTogetsiteResponse = "";
String linkTosendData = "";
String lenghtOflinstr;
const String id = "srikarkuchana8@gmail.com"; //     iotgecko login id  //jebichrise8@gmail.com   -> paaswrd 2833
const String pass = "3517"; // iotgecko login password    //arupmaxwell333@gmail.com   ->  passwrd   1132
String ssid = "iot"; // SSID/name of your wifi router or wifi hotspot
String pass_key = "project1234"; // Wifi password
String CIPSTART = "AT+CIPSTART=4,\"TCP\",\"www.iotgecko.com\",80\r\n";
String Host = " HTTP/1.1\r\nHost: www.iotgecko.com\r\n\r\n";
bool notConected = true;
bool login = false;
const int no_of_senddata = 1;
const int no_of_getdata = 0;
String iot_data[no_of_senddata];
char motor_status;
char motor_status1;
unsigned long current_time ;

int pwm_val = 0;
char  buff[15];
bool iot_control = true;
bool data_available;
int data1_no = 0;
int data2_no = 0;
int data3_no = 0;
int data4_no = 0;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //RS, EN, D4, D5, D6, D7

void setup() 
{
  // put your setup code here, to run once:
  esp8266.begin(9600);
  Serial.begin(9600);

  pinMode(VT, INPUT );
  pinMode(d0, INPUT );
  pinMode(d1, INPUT );
  pinMode(d2, INPUT );
  pinMode(d3, INPUT );
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("IOT Automated");
  lcd.setCursor(0, 1);
  lcd.print("Paralysis System");
  delay(5000);
  esp8266.listen();
  init_wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  VT_status = digitalRead(VT);
  data0 = digitalRead(d0);
  data1 = digitalRead(d1);
  data2 = digitalRead(d2);
  data3 = digitalRead(d3);
  rf_process();
}

void rf_process()
{
  if ((VT_status == 1) && (data0 == 1) && (data1 == 0) && (data2 == 0) && (data3 == 0))
  {
    lcd.clear();
    lcd.print("SOME PROBLEM");
   
    tone_buzzer(4, 500);
    
    unsigned long int past_time = millis();
    do
    {
      unsigned long int present_time = millis();
      tone_buzzer(3,200);
    if((present_time -past_time)>5000)
    {
      lcd.clear();
      lcd.print("Sending Data to ");
      lcd.setCursor(6, 1);
      lcd.print("IOT");
      delay(1000);
      iot_data[0] = dataOne;
      Serial.print("data sent:");
      Serial.println(iot_data[0]);
      while ( hit_link() == 0);
      break;
    }
    }
    while(digitalRead(d0));
    delay(100);
  }
  else if ((VT_status == 1) && (data0 == 0) && (data1 == 1) && (data2 == 0) && (data3 == 0))
  {
    lcd.clear();
    lcd.print("Call attendant");
   
    tone_buzzer(4, 500);
     unsigned long int past_time = millis();
    do
    {
      unsigned long int present_time = millis();
      tone_buzzer(3,200);
      if((present_time -past_time)>5000)
    {
      lcd.clear();
      lcd.print("Sending Data to ");
      lcd.setCursor(6, 1);
      lcd.print("IOT");
      delay(1000);
      iot_data[0] = dataTwo;
      Serial.print("data sent:");
      Serial.println(iot_data[0]);
      while ( hit_link() == 0);
      break;
    }
   }
   while(digitalRead(d1));
   delay(100);
  }
  else if ((VT_status == 1) && (data0 == 0) && (data1 == 0) && (data2 == 1) && (data3 == 0))
  {
    lcd.clear();
    lcd.print("I Need Food");
    tone_buzzer(4, 500);
   
     unsigned long int past_time = millis();
    do
    {
      unsigned long int present_time = millis();
      tone_buzzer(3,200);
    if((present_time -past_time)>5000)  
    {
      lcd.clear();
      lcd.print("Sending Data to ");
      lcd.setCursor(6, 1);
      lcd.print("IOT");
      delay(1000);
      iot_data[0] = dataThree;
      Serial.print("data sent:");
      Serial.println(iot_data[0]);
      while ( hit_link() == 0);
     
      break;
    }
    }
    while(digitalRead(d2));
        delay(100);
  }
  else if ((VT_status == 1) && (data0 == 0) && (data1 == 0) && (data2 == 0) && (data3 == 1))
  {
    lcd.clear();
    lcd.print("I Need Water");
   
    tone_buzzer(4, 500);
   
     unsigned long int past_time = millis();
    do
    {
      unsigned long int present_time = millis();
      tone_buzzer(3,200);
     if((present_time -past_time)>5000)  
    {
      lcd.clear();
      lcd.print("Sending Data to ");
      lcd.setCursor(6, 1);
      lcd.print("IOT");
      delay(1000);
      iot_data[0] = dataFour;
      Serial.print("data sent:");
      Serial.println(iot_data[0]);
      while ( hit_link() == 0);
      
      break;
    }
    }
    while(digitalRead(d3));
        delay(100);
  }

  else if ( (data0 == 0) && (data1 == 0) && (data2 == 0) && (data3 == 0))
  {
    lcd.clear();
    lcd.print("Display Unit");
    lcd.setCursor(0, 1);
    lcd.print("No Input");
    unsigned long int past_time = millis();
    do
    {
     unsigned long int present_time = millis();
     if((present_time -past_time)>5000) 
    {
      iot_data[0] = dataFive;
      Serial.print("data sent:");
      Serial.println(iot_data[0]);
      while ( hit_link() == 0);     
      break;
    }
      
    }while((!(digitalRead(d0))&&!(digitalRead(d1))&&!(digitalRead(d2))&&!(digitalRead(d3))));

    delay(200);
  }
}

void tone_buzzer(int i, int d)
{
  for (int j = 0; j < i; j++)
  {
    digitalWrite(buzzer, HIGH);
    delay(d);
    analogWrite(buzzer, LOW);
    delay(50);
  }
}
