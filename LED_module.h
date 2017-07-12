#ifndef _LED_
#define _LED_

#include "pin_layout.h"

#include <SPI.h>
#include <Wire.h>

/////////////////////////////////////////////////

class Led {
public:
  Led();
  void setup();
  void resetSignal();
  void signalKnownCard();
  void signalUnknownCard();
  void signalWifiOk();
  void signalWifiFailure();
private:
  void signalBlink();
};

#endif // _LED_
