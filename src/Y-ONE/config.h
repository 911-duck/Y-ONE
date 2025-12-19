// library for sensors
#include <NewPing.h>

// sensor
const uint8_t trig_forward = 8;
const uint8_t echo_forward = 7;

const uint8_t trig_back = 4;
const uint8_t echo_back = 2;

NewPing sonarF(trig_forward, echo_forward, 255);
NewPing sonarB(trig_back, echo_back, 255);

// motor
const uint8_t ain2 = 9;
const uint8_t ain1 = 10;
const uint8_t bin1 = 6;
const uint8_t bin2 = 5;

//servo
const uint8_t servo = 2;
uint8_t position = 90;

//radius
const uint8_t radius = 30;

//wappon
const uint8_t laser = A0;