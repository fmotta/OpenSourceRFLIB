#include <Arduino.h>

#include <MsTimer2.h>
// User settable
//  debug

#define BUTTON_DEBUG false
#define WORD_DEBUG false
//  operational value
#define INPUT_DELAY 3
#define KEYBOARD_REPEAT_DELAY 650

#include <OpenSourceRF.h>

// Initialization and global variables
OpenSourceRF osrf(10, 11);

int count = 0;
bool newLn = 0;
char lastKey = 0;

void clearLastKey() {
  osrf.clearLastKey();
}

// system set up
void setup() {
  osrf.begin(9600);                                      // OpenSourceRF device
  osrf.setDelay(INPUT_DELAY);                            // Set Delay
  Serial.begin(9600);                                    // debug serial
  MsTimer2::set(KEYBOARD_REPEAT_DELAY, clearLastKey);    // Debounce button presses
  MsTimer2::start();
}

void loop() {
  int incomingByte = 0;

  if (osrf.available() > 0) {
    newLn = 0;
    
    // read incoming byte
    incomingByte = osrf.getButtonPress();
        
    count++;
    switch (incomingByte) {
    case LEFT_BUTTON:
      Serial.println("Left Button");
      break;
    case CENTER_BUTTON:
      Serial.println("Center Button");
      break;
    case RIGHT_BUTTON:
      Serial.println("Right Button");
      break;
    case LEFT_CENTER_BUTTON:
      Serial.println("Left+Center Button");
      break;
    case RIGHT_CENTER_BUTTON:
      Serial.println("Right+Center Button");
      break;
    case LEFT_RIGHT:
      Serial.println("Left+Right Button");
      break;
    case LEFT_CENTER_RIGHT:
      Serial.println("Left+Center+Right Button");
      break;
    case REPEAT_KEY:
//      Serial.println("-repeat-");
      break;
    default:
      Serial.print("Unknown Button ");
      Serial.println(incomingByte, HEX);
      break;
      }
  }
}

