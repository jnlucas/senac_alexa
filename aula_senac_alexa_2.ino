#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#define RELAY_PIN_1 12
#define RELAY_PIN_2 14

#else
#include <ESP8266WiFi.h>
#define RELAY_PIN_1 D1
#define RELAY_PIN_2 D2


#endif
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "TemponiVtal"
#define WIFI_PASS "abcd1234"

#define LUZ_3 "LUZ 3"
#define LUZ_4 "LUZ 4"

#define TUDO "TUDO"




fauxmoESP fauxmo;



void wifiSetup() {

  // Define o como STA
  WiFi.mode(WIFI_STA);

  // Conecta
  Serial.printf("[WIFI] Conectado ao %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Espera
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  // Conectado
  Serial.println("conectado");
}



void setup() {

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  wifiSetup();
  
   // LED
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);

  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, LOW);


  fauxmo.createServer(true); // não é necessário, este é o valor padrão
  fauxmo.setPort(80); // Isso é necessário para dispositivos gen3


   fauxmo.enable(true);

  fauxmo.addDevice(TUDO);
  
  fauxmo.addDevice(LUZ_3);
  fauxmo.addDevice(LUZ_4);
  
  
  




   fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Retorno de chamada quando um comando da Alexa é recebido.
    // Você pode usar device_id ou device_name para escolher o elemento no qual realizar uma ação (relé, LED, ...)
    // O state é um booleano (ON / OFF) e value um número de 0 a 255 (se você disser "definir a luz da cozinha para 50%", receberá 128 aqui).

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    
    if ( (strcmp(device_name, TUDO) == 0) ) {
      Serial.println("TUDO by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);
        digitalWrite(RELAY_PIN_2, HIGH);
        
      } else {
        digitalWrite(RELAY_PIN_1, LOW);
        digitalWrite(RELAY_PIN_2, LOW);
      }
    }
    
    if ( (strcmp(device_name, LUZ_3) == 0) ) {
      Serial.println("RELAY 1 switched by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);
      } else {
        digitalWrite(RELAY_PIN_1, LOW);
      }
    }
    if ( (strcmp(device_name, LUZ_4) == 0) ) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, HIGH);
      } else {
        digitalWrite(RELAY_PIN_2, LOW);
      }
    }
  });

  

}

void loop() {

  
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }


}
