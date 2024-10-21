  #include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>


const char *ssid = "GalaxyAzamat";
const char *password = "vhrg8328";


AsyncWebServer server(80);


// Define stepper motor connections and steps per revolution
#define motorStep 14
#define motorDir 12
#define motorEnable 4
AccelStepper stepper(1, motorStep, motorDir);


void setup() {
  // Set up serial communication
  Serial.begin(115200);


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());


  // Initialize stepper motor
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(2000.0);


  // Define web server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body><h1>Stepper Motor Control</h1><p><a href=\"/forward500\">Forward 500</a></p><p><a href=\"/backward500\">Backward 500</a></p><p><a href=\"/backward2000\">Backward 2000</a></p><p><a href=\"/forward2000\">Forward 2000</a></p></body></html>");
  });
  server.on("/forward500", HTTP_GET, [](AsyncWebServerRequest *request){
    stepper.move(500);
    stepper.runToPosition();
    request->send(200, "text/html", "<html><body><h1>Stepper Motor Control</h1><p><a href=\"/forward500\">Forward 500</a></p><p><a href=\"/backward500\">Backward 500</a></p><p><a href=\"/backward2000\">Backward 2000</a></p><p><a href=\"/forward2000\">Forward 2000</a></p></body></html>");
  });
  server.on("/backward500", HTTP_GET, [](AsyncWebServerRequest *request){
    stepper.move(-500);
    stepper.runToPosition();
    request->send(200, "text/html", "<html><body><h1>Stepper Motor Control</h1><p><a href=\"/forward500\">Forward 500</a></p><p><a href=\"/backward500\">Backward 500</a></p><p><a href=\"/backward2000\">Backward 2000</a></p><p><a href=\"/forward2000\">Forward 2000</a></p></body></html>");
  });
 server.on("/backward2000", HTTP_GET, [](AsyncWebServerRequest *request){
    stepper.move(-2000);
    stepper.runToPosition();
    request->send(200, "text/html", "<html><body><h1>Stepper Motor Control</h1><p><a href=\"/forward500\">Forward 500</a></p><p><a href=\"/backward500\">Backward 500</a></p><p><a href=\"/backward2000\">Backward 2000</a></p><p><a href=\"/forward2000\">Forward 2000</a></p></body></html>");
  });
 server.on("/forward2000", HTTP_GET, [](AsyncWebServerRequest *request){
    stepper.move(-2000);
    stepper.runToPosition();
    request->send(200, "text/html", "<html><body><h1>Stepper Motor Control</h1><p><a href=\"/forward500\">Forward 500</a></p><p><a href=\"/backward500\">Backward 500</a></p><p><a href=\"/backward2000\">Backward 2000</a></p><p><a href=\"/forward2000\">Forward 2000</a></p></body></html>");
  });


  // Start server
  server.begin();
}


void loop() {
  // Handle any necessary background tasks
  stepper.run();
}
