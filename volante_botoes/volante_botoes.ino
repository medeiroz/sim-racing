#include <Arduino.h>
#include <Bounce2.h>    // https://github.com/thomasfredericks/Bounce2
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad
#include <TickTwo.h>    // https://github.com/sstaub/TickTwo

#define BOUNCE_WITH_PROMPT_DETECTION // Make button state changes available immediately

void batteryLoop();

#define PEDALS_SIMULATION false

#define ACCELERATION_BUTTON 7 // Define button 7 as accelerator
#define BRAKE_BUTTON 1        // Define button 1 as brake

#define BATTERY_PIN 34 // ADC pin connected to the voltage divider
#define C_LED_PIN 14   // LED indicating that it is on

#define NUM_OF_BUTTONS 14
#define NUM_OF_HAT_SWITCHES 0
#define ENABLE_X false
#define ENABLE_Y false
#define ENABLE_Z false
#define ENABLE_RX false
#define ENABLE_RY false
#define ENABLE_RZ false
#define ENABLE_SLIDER_1 false
#define ENABLE_SLIDER_2 false
#define ENABLE_RUDDER false
#define ENABLE_THROTTLE false
#define ENABLE_ACCELERATOR false
#define ENABLE_BRAKE false
#define ENABLE_STEERING false

#define MIN_PEDALS -32767
#define MAX_PEDALS 32767

Bounce debouncers[NUM_OF_BUTTONS];
BleGamepad bleGamepad("F1 Steering Wheel", "medeiroz", 100);

byte buttonPins[NUM_OF_BUTTONS] = {
    21, 19, 18, 5, 17, 16,  // Left Buttons
    13, 12, 32, 33, 25, 26, // Right Buttons
    23,                     // Left Shifter
    27                      // Right Shifter
};
byte physicalButtons[NUM_OF_BUTTONS] = {
    1, 2, 3, 4, 5, 6,    // Left Buttons 1 breaks
    7, 8, 9, 10, 11, 12, // Right Buttons 7 acceleration
    13,                  // Left Shifter
    14                   // Right Shifter
};

int acceleration = MIN_PEDALS;
int brake = MIN_PEDALS;

float batteryVoltage = 0;
float batteryPercentage = 0;

unsigned long ledPreviousMillis = 0;
unsigned long ledInterval = 5000;
unsigned long ledIntervalHigh = 100;
unsigned long ledIntervalLow = ledInterval;
bool ledState = LOW;

TickTwo batteryTicker(batteryLoop, 1000 * 10); // 10s

void setup()
{
  pinMode(C_LED_PIN, OUTPUT);

  batteryTicker.start();

  for (byte currentPinIndex = 0; currentPinIndex < NUM_OF_BUTTONS; currentPinIndex++)
  {
    pinMode(buttonPins[currentPinIndex], INPUT_PULLUP);

    debouncers[currentPinIndex] = Bounce();
    debouncers[currentPinIndex].attach(buttonPins[currentPinIndex]); // After setting up the button, set up the Bounce instance:
    debouncers[currentPinIndex].interval(5);
  }

  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setButtonCount(NUM_OF_BUTTONS);
  bleGamepadConfig.setWhichAxes(ENABLE_X, ENABLE_Y, ENABLE_Z, ENABLE_RX, ENABLE_RY, ENABLE_RZ, ENABLE_SLIDER_1, ENABLE_SLIDER_2);
  bleGamepadConfig.setWhichSimulationControls(ENABLE_RUDDER, ENABLE_THROTTLE, ENABLE_ACCELERATOR, ENABLE_BRAKE, ENABLE_STEERING);
  bleGamepadConfig.setHatSwitchCount(NUM_OF_HAT_SWITCHES);

  bleGamepadConfig.setSimulationMin(MIN_PEDALS);
  bleGamepadConfig.setSimulationMax(MAX_PEDALS);

  bleGamepad.begin(&bleGamepadConfig);

  Serial.begin(115200);
}

void loop()
{
  ledLoop();

  if (bleGamepad.isConnected())
  {
    gamepadLoop();
    batteryTicker.update();
  }

  delay(20);
}

