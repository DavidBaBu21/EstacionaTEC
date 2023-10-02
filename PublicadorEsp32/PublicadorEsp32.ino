#include <WiFi.h>
#include <PubSubClient.h>

//SENSORES INFRARROJOS
#define S1 4
#define S2 16
#define S3 17
#define S4 5
#define S5 18
#define S6 19

//WIFI, ESP - MQTT
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "SATCO-053423";
const char* password = "72474DDF";
const char* mqtt_server = "192.168.0.145";

//VARIABLES LEÍDAS Y MSJ A NODE-RED
char msg[16];
int s1, s2, s3, s4, s5, s6;

void setup() {
  //Iniciar conexión WiFi
  setup_wifi();
  delay(500);
  //Conectarse con el broker
  client.setServer(mqtt_server, 1883);
  delay(1000);
  //Iniciar pines
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
}

void loop() {
  //Verificar conexión con el broker
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  //Leer los sensores
  s1 = digitalRead(S1);
  s2 = digitalRead(S2);
  s3 = digitalRead(S3);
  s4 = digitalRead(S4);
  s5 = digitalRead(S5);
  s6 = digitalRead(S6);
  
  snprintf(msg,16,"%d,%d,%d,%d,%d,%d",s1,s2,s3,s4,s5,s6);
  //snprintf(msg,16,"%d",s1);
  client.publish("canal", msg);
  delay(3000);
}

void setup_wifi(){
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  Serial.print("Trying MQTT conection...");
  if(client.connect("ESP32")){
    Serial.println("Conexión exitosa con el broker");
  }
  else{
    Serial.println("Conexión fallida");
    Serial.print("Fallo, rc=");
    Serial.println(client.state());
  }
}
