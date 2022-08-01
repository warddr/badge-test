void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(touchRead(13));
}
