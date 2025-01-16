#include <EEPROM.h>
#include <Arduino.h>
#include <SimRacing.h> // https://github.com/dmadison/Sim-Racing-Arduino
#include <Joystick.h>  // https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Bounce2.h>   // https://github.com/thomasfredericks/Bounce2

#define EEPROM_MAGIC_VALUE 0xABCD1234

#define PIN_GAS A0
#define PIN_BRAKE A1
#define PIN_CLUTCH A3
#define PIN_LED 6
#define PIN_CALIBRATION_BUTTON 5
#define ADC_MAX 1023      // Max value of the analog inputs, 10-bit on AVR boards
#define ALWAYS_SEND false // Override the position checks, *always* send data constantly
#define LED_INTERVAL 100
#define MIN_PEDALS -32767
#define MAX_PEDALS 32767

SimRacing::TwoPedals pedals(PIN_GAS, PIN_BRAKE);
// SimRacing::ThreePedals pedals(PIN_GAS, PIN_BRAKE, PIN_CLUTCH);

Joystick_ joystick(
    JOYSTICK_DEFAULT_REPORT_ID,
    JOYSTICK_TYPE_JOYSTICK,
    0,
    0,
    false, false,
    pedals.hasPedal(SimRacing::Clutch),
    pedals.hasPedal(SimRacing::Brake),
    pedals.hasPedal(SimRacing::Gas),
    false, false, false, false, false, false);

bool ledState = LOW;
bool calibrationState = false;
bool onCalibration = false;
unsigned long ledPreviousMillis = 0;
Bounce debouncerCalibrateButton;

SimRacing::AnalogInput::Calibration gasCalibration;
SimRacing::AnalogInput::Calibration brakeCalibration;
SimRacing::AnalogInput::Calibration clutchCalibration;

float DeadzoneMin = 0.01;  // by default, 1% (trying to keep things responsive)
float DeadzoneMax = 0.025; // by default, 2.5%

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_CALIBRATION_BUTTON, INPUT_PULLUP);

  debouncerCalibrateButton.attach(PIN_CALIBRATION_BUTTON);
  debouncerCalibrateButton.interval(5);

  pedals.begin();

  Serial.println("Starting...");

  joystick.begin(false);
  joystick.setZAxisRange(0, ADC_MAX);
  joystick.setRxAxisRange(0, ADC_MAX);
  joystick.setRyAxisRange(0, ADC_MAX);

  loadCalibrationFromEEPROM();

  updateJoystick();
  resetCalibration();
}

void loop()
{
  // if (Serial.read() != -1)
  // {
  //   pedals.serialCalibration();
  //   delay(2000);
  // }

  pedals.update();

  ledLoop();
  calibrationButtonLoop();
  calibrationLoop();

  if (pedals.positionChanged() || ALWAYS_SEND)
  {
    updateJoystick();
  }

  delay(20);
}

void updateJoystick()
{
  if (pedals.hasPedal(SimRacing::Gas))
  {
    int gasPedal = pedals.getPosition(SimRacing::Gas, 0, ADC_MAX);
    joystick.setRyAxis(gasPedal);
  }

  if (pedals.hasPedal(SimRacing::Brake))
  {
    int brakePedal = pedals.getPosition(SimRacing::Brake, 0, ADC_MAX);
    joystick.setRxAxis(brakePedal);
  }

  if (pedals.hasPedal(SimRacing::Clutch))
  {
    int clutchPedal = pedals.getPosition(SimRacing::Clutch, 0, ADC_MAX);
    joystick.setZAxis(clutchPedal);
  }

  joystick.sendState();
  pedalPositionReport();
}

void pedalPositionReport()
{
  Serial.print("Pedals Position [ raw | % ]: ");

  if (pedals.hasPedal(SimRacing::Gas))
  {
    int gasPedal = pedals.getPosition(SimRacing::Gas);
    int gasPedalRaw = pedals.getPositionRaw(SimRacing::Gas);
    Serial.print("\tGas: [ ");
    Serial.print(gasPedalRaw);
    Serial.print(" | ");
    Serial.print(gasPedal);
    Serial.print("% ]");
  }

  if (pedals.hasPedal(SimRacing::Brake))
  {
    int brakePedal = pedals.getPosition(SimRacing::Brake);
    int brakePedalRaw = pedals.getPositionRaw(SimRacing::Brake);
    Serial.print("\tBrake: [ ");
    Serial.print(brakePedalRaw);
    Serial.print(" | ");
    Serial.print(brakePedal);
    Serial.print("% ]");
  }

  if (pedals.hasPedal(SimRacing::Clutch))
  {
    int clutchPedal = pedals.getPosition(SimRacing::Clutch);
    int clutchPedalRaw = pedals.getPositionRaw(SimRacing::Clutch);
    Serial.print("\tClutch: [ ");
    Serial.print(clutchPedalRaw);
    Serial.print(" | ");
    Serial.print(clutchPedal);
    Serial.print("% ]");
  }

  Serial.println();
}

void calibrationReport()
{
  Serial.print("Pedals Calibration [ min | max ]:");

  Serial.print("\tGas: [ ");
  Serial.print(gasCalibration.min);
  Serial.print(" | ");
  Serial.print(gasCalibration.max);
  Serial.print(" ]");

  Serial.print("\tBrake: [ ");
  Serial.print(brakeCalibration.min);
  Serial.print(" | ");
  Serial.print(brakeCalibration.max);
  Serial.print(" ]");

  Serial.print("\tClutch: [ ");
  Serial.print(clutchCalibration.max);
  Serial.print(" | ");
  Serial.print(clutchCalibration.min);
  Serial.print(" ]");

  Serial.println("");
}

