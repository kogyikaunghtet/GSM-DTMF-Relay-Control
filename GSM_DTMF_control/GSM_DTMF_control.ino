#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial GSM(9,10);
char *phone_no = "09761081174"; //change your phone number
unsigned long currentTime;
unsigned long loopTime1;
unsigned long loopTime2;
#define relay1 5
#define relay2 4
#define relay3 3
#define relay4 2
int light1, light2, light3, light4;

void setup() {
  pinMode(relay1, OUTPUT); digitalWrite(relay1, 1);
  pinMode(relay2, OUTPUT); digitalWrite(relay2, 1);
  pinMode(relay3, OUTPUT); digitalWrite(relay3, 1);
  pinMode(relay4, OUTPUT); digitalWrite(relay4, 1);
  Serial.begin(9600);
  GSM.begin(9600);
  Serial.println("Initializing....");
  initModule("AT", "OK", 300);
  initModule("ATE0", "OK", 300);
  initModule("AT+CPIN?", "READY", 300);
  initModule("AT+CLIP=1", "OK", 300);
  initModule("AT+DDET=1", "OK", 300);
  Serial.println("Initialized Successfully");
  light1 = EEPROM.read(1);
  light2 = EEPROM.read(2);
  light3 = EEPROM.read(3);
  light4 = EEPROM.read(4);
  relays();
}

void loop() {
  currentTime = millis();
  if (currentTime >= (loopTime1 + 2000)) {
    initModule("AT", "OK", 300);
    loopTime1 = currentTime;
  }
  if (GSM.find("CLIP:")) {
    Serial.println("RING!");
    delay(200);
    while (1) {
      if (GSM.find(phone_no)) {
        initModule("ATA", "OK", 300);
        delay(1500);
        loopTime2 = currentTime;
        while (1) {
          currentTime = millis();
          if (currentTime >= (loopTime2 + 20000)) {
            Serial.println("Call End");
            loopTime2 = currentTime;
            break;
          }
          if (GSM.find("+DTMF:")) {
            int Data = GSM.parseInt();
            switch (Data) {
              case 1: {
                  light1 = !light1;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
              case 2: {
                  light2 = !light2;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
              case 3: {
                  light3 = !light3;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
              case 4: {
                  light4 = !light4;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
              case 9: {
                  light1 = 0, light2 = 0, light3 = 0, light4 = 0;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
              case 0: {
                  light1 = 1, light2 = 1, light3 = 1, light4 = 1;
                  eeprom_write();
                  relays();
                  loopTime2 = currentTime;
                  break;
                }
            }
          }
        }
      }
      else {
        initModule("AT", "OK", 300);
        break;
      }
    }
    initModule("AT+CHUP;", "OK", 300);
  }
}


void eeprom_write() {
  EEPROM.write(1, light1);
  EEPROM.write(2, light2);
  EEPROM.write(3, light3);
  EEPROM.write(4, light4);
}

void relays() {
  digitalWrite(relay1, light1);
  digitalWrite(relay2, light2);
  digitalWrite(relay3, light3);
  digitalWrite(relay4, light4);
}

void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while (GSM.available() > 0) {
      if (GSM.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}
