#include "config.h"
#include "bitmap.h"

void eepromGet() {
  EEPROM.get(ADR_left_or_right_hand,left_or_right_hand);
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

void setup() {
  pinMode(pin_XL, INPUT);
  pinMode(pin_YL, INPUT);
  pinMode(pin_XR, INPUT);
  pinMode(pin_YR, INPUT);
  pinMode(pin_SW_R, INPUT_PULLUP);
  pinMode(pin_SW_L, INPUT_PULLUP);
  pinMode(pin_BUT_DN, INPUT_PULLUP);
  pinMode(pin_BUT_LT, INPUT_PULLUP);
  pinMode(pin_BUT_MN, INPUT_PULLUP);
  pinMode(pin_BUT_UP, INPUT_PULLUP);
  pinMode(pin_BUT_RT, INPUT_PULLUP);

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
    if (digitalRead(pin_BUT_DN) == false) {
      display.clearDisplay();
      display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
      display.display();
      return;
    }

    uint16_t R_value[] = { 0, 0 };
    uint16_t L_value[] = { 0, 0 };

    if (left_or_right_hand == 1) {
      R_value[0] = analogRead(pin_XR);
      R_value[1] = analogRead(pin_YR);
      L_value[0] = analogRead(pin_XL);
      L_value[1] = analogRead(pin_YL);
    } else {
      R_value[0] = analogRead(pin_XL);
      R_value[1] = analogRead(pin_YL);
      L_value[0] = analogRead(pin_XR);
      L_value[1] = analogRead(pin_YR);
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

    if (digitalRead(pin_SW_R) == false || digitalRead(pin_SW_L) == false) {
      Serial.print("l");
      delay(500);
    }
    delay(dataTransfer);
  }
}

void autoPilot() {
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_Group_300, 128, 64, 1);
  display.display();
  Serial.print("m");
  while (true) {
    if (digitalRead(pin_BUT_DN) == false) {
      display.clearDisplay();
      display.drawBitmap(0, 0, Dhoni, 128, 64, 1);
      display.display();
      return;
    }
  }
}

uint8_t point = 0;

void informationPrint() {
  while (true) {
    if (digitalRead(pin_BUT_DN) == false) {
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
    if (digitalRead(pin_BUT_LT) == false) {
      if (val == min) val = max;
      else val--;
      delay(10);
    } else if (digitalRead(pin_BUT_RT) == false) {
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
    if (digitalRead(pin_BUT_DN) == false) {
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
  if (digitalRead(pin_BUT_UP) == false) {
    if (point == 0) point = 4;
    else point--;
  } else if (digitalRead(pin_BUT_DN) == false) {
    if (point == 4) point = 0;
    else point++;
  }
  if (digitalRead(pin_BUT_MN) == false) {
    checkEvent();
  }
  delay(100);
}

void settings() {
  while (true) {
    if (digitalRead(pin_BUT_LT) == false) {
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
  if (digitalRead(pin_BUT_LT) == false) {
    bluetoothControl();
  } else if (digitalRead(pin_BUT_MN) == false) {
    autoPilot();
  } else if (digitalRead(pin_BUT_RT) == false) {
    settings();
  }
}