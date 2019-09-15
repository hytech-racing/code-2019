void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int cooling_sense = analogRead(A12);
  int ecu_sense = analogRead(A13);
  Serial.print("Cooling: ");
  Serial.println(cooling_sense);
  Serial.print("ECU: ");
  Serial.println(ecu_sense);
  delay(50);
}
