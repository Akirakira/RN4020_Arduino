#include <SoftwareSerial.h>

SoftwareSerial rn4020(2, 3);

struct commandTable {
  char comm[50];
  int len;
};

struct commandTable echo = {"+",1};
struct commandTable reboot = {"R,1",3};
struct commandTable list = {"LS",2};
struct commandTable advertise = {"A",1};
struct commandTable rssi = {"M",1};
struct commandTable stopAdv = {"Y",1};
struct commandTable serviceRole = {"SS,40000001",11}; //Battery + private
struct commandTable battService = {"SUW,2A19,32",11};
struct commandTable devName = {"S-,NR4020",9};
struct commandTable priService = {"PS,cb9cf8fba8104baf9ad8da094ffd166c",35};
struct commandTable priCharact = {"PC,B6F0FB56216441CFBC5867AE76320782,02,04",41}; //readOnly : 4byte Length
struct commandTable priClear = {"PZ",2};
struct commandTable reset = {"SF,2",4};
struct commandTable baudRate = {"SB,1",4}; //baud rate 9600

char recvChar;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(100);
  
  /* First Setting */
    rn4020.begin(115200);
    delay(500);
    sendRN4020(baudRate.comm, baudRate.len);
    delay(100);
    sendRN4020(reboot.comm, reboot.len);
    delay(2000);
    rn4020.begin(9600);
    delay(100);
  /* First Setting */
            
  rn4020.begin(9600);
  delay(100);
  
  pinMode(4,OUTPUT);  //WAKE_SW
  pinMode(5,OUTPUT);  //CMD
  pinMode(6,OUTPUT);  //WAKE_HW
  pinMode(A0,INPUT);

  digitalWrite(4,HIGH); //Active mode
  digitalWrite(5,LOW);  //MLDP Disable
  digitalWrite(6,HIGH); //Wake up
}

void loop() {
  // put your main code here, to run repeatedly:
  while
  (Serial.available()){
      recvChar = Serial.read(); 
      switch (recvChar){
        case '1':
            Serial.println("◆send : + -- Echo On/Off");
            sendRN4020(echo.comm, echo.len);
            break;
        case '2':
            Serial.println("◆send : S- -- Device Name Setting");
            sendRN4020(devName.comm, devName.len);
            delay(100);
            sendRN4020(reboot.comm, reboot.len);
            break;
        case '3':
            Serial.println("◆send : SUW,2A19 -- Battery Service Charact Setting");
            sendRN4020(battService.comm, battService.len);
            delay(100);
            sendRN4020(reboot.comm, reboot.len);
            break;
        case '4':
            Serial.println("◆send : LS -- Display Service List");
            sendRN4020(list.comm, list.len);
            break;
        case '5':
            Serial.println("◆send : SS -- Server Role Setting");
            sendRN4020(serviceRole.comm, serviceRole.len);
            delay(100);
            sendRN4020(reboot.comm, reboot.len);
            break;
        case '6':
            Serial.println("◆send : PS,PC -- Private Service/Charact Setting");
            sendRN4020(priService.comm, priService.len);
            delay(500);
            sendRN4020(priCharact.comm, priCharact.len);
            delay(500);
            sendRN4020(reboot.comm, reboot.len);
            delay(2000);
            break;
        case '7':
            Serial.println("◆send : SF -- Factory Reset");
            sendRN4020(reset.comm, reset.len);
            delay(300);
            sendRN4020(reboot.comm, reboot.len);
            delay(1000);
            read();
            delay(1000);
            rn4020.begin(115200);
            delay(300);
            sendRN4020(baudRate.comm, baudRate.len);
            delay(300);
            sendRN4020(reboot.comm, reboot.len);
            rn4020.begin(9600);
            break;
        case '8':
            Serial.println("◆send : R,1 -- Reboot");
            sendRN4020(reboot.comm, reboot.len);
            break;
        case '9':
            Serial.println("◆send : A -- Advertize");
            sendRN4020(advertise.comm, advertise.len);
            break;
        case 'A':
            Serial.println("◆send : Y -- Stop Advertize");
            sendRN4020(stopAdv.comm, stopAdv.len);
            break;
        case'B':
            Serial.println("◆send : PZ -- Private Service Clear");
            sendRN4020(priClear.comm, priClear.len);
            delay(500);
            break;
      }
  }
  while(rn4020.available()){
    Serial.print((char)rn4020.read());
  }
}

void read(){
  while(rn4020.available()){
    Serial.print((char)rn4020.read());
  }
}

void sendRN4020(char *p,int l){
  for(int i=0;i<l;i++){
    rn4020.write(p[i]);
    delay(20);
  }
  rn4020.write('\n');
}

