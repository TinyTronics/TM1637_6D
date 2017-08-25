/*
 * Example for 6-digit TM1637 based segment Display
 * The number of milliseconds after start will be displayed on the 6-digit screen
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "TM1637_6D.h"

#define CLK 3 //pins definitions for TM1637 and can be changed to other ports
#define DIO 2

TM1637_6D tm1637_6D(CLK,DIO);

void setup()
{
  tm1637_6D.init();
  // You can set the brightness level from 0(darkest) till 7(brightest) or use one
  // of the predefined brightness levels below
  tm1637_6D.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}
void loop()
{  
  // Print random float values to the display
  tm1637_6D.displayFloat(-1.2345);
  delay(2000);
  tm1637_6D.displayFloat(987.2345);
  delay(2000);
  tm1637_6D.displayFloat(1.2345);
  delay(2000);
  tm1637_6D.displayFloat(192837);
  delay(2000);
  tm1637_6D.displayFloat(0);
  delay(2000);
}