/*
 * ESP8266 NodeMCU AJAX Demo
 * Author:  Pluzhnik Victor
 * Date:    11.10.2019
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
#include "index.h" //Our HTML webpage contents with javascripts
 
#define LED 2  //On board LED
#define DEBUGPROG 0
 
//SSID and Password of your WiFi router
const char* ssid = "ASUS610";
const char* password = "12660123";
unsigned char Lamps[6] = {0,0,0,0,0,0};
 
ESP8266WebServer server(80); //Server on port 80
 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

//===============================================================
// This routine is executed when you press submit
//===============================================================
void getParams() {
 String Params = "";
 int i;
 for (i=0; i<6; i++)
  Params = Params + String(Lamps[i]) + " ";
 
 server.send(200, "text/plane", Params); //Send web page
}

void handleSetLamps() {
 unsigned char TLamps[6] ;
 int i;
 String s = "";
 String ts;
 TLamps[0] = server.arg("wc").toInt();
 TLamps[1] = server.arg("ww").toInt();
 TLamps[2] = server.arg("r").toInt();
 TLamps[3] = server.arg("g").toInt();
 TLamps[4] = server.arg("b").toInt();
 TLamps[5] = server.arg("uv").toInt();

 for (i=0; i<6; i++)
 {
  s = s + String(TLamps[i]) + " ";
  if (TLamps[i] != Lamps[i])
  {
    Lamps[i] = TLamps[i];
    if (i < 2)
    {
      ts = "s 2 " + String(i+1) + " " + String(Lamps[i]) + "\r";
      Serial.print(ts);
    } 
      else
    {
      ts = "s 1 " + String(i-1) + " " + String(Lamps[i]) + "\r";
      Serial.print(ts); 
    } 
  }
 }

#if DEBUGPROG
 Serial.print("Lamp parameters: ");
 Serial.println(s);
#endif
 
 server.send(200, "text/plane", s); //Send web page
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
#if !DEBUGPROG
  Serial.swap();
#endif
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
#if DEBUGPROG
  Serial.println("");
#endif
 
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if DEBUGPROG
    Serial.print(".");
#endif
  }
 
  //If connection successful show IP address in serial monitor
#if DEBUGPROG
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
#endif
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/setLamps", handleSetLamps);
  server.on("/getParams", getParams);
 
  server.begin();                  //Start server
#if DEBUGPROG
  Serial.println("HTTP server started");
#endif
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
