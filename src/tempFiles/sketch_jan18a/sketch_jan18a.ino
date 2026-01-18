#include "pins.h"
#include "sensor.h"
#include "simbols.h"

Sensor pot = Sensor(POT, 0);
Sensor btn1 = Sensor(BUTTON1, 1, INPUT_PULLUP);
Sensor btn2 = Sensor(BUTTON2, 1, INPUT_PULLUP);

void setup() {
  Serial.begin(9600);
  pot.setMaxMin(0, simbolsLength);
}

String outputV;

bool post;

void loop() {
  if (!post) {
    if (btn1.read()) outputV += simbols[pot.read()];

    Serial.print(outputV);
    Serial.println(simbols[pot.read()]);
  } else {
    Serial.print("итоговое сообщение : ");
    Serial.println(outputV);
    if (btn1.read()) {
      post = 0;
      outputV = "";
    }
  }

  if (btn2.read()) post = 1;
}
