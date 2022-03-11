#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <secrets.h>

// Setup the network
const char* ssid = "SwanSamba";   // Enter SSID here
const char* password = WIFI_PASS; // Set in build environment
const byte DNS_PORT = 53;
const byte HTTP_PORT = 80;

// Setup the servers
AsyncWebServer webServer(HTTP_PORT);
DNSServer dnsServer;


const char html_head[] PROGMEM = "<!DOCTYPE html> <html>"
  "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">"
  "<title>Swan Samba LED Control</title>"
  "<style>html { font-family: Helvetica, sans-serif; display: inline-block; margin: 0px auto; text-align: center;}"
  "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}"
  "p {font-size: 14px;color: #888;margin-bottom: 10px;}"
  ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}"
  ".button-on {background-color: #3498db;}"
  ".button-on:active {background-color: #2980b9;}"
  ".button-off {background-color: #34495e;}"
  ".button-off:active {background-color: #2c3e50;}"
  "</style>"
  "</head>";

int8_t LEDMode = 0;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");

    response->printf(html_head);
    response->print("<body>");
    response->print("<h1>Swan Samba LED Control</h1>");
    response->printf("<p>Open <a href='http://%s'>LED Control</a></p>", WiFi.softAPIP().toString().c_str());
    response->print("</body></html>");
    request->send(response);
  }
};


String SendHTML(byte ledMode){

  String page = String(html_head);
  page +="<body>";
  page +="<h1>Swan Samba LED Control</h1>";
  page +="<h2>Current Mode: " + String(ledMode) + "</h2>";

  switch (ledMode)
  {
  case 1:
    page +="<a class=\"button button-on\" href=\"/mode?mode=1\">ON</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">OFF</a>";
    break;

  case 2:
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">OFF</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=2\">ON</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">OFF</a>";
    break;

  case 3:
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">OFF</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=3\">ON</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">OFF</a>";
    break;

  case 4:
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">ON</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">OFF</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=4\">ON</a>";
    break;

  default:
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">OFF</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">OFF</a>";
    break;
  }
  
  page +="</body>";
  page +="</html>";
  return page;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  delay(100);

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", SendHTML(LEDMode)); 
  });

  webServer.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request){
    String mode;
    if (request->hasParam("mode")) {
        mode = request->getParam("mode")->value();
    } else {
        request->send(400, "text/plain", "No mode sent");
    }
    LEDMode = mode.toInt();
    Serial.println("Setting Mode " + mode);
    request->send(200, "text/html", SendHTML(LEDMode)); 
  });

  webServer.onNotFound(notFound);
  
  // Setup a captive portal, responding to all DNS requests
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.println("DNS server started");
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  //only when requested from AP

  webServer.begin();
  Serial.println("HTTP server started on " + WiFi.softAPIP().toString());

}

void loop() {
  dnsServer.processNextRequest();
}



