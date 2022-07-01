#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <secrets.h>
#include <markup.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Setup the network
const char* ssid = "SwanSamba";   // Enter SSID here
const char* password = WIFI_PASS; // Set in build environment
const byte DNS_PORT = 53;
const byte HTTP_PORT = 80;

// Setup the servers
AsyncWebServer webServer(HTTP_PORT);
DNSServer dnsServer;

// Setup the nRF24L01 radio
#define NRF24L01_PIN_CE	17
#define NRF24L01_PIN_CS	5
RF24 radio(NRF24L01_PIN_CE, NRF24L01_PIN_CS);
const byte address[6] = "00001";


int8_t LEDMode = 0;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");

    response->printf(html_head);
    response->print("<body>");
    response->print("<h1>Swan Samba LED Control</h1>");
    response->printf("<p style='color:white'>Open <a href='http://%s' style='color:white'>LED Control</a></p>", WiFi.softAPIP().toString().c_str());
    response->print("</body></html>");
    request->send(response);
  }
};

String SendHTML(byte ledMode){

  String page = String(html_head);
  page +="<body>";
  page +="<h1>Swan Samba LED Control</h1>";
  // page +="<h2>Current Mode: " + String(ledMode) + "</h2>";

  switch (ledMode)
  {
  case 1:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 2:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 3:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 4:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 5:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 6:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 7:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 8:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 9:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 10:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 11:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">Off</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
    break;

  case 12:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-on\" href=\"/mode?mode=0\">OFF</a>";
    page +="</div>";
    break;

  default:
    page +="<div class=\"col\">";
    page +="<h3>Solid</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=1\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=2\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=3\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=4\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Chase</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=5\">Green</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=6\">Yellow</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=7\">Blue</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=8\">Red</a>";
    page +="</div>";
    page +="<div class=\"col\">";
    page +="<h3>Effects</h3>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=9\">Spin</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=10\">Bounce</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=11\">Disco</a>";
    page +="<a class=\"button button-off\" href=\"/mode?mode=12\">Flag</a>";
    page +="</div>";
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
  Serial.begin(115200);
   while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  radio.openWritingPipe(address);
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  radio.stopListening();          // put radio in TX mode

  radio.printPrettyDetails(); // (larger) function that prints human readable data


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

    radio.write(&LEDMode, sizeof(mode));

    request->send(200, "text/html", SendHTML(LEDMode));
  });

  webServer.onNotFound(notFound);
    
  // Setup a captive portal, responding to all DNS requests with the ESP's IP
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.println("DNS server started");
 
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  //only when requested from AP

  webServer.begin();
  Serial.println("HTTP server started on " + WiFi.softAPIP().toString());

}

void loop() {
  dnsServer.processNextRequest();
}



