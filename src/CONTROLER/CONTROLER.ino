#include "config.h"
#include "bitmap.h"
#include "simbols.h"
#include "sensor.h"

void eepromGet() {
  EEPROM.get(ADR_left_or_right_hand, left_or_right_hand);
  EEPROM.get(ADR_sensitive, sensitive);
  EEPROM.get(ADR_dataTransfer, dataTransfer);
  EEPROM.get(ADR_servoSpeed, servoSpeed);
}

void eepromSave() {
  EEPROM.put(ADR_left_or_right_hand, left_or_right_hand);
  EEPROM.put(ADR_sensitive, sensitive);
  EEPROM.put(ADR_dataTransfer, dataTransfer);
  EEPROM.put(ADR_servoSpeed, servoSpeed);
}

Sensor pXL = Sensor(pin_XL, 0);
Sensor pYL = Sensor(pin_YL, 0);
Sensor pXR = Sensor(pin_XR, 0);
Sensor pYR = Sensor(pin_YR, 0);
Sensor pSR = Sensor(pin_SW_R, 1, INPUT_PULLUP);
Sensor pSL = Sensor(pin_SW_L, 1, INPUT_PULLUP);
Sensor pBD = Sensor(pin_BUT_DN, 1, INPUT_PULLUP);
Sensor pBL = Sensor(pin_BUT_LT, 1, INPUT_PULLUP);
Sensor pBM = Sensor(pin_BUT_MN, 1, INPUT_PULLUP);
Sensor pBU = Sensor(pin_BUT_UP, 1, INPUT_PULLUP);
Sensor pVR = Sensor(pin_BUT_RT, 1, INPUT_PULLUP);
Sensor pot = Sensor(pin_POT, 0);

void setup() {

  pot.setMaxMin(0, simbolsLength);

  eepromGet();

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0, virat_kholi, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
  display.display();
}

uint8_t flag_laser = false;  //flag = true

void bluetoothControl() {
  display.clearDisplay();
  display.drawBitmap(0, 0, Rohit, 128, 64, 1);
  display.display();
  while (true) {
    if (pBD.read()) {
      display.clearDisplay();
      display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
      display.display();
      return;
    }

    uint16_t R_value[] = { 0, 0 };
    uint16_t L_value[] = { 0, 0 };

    if (left_or_right_hand == 1) {
      R_value[0] = pXR.read();
      R_value[1] = pYR.read();
      L_value[0] = pXL.read();
      L_value[1] = pYL.read();
    } else {
      R_value[0] = pXL.read();
      R_value[1] = pYL.read();
      L_value[0] = pXR.read();
      L_value[1] = pYR.read();
    }

    if (L_value[0] > 1024 - sensitive && L_value[1] < 1024 - sensitive && L_value[1] > sensitive) {
      Serial.print("b");
    } else if (L_value[0] > 1024 - sensitive && L_value[1] > 1024 - sensitive) {
      Serial.print("c");
    } else if (L_value[0] < 1024 - sensitive && L_value[0] > sensitive && L_value[1] > 1024 - sensitive) {
      Serial.print("d");
    } else if (L_value[0] < sensitive && L_value[1] > 1024 - sensitive) {
      Serial.print("e");
    } else if (L_value[0] < sensitive && L_value[1] < 1024 - sensitive && L_value[1] > sensitive) {
      Serial.print("f");
    } else if (L_value[0] < sensitive && L_value[1] < sensitive) {
      Serial.print("g");
    } else if (L_value[0] > sensitive && L_value[0] < 1024 - sensitive && L_value[1] < sensitive) {
      Serial.print("h");
    } else if (L_value[0] > 1024 - sensitive && L_value[1] < sensitive) {
      Serial.print("i");
    } else {
      Serial.print("a");
    }

    if (R_value[1] < sensitive) {
      Serial.print("k");
      delay(servoSpeed);
    } else if (R_value[1] > 1024 - sensitive) {
      Serial.print("j");
      delay(servoSpeed);
    }

    if (pSR.read() || pSL.read()) {
      Serial.print("l");
      delay(500);
    }
    delay(dataTransfer);
  }
}

