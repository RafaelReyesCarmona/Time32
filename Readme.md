<img src="images/icons8-time-machine-48.png" width=48 height=48 align=right>

# Time32 Library
[![Version: v1.1.4](https://img.shields.io/badge/Version-v1.1.4-blue?style=for-the-badge&logo=v)]()

Time32 is a library that provides timekeeping functionality for Arduino and ESP32.

Time32 is an adapted code from Time by *Michael Margolis*.

The Time code is derived from the Playground DateTime library but is updated
to provide an API that is more flexible and easier to use.

A primary goal was to enable date and time functionality that can be used with
a variety of external time sources with minimum differences required in sketch logic.

Time32 (v1.1.3) introduces changes to prevent "conflicting declaration 'typedef time_t'" on ESP32 
enviroments. It define new typedef time32_t to avoid conflict with newlib or other libs.
And it add support to fix 2106 problem when use a 32 mcu like ESP32. The limit is 65535 year. When use
please set a max timestamp of 2005949145599. (Thanks to Francois Victor "rmslu" fvictor@gmail.lu for the test).

New version of Time32 extend support at the end of 4294967295 year. Added a new method to calculate time
translating unix time to gregorian calendar so fastest.

Use "Time32Lib_test.ino" on example directory for test. T135536014634284790
```
EPOCH TIME: 135536014634284790 Human: 23:59:50 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284791 Human: 23:59:51 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284792 Human: 23:59:52 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284793 Human: 23:59:53 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284794 Human: 23:59:54 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284795 Human: 23:59:55 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284796 Human: 23:59:56 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284797 Human: 23:59:57 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284798 Human: 23:59:58 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284799 Human: 23:59:59 - 31/12/4294967295 GMT
EPOCH TIME: 135536014634284800 Human: 0:00:00 - 1/1/0 GMT
EPOCH TIME: 135536014634284801 Human: 0:00:01 - 1/1/0 GMT
EPOCH TIME: 135536014634284802 Human: 0:00:02 - 1/1/0 GMT
EPOCH TIME: 135536014634284803 Human: 0:00:03 - 1/1/0 GMT
EPOCH TIME: 135536014634284804 Human: 0:00:04 - 1/1/0 GMT
EPOCH TIME: 135536014634284805 Human: 0:00:05 - 1/1/0 GMT
EPOCH TIME: 135536014634284806 Human: 0:00:06 - 1/1/0 GMT
EPOCH TIME: 135536014634284807 Human: 0:00:07 - 1/1/0 GMT

```

New function is implemented to calc leap seconds.
Info from https://www.iana.org/time-zones Version 2025b tzdb-2025b.tar.lz. Leap seconds info
is valid until 28th December of 2025 00:00:00 UTC.

Example sketches are updated to illustrate how similar sketch code can be used with: 
a Real Time Clock, internet NTP time service, GPS time data, and Serial time messages 
from a computer for time synchronization.

## Functionality

To use the Time library in an Arduino sketch, include TimeLib32.h.

```c
#include <TimeLib32.h>
```

The functions available in the library include

```c
hour();            // the hour now  (0-23)
minute();          // the minute now (0-59)
second();          // the second now (0-59)
day();             // the day now (1-31)
weekday();         // day of the week (1-7), Sunday is day 1
month();           // the month now (1-12)
year();            // the full digits year: from 1970 to 4294967295 
```

there are also functions to return the hour in 12-hour format

```c
hourFormat12();    // the hour now in 12 hour format
isAM();            // returns true if time now is AM
isPM();            // returns true if time now is PM

now();             // returns the current time as seconds since Jan 1 1970
```

The time and date functions can take an optional parameter for the time. This prevents
errors if the time rolls over between elements. For example, if a new minute begins
between getting the minute and second, the values will be inconsistent. Using the
following functions eliminates this problem

```c
time32_t t = now(); // store the current time in time variable t
hour(t);          // returns the hour for the given time t
minute(t);        // returns the minute for the given time t
second(t);        // returns the second for the given time t
day(t);           // the day for the given time t
weekday(t);       // day of the week for the given time t
month(t);         // the month for the given time t
year(t);          // the year for the given time t
leap_seconds(t);  // returns leap seconds for the given time t
```

Functions for managing the timer services are:

```c
setTime(t);                      // set the system time to the give time t
setTime(hr,min,sec,day,mnth,yr); // alternative to above, yr is 2 or 4 digit yr
                                 // (2010 or 10 sets year to 2010)
adjustTime(adjustment);          // adjust system time by adding the adjustment value
timeStatus(); // indicates if time has been set and recently synchronized
              // returns one of the following enumerations:
              - timeNotSet    // the time has never been set, the clock started on Jan 1, 1970
              - timeNeedsSync // the time had been set but a sync attempt did not succeed
              - timeSet       // the time is set and is synced
```

Time and Date values are not valid if the status is `timeNotSet`. Otherwise, values can be used but
the returned time may have drifted if the status is `timeNeedsSync`. 	

```c
setSyncProvider(getTimeFunction);  // set the external time provider
setSyncInterval(interval);         // set the number of seconds between re-sync
```

There are many convenience macros in the `TimeLib32.h` file for time constants and conversion
of time units.

To use the library, copy the download to the Library directory.

## Examples

The Time32 directory contains the Time32 library and some example sketches
illustrating how the library can be used with various time sources:

- `TimeSerial.pde` shows Arduino as a clock without external hardware.
  It is synchronized by time messages sent over the serial port.
  A companion Processing sketch will automatically provide these messages
  if it is running and connected to the Arduino serial port.

- `TimeSerialDateStrings.pde` adds day and month name strings to the sketch above.
  Short (3 characters) and long strings are available to print the days of
  the week and names of the months.

- `TimeRTC` uses a DS1307 real-time clock to provide time synchronization.
  The basic [DS1307RTC library][1] must be downloaded and installed,
  in order to run this sketch.

- `TimeRTCSet` is similar to the above and adds the ability to set the Real Time Clock.

- `TimeRTCLog` demonstrates how to calculate the difference between times.
  It is a very simple logger application that monitors events on digital pins
  and prints (to the serial port) the time of an event and the time period since
  the previous event.

- `TimeNTP` uses the Arduino Ethernet shield to access time using the internet NTP time service.
  The NTP protocol uses UDP and the UdpBytewise library is required, see:
  <http://bitbucket.org/bjoern/arduino_osc/src/14667490521f/libraries/Ethernet/>

- `TimeGPS` gets time from a GPS.
  This requires the TinyGPS library from Mikal Hart:
  <http://arduiniana.org/libraries/TinyGPS>

- `ESP32` show the use of the library on ESP32 boards. This calc the Sunrise, Sunset and Solar Noon
  of a location for a entire year.
  This requires the Timezone32 and Sunrise libraries. See in my GitHub:
  <https://github.com/RafaelReyesCarmona> 

- `Time32Lib_test` is similar to TimeSerial, showing epoch unix time and human date conversion (GMT).
  Use to testing epoch time conversion:
   * Messages consist of the letter T followed by digits time (as seconds since Jan 1 1970)
 you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 T1357041600
  * Typing letter D the clock will set at EPOCH TIME: 2005949145590 Human: 23:59:50 - 31/12/65535 GMT
Wait ten seconds to see if 65535 year problem is solved.
  * Message consist of the letter S followed by digits year (S2025) the coclk will be set at 
00:00:00 - Jan 1 of the give year. 

Some epoch samples to test:
```
T1000990619884799   - EPOCH TIME: 1000990619884799 Human: 23:59:59 - 31/12/31.722.099 GMT
T1570278033964800   - EPOCH TIME: 1570278033964800 Human: 0:00:00 - 1/1/49.762.100 GMT S49762100
T1696505841964800   - EPOCH TIME: 1696505841964800 Human: 0:00:00 - 1/1/53.762.100 GMT
T2958783921964800   - EPOCH TIME: 2958783921964800 Human: 0:00:00 - 1/1/93.762.100 GMT S93762100
T15581564721964800  - EPOCH TIME: 15581564721964800 Human: 0:00:00 - 1/1/493.762.100 GMT S493762100
T28204345521964800  - EPOCH TIME: 28204345521964800 Human: 0:00:00 - 1/1/893.762.100 GMT S893762100
T40827126321964800  - EPOCH TIME: 40827126321964800 Human: 0:00:00 - 1/1/1.293.762.100 GMT
T53449907121964800  - EPOCH TIME: 53449907121964800 Human: 0:00:00 - 1/1/1.693.762.100 GMT
T66072687921964800  - EPOCH TIME: 66072687921964800 Human: 0:00:00 - 1/1/2.093.762.100 GMT
T78695468721964800  - EPOCH TIME: 78695468721964800 Human: 0:00:00 - 1/1/2.493.762.100 GMT
T91318249521964800  - EPOCH TIME: 91318249521964800 Human: 0:00:00 - 1/1/2.893.762.100 GMT
T103941030321964800 - EPOCH TIME: 103941030321964800 Human: 0:00:00 - 1/1/3.293.762.100 GMT
T116563811121964800 - EPOCH TIME: 116563811121964800 Human: 0:00:00 - 1/1/3.693.762.100 GMT
T129186591921964800 - EPOCH TIME: 129186591921964800 Human: 0:00:00 - 1/1/4.093.762.100 GMT
T129199214702764800 - EPOCH TIME: 129199214702764800 Human: 0:00:00 - 1/1/4.094.162.100 GMT
T135510605102764800 - EPOCH TIME: 135510605102764800 Human: 0:00:00 - 1/1/4.294.162.100 GMT S4294162100
T135536008953916800 - EPOCH TIME: 135536008953916800 Human: 0:00:00 - 1/1/4.294.967.116 GMT S4294967116
T135536014602748800 - EPOCH TIME: 135536014602748800 Human: 0:00:00 - 1/1/4.294.967.295 GMT S4294967295
```

## Differences for Time by *Michael Margolis*

Differences between Time code and the playground DateTime library
although the Time library is based on the DateTime codebase, the API has changed.
Changes in the Time library API:

- time elements are functions returning `int` (they are variables in DateTime)
- Years start from 1970
- days of the week and months start from 1 (they start from 0 in DateTime)
- DateStrings do not require a separate library
- time elements can be accessed non-atomically (in DateTime they are always atomic)
- function added to automatically sync time with external source
- `localTime` and `maketime` parameters changed, `localTime` renamed to `breakTime`

## Technical Notes

Internal system time is based on the standard Unix `time_t`.
The value is the number of seconds since Jan 1, 1970.
System time begins at zero when the sketch starts.

The internal time can be automatically synchronized at regular intervals to an external time source.
This is enabled by calling the `setSyncProvider(provider)` function - the provider argument is
the address of a function that returns the current time as a `time32_t`.
See the sketches in the examples directory for usage.

The default interval for re-syncing the time is 5 minutes but can be changed by calling the
`setSyncInterval(interval)` method to set the number of seconds between re-sync attempts.

The Time32 library defines a structure for holding time elements that is a compact version of the C `tm` structure.
All the members of the `tm` structure are bytes excepts year that is 16bits. The year is offset from 1970.
Convenience macros provide conversion for make use more easy.

Low-level functions to convert between system time and individual time elements are provided:

```c
breakTime(time, &tm);  // break time_t into elements stored in tm struct
makeTime(&tm);         // return time_t from elements stored in tm struct
```
## Changelog
### V1.1.4
  *  Added support fix 65535 year problem. Maximal time is epoch 135536014634284799 - 23:59:59 Dec 31 4294967295.
  * Fast calculate to convert and set epoch unix time to gregorian calendar.
  * Update leap seconds information until 28th December of 2025 00:00:00 UTC.

### V1.1.3
  * Fixed definition #define DAYS_PER_A_WEEK ((time32_t)(7UL)) to prevent a conflict with NEOGPS library.
### V1.1.2
  * Fixed leap_seconds function and data.
### V1.1.1
  * Fixed some spanish words.
### V1.1
  * Added 'Spanish' language support.
### V1.0.1
  * fixed 'setTime' to use new time32_t.
  * fixed return values of 'year'. 
### V1.0
  * fixed "conflicting declaration 'typedef time_t'" on ESP32 enviroment. 
  * added typedef time32_t to avoid conflict with newlib or other libs.
  * added support fix 2106 problem. Tested until 31th December of 16383. The limit is 65535 year. When use please set a max timestamp of 2005949141999. (Thanks to rmslu for the test).
  * added leap_seconds function to calc leap seconds. 
### Time by *Michael Margolis*
  * 1.0  6  Jan 2010 - initial release
  * 1.1  12 Feb 2010 - fixed leap year calculation error
  * 1.2  1  Nov 2010 - fixed setTime bug (thanks to Korman for this)
  * 1.3  24 Mar 2012 - many edits by Paul Stoffregen: fixed timeStatus() to update
                     status, updated examples for Arduino 1.0, fixed ARM
                     compatibility issues, added TimeArduinoDue and TimeTeensy3
                     examples, add error checking and messages to RTC examples,
                     add examples to DS1307RTC library.
  * 1.4  5  Sep 2014 - compatibility with Arduino 1.5.7
## License

This file is part of Time32 Library.

Time32 Library is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Time32 Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Time32 Library.  If not, see <https://www.gnu.org/licenses/>.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

## Authors
### Time32
Copyright © 2022-2025 Francisco Rafael Reyes Carmona.
Contact me: rafael.reyes.carmona@gmail.com

### Time
Copyright (c) Michael Margolis 2009-2014
## Credits

<a target="_blank" href="https://icons8.com/icon/QDgOnr6UAOmg/time-machine">Time Machine</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>
