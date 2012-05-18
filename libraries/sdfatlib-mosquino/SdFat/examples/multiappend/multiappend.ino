/*
 * Append Example
 *
 * This sketch shows how to use open for append.
 * The sketch will append 100 line each time it opens the file.
 * The sketch will open and close the file 100 times.
 *
 * This example has been slightly modified for the Mosquino SD Shield 
 * to demonstrate shutting down power to the SD card between write sessions
 * via its SHDN signal.
 */
#include <SdFat.h>

// SD chip select pin
const uint8_t chipSelect = 0;
// SD Shutdown pin
const uint8_t shutdownPin = 1;

// file system object
SdFat sd;

// create Serial stream
ArduinoOutStream cout(Serial);

// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void setup() {
  // filename for this example
  char name[] = "APPEND.TXT";

  Serial.begin(9600);

  pinMode(shutdownPin, OUTPUT);
  digitalWrite(shutdownPin, HIGH); // disable SD power

  // pstr() stores strings in flash to save RAM
  cout << endl << pstr("Type any character to start\n");
  while (!Serial.available());


  cout << pstr("Appending to: ") << name;

  for (uint8_t i = 0; i < 100; i++) {
    // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
    // breadboards.  use SPI_FULL_SPEED for better performance.
    digitalWrite(shutdownPin, LOW); // enable SD power
    if (!sd.init(SPI_HALF_SPEED, chipSelect)) sd.initErrorHalt();

    // open stream for append
    ofstream sdout(name, ios::out | ios::app);
    if (!sdout) error("open failed");

    // append 100 lines to the file
    for (uint8_t j = 0; j < 100; j++) {
      // use int() so byte will print as decimal number
      sdout << "line " << int(j) << " of pass " << int(i);
      sdout << " millis = " << millis() << endl;
    }
    // close the stream
    sdout.close();

    if (!sdout) error("append data failed");

    // output progress indicator
    if (i % 25 == 0) cout << endl;
    cout << '.';
    digitalWrite(shutdownPin, HIGH); // disable SD power
    delay(1000);  // delay to help show the SD card really did get unpowered and correctly re-initialized
  }
  cout << endl << "Done" << endl;
}
//------------------------------------------------------------------------------
void loop() {}