void gamepadLoop()
{
  bool sendReport = false;

  for (byte currentIndex = 0; currentIndex < NUM_OF_BUTTONS; currentIndex++)
  {
    debouncers[currentIndex].update();

    if (PEDALS_SIMULATION && physicalButtons[currentIndex] == ACCELERATION_BUTTON)
    {
      if (debouncers[currentIndex].fell())
      {
        Serial.println("Accelerator pressed");
        sendReport = true;
        accelerate();
      }
      if (debouncers[currentIndex].rose())
      {
        Serial.println("Accelerator released");
        sendReport = true;
        decelerate();
      }
    }

    if (PEDALS_SIMULATION && physicalButtons[currentIndex] == BRAKE_BUTTON)
    {
      if (debouncers[currentIndex].fell())
      {
        Serial.println("Brake pressed");
        sendReport = true;
        brakeAction();
      }
      if (debouncers[currentIndex].rose())
      {
        Serial.println("Brake released");
        sendReport = true;
        releaseBrake();
      }
    }

    // OTHER BUTTONS
    if (!PEDALS_SIMULATION || (physicalButtons[currentIndex] != ACCELERATION_BUTTON && physicalButtons[currentIndex] != BRAKE_BUTTON))
    {
      if (debouncers[currentIndex].fell())
      {
        bleGamepad.press(physicalButtons[currentIndex]);
        sendReport = true;
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" pressed.");
      }

      if (debouncers[currentIndex].rose())
      {
        bleGamepad.release(physicalButtons[currentIndex]);
        sendReport = true;
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" released.");
      }
    }
  }

  if (sendReport)
  {
    report();
  }

  delay(20);
}

// For PEDALS_SIMULATION
void accelerate()
{
  for (int i = acceleration; i < MAX_PEDALS; i += 2048)
  {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MAX_PEDALS;
  report();
}

// for PEDALS_SIMULATION
void decelerate()
{
  for (int i = acceleration; i > MIN_PEDALS; i -= 2048)
  {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MIN_PEDALS;
  report();
}

// for PEDALS_SIMULATION
void brakeAction()
{
  for (int i = brake; i < MAX_PEDALS; i += 1024)
  {
    brake = i;
    report();
    delay(2);
  }
  brake = MAX_PEDALS;
  report();
}

// for PEDALS_SIMULATION
void releaseBrake()
{
  for (int i = brake; i > MIN_PEDALS; i -= 1024)
  {
    brake = i;
    report();
    delay(2);
  }
  brake = MIN_PEDALS;
  report();
}

void report()
{
  bleGamepad.sendReport();

  if (PEDALS_SIMULATION)
  {
    bleGamepad.setAccelerator(acceleration);
    bleGamepad.setBrake(brake);
    Serial.print("Report: ");
    Serial.print(" Accelerator: ");
    Serial.print(acceleration);
    Serial.print(" | Brake: ");
    Serial.print(brake);
    Serial.println("");
  }
}

void batteryLoop()
{
  int adcValue = analogRead(BATTERY_PIN);
  float voltage = adcValue * (3.3 / 4095.0); // 3.3V is the reference voltage of the ADC
  batteryVoltage = voltage * 2;              // Battery voltage is double the read voltage (voltage divider with R1 = R2 = 150Ω)

  batteryPercentage = (batteryVoltage - 3.0) / (4.2 - 3.0) * 100.0; // Battery percentage calculation based on 3.0V to 4.2V range

  if (batteryPercentage > 100)
    batteryPercentage = 100;
  if (batteryPercentage < 0)
    batteryPercentage = 0;

  batteryReport();
}

void batteryReport()
{
  bleGamepad.setBatteryLevel(batteryPercentage);

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print(" V, ");
  Serial.print("Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.print(" %");
  Serial.print(", ADC value: ");
  Serial.print(analogRead(BATTERY_PIN));
  Serial.println("");
}

void ledLoop()
{
  unsigned long currentMillis = millis();
  unsigned long actualLedInterval = ledState == LOW ? ledIntervalLow : ledIntervalHigh;

  if (currentMillis - ledPreviousMillis >= actualLedInterval)
  {
    ledPreviousMillis = currentMillis;
    ledState = ledState == LOW ? HIGH : LOW;
    digitalWrite(C_LED_PIN, ledState);

    if (ledState == LOW)
    {
      ledIntervalLow = bleGamepad.isConnected() && ledIntervalLow == ledInterval ? ledIntervalHigh : ledInterval;
    }
  }
}
