/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "DS3231.h"
#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>



#define CS_SD     53

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1
RTClib RTC;
DateTime now;
File logFile;


char tmpfilename[20] = "";
char buff[100] = "";
Adafruit_Fingerprint finger1 = Adafruit_Fingerprint(&Serial3);
Adafruit_Fingerprint finger2 = Adafruit_Fingerprint(&Serial2);

int pos = 0;

Servo servo1;
Servo servo2;


void setup()
{
  lcd.begin(16, 2);
  lcd.backlight();
  servo1.attach(2);
  servo2.attach(3);
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger1.begin(57600);
  finger2.begin(57600);

  if (finger1.verifyPassword()) {
    Serial.println("Found fingerprint 1 sensor!");
  } else {
    Serial.println("Did not find fingerprint 1 sensor :(");
    while (1) {
      delay(1);
    }
  }
  if (finger2.verifyPassword()) {
    Serial.println("Found fingerprint 2 sensor!");
  } else {
    Serial.println("Did not find fingerprint 2 sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    while (1);
  }
  else {
    Serial.println("SD initialized...");
  }

  finger1.getTemplateCount();
  finger2.getTemplateCount();

  Serial.print("Sensor 1 contains "); Serial.print(finger1.templateCount); Serial.println(" templates");
  Serial.print("Sensor 2 contains "); Serial.print(finger1.templateCount); Serial.println(" templates");

  Serial.println("Waiting for valid finger...");

  now = RTC.now();
  memset((void*)tmpfilename, 0, sizeof(tmpfilename));
  sprintf(tmpfilename, "%4.2d%.2d%.2d.csv", now.year(), now.month(), now.day());
  Serial.println("Logging file to : " + (String)tmpfilename);

}

void loop()                     // run over and over again
{
  //  Serial.println("==========FingerPrint 1=========");
  int tmpID = -1;
  tmpID = getFingerprintIDez(&finger1);
  if (tmpID != -1)
  {
    now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    

    logFile = SD.open(tmpfilename, FILE_WRITE);
    if (logFile)
    {
      Serial.println("Logging file to : " + (String)tmpfilename);
      memset((void*)buff, 0, sizeof(buff));
      sprintf(buff, "%4.2d-%.2d-%.2d,%.2d:%.2d:%.2d,MASUK,%d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), tmpID);
      Serial.println("Saving : " + (String)buff);
      logFile.println(buff);
      logFile.close();
      Serial.println("Logging Done");
    }
    else
    {
      Serial.println("SD Error...");
    }

    for (pos = 0; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(1000);
    for (pos = 120; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    Serial.println("======End of FingerPrint 1======");
  }

  //  Serial.println("==========FingerPrint 2=========");
  tmpID = getFingerprintIDez(&finger2);
  if (tmpID != -1)
  {
    now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    logFile = SD.open(tmpfilename, FILE_WRITE);
    if (logFile)
    {
      Serial.println("Logging file to : " + (String)tmpfilename);
      memset((void*)buff, 0, sizeof(buff));
      sprintf(buff, "%4.2d-%.2d-%.2d,%.2d:%.2d:%.2d,KELUAR,%d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), tmpID);
      Serial.println("Saving : " + (String)buff);
      logFile.println(buff);
      logFile.close();
      Serial.println("Logging Done");
    }
    else
    {
      Serial.println("SD Error...");
    }
    for (pos = 0; pos <= 110; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(1000);
    for (pos = 110; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    lcd.clear();
    lcd.setCursor(0, 0); //First line
    lcd.print("Terima Kasih");
    lcd.setCursor(0, 1); //Second line
    //lcd.print("*Ardumotive.com*");
    Serial.println("======End of FingerPrint 2======");
    delay(3000);
  }

    lcd.clear();
    lcd.setCursor(0, 0); //First line
    lcd.print("Silahkan Letakan ");
    lcd.setCursor(0, 1); //Second line
    lcd.print("   Sidik Jari");
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID(Adafruit_Fingerprint * finger) {
  uint8_t p = finger->getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger->image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger->fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger->fingerID);
  Serial.print(" with confidence of "); Serial.println(finger->confidence);

  return finger->fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez(Adafruit_Fingerprint * finger) {
  uint8_t p = finger->getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger->image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger->fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger->fingerID);
  Serial.print(" with confidence of "); Serial.println(finger->confidence);
  return finger->fingerID;
}
