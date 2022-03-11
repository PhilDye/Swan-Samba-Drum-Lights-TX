#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <secrets.h>

// Setup the WiFi
const char* ssid = "SwanSamba";   // Enter SSID here
const char* password = WIFI_PASS; // Set in build environment
IPAddress local_ip(10,8,4,1);
IPAddress gateway(10,8,4,1);
IPAddress subnet(255,255,255,0);
const byte DNS_PORT = 53;
const byte HTTP_PORT = 80;

// Setup the servers
WebServer webServer(HTTP_PORT);
DNSServer dnsServer;



byte LEDMode = 0;



String SendHTML(byte ledMode){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica, sans-serif; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Swan Samba Drum Lights</h1>\n";
  
  ptr +="<h2>Current Mode: " + String(ledMode) + "</h2>";

  switch (ledMode)
  {
  case 1:
    ptr +="<a class=\"button button-on\" href=\"/mode1\">ON</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode2\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode3\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode4\">OFF</a>\n";
    break;

  case 2:
    ptr +="<a class=\"button button-off\" href=\"/mode1\">OFF</a>\n";
    ptr +="<a class=\"button button-on\" href=\"/mode2\">ON</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode3\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode4\">OFF</a>\n";
    break;

  case 3:
    ptr +="<a class=\"button button-off\" href=\"/mode1\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode2\">OFF</a>\n";
    ptr +="<a class=\"button button-on\" href=\"/mode3\">ON</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode4\">OFF</a>\n";
    break;

  case 4:
    ptr +="<a class=\"button button-off\" href=\"/mode1\">ON</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode2\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode3\">OFF</a>\n";
    ptr +="<a class=\"button button-on\" href=\"/mode4\">ON</a>\n";
    break;

  default:
    ptr +="<a class=\"button button-off\" href=\"/mode1\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode2\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode3\">OFF</a>\n";
    ptr +="<a class=\"button button-off\" href=\"/mode4\">OFF</a>\n";
    break;
  }
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void handle_NotFound(){
  webServer.send(404, "text/plain", "Not found");
}

void handle_root() {
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}

void handle_mode0() {
  LEDMode = 0;
  Serial.println("Setting Mode 0");
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}

void handle_mode1() {
  LEDMode = 1;
  Serial.println("Setting Mode 1");
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}

void handle_mode2() {
  LEDMode = 2;
  Serial.println("Setting Mode 2");
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}

void handle_mode3() {
  LEDMode = 3;
  Serial.println("Setting Mode 3");
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}

void handle_mode4() {
  LEDMode = 4;
  Serial.println("Setting Mode 4");
  webServer.send(200, "text/html", SendHTML(LEDMode)); 
}




void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  webServer.on("/", handle_root);
  webServer.on("/mode0", handle_mode0);
  webServer.on("/mode1", handle_mode1);
  webServer.on("/mode2", handle_mode2);
  webServer.on("/mode3", handle_mode3);
  webServer.on("/mode4", handle_mode4);
  webServer.onNotFound(handle_root);
  
    // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", local_ip);

  webServer.begin();
  Serial.println("HTTP server started");

}

void loop() {

  dnsServer.processNextRequest();
  webServer.handleClient();
}



