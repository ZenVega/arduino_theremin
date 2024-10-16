// #define LED_OUT 9
// #define JOY_X A0

// libraries
#include <arduino.h>
#include <math.h>
// pin definitions
#define PHONES 9
#define BUTTON_PIN 4
#define JOY_X 0
#define POTI_IN A3

// variable definitions
long val = 0;        // stores raw value from photocell
long maxread = 1023; // maximum value from calibration phase
long minread = 0;    // minimum value from calibration phase
double f = 0;        // frequency of sound
double normf = 0;    // normalized frequency
// double logf = 0;      // logarithm of normalized frequency
// int ilogf = 0;        // rounded logarithm
int i = 0;           // loop dummy variable
double factor = 0;   // scaling factor for calibration
double shift = 0;    // shift for calibration
long maxfreq = 2096; // maximum desired frequency after calibration
long minfreq = 131;  // minimum desired frequency after calibration

// magic numbers that make the intervals sound pleasing
// double gap = 1.148698355;  //ratio of consecutive notes (pentatonic)
//  it's the 5th root of 2
// double gap = 1.059463094;  //ratio of consecutive notes (chromatic)
//  its the 12th root of 2

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(10);
  factor = (double)(maxfreq - minfreq) / ((double)maxread - minread);
  shift = minfreq;
}

void loop()
{
  int sw_on = digitalRead(BUTTON_PIN);

  // normf = f / (double) minfreq;  // Dividing an exponential function by the min value
  // logf = log(normf) / log(gap); // allows us to take the log (base gap) and the result
  // ilogf = round(logf);           // is the number of notes above the lowest, once we round it.
  // f = minfreq * pow(gap,ilogf);  // we better "unlog" it.

  while (sw_on == 0)
  {
    //val = analogRead(JOY_X);
    val = maxread + 1 - (long)analogRead(POTI_IN);
    //val = analogRead(POTI_IN);
    val = pow(2, val * (10.0 / 1023.0));  // read photocell
    f = factor * val + shift; // this linearly maps the frequency to
    Serial.print("INPUT ");
    Serial.print(val);
    Serial.print("\n");
    tone(PHONES, f, 20); // this produces the tone signal
    sw_on = digitalRead(BUTTON_PIN);
  }
}