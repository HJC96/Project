#include <ESP8266WiFi.h>
 
const char* ssid = "Sirlab";           // your ssid
const char* password = "sirlab2020";      // your password
const char* host = "google.com";
  const int httpPort = 80;

WiFiClient client;
 
void setup() {
  Serial.begin(9600); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {        
    delay(500);
  }
}
 
void loop() {
  Serial.println(getTime());
  delay(5000);
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
