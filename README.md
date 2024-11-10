# paste-extruder

thread_test_02:
- pitch: 1 mm
- inner_thread XY-scale: 0.30 mm
- PETG_04: ok_tight

thread_test_03:
- pitch: 2 mm
- inner_thread XY-scale: 0.25 mm
- result: ok_tight
- PETG_04: ok_tight

Test AccelStepper:

```c++
const int DIR = 12;
const int STEP = 14;

const int speed = 500; // MAX_SPEED = 250  MIN_SPEED = 1000

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void moveF() {
  digitalWrite(DIR, HIGH);
  digitalWrite(STEP, HIGH);
  delayMicroseconds(speed);
  digitalWrite(STEP, LOW);
  delayMicroseconds(speed);
}

void moveB() {
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, HIGH);
  delayMicroseconds(speed);
  digitalWrite(STEP, LOW);
  delayMicroseconds(speed);
}

void loop()
{
  moveF();
}
```


TBF:

```c++
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
JSONVar readings;

const char *ssid = "GalaxyAzamat";
const char *password = "vhrg8328";

const int DIR = 12;
const int STEP = 14;

int speed = 500; // MAX_SPEED = 250  MIN_SPEED = 1000
int direction = 0;
int slider = 0;

const char index_html[] PROGMEM = R"rawliteral(
)rawliteral";

String getSensorReadings(){
  readings["s"] = String(slider);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    
    JSONVar myObject = JSON.parse((const char*)data);
    if (myObject.hasOwnProperty("slider")) {
      slider = (int)myObject["slider"];
      speed = map(slider, 0, 1024, 1000, 250);
    }
    else if (myObject.hasOwnProperty("dir")) {
      direction = (int)myObject["dir"];
      move(direction, speed);      
    }

    String sensorReadings = getSensorReadings();
    notifyClients(sensorReadings);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void move(int direction, int speed) {
  if (direction == 0) {
    // stop
  }
  else if (direction == 3){
    digitalWrite(DIR, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP, LOW); // ?
    delayMicroseconds(speed);
  }
  else if (direction == 4){
    digitalWrite(DIR, LOW);
    delayMicroseconds(speed);
    digitalWrite(STEP, LOW);
    delayMicroseconds(speed);
  }
}


void loop()
{
}
```