void ledLoop()
{
  unsigned long currentMillis = millis();

  if (onCalibration)
  {
    if (currentMillis - ledPreviousMillis >= LED_INTERVAL)
    {
      ledPreviousMillis = currentMillis;
      ledState = ledState == LOW ? HIGH : LOW;
      digitalWrite(PIN_LED, ledState);
    }
  }
  else if (calibrationState != ledState)
  {
    digitalWrite(PIN_LED, calibrationState);
  }
}

void calibrationButtonLoop()
{
  debouncerCalibrateButton.update();
  if (debouncerCalibrateButton.fell())
  {
    onCalibration ? finishCalibration() : startCalibration();
  }
}

void calibrationLoop()
{
  if (onCalibration)
  {
    if (pedals.hasPedal(SimRacing::Gas))
    {
      int gasPedal = pedals.getPositionRaw(SimRacing::Gas);
      if (gasPedal > gasCalibration.max)
      {
        gasCalibration.max = gasPedal;
      }
      if (gasPedal < gasCalibration.min)
      {
        gasCalibration.min = gasPedal;
      }
    }

    if (pedals.hasPedal(SimRacing::Brake))
    {
      int brakePedal = pedals.getPositionRaw(SimRacing::Brake);
      if (brakePedal > brakeCalibration.max)
      {
        brakeCalibration.max = brakePedal;
      }
      if (brakePedal < brakeCalibration.min)
      {
        brakeCalibration.min = brakePedal;
      }
    }

    if (pedals.hasPedal(SimRacing::Clutch))
    {
      int clutchPedal = pedals.getPositionRaw(SimRacing::Clutch);
      if (clutchPedal > clutchCalibration.max)
      {
        clutchCalibration.max = clutchPedal;
      }
      if (clutchPedal < clutchCalibration.min)
      {
        clutchCalibration.min = clutchPedal;
      }
    }
  }
}

void startCalibration()
{
  Serial.println("Starting Calibration");
  onCalibration = true;
  calibrationState = false;
  resetCalibration();
}

void finishCalibration()
{
  onCalibration = false;
  calibrationState = true;

  disccountDeadZone(gasCalibration);
  disccountDeadZone(brakeCalibration);
  disccountDeadZone(clutchCalibration);

  pedals.setCalibration(gasCalibration, brakeCalibration);
  // pedals.setCalibration(gasCalibration, brakeCalibration, clutchCalibration);

  Serial.println("Has been calibrated");

  calibrationReport();

  saveCalibrationToEEPROM();
}

void resetCalibration()
{
  gasCalibration.max = MIN_PEDALS;
  gasCalibration.min = MAX_PEDALS;

  brakeCalibration.max = MIN_PEDALS;
  brakeCalibration.min = MAX_PEDALS;

  clutchCalibration.max = MIN_PEDALS;
  clutchCalibration.min = MAX_PEDALS;
}

void disccountDeadZone(SimRacing::AnalogInput::Calibration &pedalCalibration)
{
  const int range = abs(pedalCalibration.max - pedalCalibration.min);
  const int dzMin = DeadzoneMin * (float)range;
  const int dzMax = DeadzoneMax * (float)range;

  // non-inverted
  if (pedalCalibration.max >= pedalCalibration.min)
  {
    pedalCalibration.max -= dzMax; // 'cut' into the range so it limits sooner
    pedalCalibration.min += dzMin;
  }
  // inverted
  else
  {
    pedalCalibration.max += dzMax;
    pedalCalibration.min -= dzMin;
  }
}

void saveCalibrationToEEPROM()
{
  EEPROM.put(0, EEPROM_MAGIC_VALUE);

  EEPROM.put(sizeof(EEPROM_MAGIC_VALUE), gasCalibration);
  EEPROM.put(sizeof(EEPROM_MAGIC_VALUE) + sizeof(gasCalibration), brakeCalibration);
  EEPROM.put(sizeof(EEPROM_MAGIC_VALUE) + sizeof(gasCalibration) + sizeof(brakeCalibration), clutchCalibration);

  Serial.println("Calibration saved to EEPROM");
}

void loadCalibrationFromEEPROM()
{
  uint32_t magicValue;
  EEPROM.get(0, magicValue);

  // Verifica se o valor de verificação está correto
  if (magicValue == EEPROM_MAGIC_VALUE)
  {
    EEPROM.get(sizeof(magicValue), gasCalibration);
    EEPROM.get(sizeof(magicValue) + sizeof(gasCalibration), brakeCalibration);
    EEPROM.get(sizeof(magicValue) + sizeof(gasCalibration) + sizeof(brakeCalibration), clutchCalibration);

    calibrationState = true;
    pedals.setCalibration(gasCalibration, brakeCalibration);
    // pedals.setCalibration(gasCalibration, brakeCalibration, clutchCalibration);

    Serial.println("Calibration loaded from EEPROM");
    calibrationReport();
  }
  else
  {
    Serial.println("No valid calibration data found in EEPROM.");
    startCalibration();
  }
}

void clearEEPROM()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0xFF); // Ou use 0x00 para zerar a EEPROM
  }
  Serial.println("EEPROM cleared.");
}