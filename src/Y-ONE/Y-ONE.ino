#include "config.h"

/***functions***/

uint8_t readSensorF() {
  if (sonarF.ping_cm() != 0) return sonarF.ping_cm();
  else return 255;
}

uint8_t readSensorB() {
  if (sonarB.ping_cm() != 0) return sonarB.ping_cm();
  else return 255;
}

void servoAttach(uint16_t pos) {
  pos = map(pos, 0, 180, 800, 2500);
  digitalWrite(servo, HIGH);
  delayMicroseconds(pos);
  digitalWrite(servo, LOW);
}

void drive(int L, int R) {
  L = constrain(L, -255, 255);
  R = constrain(R, -255, 255);

  if (L < 0) {
    digitalWrite(ain1, HIGH);
    analogWrite(ain2, 255 - abs(L));
  } else if (L > 0) {
    digitalWrite(ain2, HIGH);
    analogWrite(ain1, 255 - abs(L));
  } else if (L == 0) {
    digitalWrite(ain1, HIGH);
    digitalWrite(ain2, HIGH);
  }

  if (R < 0) {
    digitalWrite(bin1, HIGH);
    analogWrite(bin2, 255 - abs(L));
  } else if (R > 0) {
    digitalWrite(bin2, HIGH);
    analogWrite(bin1, 255 - abs(L));
  } else if (R == 0) {
    digitalWrite(bin1, HIGH);
    digitalWrite(bin2, HIGH);
  }
}

//autoPilot mode
void eventCheck() {
  if (readSensorF() > radius) {
    drive(200, 200);
  } else if (readSensorF() <= radius) {
    drive(0, 0);
    servoAttach(0);
    delay(1000);
    uint8_t readTemp = readSensorF();
    servoAttach(90);
    delay(1000);
    servoAttach(180);
    delay(1000);
    uint8_t readTemp2 = readSensorF();
    if (readTemp > readTemp2 && readTemp > radius) {
      servoAttach(90);
      delay(1000);
      while (readSensorF() < 60) {
        drive(200, -200);
      }
      delay(1000);
    } else if (readTemp2 > readTemp && readTemp2 > radius) {
      servoAttach(90);
      delay(1000);
      while (readSensorF() < 60) {
        drive(-200, 200);
      }
      delay(1000);
    } else {
      servoAttach(90);
      while (readSensorF() < 60) {
        drive(-200, 200);
      }
      delay(1000);
      // backDrive(2000);
    }
  }
}

void backDrive(uint16_t timeout) {
  while (readSensorB() > radius) {
    drive(-200, -200);
    delay(timeout);
    drive(0, 0);
    servoAttach(0);
    delay(1000);
    uint8_t readTemp = readSensorF();
    servoAttach(90);
    delay(1000);
    servoAttach(180);
    delay(1000);
    uint8_t readTemp2 = readSensorF();
    if (readTemp > radius && readTemp >= readTemp2) {
      servoAttach(90);
      while (readSensorF() < radius) {
        drive(200, -200);
      }
      delay(1000);
      return;
    } else if (readTemp2 > radius && readTemp <= readTemp2) {
      servoAttach(90);
      while (readSensorF() < radius) {
        drive(-200, 200);
      }
      delay(1000);
      return;
    } else {
      servoAttach(90);
      delay(1000);
    }
  }
}

//bluetoothControl mode

//states
const char CMD_NO = 'a';
const char UP = 'b';
const char RIGHT = 'd';
const char DOWN = 'f';
const char LEFT = 'h';

char save_state = CMD_NO;

const uint16_t save_size = 255;
uint16_t index = 1;

char save_char[save_size] = {};
uint32_t save_time[save_size] = {};

uint8_t state_laser = false;

uint32_t save_last_time = 0;

void save_history(uint32_t timeout) {
  if (index != save_size) {
    if (save_state == UP) {
      save_char[index] = DOWN;
      save_time[index] = timeout;
      index+=1;
    } else if (save_state == DOWN) {
      save_char[index] = UP;
      save_time[index] = timeout;
      index+=1;
    } else if (save_state == RIGHT) {
      save_char[index] = LEFT;
      save_time[index] = timeout;
      index+=1;
    } else if (save_state == LEFT) {
      save_char[index] = RIGHT;
      save_time[index] = timeout;
      index+=1;
    }
  }
}

uint8_t backToHome() {
  save_last_time = millis();

  while (index != 0) {
    if (save_char[index] == UP) {  //up
      drive(255, 255);
    }else if (save_char[index] == RIGHT) {  //right
      drive(255, -255);
    }else if (save_char[index] == LEFT) {  //left
      drive(-255, 255);
    }else if (save_char[index] == DOWN) {  //down
      drive(-255, -255);
    }
    if (millis() - save_last_time > save_time[index]) {
      index -= 1;
      save_last_time = millis();
    }
  }
  drive(0, 0);
  return true;
}

void bluetoothControl() {
  if (Serial.available()) {
    //input
    char input = Serial.read();

    //command orintation
    if (input == 'm') {
      Serial.print("hi");
      drive(0,0);
      uint8_t result = backToHome();
    }
    if (input == 'a') {  //no command
      drive(0, 0);
      save_history(millis() - save_last_time);
      save_state = CMD_NO;
      save_last_time = 0;
    }
    if (input == 'b') {  //up
      drive(255, 255);
      save_state = UP;
      if (save_last_time == 0) save_last_time = millis();
    }
    if (input == 'd') {  //right
      drive(255, -255);
      save_state = RIGHT;
      if (save_last_time == 0) save_last_time = millis();
    }
    if (input == 'h') {  //left
      drive(-255, 255);
      save_state = LEFT;
      if (save_last_time == 0) save_last_time = millis();
    }
    if (input == 'f') {  //down
      drive(-255, -255);
      save_state = DOWN;
      if (save_last_time == 0) save_last_time = millis();
    }
    if (input == 'j') {  //tower turn to right
      if (position != 0) {
        position = position - 5;
      }
      servoAttach(position);
    }
    if (input == 'k') {  //tower turn to left
      servoAttach(0);
      if (position != 180) {
        position = position + 5;
      }
      servoAttach(position);
    }
    if (input == 'l') {  //FIRE
      state_laser = !state_laser;
      digitalWrite(laser, state_laser);
    }
  }
}


void testServo() {
  servoAttach(0);
  delay(500);
  servoAttach(90);
  delay(500);
  servoAttach(180);
  delay(500);
}

void setup() {
  //pin setup
  pinMode(bin2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(servo, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(trig_forward, OUTPUT);
  pinMode(echo_forward, INPUT);
  pinMode(trig_back, OUTPUT);
  pinMode(echo_back, INPUT);

  //serial setup
  Serial.begin(9600);

  //servo begin setup
  servoAttach(position);
}

void loop() {
  bluetoothControl();
}