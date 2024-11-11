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
  <!DOCTYPE HTML><html>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>
    body {height: 100vh;overflow: hidden;margin: 0;}
    .parent {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        grid-template-rows: repeat(2, 1fr);
        grid-column-gap: 0px;
        grid-row-gap: 0px;
        height: 100%;
        }
        
        .div1 { grid-area: 1 / 2 / 3 / 3; height: 100vh;}
        .div2 { grid-area: 1 / 1 / 2 / 2; padding: 0.5rem;}
        .div3 { grid-area: 2 / 1 / 3 / 2; }
        .parent-j {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(3, 1fr);
        grid-column-gap: 0px;
        grid-row-gap: 0px;
        height: 100%;
        /* justify-items: center;
        align-items: center; */
        }

        .div-u { grid-area: 1 / 2 / 2 / 3; border: 1px solid gray;padding:2rem 3rem; border-radius:100% 100% 0 0;/**display:flex;flex-wrap:nowrap;justify-content:center;align-items:center;**/}
        .div-l { grid-area: 2 / 1 / 3 / 2; border: 1px solid gray;padding:2.95rem 2.4rem; border-radius:100% 0 0 100%;}
        .div-d { grid-area: 3 / 2 / 4 / 3; border: 1px solid gray;padding:2rem 3rem; border-radius:0 0 100% 100%;}
        .div-r { grid-area: 2 / 3 / 3 / 4; border: 1px solid gray;padding:2.95rem 2.4rem; border-radius:0 100% 100% 0;}
        .div-s { grid-area: 2 / 2 / 3 / 3; border: 1px solid rgb(233, 80, 80);padding:3rem 2.3rem; }

        .analog {border:1px solid rgb(216, 211, 211);display: flex;border-radius:15px;justify-content: space-between;align-items: stretch;}
        .analog-value{font-size:5rem;}
        .btn-set{width: 35%;border-radius:0 10px 10px 0;border:none;background:#524FF0;color:white;font-size:1.3rem}
        .analog-data{display: flex;flex-wrap: wrap;align-content: stretch;justify-content: flex-start;padding: 0.5rem;}
        .div-slider{width:100%;}
        .slider{width: 90%;}
        .circ-btn{border-radius:100%;border:1px solid red;width:180px;height:180px;background:none;}

        @media screen and (orientation: portrait) {
            .parent {
                display: grid;
                grid-template-columns: 1fr;
                grid-template-rows: repeat(3, 1fr);
                grid-column-gap: 0px;
                grid-row-gap: 0px;
                }

                .div1 { grid-area: 3 / 1 / 4 / 2; height: auto;}
                .div2 { grid-area: 2 / 1 / 3 / 2; }
                .div3 { grid-area: 1 / 1 / 2 / 2; }
        }
</style>
<div class="parent">
    <div class="div1">
        <div class="parent-j">
            <button class="div-l" id="btn-lt">CW</button>
            <button class="div-r" id="btn-rt">AC</button>
            <button class="div-s" id="btn-sp">STOP</button>
        </div>
    </div>
    <div class="div2">
        <div class="analog">
            <div class="analog-data"><div class="analog-value" id="slider-txt">0</div>
                <div class="div-slider"><input id="slider-val" class="slider" type="range" value="0" min="0" max="255"/></div>
            </div> 
            <button class="btn-set" id="btn-set">SET</button>
        </div>
    </div>
    <div class="div3"><button class="circ-btn" id="btn-fire">RELAY</button><button class="circ-btn" id="btn-led">LED</button></div>
</div>

<script>
let gateway = `ws://${window.location.hostname}/ws`;

let sliderTxt = document.querySelector("#slider-txt");
let sliderVal = document.querySelector("#slider-val");
let fireBtn = document.querySelector("#btn-fire");
let ledBtn = document.querySelector("#btn-led");
let ledState = 0;
let fireState = 0;

let websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    initButtons();
}

function initButtons() {
  document.querySelector('#btn-lt').addEventListener('click', ()=>{ websocket.send(JSON.stringify({dir:3})) });
  document.querySelector('#btn-rt').addEventListener('click', ()=>{ websocket.send(JSON.stringify({dir:4})) });
  document.querySelector('#btn-sp').addEventListener('click', ()=>{ websocket.send(JSON.stringify({dir:0})) });

  document.querySelector('#btn-set').addEventListener('click', ()=>{ websocket.send(JSON.stringify({slider: parseInt(sliderVal.value)})) });
  document.querySelector('#btn-led').addEventListener('click', ()=>{ 
    ledState = !ledState;
    toggleBg(ledBtn, ledState);
    websocket.send(JSON.stringify({led:ledState ? 1 : 0})); 
  });
  
  document.querySelector('#btn-fire').addEventListener('click', ()=>{ 
    fireState = !fireState;
    toggleBg(fireBtn, fireState);
    websocket.send(JSON.stringify({fire:fireState ? 1 : 0})); 
  });

  document.querySelector('#slider-val').addEventListener('change', ()=>{
    document.querySelector("#slider-txt").innerHTML = sliderVal.value;
  });
}

function toggleBg(btn, state) {
    if (state) {
        btn.style.background = '#ff0000';
    }
    else {
        btn.style.background = '#ffffff';
    }
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function getReadings(){
    websocket.send("getReadings");
}

function onMessage(event) {
    websocket.send("getReadings");
}</script>
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
      speed = map(slider, 0, 255, 1000, 250);
    }
    else if (myObject.hasOwnProperty("dir")) {
      direction = (int)myObject["dir"];
      move(direction, speed);      
    }

    String sensorReadings = getSensorReadings();
    notifyClients(sensorReadings);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  initWiFi();
  initWebSocket();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });
  server.begin();
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
