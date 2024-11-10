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
