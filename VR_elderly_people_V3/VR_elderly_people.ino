/* Emanuele Negrino - Erasmus+ - IIS Natta Demabrosis */
/* Arduino CODE VR lderly people without OLED screen */
#include <MPU6050_tockn.h>  //|
#include <Servo.h>e         //|
#include <Wire.h>           //| Libraries

#define Serv_pin1 3         //|
#define Serv_pin2 2         //|
#define Pin_Switch 6        //| Pins definitions

Servo servo1;               //|
Servo servo2;               //| Creation of Servo objects

MPU6050 mpu6050(Wire);      //| Creation of mpu6050 object

bool flag1 = 0;                               //|
bool flag2 = 0;                               //|
int VAR_spruzzo = 0;                          //| Declaration of variables for spray

bool key = 1;                                 //|
long t = 0;                                   //|
long debounce_delay = 3000;                   //|
long tempo_reset_servo = 1000;                //|
unsigned long t1 = 0;                         //|
unsigned long t2 = 0;                         //| Declaration of variable for timers

void setup() {                                // __________________ Setup function
  servo1.attach(Serv_pin1);                   //|                   
  servo2.attach(Serv_pin2);                   //| Initializzation of servo motors
  servo1.write(0);                            //  First position for servo1
  servo2.write(165);                          //  First position for servo2 (not 0 because it is mirrored)
  pinMode(Pin_Switch,INPUT);                  //  Pins initializzation
  Serial.begin(9600);                         //  Initializzation serial comunication 
  Wire.begin();                               //  Initializzation I2C comunication
  mpu6050.begin();                            //|
  mpu6050.calcGyroOffsets(false);             //| Initializzation IMU sensor MPU6050
}                                             // __________________ End Setup function

void loop() {                                                                   // __________________ Loop funcrion
  if(millis()-t1 >= tempo_reset_servo && flag1==1){                             //| If the servo reset time has elapsed and flag1 is 1:
    servo1.write(0);                                                            //| Set servo1 to the normal pos
    flag1=0;                                                                    //| Set flag1 to 0
  }                                                                             //|
  if(millis()-t2 >= tempo_reset_servo && flag2==1){                             //| If the servo reset time has elapsed and flag2 is 1:
    servo2.write(165);                                                          //| Set servo2 to the normal pos
    flag2=0;                                                                    //| Set flag2 to 0
  }                                                                             //|
  mpu6050.update();                                                             //  Update function for IMU sensor
  Serial.print("angleX : ");                                                    //|
  Serial.print(-(int)mpu6050.getAngleX());                                      //|   
  Serial.print("\tangleY : ");                                                  //|
  Serial.print(-(int)mpu6050.getAngleY());                                      //|       
  Serial.print("\tangleZ : ");                                                  //|
  Serial.println(-(int)mpu6050.getAngleZ());                                    //| Print on the serial monitor the angles from IMU sensor
  if(digitalRead(Pin_Switch)){                                                  //| If the main switch is on:
    if(-(int)mpu6050.getAngleZ()>5 && -(int)mpu6050.getAngleZ()<100){           //| | If the Z angle is between 5 and 100 degrees:
      if(-(int)mpu6050.getAngleY()>-30 && -(int)mpu6050.getAngleY()<30){        //| | | If the Y angle is between -30 and 30 degrees:
        if(key){                                                                //| | | | If key is 1:
          t = millis();                                                         //| | | | Set t to the current time
          key=0;                                                                //| | | | Set key to 0
        }                                                                       //| | | |
        if ( (millis() - t) > debounce_delay) {                                 //| | | | If the debounce time has elapsed:
          Serial.println("Q1");                                                 //| | | | Print on serial monitor "Q1"
          if(VAR_spruzzo<3){                                                    //| | | | | If the spray counter is less than 3:
            Spray1();                                                           //| | | | | Run Spray1 function
          }                                                                     //| | | | |
          key=1;                                                                //| | | | Set key to 1
        }                                                                       //| | | |
      }else{                                                                    //| | | Otherwise [angleY<-30 or angleY>30]:
        key=1;                                                                  //| | | Set key to 1
      }                                                                         //| | |
    }else if(-(int)mpu6050.getAngleZ()>-100 && -(int)mpu6050.getAngleZ()<-5){   //| | Otherwise if Z angle is between -100 and -5 degrees:
      if(-(int)mpu6050.getAngleY()>-30 && -(int)mpu6050.getAngleY()<30){        //| | | If the Y angle is between -30 and 30 degrees:
        if(key){                                                                //| | | | If key is 1:
          t = millis();                                                         //| | | | Set t to the current time
          key=0;                                                                //| | | | Set key to 0
        }                                                                       //| | | |
        if ( (millis() - t) > debounce_delay) {                                 //| | | | If the debounce time has elapsed:
          Serial.println("Q2");                                                 //| | | | Print on serial monitor "Q2"
          if(VAR_spruzzo<3){                                                    //| | | | | If the spray counter is less than 3:
            Spray2();                                                           //| | | | | Run Spray2 function
          }                                                                     //| | | | |
          key=1;                                                                //| | | | Set key to 1
        }                                                                       //| | | |
      }else{                                                                    //| | | Otherwise [angleY<-30 or angleY>30]:
        key=1;                                                                  //| | | Set key to 1
      }                                                                         //| | |
    }else{                                                                      //| | Otherwise [-5<angleZ<5 or angleZ<-100 or angleZ>100]:
      VAR_spruzzo=0;                                                            //| | Set spray counter to 0
      key=1;                                                                    //| | Set key to 1
    }                                                                           //| |
  }                                                                             //|
}                                                                               // __________________ End Loop function

void Spray1(){            //| __________________ Spray1 function
  VAR_spruzzo++;          //| Increase the spray counter by one
  servo1.write(165);      //| Move servo1 to 165 degrees
  t1=millis();            //| Set t1 to the current time
  flag1=1;                //| Set flag1 to 1
}                         //| __________________ End Spray1 function

void Spray2(){            //| __________________ Spray2 function
  VAR_spruzzo++;          //| Increase the spray counter by one
  servo2.write(0);        //| Move servo2 to 0 degrees (not 165 because it is mirrored)
  t2=millis();            //| Set t2 to the current time
  flag2=1;                //| Set flag2 to 1
}                         //| __________________ End Spray2 function
