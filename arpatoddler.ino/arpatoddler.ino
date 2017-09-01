// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  2;      // the number of the LED pin
const long LOWEST_BPM = 30L;
const long HIGHEST_BPM = 220L;
const long LOWEST_BPM_INTERVAL = (60L * 1000 / (2 * LOWEST_BPM) );
const long HIGHEST_BPM_INTERVAL = (60L * 1000 / (2 * HIGHEST_BPM) );
const long MAX_ANALOG_READ = 1024L;

const int portPin = 0;

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
int analogVal = 0;
long bpm_interval = 0;

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);      

  Serial.begin(9600);
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();

  analogVal = analogRead(portPin);
  bpm_interval = LOWEST_BPM_INTERVAL + (HIGHEST_BPM_INTERVAL - LOWEST_BPM_INTERVAL) * analogVal / MAX_ANALOG_READ;
 
  if(currentMillis - previousMillis > bpm_interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    Serial.print(analogVal);
    Serial.print("\n");

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

