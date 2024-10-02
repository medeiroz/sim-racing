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

#define ACCELERATION_BUTTON 7  // Defina o botão 7 como acelerador
#define BRAKE_BUTTON 1         // Defina o botão 1 como freio

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
#define enableThrottle true
#define enableAccelerator true
#define enableBrake true
#define enableSteering false

#define MIN_PEDALS -32767
#define MAX_PEDALS 32767


Bounce debouncers[numOfButtons];
BleGamepad bleGamepad("Volante", "medeiroz", 100);

byte buttonPins[numOfButtons] = {
    4 , 5, 15, 16, 17, 18,   // Left Buttons
    13 , 14, 27, 26, 25, 33, // Right Buttons
    19,                     // Left Shifter
    21,                     // Right Shifter
};
byte physicalButtons[numOfButtons] = {
    5, 3, 1, 6, 2, 4,             // Left Buttons 1 breaks
    7, 10, 8, 12, 9, 11,          // Right Buttons 7 acceleration
    14,                           // Left Shifter
    13,                           // Right Shifter
};

int acceleration = MIN_PEDALS;
int brake = MIN_PEDALS;

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
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setButtonCount(numOfButtons);
  bleGamepadConfig.setWhichAxes(enableX, enableY, enableZ, enableRX, enableRY, enableRZ, enableSlider1, enableSlider2);
  bleGamepadConfig.setWhichSimulationControls(enableRudder, enableThrottle, enableAccelerator, enableBrake, enableSteering);
  bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);

  bleGamepadConfig.setSimulationMin(-32767);
  bleGamepadConfig.setSimulationMax(32767);

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

      if (physicalButtons[currentIndex] == ACCELERATION_BUTTON)
      {
        if (debouncers[currentIndex].fell()) {
          Serial.println("Acelerador pressionado");
          sendReport = true;
          acelerar();
        }
        if (debouncers[currentIndex].rose()) {
          Serial.println("Acelerador Solto");
          sendReport = true;
          desacelerar();
        }
      }

      if (physicalButtons[currentIndex] == BRAKE_BUTTON)
      {
        if (debouncers[currentIndex].fell()) {
          Serial.println("Freio pressionado");
          sendReport = true;
          freiar();
        }
        if (debouncers[currentIndex].rose()) {
          Serial.println("Freio solto");
          sendReport = true;
          desfreiar();
        }
      }

      // OUTROS BOTOES
      if (physicalButtons[currentIndex] != ACCELERATION_BUTTON && physicalButtons[currentIndex] != BRAKE_BUTTON)
      {
          if (debouncers[currentIndex].fell())
          {
            bleGamepad.press(physicalButtons[currentIndex]);
            sendReport = true;
            Serial.print("Button ");
            Serial.print(physicalButtons[currentIndex]);
            Serial.println(" pressionado.");
          }
          
          
          if (debouncers[currentIndex].rose())
          {
            bleGamepad.release(physicalButtons[currentIndex]);
            sendReport = true;
            Serial.print("Button ");
            Serial.print(physicalButtons[currentIndex]);
            Serial.println(" liberado.");
          }
      }
    }

    if (sendReport)
    {
      report();
    }
    

    // delay(20);	// (Un)comment to remove/add delay between loops
  }
}

void acelerar() {
  for (int i = acceleration; i < MAX_PEDALS; i += 1024) {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MAX_PEDALS;
  report();
}

void desacelerar() {
  for (int i = acceleration; i > MIN_PEDALS; i -= 1024) {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MIN_PEDALS;
  report();
}

void freiar() {
  for (int i = brake; i < MAX_PEDALS; i += 1024) {
    brake = i;
    report();
    delay(2);
  }
  brake = MAX_PEDALS;
  report();
}

void desfreiar() {
  for (int i = brake; i > MIN_PEDALS; i -= 1024) {
    brake = i;
    report();
    delay(2);
  }
  brake = MIN_PEDALS;
  report();
}

void report() {
  bleGamepad.setAccelerator(acceleration);
  // bleGamepad.setThrottle(acceleration);
  bleGamepad.setBrake(brake);
  bleGamepad.sendReport();
  Serial.print("Report: ");
  Serial.print(" Acelerador: ");
  Serial.print(acceleration);
  Serial.print(" | Freio: ");
  Serial.print(brake);
  Serial.println("");
}
