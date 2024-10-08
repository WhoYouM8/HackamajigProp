//Using and Arduino Mega 2560 in order to have enough pins

//Libraries Used
#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

//TMRpcm and SD Library Setups
TMRpcm audio;    // Create an object for the TMRpcm library
const int chipSelect = 10; // SD card module chip select pin

//Pre Setup Section
//PIN ASSIGNMENTS (Change the number to the corresponding pin you decide to use)
//  LED Pins (Only requires basic Degital Pins)
const int led1 = 4;
const int led2 = 5;
const int led3 = 6;
const int led4 = 7;
const int led5 = 8;
//  Button Pins. (Only Requires Basic Digital Pins)
const int button1 = 22;
const int button2 = 23;
const int button3 = 24;
const int button4 = 25;
//  Servo Pins (Requires PWM pins.)
const int servoPin = 3; //Must be a PWM pin.
//  Speaker Pins
const int speakerPin1 = 9;  // Set the pin for the speaker. Must be a PWM pin linked to Timer 1 on the board. (Most Arduino's this means pin 9 or 10)

//Global Variables
const int servoSpeed = 100; // Can be any value from 0-255. 0 is stopped 255 is full speed.
unsigned long currentMillis = 0;

//Audio File Names
char button1Audio[] = "button1audio.wav";  // Replace the text inside the quotes with the actual file names on the root directory of SD card.
char button2Audio[] = "button2audio.wav";  // Replace the text inside the quotes with the actual file names on the root directory of SD card.
char button3Audio[] = "button3audio.wav";  // Replace the text inside the quotes with the actual file names on the root directory of SD card.
char button4Audio[] = "button4audio.wav";  // Replace the text inside the quotes with the actual file names on the root directory of SD card.

//Button Press Millis Setup
const long button1Interval = 10000; // Set To length of WAV file for this button
const long button2Interval = 10000; // Set To length of WAV file for this button
const long button3Interval = 10000; // Set To length of WAV file for this button
const long button4Interval = 10000; // Set To length of WAV file for this button
unsigned long button1PrevMillis = 0;
unsigned long button2PrevMillis = 0;
unsigned long button3PrevMillis = 0;
unsigned long button4PrevMillis = 0;

//Button Active Variables
bool button1Active = false;
bool button2Active = false;
bool button3Active = false;
bool button4Active = false;
bool anyButtonActive = false;

//LED Millis Setup
int ledState = 0;
const long led1Interval = 500;
const long led2Interval = 500;
const long led3Interval = 500;
const long led4Interval = 500;
unsigned long ledPrevMillis = 0;

//Setup and initialize. This only runs once.
void setup()
{
  Serial.begin(9600);

  //Initializes the SD card and checks to make sure it was successful.
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card initialization failed.");
    return;
  }
  Serial.println("SD card ready.");

  //Setup Pin Modes
  //  Output Pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(servoPin, OUTPUT);
  //  Input Pins
  pinMode(button1, INPUT_PULLUP); //Only use the _PULLUP part if you don't wire your own resistors inbetween the Button and 5v pin. (this goes for all buttons)
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  //Audio Output Setup
  audio.speakerPin = speakerPin1;
  audio.setVolume(6); // Set Volume (0-7)
  audio.quality(1); // Default Quality= 0, Higher Quality = 1
}

//This loop is what runs continuously over and over.
void loop()
{

  //Get Current Millis
  currentMillis = millis();

  //Check if any button is active
  if (button1Active || button2Active || button3Active || button4Active)
  {
    anyButtonActive = true;
  }
  else
  {
    anyButtonActive = false;
  }

  //Checks to see if timer has ran out on the Audio file (activeButton). If so, it stops everything.
  if (button1Active && currentMillis - button1PrevMillis >= button1Interval)
  {
    stopAllActions();
  }
  else if (button2Active && currentMillis - button2PrevMillis >= button2Interval)
  {
    stopAllActions();
  }
  else if (button3Active && currentMillis - button3PrevMillis >= button3Interval)
  {
    stopAllActions();
  }
  else if (button4Active && currentMillis - button4PrevMillis >= button4Interval)
  {
    stopAllActions();
  }

  //Starts the corresponding LED pattern depending on active button
  if (button1Active && currentMillis - ledPrevMillis >= led1Interval)
  {
    led1Sequence();
  }
  else if (button2Active && currentMillis - ledPrevMillis >= led2Interval)
  {
    led2Sequence();
  }
  else if (button3Active && currentMillis - ledPrevMillis >= led3Interval)
  {
    led3Sequence();
  }
  else if (button4Active && currentMillis - ledPrevMillis >= led4Interval)
  {
    led4Sequence();
  }

  //Check For Button Presses
  bool button1Press = !digitalRead(button1);
  bool button2Press = !digitalRead(button2);
  bool button3Press = !digitalRead(button3);
  bool button4Press = !digitalRead(button4);

  if (button1Press)
  {
    button1Pressed();
  }
  else if (button2Press)
  {
    button2Pressed();
  }
  else if (button3Press)
  {
    button3Pressed();
  }
  else if (button4Press)
  {
    button4Pressed();
  }
}

//Custom Functions (States) That are called in the loop function.
void button1Pressed()
{
  //The below "if" statement is a failsafe to prevent buttons from overriding or doubling onto each other. It will stop everything before starting a new button press action.
  if (anyButtonActive)
  {
    stopAllActions();
  }
  button1PrevMillis = millis();
  button1Active = true;
  analogWrite(servoPin, servoSpeed);
  audio.play(button1Audio); // Plays the WAV file (must be on the SD card)
}

