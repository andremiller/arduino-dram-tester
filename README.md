# arduino-dram-tester
Test DRAM with Arduino Uno

## Basic DRAM tester, outputs results to Serial, based on:

* https://www.insentricity.com/a.cl/252
* https://github.com/FozzTexx/DRAM-Tester
* http://www.chronworks.com/DRAM

## Pinouts:

* DRAM pin -> Arduino UNO pin
* DIN -> 2
* DOUT -> 3
* CAS -> 5
* RAS -> 6
* WE -> 7
* A0 to A5 -> 8 to 13
* A6 to A9 -> A0 to A3

(Also connect DRAM supply pins)

The number of address lines is configurable by setting ADDR_BITS

## Changes to original code:

* Writes output to serial, displaying which row and col failed
* Times the tests and writes how long it took, just out of curiosity so you can calculate of refresh is happening fast enough
* Does pattern tests first (10101... and 01010...) because this identified failed ram faster for me
* Added a random bit write test
