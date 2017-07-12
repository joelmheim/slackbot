#include <Event.h>
#include <Timer.h>

#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include "secrets.h"
#include "pin_layout.h"
#include "RFID_module.h"
#include "User_module.h"
#include "Wifi_module.h"

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

// BUTTONS        VALUES     RESISTORS
// coffee         250-252    10 kOhm
// lunch          708-711    4.1 kOhm

Timer t;
int resetEvent;
int wifiEvent;
int userEvent;

// Pin for buttons (buttons share one analog pin) -- Need to move this to pinlayout.h
const int BUTTON_PIN = A0; 

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

Rfid_unit rfid_unit(&rfid);
Led led;
User user;
Wifi wifi(&led);

// variables will change:
long timing = 0;
int buttonState = 0;     // 0 == no button pressed
                         // 1 == coffee button pressed
                         // 2 == lunch button pressed
int lastButtonState = 0; // previous state of the button

void setup() {
  Serial.begin(115200);
  rfid_unit.initializeRFID();                          // Setup for RFID MFRC522 */
  // initialize the pushbutton pin as an input:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  led.setup();
  
  //displaySetup();

  wifiEvent = t.every(60 * 1000, checkWifiConnection, (void*)2);
  
  wifi.setup();

  if (WiFi.status() == WL_CONNECTED) {
    user.initializeUsers();
    if (user.initialized) {
      led.resetSignal();
      userEvent = t.every(5 * 60 * 1000, updateUserSetup, (void*)3);
    } else {
      userEvent = t.every(10 * 1000, updateUserSetup, (void*)3);
    }
  } else {
    Serial.println(F("\nWaiting for wifi connection..."));
    userEvent = t.every(60 * 1000, updateUserSetup, (void*)3);
  }
}

void loop() {
  // if we have gone to the next millisecond
  if (millis() != timing) {
    if (user.initialized) {
      if (rfid_unit.RFID_scan()) {
        resetUserID((void*)0);
        boolean match = user.checkCard(rfid_unit.uidString);
    
        if(match) {
          led.signalKnownCard();
        } else {
          led.signalUnknownCard();
        }

        resetEvent = t.after(5000, resetUserID, (void*)0);
      }
    }
    timing = millis();
  }
  if (user.activeUser()) {
    checkButtons();
  }
  t.update();
}

void resetUserID(void *context) {
  Serial.println("Reseting user id.");
  user.reset();
  led.resetSignal();
}

void updateUserSetup(void* context) {
  user.setup();
}

void checkWifiConnection(void* context) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\nWifi connection ok."));
    led.resetSignal();
  } else {
    Serial.println(F("Reconnecting wifi."));
    WiFi.disconnect();
    wifi.resetCounters();
    wifi.setup();
  }
}

int httpPost(String body) {
  Serial.println(F("Posting to Slack."));
  WiFiClientSecure client;
  const int httpPort = 443;
  if (!client.connect(slack_host, httpPort)) {
    Serial.println(F("connection failed"));
    return 0;
  }

  // We now create a URI for the request
  Serial.print(F("Requesting URL: "));
  Serial.println(slack_url);

  // This will send the request to the server
  client.print(String("POST ") + slack_url + " HTTP/1.1\r\n" +
               "Host: " + slack_host + "\r\n" + 
               "Connection: close\r\nContent-Type: application/json\r\n" +
               "Content-Length: " + body.length() + "\r\n\r\n" +
               body);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  client.stop();
  return 0;
}

boolean checkButtons() {
  boolean messageSent = false;
  // read the state of the pushbutton value:
  buttonState = readButtonState();
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState != 0) {
      Serial.println(F("on"));
    } else {
      Serial.println(F("off"));
      Serial.print(F("Button pressed: "));
      Serial.println(lastButtonState);
      String currentUser = user.currentUser();
      if (user.activeUser()) {
        Serial.print(F("Posting to slack: "));
        if (lastButtonState == 1) {   
          String coffeePost = "{'text': 'Time for a break? <@" +  currentUser + "> would like a *coffee*! Want to join?', 'channel': '#trondheim', 'username': 'coffee_bot' , 'icon_emoji': ':coffee:'}";
          Serial.println(coffeePost);
          httpPost(coffeePost);
          messageSent = true;
        } else if (lastButtonState == 2) {
          String lunchPost = "{'text': 'Time for lunch? <@" + currentUser + "> is going to *lunch*! :lunsj:. Hungry?', 'channel': '#trondheim', 'username': 'lunch_bot' , 'icon_emoji': ':knife_fork_plate:'}";
          Serial.println(lunchPost);
          httpPost(lunchPost);
          messageSent = true;
        }
      }
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;  
  return messageSent;
}

int readButtonState() {
  /* Buttonstate returned will be
   *  0: No button pressed
   *  1: Coffee button pressed
   *  2: Lunch button pressed
   */
  int buttonValue = 0;
  
  buttonValue = analogRead(BUTTON_PIN);
  if (buttonValue > 500) {
    return 2;
  } else if (buttonValue > 100) {
    return 1;
  } else {
    return 0;
  }
}

void displaySetup() {
  Serial.println(F("\nInitializing display"));
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello from:");
  display.println("Slackbot");
  display.display();
}

