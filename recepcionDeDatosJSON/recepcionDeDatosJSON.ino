#include "WiFi.h"
#include "AsyncUDP.h"
#include <ArduinoJson.h>
#include <M5Stack.h>
const char * ssid = "Grupo8";
const char * password = "123456789";
char texto[200]; //array para recibir los datos como texto
int hora;
boolean rec = 0;
AsyncUDP udp;
int ActualUID = 0;
String mander="La id de la bici es:";

void setup()
{
  M5.begin();
  Serial.begin(9600);
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
      int i = 200;
      while (i--) {
        *(texto + i) = *(packet.data() + i);
      }
      rec = 1; //recepcion de un mensaje
    });
  }
}
void loop()
{
  
  if (rec) {
    rec = 0;
    udp.broadcastTo("Recibido", 8888); //Confirmación
    udp.broadcastTo(texto, 8888); //reenvía lo recibido
    hora = atol(texto); //paso de texto a int
    
  
    StaticJsonDocument<200> jsonBufferRecv; //definición buffer para almacenar el objeto JSON, 200 máximo
    DeserializationError error = deserializeJson(jsonBufferRecv, texto); //paso de texto a formato JSON
    if (error)
      return;
    serializeJson(jsonBufferRecv, Serial); //envío por el puerto serie el objeto "jsonBufferRecv"
    Serial.println(); //nueva línea
    int segundo = jsonBufferRecv ["segundo"]; //extraigo el dato "Segundo" del objeto " jsonBufferRecv " y lo
    //almaceno en la variable "segundo"
    //Serial.println(segundo); //envío por el puerto serie la variable segundo
     ActualUID= jsonBufferRecv ["ActualUID"]; //extraigo el dato "Segundo" del objeto " jsonBufferRecv " y lo
    //almaceno en la variable "segundo"
    //Serial.println(ActualUID); //envío por el puerto serie la variable segundo
    Serial.print(Serial.available());
  delay(1000);
  Serial.println(mander + ActualUID);
  if (Serial.available() >= 0) {
      char command = Serial.read();
     
      switch (command) {
        case 'H': Serial.println("Hola Mundo");
          break;
        case 'I': Serial.println(ActualUID);
          break;
      }
    } Serial.flush(); 
  }
  
}
