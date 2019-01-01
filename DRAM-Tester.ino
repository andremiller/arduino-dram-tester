// -*- C -*-

/* Simple DRAM tester
 * Andre Miller http://www.andremiller.net/
 * Based on:  http://insentricity.com/a.cl/252  Chris Osborn <fozztexx@fozztexx.com>
 *            https://github.com/FozzTexx/DRAM-Tester
 */

// Tested on Arduino UNO

#define DIN             2
#define DOUT            3
#define CAS             5
#define RAS             6
#define WE              7

#define ADDR_BITS       9  // 9 for 256, 10 is max address bits

void fillSame(int val);
void fillAlternating(int start);

void setup()
{
  int mask;
  Serial.begin(115200);
  Serial.println("SETUP");
  Serial.print("Number of address bits: ");
  Serial.println(ADDR_BITS);

  
  pinMode(DIN, OUTPUT);
  pinMode(DOUT, INPUT);

  pinMode(CAS, OUTPUT);
  pinMode(RAS, OUTPUT);
  pinMode(WE, OUTPUT);

  /* 10 is max address bits, even if chip is smaller */
  mask = (1 << 10) - 1; 
  DDRB = mask & 0x3f;
  mask >>= 6;
  DDRC = mask & 0x0f;
  
  digitalWrite(CAS, HIGH);
  digitalWrite(RAS, HIGH);
  digitalWrite(WE, HIGH);
}

void loop()
{
  static int i=1;
  Serial.print("START ITERATION: ");
  Serial.println(i);

  fillAlternating(1);
  fillAlternating(0);
  fillSame(0);
  fillSame(1);
  fillRandom(10);
  fillRandom(200);

  Serial.print("END ITERATION: ");
  Serial.println(i);
  i+=1;
  
}

static inline int setAddress(int row, int col, int wrt)
{
  int val = 0;


  PORTB = row & 0x3f;
  PORTC = (PORTC & 0xf0) | (row >> 6) & 0x0f;
  digitalWrite(RAS, LOW);

  if (wrt)
    digitalWrite(WE, LOW);

  PORTB = col & 0x3f;
  PORTC = (PORTC & 0xf0) | (col >> 6) & 0x0f;
  digitalWrite(CAS, LOW);

  if (wrt)
    digitalWrite(WE, HIGH);
  else
    val = digitalRead(DOUT);
 
  digitalWrite(CAS, HIGH);
  digitalWrite(RAS, HIGH);

  return val;
}

void fail(int row, int col, int val)
{

  Serial.print("*** FAIL row ");
  Serial.print(row);
  Serial.print(" col ");
  Serial.print(col);
  Serial.print(" was expecting ");
  Serial.print(val);
  Serial.print(" got ");
  Serial.println(!val);

  while (1)
    ;
}

void fillSame(int val)
{
  int row, col;

  unsigned long writeStartMillis;
  unsigned long writeEndMillis;
  unsigned long readStartMillis;
  unsigned long readEndMillis;

  Serial.print("  Setting all bits set to: ");
  Serial.println(val);
  digitalWrite(DIN, val);

  Serial.println("    Write");
  writeStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++)
    for (row = 0; row < (1 << ADDR_BITS); row++)
      setAddress(row, col, 1);
  writeEndMillis = millis();

  /* Reverse DIN in case DOUT is floating */
  digitalWrite(DIN, !val);

  
  Serial.println("    Read");
  readStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++)
    for (row = 0; row < (1 << ADDR_BITS); row++)
      if (setAddress(row, col, 0) != val)
        fail(row, col, val);
  readEndMillis = millis();

  Serial.print("    Pass ");
  Serial.print("Write: ");
  Serial.print(writeEndMillis - writeStartMillis);
  Serial.print("ms Read: ");
  Serial.print(readEndMillis - readStartMillis);
  Serial.println("ms");
  return;
}

void fillAlternating(int start)
{
  int row, col, i;

  unsigned long writeStartMillis;
  unsigned long writeEndMillis;
  unsigned long readStartMillis;
  unsigned long readEndMillis;

  Serial.print("  Alternating bits starting with: ");
  Serial.println(start);

  Serial.println("    Write");
  i = start;
  writeStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++) {
    for (row = 0; row < (1 << ADDR_BITS); row++) {
      digitalWrite(DIN, i);
      i = !i;
      setAddress(row, col, 1);
    }
  }
  writeEndMillis = millis();

  Serial.println("    Read");
  readStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++) {
    for (row = 0; row < (1 << ADDR_BITS); row++) { 
      if (setAddress(row, col, 0) != i)
        fail(row, col, i);

      i = !i;
    }
  }
  readEndMillis = millis();
  
  Serial.print("    Pass ");
  Serial.print("Write: ");
  Serial.print(writeEndMillis - writeStartMillis);
  Serial.print("ms Read: ");
  Serial.print(readEndMillis - readStartMillis);
  Serial.println("ms");
  return;
}

void fillRandom(int seed)
{
  int row, col, i;

  unsigned long writeStartMillis;
  unsigned long writeEndMillis;
  unsigned long readStartMillis;
  unsigned long readEndMillis;

  randomSeed(seed);

  Serial.print("  Random bit values with seed: ");
  Serial.println(seed);

  Serial.println("    Write");
  writeStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++) {
    for (row = 0; row < (1 << ADDR_BITS); row++) {
      i = random(0,2);
      //i = 1;
      //Serial.println(i);
      digitalWrite(DIN, i);
      setAddress(row, col, 1);
    }
  }
  writeEndMillis = millis();

  randomSeed(seed);

  Serial.println("    Read");
  readStartMillis = millis();
  for (col = 0; col < (1 << ADDR_BITS); col++) {
    for (row = 0; row < (1 << ADDR_BITS); row++) { 
      i = random(0,2);
      //i=1;
      //Serial.println(i);
      if (setAddress(row, col, 0) != i)
        fail(row, col, i);
    }
  }
  readEndMillis = millis();
  
  Serial.print("    Pass ");
  Serial.print("Write: ");
  Serial.print(writeEndMillis - writeStartMillis);
  Serial.print("ms Read: ");
  Serial.print(readEndMillis - readStartMillis);
  Serial.println("ms");
  return;
}



