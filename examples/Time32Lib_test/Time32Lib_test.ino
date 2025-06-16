#include <Arduino.h>

/* 
 * Time32Lib_test.ino
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by digits time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 * T1357041600
 * 
 * Typing letter D the clock will set at EPOCH TIME: 2005949145590 Human: 23:59:50 - 31/12/65535 GMT
 * Wait ten seconds to see if 65535 year problem is solved.
 * 
 * Message consist of the letter S followed by digits year (S2025) the coclk will be set at 
 * 00:00:00 - Jan 1 of the give year. 
 *
 * A Processing example sketch to automatically send the messages is included in the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 */ 

#define USE_UINT64_T 1  // USE_UINT64_T 1 only on ESP32 to fix 2106 problem.

#include "TimeLib32.h"

#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_SET_DEFAULT 'D' // Header tag for serial time sync message time default
#define TIME_SET_TIME 'S' // Header tag for serial time sync message time default
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print("EPOCH TIME: ");
  Serial.print(now());
  Serial.print(" Human: ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" - ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year()); 
  Serial.println(" GMT"); 
}

void processSyncMessage() {
  const time32_t DEFAULT_TIME = 2005949145590UL; // Dec 31 65535 23:59:50
  char ct = Serial.read();
  if(ct == TIME_HEADER) {
    time32_t time_serial=0;
    while(Serial.available()) {
      char incomingChar = Serial.read();
    
      if (incomingChar >= '0' && incomingChar <= '9')
         time_serial = (time_serial * 10) + (incomingChar - '0');
    }
    setTime(time_serial); // Sync Arduino clock to the time received on the serial port
  } else 
  if(ct == TIME_SET_DEFAULT) {
    setTime(DEFAULT_TIME);
  } else
  if(ct == TIME_SET_TIME) {
    uint32_t time_serial=0;
    while(Serial.available()) {
      char incomingChar = Serial.read();
    
      if (incomingChar >= '0' && incomingChar <= '9')
         time_serial = (time_serial * 10) + (incomingChar - '0');
    }
    setTime(00,00,00,01,01,time_serial);
  } 

}

time32_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop(){    
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  delay(1000);
}
