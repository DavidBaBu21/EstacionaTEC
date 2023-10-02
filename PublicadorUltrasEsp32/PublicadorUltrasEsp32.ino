#include <WiFi.h>
#include <PubSubClient.h>

//SENSORES INFRARROJOS
#define S1_trig 2
#define S1_echo 4
#define S2_trig 16
#define S2_echo 17
#define S3_trig 12
#define S3_echo 13
#define S4_trig 27
#define S4_echo 14
#define S5_trig 25
#define S5_echo 26
#define S6_trig 32
#define S6_echo 33

//WIFI, ESP - MQTT
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "SATCO-053423";
const char* password = "72474DDF";
const char* mqtt_server = "192.168.0.145";

//VARIABLES LEÍDAS Y MSJ A NODE-RED
char msg2[16];
long duracion, distancia;
int s1, s2, s3, s4, s5, s6;

void setup() {
  //Iniciar conexión WiFi
  setup_wifi();
  delay(500);
  //Conectarse con el broker
  client.setServer(mqtt_server, 1883);
  delay(1000);
  //Iniciar pines
  pinMode(S1_trig, OUTPUT);
  pinMode(S1_echo, INPUT);
  pinMode(S2_trig, OUTPUT);
  pinMode(S2_echo, INPUT);
  pinMode(S3_trig, OUTPUT);
  pinMode(S3_echo, INPUT);
  pinMode(S4_trig, OUTPUT);
  pinMode(S4_echo, INPUT);
  pinMode(S5_trig, OUTPUT);
  pinMode(S5_echo, INPUT);
  pinMode(S6_trig, OUTPUT);
  pinMode(S6_echo, INPUT);
}

void loop() {
  //Verificar conexión con el broker
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  //Leer los sensores
  sensor1();
  sensor2();
  sensor3();
  sensor4();
  sensor5();
  sensor6();
  
  snprintf(msg2,16,"%d,%d,%d,%d,%d,%d",s1,s2,s3,s4,s5,s6);
  //snprintf(msg2,16,"%d",s1);
  //Serial.print(msg2);
  client.publish("canal2", msg2);
  delay(1000);
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
  if(client.connect("EstacionamientoSur")){
    Serial.println("Conexión exitosa con el broker");
  }
  else{
    Serial.println("Conexión fallida");
    Serial.print("Fallo, rc=");
    Serial.println(client.state());
  }
}

void sensor1(){
  digitalWrite(S1_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S1_trig, LOW);

  duracion = pulseIn(S1_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  if (distancia < 20){
    s1 = 0;
  }
  else{
    s1 = 1;
  }
}
void sensor2(){
  digitalWrite(S2_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S2_trig, LOW);

  duracion = pulseIn(S2_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  if (distancia < 20){
    s2 = 0;
  }
  else{
    s2 = 1;
  }
}
void sensor3(){
  digitalWrite(S3_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S3_trig, LOW);
   
  duracion = pulseIn(S3_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  if (distancia < 20){
    s3 = 0;
  }
  else{
    s3 = 1;
  }
}
void sensor4(){
  digitalWrite(S4_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S4_trig, LOW);
  
  duracion = pulseIn(S4_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  if (distancia < 20){
    s4 = 0;
  }
  else{
    s4 = 1;
  }
}
void sensor5(){
  digitalWrite(S5_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S5_trig, LOW);

  duracion = pulseIn(S5_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  if (distancia < 20){
    s5 = 0;
  }
  else{
    s5 = 1;
  }
}
void sensor6(){
  digitalWrite(S6_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S6_trig, LOW);

  duracion = pulseIn(S6_echo, HIGH);
  distancia = (duracion/2)/29;
  Serial.println(distancia);
  Serial.println("...");
  if (distancia < 20){
    s6 = 0;
  }
  else{
    s6 = 1;
  }
}
