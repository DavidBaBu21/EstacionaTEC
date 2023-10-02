#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>

//#include <Servo.h>

//PINES RFID y servo
#define SS_PIN 5
#define RST 27
#define PIN_SG90 2
Servo pluma;

//Objeto RFID, WIFI, ESP - MQTT
MFRC522 rfid(SS_PIN, RST);
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "SATCO-053423";
const char* password = "72474DDF";
const char* mqtt_server = "192.168.0.145";

//Variables leídas y msj a node-red
char msg[16];
String tag;
String lectura[16];

void setup() {
  Serial.begin(115200);
  //Iniciar conexión wifi
  setup_wifi();
  delay(500);
  //Iniciar RFID
  pluma.setPeriodHertz(50); // PWM frequency for SG90
  pluma.attach(PIN_SG90, 500, 2400); // Minimum and maximum pulse width (in µs) to go from 0° to 180
  //pluma.write(90);
  SPI.begin();
  rfid.PCD_Init();
  //Conectarse con el broker
  client.setServer(mqtt_server, 1883);
  delay(1000);
  client.setCallback(callback);
}

void loop() {
  //Verificar conexión con el broker
  if(!client.connected()){
    reconnect();
  }

  client.loop();
  //Lectura de RFID
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        lectura[i] =(String(rfid.uid.uidByte[i], HEX));
        Serial.print(lectura[i]);
      }
      Serial.println();
      tag = (lectura[0] + lectura[1] + lectura[2] + lectura[3]);
      tag.toCharArray(msg,16);
      Serial.println(msg);
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      client.publish("canal3", msg);
    }
    //Serial.println();
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
    client.subscribe("canal4");
  }
  else{
    Serial.println("Conexión fallida");
    Serial.print("Fallo, rc=");
    Serial.println(client.state());
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  payload[length]='\O';
  String val = String((char*)payload);
  Serial.println(val[0]);
  if(val[0]=='1'){
    //for (int pos = 0; pos <= 90; pos += 1) {
    pluma.write(90);
    //  delay(10);
    //}
    //delay(200);
    //pluma.detach();
      //delay(10);
    delay(7000);
    //pluma.attach(PIN_SG90, 500, 2400);
    //for (int pos = 90; pos >= 0; pos -= 1) {
    pluma.write(180);
      //delay(10);
    //}
    //delay(200);
    //pluma.detach();
     // delay(10);
    
  }
}
