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
#include <AccelStepper.h>

#define motorStep 14
#define motorDir 12
#define motorEnable 4
AccelStepper stepper(1, motorStep, motorDir);


void setup() {
  Serial.begin(115200);
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(2000.0);
  //stepper.setSpeed(150);  
}

void loop() {
  stepper.setSpeed(350); 
  delay(2000);
  stepper.setSpeed(-350); 
  delay(2000);
  stepper.stop();
}
```
