#include <Arduino.h>
#include <AirwellIR.h>

IRSenderESP32 irSender(2,0);     // IR led on Duemilanove digital pin 3, using Arduino PWM
AirwellIR  *airIR;

void setup() {
  Serial.begin(115200);
  delay(500);
  airIR = new AirwellIR();
  Serial.println(F("Starting"));
}

void loop() {
  const char* buf;

  Serial.print(F("Sending IR to "));
  
  // Print the model
  buf = airIR->model();
  
  // 'model' is a PROGMEM pointer, so need to write a byte at a time
  while (char modelChar = pgm_read_byte(buf++)) { Serial.print(modelChar); }
  
  Serial.print(F(", info: "));
 // Print the info
  buf = airIR->info();
  // 'info' is a PROGMEM pointer, so need to write a byte at a time
  while (char infoChar = pgm_read_byte(buf++)){ Serial.print(infoChar); }
  Serial.println();

  delay(5000);
  
  airIR->send(irSender, POWER_ON, MODE_HEAT, FAN_3, 30);
  Serial.println(F("MODE_HEAT ON"));
  delay(3000);
  for(int i = 16; i<30; i++){
    airIR->send(irSender, POWER_ON, MODE_COOL, FAN_1, i);
    Serial.println(F("MODE_COOL ON"));
    delay(1500); 
  }
  airIR->send(irSender, POWER_ON, MODE_FAN, FAN_1, 23);
  delay(3000);
  airIR->send(irSender, POWER_ON, MODE_AUTO, FAN_AUTO, 23);
  delay(3000);
  airIR->send(irSender, POWER_OFF, MODE_COOL, FAN_1, 18);
  Serial.println(F("MODE_COOL OFF"));
  delay(5000);
}
