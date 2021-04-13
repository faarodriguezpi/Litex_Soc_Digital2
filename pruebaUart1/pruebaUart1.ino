#include <Wire.h>

char dato = 'a';

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup Done");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    dato = Serial.read();
  }

  
  Serial.print(dato);
  
}