void button2Pressed()
{
  if (anyButtonActive)
  {
    stopAllActions();
  }
  button2PrevMillis = millis();
  button2Active = true;
  analogWrite(servoPin, servoSpeed);
  audio.play(button2Audio);
}

void button3Pressed()
{
  if (anyButtonActive)
  {
    stopAllActions();
  }
  button3PrevMillis = millis();
  button3Active = true;
  analogWrite(servoPin, servoSpeed);
  audio.play(button3Audio);
}

void button4Pressed()
{
  if (anyButtonActive)
  {
    stopAllActions();
  }
  button4PrevMillis = millis();
  button4Active = true;
  analogWrite(servoPin, servoSpeed);
  audio.play(button4Audio);
}

void led1Sequence()  //Edit each case state or add or subtract a case state to control what you want to happen at each interval of the led sequence.
{
  switch (ledState)
  {
    case 0:
      ledPat(1, 0, 0, 0, 0);  // 0 = LOW or OFF and 1 = HIGH or ON.
      ledState = 1;
      break;

    case 1:
      ledPat(1, 1, 0, 0, 0);
      ledState = 2;
      break;

    case 2:
      ledPat(1, 1, 1, 0, 0);
      ledState = 3;
      break;

    case 3:
      ledPat(1, 1, 1, 1, 0);
      ledState = 4;
      break;

    case 4:
      ledPat(0, 1, 1, 1, 1);
      ledState = 5;
      break;

    case 5:
      ledPat(0, 0, 1, 1, 1);
      ledState = 6;
      break;

    case 6:
      ledPat(0, 0, 0, 1, 1);
      ledState = 7;
      break;

    case 7:
      ledPat(0, 0, 0, 0, 1);
      ledState = 0;
      break;
  }

  ledPrevMillis = currentMillis; //Resets the interval
}

void led2Sequence()
{
  switch (ledState)
  {
    case 0:
      ledPat(1, 0, 0, 0, 0);
      ledState = 1;
      break;

    case 1:
      ledPat(1, 1, 0, 0, 0);
      ledState = 2;
      break;

    case 2:
      ledPat(0, 1, 1, 0, 0);
      ledState = 3;
      break;

    case 3:
      ledPat(0, 0, 1, 1, 0);
      ledState = 4;
      break;

    case 4:
      ledPat(0, 0, 0, 1, 1);
      ledState = 5;
      break;

    case 5:
      ledPat(0, 0, 1, 1, 0);
      ledState = 6;
      break;

    case 6:
      ledPat(0, 1, 1, 0, 0);
      ledState = 1;
      break;
  }

  ledPrevMillis = currentMillis; //Resets the interval
}

void led3Sequence()
{
  switch (ledState)
  {
    case 0:
      ledPat(1, 0, 0, 0, 0);
      ledState = 1;
      break;

    case 1:
      ledPat(1, 1, 0, 0, 0);
      ledState = 2;
      break;

    case 2:
      ledPat(1, 1, 1, 0, 0);
      ledState = 3;
      break;

    case 3:
      ledPat(1, 1, 1, 1, 0);
      ledState = 4;
      break;

    case 4:
      ledPat(0, 1, 1, 1, 1);
      ledState = 5;
      break;

    case 5:
      ledPat(0, 0, 1, 1, 1);
      ledState = 6;
      break;

    case 6:
      ledPat(0, 0, 0, 1, 1);
      ledState = 7;
      break;

    case 7:
      ledPat(0, 0, 0, 0, 1);
      ledState = 0;
      break;
  }

  ledPrevMillis = currentMillis; //Resets the interval
}

void led4Sequence()
{
  switch (ledState)
  {
    case 0:
      ledPat(1, 0, 0, 0, 0);
      ledState = 1;
      break;

    case 1:
      ledPat(1, 1, 0, 0, 0);
      ledState = 2;
      break;

    case 2:
      ledPat(1, 1, 1, 0, 0);
      ledState = 3;
      break;

    case 3:
      ledPat(1, 1, 1, 1, 0);
      ledState = 4;
      break;

    case 4:
      ledPat(0, 1, 1, 1, 1);
      ledState = 5;
      break;

    case 5:
      ledPat(0, 0, 1, 1, 1);
      ledState = 6;
      break;

    case 6:
      ledPat(0, 0, 0, 1, 1);
      ledState = 7;
      break;

    case 7:
      ledPat(0, 0, 0, 0, 1);
      ledState = 0;
      break;
  }

  ledPrevMillis = currentMillis; //Resets the interval
}

//This Function will simplify the LED pattern control
void ledPat(int led1o, int led2o, int led3o, int led4o, int led5o)
{
  digitalWrite(led1, (led1o == 1) ? HIGH : LOW);
  digitalWrite(led2, (led2o == 1) ? HIGH : LOW);
  digitalWrite(led3, (led3o == 1) ? HIGH : LOW);
  digitalWrite(led4, (led4o == 1) ? HIGH : LOW);
  digitalWrite(led5, (led5o == 1) ? HIGH : LOW);
}

//The Function below stops the servo, the audio and turns off all the LEDs.
void stopAllActions()
{
  analogWrite(servoPin, 0);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  audio.stopPlayback();
  button1Active = false;
  button2Active = false;
  button3Active = false;
  button4Active = false;
  ledState = 0;
}