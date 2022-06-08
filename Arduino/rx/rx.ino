#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX from motors, TX for motors
uint8_t _return_packet[7];     // Packet structure in which we save recieved packets from the motor
uint8_t MOTOR_ID_ADDRESS= 2;
uint8_t MOTOR_ANGLE= 13;
uint8_t TORQUE= 11;
uint8_t PRESENT_ANGLE= 16;

void setup(){
  Serial.begin(57600);   // Used Serial to be able to see all the packets sent and recieved from the motors
  mySerial.begin(57600); // Used Software Serial to be able to transmit and recieve commands to motors
  Serial.println("Identification: Source"); 
}

void loop(){
  //Serial.println(_reset());        // This command resets the motor id
  //_write(1,MOTOR_ID_ADDRESS,2);    // This command sets id of the motor from 1 to 2. 
  //Serial.println(_ping(2));        // This example is for checking motor id 2 connected or not.
  //_write(2,MOTOR_ANGLE,90);        // This example will command motor #2 to move to 90. (Min-0 Max-180)
  //_write(2,TORQUE,1);              // This example will enable torque on motor #2
  //_write(2,TORQUE,1);              // This example will disable torque on motor #2
  //Serial.println(_read(2,PRESENT_ANGLE));     // This example reads the present #2 id motor angle 
  //byte MotorIds[]={1,2,3};
  //byte MotorAngles[]={90,90,90};
  //byte No_of_Motors=3;
  //_syncwrite(MotorIds,  MotorAngles,  No_of_Motors); // This example is for set angle for servo motor ids 1,2,3 in sync mode
}

byte _read(uint8_t MotorID, uint8_t parameter){
  uint8_t arrays[]={255,253,2,4,2,MotorID,parameter,251};
  mySerial.write(arrays,sizeof(arrays));
  delay(20);
  if(mySerial.available() > 0){
        mySerial.readBytes(_return_packet,7);
  }
  delay(80);
  return _return_packet[4];
}

boolean _reset(){
  uint8_t arrays[]={255,253,5};
  mySerial.write(arrays,sizeof(arrays));
  delay(20);
  if(mySerial.available() > 0){
        mySerial.readBytes(_return_packet,7);
  }
  delay(80);
  if(_return_packet[5] > 0){
    return true;
  }
  else{
    return false;
  }
}

boolean _ping(uint8_t motorid){
  uint8_t arrays[]={255,253,1,2,motorid,251};
  mySerial.write(arrays,sizeof(arrays));
  delay(20);
  if(mySerial.available() >0){
        mySerial.readBytes(_return_packet,7);
  }
  delay(80);
  if(_return_packet[2] > 0){
    return true;
  }
  else{
    return false;
  }
}

void _write(uint8_t motorid,uint8_t address, uint8_t value){
  uint8_t arrays[]={255,253,1,6,3,motorid,address,value,0,251};
  mySerial.write(arrays,sizeof(arrays));
  delay(100);
}

void _syncwrite(uint8_t motorids[],uint8_t motor_angless[],uint8_t idsize){
    uint8_t arrays[4+1+(idsize*4)+1];
    uint8_t p;
    uint8_t L_motor_angless=0;
    uint8_t R_motor_angless=0;
    
    arrays[0]=255;
    arrays[1]=253;
    arrays[2]=4;
    arrays[3]=(1+4*idsize+1);
    arrays[4]=4;
    for(p=0;p<idsize;p=p+1){
      arrays[5+(p*4)]=motorids[p];
      arrays[5+(p*4)+1]=1;
      
      if(motor_angless[p] > 127){
        L_motor_angless=127;
        R_motor_angless=motor_angless[p]-127;
      }
      else{
        L_motor_angless=motor_angless[p];
        R_motor_angless=0;
      }
      arrays[5+(p*4)+2]=L_motor_angless;
      arrays[5+(p*4)+3]=R_motor_angless;
    }
     arrays[sizeof(arrays)-1]=251;
     mySerial.write(arrays,sizeof(arrays));
     delay(100);
}
