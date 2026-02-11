#include "Ryzobee.h"

const int buttonPin = 0;

// Button state variables
int buttonState;         
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debounce delay (milliseconds)

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("Button detection program started");
}

void loop() {
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) { // Button pressed (LOW due to pull-up)
        Serial.println("Button pressed");
      } else { // Button released
        Serial.println("Button released");
      }
    }
  }
  
  lastButtonState = reading;
}
