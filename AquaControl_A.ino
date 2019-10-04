/*
 * ESP8266 NodeMCU AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
#include "index.h" //Our HTML webpage contents with javascripts
 
#define LED 2  //On board LED
#define DEBUGPROG 1
 
//SSID and Password of your WiFi router
const char* ssid = "ASUS610";
const char* password = "12660123";
 
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
void handleForm() {
 String firstName = server.arg("firstname"); 
 String lastName = server.arg("lastname"); 
 String lvalue = server.arg("lvalue"); 
 
 Serial.print("s 2 1 "+lvalue+"\r");

 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}

void handleSetParams() {
 String wc_percent = server.arg("WCValue"); 
 String ww_percent = server.arg("WWValue"); 
#if DEBUGPROG 
 Serial.println("In handleSetParams() function.");
#endif

 String s = "<a href='/'> Params: wc=" + wc_percent + "%; ww=" + ww_percent + "%</a>";
 server.send(200, "text/html", s); //Send web page
}
 
void handleLED() {
 String ledState = "OFF";
 String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
 //Serial.println(t_state);
 if(t_state == "1")
 {
  digitalWrite(LED,LOW); //LED ON
  ledState = "ON"; //Feedback parameter
 }
 else
 {
  digitalWrite(LED,HIGH); //LED OFF
  ledState = "OFF"; //Feedback parameter  
 }
 
 server.send(200, "text/plane", ledState); //Send web page
}

void handleSetLamps() {
 String color = server.arg("color");
 String power = server.arg("power");
 Serial.println("In handleSetLamps()...");
 if(color == "wc")
 {
   Serial.print("wc=");
   Serial.println(power);
 }
 //server.send(200, "text/plane", color); //Send web page
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
  server.on("/action_page", handleForm); //form action is handled here
  server.on("/setLED", handleLED);
  server.on("/set_params", handleSetParams);
  server.on("/setLamps", handleSetLamps);
 
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
