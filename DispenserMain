#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN 2
#define BUTTON_PIN 9
#define LDR_PIN 27
#define LDR_THRESHOLD 400

Servo servo;
bool lastButtonState = LOW;

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN, 500, 2500);
  pinMode(BUTTON_PIN, INPUT);  // Ekstern pulldown modstand på pin 11
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN) == HIGH;

  // Detekter stigende flanke (kun når knappen netop trykkes ned)
  if (buttonState && !lastButtonState) {
    int ldrValue = analogRead(LDR_PIN);

    Serial.print("Knap trykket – LDR værdi: ");
    Serial.println(ldrValue);

    if (ldrValue < LDR_THRESHOLD) {
      Serial.println("OK! Skubber brik ");
      servo.write(155);
      delay(1000);
      servo.write(0);
    } else {
      Serial.println("FEJL: Der mangler brikker!");
    }
  }

  lastButtonState = buttonState;
  delay(10);
}
