#include "LED_module.h"

Led::Led() {
  
}

void Led::setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

void Led::resetSignal() {
  //digitalWrite(RED_LED_PIN, HIGH);
  //digitalWrite(GREEN_LED_PIN, HIGH);
  //delay(200);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void Led::signalKnownCard() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void Led::signalUnknownCard() {
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
}


void Led::signalWifiOk() {
  signalBlink();
  signalKnownCard();
}

void Led::signalWifiFailure() {
  signalBlink();
  signalUnknownCard();
}

void Led::signalBlink() {
  for (int i=0; i<4; i++) {
    signalUnknownCard();
    delay(200);
    signalKnownCard();
    delay(200);
  }
}

