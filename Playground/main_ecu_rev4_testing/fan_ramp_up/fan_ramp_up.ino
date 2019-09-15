/*
 * Nathan Cheek
 * 2016-11-23
 * Test cooling board
 */
#include <Metro.h>

int p = 0;
//bool rising = true;

Metro timer = Metro(20);
Metro r = Metro(12000);

void setup() {
  // put your setup code here, to run once:
  pinMode(A6, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(A6, p);
  analogWrite(A8, p);
  analogWrite(A9, p);

  if (timer.check() && p < 210) {
    p++;
    Serial.println(p);
  }
  if (r.check()) {
    //p = 0;
  }
}
