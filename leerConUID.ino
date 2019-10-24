#define BLANCO 0XFFFF
#define NEGRO 0
#define ROJO 0xF800
#define VERDE 0x07E0
#define AZUL 0x001F
#include <SPI.h>
#include <MFRC522.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>
#include "AsyncUDP.h"
#include <TimeLib.h>
#include <ArduinoJson.h>
const char* ntpServer = "hora.rediris.es";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char * ssid = "Grupo8";
const char * password = "123456789";
AsyncUDP udp;
StaticJsonDocument<200> jsonBuffer; //tamaño maximo de los datos


#define RST_PIN 2 //Pin 9 para el reset del RC522 no es necesario conctarlo
#define SS_PIN 21 //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
MFRC522::StatusCode status; //variable to get card status
void setup() {
  M5.begin();
  setTime (17, 44, 0, 23, 10, 2019); //hora minuto segundo dia mes año
  M5.Lcd.setTextSize(2);
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin(); //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
    if (udp.listen(8888)) {
      Serial.print("UDP Listening on IP: ");
      Serial.println(WiFi.localIP());
      udp.onPacket([](AsyncUDPPacket packet) {
        Serial.write(packet.data(), packet.length());
        Serial.println();
      });
    }
     configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
     printLocalTime();
    Serial.println("PASE BICICLETA");
    M5.Lcd.setCursor(30, 10);
    M5.Lcd.setTextColor(BLANCO);
    M5.Lcd.println("PASE BICICLETA");
  }
  byte ActualUID[7]; //almacenará el código del Tag leído
  byte Bicicleta1[7] = {0x04, 0x4D, 0x49, 0x22, 0xEE, 0x64, 0x80} ; //código del usuario 1
  byte Bicicleta2[7] = {0x04, 0x18, 0xBE, 0x5A, 0x51, 0x59, 0x81} ; //código del usuario 2
  byte Bicicleta3[7] = {0x04, 0x55, 0x49, 0x22, 0xEE, 0x64, 0x80} ; //código del usuario 3
  byte Bicicleta4[7] = {0x04, 0x59, 0x49, 0x22, 0xEE, 0x64, 0x80} ; //código del usuario 4
  byte Bicicleta5[7] = {0x04, 0x5D, 0x49, 0x22, 0xEE, 0x64, 0x80} ; //código del usuario 5
  void loop() {
    // Revisamos si hay nuevas tarjetas presentes
    if ( mfrc522.PICC_IsNewCardPresent())
    {
      //Seleccionamos una tarjeta
      if ( mfrc522.PICC_ReadCardSerial())
      {
        // Enviamos serialemente su UID
        Serial.println();
        Serial.print(F("BICICLETA:"));
        M5.Lcd.setCursor(0, 30);
        M5.Lcd.fillScreen(NEGRO);
        M5.Lcd.setTextColor(AZUL);
        M5.Lcd.print(F("BICICLETA:"));
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          M5.Lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          M5.Lcd.print(mfrc522.uid.uidByte[i], HEX);
          ActualUID[i] = mfrc522.uid.uidByte[i];
        }
        Serial.print(" ");
        M5.Lcd.print(" ");
        //comparamos los UID para determinar si es uno de nuestros usuarios
        if (compareArray(ActualUID, Bicicleta1, 7))
        { Serial.println("Bicicleta 1");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(VERDE);
          M5.Lcd.println("Bicicleta 1");
          jsonBuffer["ActualUID"] = 1; // Enviamos la mac de la bicicleta

          lectura_datos();
          si();
        }
        else if (compareArray(ActualUID, Bicicleta2, 7))
        { Serial.println("Bicicleta 2");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(VERDE);
          M5.Lcd.println("Bicicleta 2");
                    jsonBuffer["ActualUID"] = 2; // Enviamos la mac de la bicicleta

          lectura_datos();
          si();
        }
        else if (compareArray(ActualUID, Bicicleta3, 7))
        { Serial.println("Bicicleta 3");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(VERDE);
          M5.Lcd.println("Bicicleta 3");
                    jsonBuffer["ActualUID"] = 3; // Enviamos la mac de la bicicleta

          lectura_datos();
          si();
        }
        else if (compareArray(ActualUID, Bicicleta4, 7))
        { Serial.println("Bicicleta 4");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(VERDE);
          M5.Lcd.println("Bicicleta 4");
                    jsonBuffer["ActualUID"] = 4; // Enviamos la mac de la bicicleta

          lectura_datos();
          si();
        }
        else if (compareArray(ActualUID, Bicicleta5, 7))
        { Serial.println("Bicicleta 5");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(VERDE);
          M5.Lcd.println("Bicicleta 5");
          jsonBuffer["ActualUID"] = 5; // Enviamos la mac de la bicicleta

          lectura_datos();
          si();
        }
        else
        { Serial.println("?????");
          M5.Lcd.setCursor(0, 60);
          M5.Lcd.setTextColor(ROJO);
          M5.Lcd.println("?????");
          no();
        }
        char texto[600];
        jsonBuffer["Hora"] = hour(); //Datos introducidos en el objeto “jsonbuffer"
        jsonBuffer["Minuto"] = minute(); //3 campos
        jsonBuffer["Segundo"] = second(); //

        serializeJson(jsonBuffer, texto); //paso del objeto “jsonbuffer" a texto para
        //transmitirlo
        udp.broadcastTo(texto, 8888); //se envía por el puerto 1234 el JSON
        //como texto
        delay(2000);

        // Terminamos la lectura de la tarjeta tarjeta actual
        mfrc522.PICC_HaltA();
        M5.Lcd.setCursor(30, 140);
        M5.Lcd.setTextColor(BLANCO);
        M5.Lcd.println("PASE OTRA BICICLETA");
      }
    }
  }
  //Función para comparar dos vectores
  boolean compareArray(byte array1[], byte array2[], int n_byte)
  {
    for (int i = 0; i < n_byte; i++)
    {
      if (array1[i] != array2[i])return (false);
    }
    return (true);
  }
  void si ()
  { M5.Lcd.setTextSize(4);
    M5.Lcd.setCursor(150, 90);
    M5.Lcd.setTextColor(VERDE);
    M5.Lcd.println("SI");
    M5.Lcd.setTextSize(2);
  }
  void no ()
  { M5.Lcd.setTextSize(4);
    M5.Lcd.setCursor(150, 90);
    M5.Lcd.setTextColor(ROJO);
    M5.Lcd.println("NO");
    M5.Lcd.setTextSize(2);
  }
  void lectura_datos()
  {
    byte buffer_1[18]; //buffer intermedio para leer 16 bytes
    byte buffer[66]; //data transfer buffer (64+2 bytes data+CRC)
    byte tam = sizeof(buffer);
    byte tam1 = sizeof(buffer_1);
    uint8_t pageAddr = 0x06; //In this example we will write/read 64 bytes (page 6,7,8 hasta la 21).
    //Ultraligth mem = 16 pages. 4 bytes per page.
    //Pages 0 to 4 are for special functions.
    // Read data ***************************************************
    //En esta función los datos se leen de 16 bytes en 16 y se almacenan en buffer_1 (de 16+2 bytes)
    //para despues transferirlos a buffer que tiene un tamaño mayor
    //Serial.println(F("Reading data ... "));
    for (int i = 0; i < (tam - 2) / 16; i++)
    {
      //data in 4 block is readed at once 4 bloques de 4 bytes total 16 bytes en cada lectura.
      status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr + i * 4, buffer_1, &tam1);
      // if (status != MFRC522::STATUS_OK) {
      // Serial.print(F("MIFARE_Read() failed: "));
      // Serial.println(mfrc522.GetStatusCodeName(status));
      // return;
      // }
      //copio los datos leidos en buffer_1 a la posición correspondiente del buffer
      for (int j = 0; j < 16; j++)
      {
        buffer[j + i * 16] = buffer_1[j];
      }
    }
    //Presentacion de los datos ledidos por el puerto serie y por el M5Stack
    Serial.print(F("Readed data: "));
    //Dump a byte array to Serial
    for (byte i = 0; i < (tam - 2); i++) {
      Serial.write(buffer[i]);
    }
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 160);
    M5.Lcd.setTextColor(VERDE);
    for (byte i = 0; i < (tam - 2); i++) {
      M5.Lcd.print((char)buffer[i]);
    }
  }

  void printLocalTime()
  {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      M5.Lcd.printf("Failed to obtain time");
      return;
    }
    M5.Lcd.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    M5.Lcd.println(&timeinfo, "%H:%M:%S");
    
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.println(&timeinfo, "%H:%M:%S");
    int Hora = timeinfo.tm_hour;
    M5.Lcd.println(Hora);
  }
