#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

#define D0 16
#define D1 5

const char* ssid = "Sirlab";
const char* password =  "sirlab2020";
const char* mymqtt_client_id = "pibo";
const char* mqtt_topic = "pibo";
const char* mqttServer = "192.168.0.39";
const char* host = "google.com";
const int httpPort = 80;
bool check = true;
bool tick = true;
char message_buff[100]; //initialize storage buffer
WiFiClient wifiClient, client; //클라이언트로 작동
Servo servo;

int MotorPin = D0; //GPIO 16
int PresentT = 0;
int ReservationT = 0;

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

  int state = digitalRead(MotorPin); 
  int angle = 90;
  servo.write(90);
  if (msgString == "Light/ON"){
    for(int i = 0; i < 90; i++)
      {
        angle = angle + 1;                       
        servo.write(angle); 
        delay(10);
      }
      Serial.println("Light ON");
  }
  else if (msgString == "Light/OFF"){
    for(int i = 0; i < 90; i++)
      {
        angle = angle - 1;                       
        servo.write(angle); 
        delay(10);
      }
      Serial.println("Light OFF");
  }else {
    Serial.println(msgString);
    ReservationT = msgString.toInt();
   }
  servo.write(90);
}
PubSubClient client1(mqttServer, 1883, callback, wifiClient);

void setup() {
 
  Serial.begin(9600);
  delay(10);
  
  //pinMode(MotorPin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output 
  //digitalWrite(MotorPin, LOW); //Motor off 
  servo.attach(MotorPin);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP()); 
  if (client1.connect(mymqtt_client_id)){
    client1.publish(mqtt_topic, "Publishing message from my nodeMCU");
    client1.subscribe(mqtt_topic);
  }
  servo.write(90);
}
 
void loop() {
  int angle = 90;
  String temp1 = getTime().substring(0,2);
  String temp2 = getTime().substring(3,5);
  Serial.print("현재 시간 = ");
  Serial.print(getTime());
  Serial.println("");
  char ch1[32] = {0};
  char ch2[32] = {0};
  for(int i=0;i<2;i++){
    ch1[i]=temp1.charAt(i);
    ch2[i]=temp2.charAt(i);
  }
  String TEMP = strcat(ch1,ch2);
  PresentT = TEMP.toInt();
  
  PresentT = PresentT + 900; //  
  if(PresentT > 2359)
    PresentT - 2400;
  Serial.print("PresentT = ");
  Serial.print(PresentT);
  Serial.println("");

  if(tick){
    if(ReservationT - PresentT == 0){
      for(int i = 0; i < 90; i++)
      {
        angle = angle + 1;                       
        servo.write(angle); 
        delay(10);
      }
        Serial.println("Light ON");
        tick = false;
        servo.write(90);  
   }
  }
  else if(ReservationT - PresentT != 0)
    tick = true;
 
  client1.loop();
}


String getTime() {
  while (!client.connect(host, httpPort))
  {}
   
  // We now create a URI for the request
  String url = "/";
  //  url += streamId;
  //  url += "?private_key=";
  //  url += privateKey;
  //  url += "&value=";
  //  url += value;
   
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  while(!client.available()) {} // 서버접속 요청 후 클라이언트 검사하여 없다면 NULL을 반환
  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String timeData = client.readStringUntil('\r');
                client.stop();
                timeData = timeData.substring(17,22);
                return timeData;
              }
            }
          }
        }
      }
    }
  }
} 
