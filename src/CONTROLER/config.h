#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

const uint8_t pin_XL = A1;
const uint8_t pin_YL = A0;

const uint8_t pin_XR = A3;
const uint8_t pin_YR = A2;

const uint8_t pin_SW_R = 5;
const uint8_t pin_SW_L = 3;

const uint8_t pin_BUT_UP = 8;
const uint8_t pin_BUT_DN = 4;
const uint8_t pin_BUT_LT = 2;
const uint8_t pin_BUT_RT = 7;
const uint8_t pin_BUT_MN = 12;

uint8_t ADR_left_or_right_hand = 0;
uint8_t ADR_sensitive = 10;
uint8_t ADR_dataTransfer = 20;
uint8_t ADR_servoSpeed = 30;

uint16_t left_or_right_hand = 1;
uint16_t sensitive = 255;
uint16_t dataTransfer = 0;
uint16_t servoSpeed = 500;