String outputV;

bool post;

void AIhelper() {
  while (true) {
    display.clearDisplay();
    display.setTextSize(1);               // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);  // Draw 'inverse' text
    display.setCursor(1, 1);
    display.println(F("AI: hi, can i help you"));

    if (!post) {
      if (pBL.read()) outputV += simbols[pot.read()];
      display.setCursor(1, 50);
      display.print("YOU: ");
      display.print(outputV);
      display.println(simbols[pot.read()]);
    } else {
      if (pBL.read()) {
        post = 0;
        outputV = "";
      }
    }

    if (pBM.read()) post = 1;
    if (pBD.read()) {
      display.clearDisplay();
      display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
      display.display();
      return;
    }
  }
  display.display();
}

uint8_t point = 0;

void informationPrint() {
  while (true) {
    if (pBD) {
      return;
    }
    display.clearDisplay();
    display.drawBitmap(0, 0, real_white_display, 128, 64, 1);
    display.setTextSize(1);               // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);  // Draw 'inverse' text
    display.setCursor(0, 24);
    display.println(F("  date: 17:08:2025\n"
                      "   create: MGDDIM\n"));
    display.display();  // Show initial text
  }
}

void drawPoint() {
  display.setCursor(0, 8 * point + 17);
  display.println(F(" >"));
}

uint16_t Regut(uint16_t val, uint16_t max, uint16_t min, String name) {
  while (true) {
    if (pBL.read()) {
      if (val == min) val = max;
      else val--;
      delay(10);
    } else if (pBR.read()) {
      if (val == max) val = min;
      else val++;
      delay(10);
    }
    display.clearDisplay();
    display.drawBitmap(0, 0, real_white_display, 128, 64, 1);
    display.setTextSize(1);               // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);  // Draw 'inverse' text
    display.setCursor(0, 28);
    display.print(" ");
    display.print(name);
    display.print(" <");
    display.print(val);
    display.print(">");
    display.display();  // Show initial text
    if (pBD.read()) {
      return val;
    }
  }
}

void checkEvent() {
  switch (point) {
    case 0:
      {
        left_or_right_hand = Regut(left_or_right_hand, 1, 0, "left/right mode");
        break;
      }
    case 1:
      {
        servoSpeed = Regut(servoSpeed, 500, 100, "servo speed");
        break;
      }
    case 2:
      {
        sensitive = Regut(sensitive, 500, 100, "sensitive");
        break;
      }
    case 3:
      {
        dataTransfer = Regut(dataTransfer, 1023, 0, "data transfer");
        break;
      }
    case 4:
      {
        informationPrint();
        break;
      }
  }
}

void checkPointPosition() {
  if (pBU.read()) {
    if (point == 0) point = 4;
    else point--;
  } else if (pBD.read()) {
    if (point == 4) point = 0;
    else point++;
  }
  if (pBM.read()) {
    checkEvent();
  }
  delay(100);
}

void settings() {
  while (true) {
    if (pBL.read()) {
      display.clearDisplay();
      display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
      display.display();
      delay(500);
      eepromSave();
      return;
    }
    checkPointPosition();
    display.clearDisplay();
    display.drawBitmap(0, 0, white_display, 128, 64, 1);
    display.setTextSize(1);               // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);  // Draw 'inverse' text
    display.setCursor(0, 17);
    display.println(F("  left/right mode\n"
                      "  servo speed\n"
                      "  sensitive\n"
                      "  data transfer speed\n"
                      "  information\n"));
    drawPoint();
    display.display();  // Show initial text
  }
}

void loop() {
  if (pBL.read() == false) {
    bluetoothControl();
  } else if (pBM.read() == false) {
    autoPilot();
  } else if (pBR.read() == false) {
    settings();
  }
}
