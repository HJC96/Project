#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <ESP8266HTTPClient.h>

#define D0 16
#define D1 5

const char* ssid = "Sirlab";
const char* password =  "sirlab2020";
const char* mymqtt_client_id = "pibo";
const char* mqtt_topic = "pibo";
const char* mqttServer = "192.168.0.39";
 
char message_buff[100]; //initialize storage buffer
WiFiClient wifiClient; //클라이언트로 작동

int MotorPin = D0; //GPIO 16
int i = 0;
int reservationT = 0;
int presentT = 1;

void callback(char *mqtt_topic, byte *payload, unsigned int length){
  int i = 0;
  Serial.println("Message arrived: topic: " + String(mqtt_topic));
  Serial.println("Length: "+ String(length,DEC));
  //create character buffer with ending null terminator (string)
  for(i=0; i<length; i++){
    message_buff[i] = payload[i];
  }
  message_buff[i]= '\0';
  String msgString = String(message_buff);
  Serial.println("Payload: "+ msgString);
//전송된 메시지가 "LED"이면 LED를 받을 때마다 켜고 끈다.(토글)
  int state = digitalRead(MotorPin); 
  if (msgString == "Light/ON"){
    digitalWrite(MotorPin, HIGH);
    Serial.println("Light ON");
  }
  else if (msgString == "Light/OFF"){
    digitalWrite(MotorPin, LOW);
    Serial.println("Light OFF");

  }
}
PubSubClient client(mqttServer, 1883, callback, wifiClient);

void setup() {

  Serial.begin(9600);
  delay(10);
  
  pinMode(MotorPin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output 
  digitalWrite(MotorPin, LOW); //Motor off     
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP()); 
  if (client.connect(mymqtt_client_id)){
    client.publish(mqtt_topic, "Publishing message from my nodeMCU");
    client.subscribe(mqtt_topic);
  }
}
 
void loop() {
  int state = digitalRead(MotorPin); 

// state == 0(불이 꺼진 상태)일때만 동작 
  if(!state){
    if((reservationT - presentT) == 0){
     digitalWrite(MotorPin, HIGH);
     Serial.println("Light ON");
    }
  }     
  client.loop();
}
