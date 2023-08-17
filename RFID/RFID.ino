#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>

#define SS_PIN 5
#define RST 27

MFRC522 rfid(SS_PIN, RST);
WiFiClient rfidClient;
PubSubClient client(rfidClient);
const char* ssid = "PonysITM";
const char* password = "";
const char* mqtt_server = "10.31.0.249";

char msg[16];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if(!client.connected()){
    reconnect();
  }

  client.loop();

  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
        msg[i] = rfid.uid.uidByte[i];
      }
      Serial.println();

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      client.publish("tag", msg);
    }
  }
}

void setup_wifi(){
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  Serial.print("Trying MQTT conection...");
  if(client.connect("RFID")){
    Serial.println("Conexión exitosa con el broker");
  }
  else{
    Serial.println("Conexión fallida");
    Serial.print("Fallo, rc=");
    Serial.println(client.state());
  }
}
