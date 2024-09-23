/*
 * This code programs a number of pins on an ESP32 as buttons on a BLE gamepad
 *
 * It uses arrays to cut down on code
 *
 * Uses the Bounce2 library to debounce all buttons
 *
 * Uses the rose/fell states of the Bounce instance to track button states
 *
 * Before using, adjust the numOfButtons, buttonPins and physicalButtons to suit your senario
 *
 */

#define BOUNCE_WITH_PROMPT_DETECTION // Make button state changes available immediately

#include <Arduino.h>
#include <Bounce2.h>    // https://github.com/thomasfredericks/Bounce2
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define numOfButtons 14
#define numOfHatSwitches 0
#define enableX false
#define enableY false
#define enableZ false
#define enableRX false
#define enableRY false
#define enableRZ false
#define enableSlider1 false
#define enableSlider2 false
#define enableRudder false
#define enableThrottle false
#define enableAccelerator false
#define enableBrake false
#define enableSteering false

Bounce debouncers[numOfButtons];
BleGamepad bleGamepad("Volante", "medeiroz", 100);

byte buttonPins[numOfButtons] = {
    4, 5, 15, 16, 17, 18,   // Left Buttons
    13, 14, 27, 26, 25, 33, // Right Buttons
    19,                     // Left Shifter
    21,                     // Right Shifter
};
byte physicalButtons[numOfButtons] = {
    1, 2, 3, 4, 5, 6,    // Left Buttons
    7, 8, 9, 10, 11, 12, // Right Buttons
    13,                  // Left Shifter
    14,                  // Right Shifter
};

void setup()
{
  for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++)
  {
    pinMode(buttonPins[currentPinIndex], INPUT_PULLUP);

    debouncers[currentPinIndex] = Bounce();
    debouncers[currentPinIndex].attach(buttonPins[currentPinIndex]); // After setting up the button, setup the Bounce instance :
    debouncers[currentPinIndex].interval(5);
  }

  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setButtonCount(numOfButtons);
  bleGamepadConfig.setAutoReport(false);
  bleGamepad.begin(&bleGamepadConfig);

  // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again

  Serial.begin(115200);
}

void loop()
{
  if (bleGamepad.isConnected())
  {
    bool sendReport = false;

    for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
    {
      debouncers[currentIndex].update();

      if (debouncers[currentIndex].fell())
      {
        bleGamepad.press(physicalButtons[currentIndex]);
        sendReport = true;
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" pushed.");
      }
      else if (debouncers[currentIndex].rose())
      {
        bleGamepad.release(physicalButtons[currentIndex]);
        sendReport = true;
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" released.");
      }
    }

    if (sendReport)
    {
      bleGamepad.sendReport();
    }

    // delay(20);	// (Un)comment to remove/add delay between loops
  }
}
