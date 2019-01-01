# arduino-dram-tester
Test DRAM with Arduino Uno

Basic DRAM tester, outputs results to Serial, based on:

* https://www.insentricity.com/a.cl/252
* https://github.com/FozzTexx/DRAM-Tester
* http://www.chronworks.com/DRAM

Changes to original code:

* Writes output to serial, displaying which row and col failed
* Times the tests and writes how long it took, just out of curiosity so you can calculate of refresh is happening fast enough
* Does pattern tests first (10101... and 01010...) because this identified failed ram faster for me
* Added a random bit write test
