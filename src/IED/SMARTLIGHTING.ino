#include "Adafruit_VL53L0X.h"
#include <SoftwareSerial.h>
/******speaker ******/
//#include "SD.h"
//#define SD_ChipSelectPin 4
//#include "TMRpcm.h"
//#include "SPI.h"

#define IRsensor1 9
#define IRsensor2 10
/*"301 200" indicates that lights on "301 201" indicates that lights off */

const int pingnum=12;
SoftwareSerial mySerial(4,5);
int IRstate1=0;
int laststate1=0; 
int IRstate2=0;
int laststate2=0;
int time1;
int time2=0;
int IRflag1;
int IRflag2;
//int leave=0;//the flag leaving
//int enter=0;// the flag entering
int numofpeople=0; //the number of people inside the room
int lightonflag=0;// the flag reflects that if the flag is on 
Adafruit_VL53L0X rs = Adafruit_VL53L0X();// the range sensor
int rs_range=0;// the distance between the ceiling and the object
int breaksignal;
int red;
int green;
int blue;
int redpin=6;
int bluepin=8;
int greenpin=7;
int i=0;
int pushbuttom=18;
int pushbuttom2=19;
int YES=0;
int NO=0;
int falling=0;
int sending;
char sendingdata;
int time_differ;
int startsignal;
int endsignal;
boolean clearing;

void setup() {
  
  // put your setup code here, to run once:
 // pinMode(pingnum,OUTPUT);
//  tmrpcm.speakerPin = 9;//the pin for the speaker
  Serial.begin(9600);
  mySerial.begin(9600);

Serial.print("311 200");
  if (!rs.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
   
   Serial.print("301 200");

/*  if (!SD.begin(SD_ChipSelectPin)) {
      Serial.println("SD fail");
      return;
  }*/

  
  pinMode(IRsensor1,INPUT);
  pinMode(IRsensor2,INPUT);
  pinMode(pushbuttom,INPUT);
   
  digitalWrite(IRsensor1,HIGH);//set high impedance
  digitalWrite(IRsensor2,HIGH);//set high impedance
  
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;// the variable from the  VL530L0X liabrary
  
  // put your main code here, to run repeatedly:
  
  YES=digitalRead(pushbuttom);
  LoEroom();
  numofpeople=1;
  
  while (numofpeople>0){
    sending=0;
    LoEroom();
    iflighton();
    delay(1000);
    Serial.print("num of people : ");
    Serial.print(numofpeople);
    Serial.print("\n");
    if(lightonflag==0){
       lightup();
       mySerial.print("301 200");
    }
    while(1){
       LoEroom();
       if(numofpeople==0){
        break;
       }
       //rs_range=measure.RangeMilliMeter;
       rs.rangingTest(&measure, false);
       if (measure.RangeStatus != 4) {  // phase failures have incorrect data
            Serial.print("Distance (mm): ") ;Serial.println(measure.RangeMilliMeter);
            rs_range=measure.RangeMilliMeter;
            LoEroom();
       }
      Serial.print("num of people : ");
      Serial.print(numofpeople);
      Serial.print("\n");
      
       delay(100);
       if(rs_range>150){
         break;
      }
    }
    
   
    while(rs_range>150){
       boolean nonemergency=false;
       clearing=false;
       startsignal=0;
       Serial.print("Distance (mm): ") ;Serial.println(measure.RangeMilliMeter);
       Serial.print("Are you ok \n");
       YES=digitalRead(pushbuttom);
       time1=millis();
       time2=millis();
       time_differ=time2-time1;
       while(time_differ<5000){
        time2=millis();
        time_differ=time2-time1;
        YES=digitalRead(pushbuttom);
        NO=digitalRead(pushbuttom2);
        if(YES==LOW){
          nonemergency=true;
          Serial.print("YES");
          break;
          }
        if(NO==LOW){
          
          break;
          }       
       }
       if(nonemergency==false){
          while(clearing==false){
            YES=digitalRead(pushbuttom);
            blinklight();
            Serial.print("here"); 
            checkclear(); 
            Serial.print(clearing);
            if(sending==0){
              mySerial.print("201 200");
              sending=1;     
            }
          }
       }
       mySerial.print("201 201");
       rs.rangingTest(&measure, false);
       rs_range=measure.RangeMilliMeter;
    }
    setColor(255,255,255);
  }
  if(numofpeople==0){
    lightdown();
    mySerial.print("301 201");
    delay(50);
  }
  Serial.print("nobody is in the room");
  
}

void lightup(){
  //turn on the light 
  setColor(255,255,255);
}

void lightdown(){
  //turn off the light
  setColor(0,0,0);
}

void blinklight(){
  //blink the bulb
  setColor(255,0,0);   
  delay(100);                       
  setColor(0,0,0);    
  delay(100); 
}

void setColor(int red, int green, int blue){
  // the function used to change the color the RGB
  analogWrite(redpin,red);
  analogWrite(bluepin,blue);
  analogWrite(greenpin,green);
  
}

void LoEroom(){
  //the function used to leave or enter the room
  IRstate1=digitalRead(IRsensor1);//4
  delay(10);
  IRstate2=digitalRead(IRsensor2);//5
  int happen=0;
  if((!IRstate1)&&(laststate1)&&(happen==0)){
    //outer sensor
    while(1){
    IRstate2=digitalRead(IRsensor2);//5
    if((!IRstate2)&&(laststate2)){
      numofpeople++; 
      happen=1;
      break;
      }
      laststate2=IRstate2;
    }
    }
   
  if((!IRstate2)&&(laststate2)&&(happen==0)){
     while(1){
    //inner sensor
    IRstate1=digitalRead(IRsensor1);//4
    if((!IRstate1)&&(laststate1)){
      if(numofpeople==0){
        break;
      }
      numofpeople--;
      happen=1;
      break;
      }
      laststate1=IRstate1;
     }
    }
  laststate1=IRstate1;
  laststate2=IRstate2;
  
}

int readvalue(){
  int c;
  if(mySerial.available()){
    c=mySerial.parseInt();
    return c;
  }
}

void sendvalue(char data) {  // int if need error checking -- see "Hints for Final Report" on Google Drive
   mySerial.print(data);
   }

void iflighton(){
  if((red==0)&&(green==0)&&(blue==0)){
    lightonflag=0; 
  }
  else{
    lightonflag=1;
  }
}

void checkclear(){
    
    startsignal=readvalue();
    Serial.print(startsignal);
    if(startsignal==256){
      endsignal=0;
      while(endsignal!=256){
        if (mySerial.available()){
          Serial.print(clearing);
          endsignal=readvalue();
          if (endsignal == 256) break;
          if(endsignal==128){
             clearing=true;          
            }
          }
       }
   }
}

