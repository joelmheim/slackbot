#ifndef _PIN_LAYOUT_
#define _PIN_LAYOUT_

#include <Wire.h>

// constants won't change. They're used here to 
// set pin numbers:

// NodeMCU pins
//const int A0 =  0; // ADC0
//const int D0 = 16; // GPIO16
//const int D1 = 5;  // GPIO5
//const int D2 = 4;  // GPIO4
//const int D3 = 0;  // GPIO0
//const int D4 = 2;  // GPIO2
//const int D5 = 14; // GPIO14
//const int D6 = 12; // GPIO12
//const int D7 = 13; // GPIO13
//const int D8 = 15; // GPIO15
const int RX = 3;  // GPIO3
const int TX = 1;  // GPIO1
const int SD1 = 8; // GPIO8
const int SD2 = 9; // GPIO9
const int SD3 = 10; // GPIO10



// Pins for RFID reader
const int RFID_RST_PIN = 4; //D2;     
const int RFID_SS_PIN = 2; //D4;
//const int RFID_SCK_PIN = D5;
//const int RFID_MOSI_PIN = D7;
//const int RFID_MISO_PIN = D6;

const int RED_LED_PIN = 16; //D0;
const int GREEN_LED_PIN = 15; //D8;

#endif // _PIN_LAYOUT_

