#include <Arduino.h>
#include <Bounce2.h>    // https://github.com/thomasfredericks/Bounce2
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad
#include <TickTwo.h> // https://github.com/sstaub/TickTwo

#define BOUNCE_WITH_PROMPT_DETECTION // Make button state changes available immediately

void batteryLoop();

#define PEDALS_SIMULATION false;

#define ACCELERATION_BUTTON 99  // Defina o botão 7 como acelerador
#define BRAKE_BUTTON 100         // Defina o botão 1 como freio

#define BATTERY_PIN 34 // Pino ADC conectado ao divisor de tensão
#define C_LED_PIN 14 // led indicativo que esta ligado

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

#define MIN_PEDALS -32767
#define MAX_PEDALS 32767

Bounce debouncers[numOfButtons];
BleGamepad bleGamepad("Volante F1", "medeiroz", 100);

byte buttonPins[numOfButtons] = {
    21, 19, 18, 5, 17, 16,   // Left Buttons
    13, 12, 32, 33, 25, 26, // Right Buttons
    23,                     // Left Shifter
    27,                     // Right Shifter
};
byte physicalButtons[numOfButtons] = {
    1, 2, 3, 4, 5, 6,             // Left Buttons 1 breaks
    7, 8, 9, 10, 11, 12,          // Right Buttons 7 acceleration
    13,                           // Left Shifter
    14,                           // Right Shifter
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
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);

  batteryTicker.start();

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

  bleGamepadConfig.setSimulationMin(MIN_PEDALS);
  bleGamepadConfig.setSimulationMax(MAX_PEDALS);

  bleGamepad.begin(&bleGamepadConfig);


  // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again

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

void gamepadLoop() {
  
  bool sendReport = false;

  for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
  {
    debouncers[currentIndex].update();

    if (PEDALS_SIMULATION && physicalButtons[currentIndex] == ACCELERATION_BUTTON)
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

    if (PEDALS_SIMULATION && physicalButtons[currentIndex] == BRAKE_BUTTON)
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
  
  delay(20);
}

// For PEDALS_SIMULATION
void acelerar() {
  for (int i = acceleration; i < MAX_PEDALS; i += 2048) {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MAX_PEDALS;
  report();
}

// for PEDALS_SIMULATION
void desacelerar() {
  for (int i = acceleration; i > MIN_PEDALS; i -= 2048) {
    acceleration = i;
    report();
    delay(2);
  }
  acceleration = MIN_PEDALS;
  report();
}

// for PEDALS_SIMULATION
void freiar() {
  for (int i = brake; i < MAX_PEDALS; i += 1024) {
    brake = i;
    report();
    delay(2);
  }
  brake = MAX_PEDALS;
  report();
}

// for PEDALS_SIMULATION
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
  bleGamepad.sendReport();

  if (PEDALS_SIMULATION) {
    bleGamepad.setAccelerator(acceleration);
    // bleGamepad.setThrottle(acceleration);
    bleGamepad.setBrake(brake);
    Serial.print("Report: ");
    Serial.print(" Acelerador: ");
    Serial.print(acceleration);
    Serial.print(" | Freio: ");
    Serial.print(brake);
    Serial.println("");
  }
}

void batteryLoop() {
  // Leitura da tensão no pino ADC
  int adcValue = analogRead(BATTERY_PIN);

  // Converta o valor ADC para a tensão lida no divisor de tensão
  float voltage = adcValue * (3.3 / 4095.0); // 3.3V é a tensão de referência do ADC

  // A tensão da bateria é o dobro da tensão lida (divisor de tensão com R1 = R2 = 150Ω)
  batteryVoltage = voltage * 2;

  // Cálculo da porcentagem da bateria (com base nos valores de 3.0V a 4.2V)
  batteryPercentage = (batteryVoltage - 3.0) / (4.2 - 3.0) * 100.0;

  // Limite a porcentagem entre 0 e 100
  if (batteryPercentage > 100) batteryPercentage = 100;
  if (batteryPercentage < 0) batteryPercentage = 0;

  batteryReport();
}

void batteryReport() {
  bleGamepad.setBatteryLevel(batteryPercentage);

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print(" V, ");
  Serial.print("Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.print(" %");
  Serial.print(", ADC value: ");
  Serial.print(adcValue);
  Serial.println("");
}

void ledLoop() {
  unsigned long currentMillis = millis();

  unsigned long actualLedInterval = ledState == LOW ? ledIntervalLow : ledIntervalHigh;

  if (currentMillis - ledPreviousMillis >= actualLedInterval) {
    ledPreviousMillis = currentMillis;

    ledState = ledState == LOW ? HIGH : LOW;
    digitalWrite(C_LED_PIN, ledState);

    if (ledState == LOW) {
      ledIntervalLow = bleGamepad.isConnected() && ledIntervalLow == ledInterval ? ledIntervalHigh : ledInterval;
    }
  }
}



