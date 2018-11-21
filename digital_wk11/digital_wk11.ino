#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "IOT_LAB";
const char* password = "password";

WebServer server(80);
char webSite[1000];
void handleSensorData(){
  int sensor = analogRead(36);
  String json="{\"vr\":";
  json+=sensor;
     json+="}";
     server.send(200,"application/json",json);
}
//server.on("/", handleroot);
void handleRoot() {
  server.send(200, "text/html", webSite);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
    snprintf(webSite, 1000, "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <script src=\"https://code.jquery.com/jquery-2.2.4.min.js\"></script> <script>setInterval(requestData, 500); function requestData(){$.get(\"/sensors\") .done(function(data){if(data){$(\"#resis\").text(data.vr);}else{$(\"#resis\").text(\"?\");}}) .fail(function(){console.log(\"fail\");});}</script> <title>VR Reader</title></head><body> <div class=\"container\"> <br> <br> <br> <br> <br> <br> <br> <br> <br> <br> <h1> <h1> <h1>Variable Resistor=<span id=\"resis\"></span></h1></div></body></html>");
  
  Serial.begin(115200);
  //buildWeb();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.on("/sensors",handleSensorData);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
