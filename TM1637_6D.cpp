/*
 * TM1637_6D.cpp
 * A library for the 6 digit TM1637 based segment display
 *
 * Modified for 6 digits and points by: TinyTronics.nl
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
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
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x00,0x40};//0~9, 10=blank digit, 11=dash/minus character

static int8_t SegmentToAddressMapping[] = {3,4,5,0,1,2}; // Right to left, segments are 3,4,5,0,1,2						   

const int8_t zeroListDisp[6] = {10,10,10,10,10,10}; // fill array with value for blank digit
const int8_t zeroListDispPoint[6] = {0x00,0x00,0x00,0x00,0x00,0x00}; // don't show any points
  
TM1637_6D::TM1637_6D(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1637_6D::init(void)
{
  clearDisplay();
}

int TM1637_6D::writeByte(int8_t wr_data)
{
  uint8_t i,count1;
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;
    digitalWrite(Clkpin,HIGH);

  }
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);
  pinMode(Datapin,INPUT);

  bitDelay();
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0) 
  {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
  }
  bitDelay();
  pinMode(Datapin,OUTPUT);
  bitDelay();
  
  return ack;
}
//send start signal to TM1637
void TM1637_6D::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,LOW);
}
//End of transmission
void TM1637_6D::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH);
}

//display function.Write to full-screen.
void TM1637_6D::display(int8_t DispData[], int8_t DispPointData[])
{
  int8_t SegDataMapped[6];
  coding(DispData, DispPointData, SegDataMapped);
  displayCodedByte(SegDataMapped);
}

void TM1637_6D::displayCodedByte(int8_t SegData[])
{
  int8_t SegDataMapped[6];

  for(int8_t i=0;i<6;i++)
  {
    SegDataMapped[SegmentToAddressMapping[i]] = SegData[i];
  } 
  
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(Cmd_SetAddr);//
  for(int i=0;i < 6;i ++)
  {
    writeByte(SegDataMapped[i]);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

//******************************************
void TM1637_6D::display(uint8_t BitAddr,int8_t DispData,int8_t DispPointData)
{
  int8_t SegData;
  SegData = coding(DispData, DispPointData);
  displayCodedByte(BitAddr, SegData);
}

void TM1637_6D::displayCodedByte(uint8_t BitAddr,int8_t SegData)
{
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(SegmentToAddressMapping[BitAddr]|0xc0);//
  writeByte(SegData);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

// Displays 6 dashes are error marker
void TM1637_6D::displayError()
{
  int8_t tempListDisp[6] = {11,11,11,11,11,11}; // fill array with dashes character(11th)
  int8_t tempListDispPoint[6] = {0x00,0x00,0x00,0x00,0x00,0x00}; // don't show any points
  display(tempListDisp, tempListDispPoint);
}

void TM1637_6D::displayInteger(int32_t intdisplay, bool leading_zeros)
{
  int8_t tempListDisp[6];
  int8_t tempListDispPoint[6];
  int8_t SegDataMapped[6];
  
  buildIntegerCodedByteArray(intdisplay, leading_zeros, SegDataMapped);
  displayCodedByte(SegDataMapped);
}

void TM1637_6D::buildIntegerCodedByteArray(int32_t intdisplay, bool leading_zeros, int8_t SegDataOut[6])
{
  int8_t tempListDisp[6];
  int8_t tempListDispPoint[6];
  int8_t SegDataMapped[6];	  
  
  buildIntegerDispArray(intdisplay, leading_zeros, tempListDisp, tempListDispPoint);
  coding(tempListDisp, tempListDispPoint, SegDataOut);
}

void TM1637_6D::buildIntegerDispArray(int32_t intdisplay, bool leading_zeros, int8_t outListDispData[6], int8_t outListDispPointData[6])
{
  int8_t i = 0;

  memcpy(outListDispData, zeroListDisp, sizeof(zeroListDisp));
  memcpy(outListDispPointData, zeroListDispPoint, sizeof(zeroListDispPoint));
  
  // String for converting millis value to seperate characters in the string
  String intstring;
  
  // if the integer is bigger than 6 characters, display an error(dashes)
  if(intdisplay > 999999 || intdisplay < -99999)
  {
	displayError();
  }
  else
  {  
	if(intdisplay < 0)
	{
      intstring =  String((intdisplay*-1), DEC); // convertering the inverted integer to a string
	  if(leading_zeros)
	  { 
        intstring = "000000" + intstring;
      } // Adding some extra leading zeros
	  for(i = 0; i < intstring.length(); i++) 
      {
	    // number "0" in ASCII is 48 in decimal. If we substract the character value
        // with 48, we will get the actual number it is representing as a character
	    outListDispData[i] = intstring[intstring.length()-i-1] - 48;
      }
      outListDispData[5] = 11;// adding the dash/minus later for this negative integer
	}
	else
	{
      intstring =  String(intdisplay, DEC); // convertering integer to a string
	  if(leading_zeros){ intstring = "000000" + intstring;} // Adding some extra leading zeros
      for(i = 0; i < intstring.length(); i++) 
      {
	    // number "0" in ASCII is 48 in decimal. If we substract the character value
        // with 48, we will get the actual number it is representing as a character
	    outListDispData[i] = intstring[intstring.length()-i-1] - 48;
      }
	}
  }
}

void TM1637_6D::displayFloat(float floatdisplay)
{
  int8_t SegDataMapped[6];
  buildFloatCodedByteArray(floatdisplay, SegDataMapped);
  displayCodedByte(SegDataMapped);
}

void TM1637_6D::buildFloatCodedByteArray(float floatdisplay, int8_t SegDataOut[6])
{
  int8_t tempListDisp[6];
  int8_t tempListDispPoint[6];
	  
  buildFloatDispArray(floatdisplay, tempListDisp, tempListDispPoint);
  coding(tempListDisp, tempListDispPoint, SegDataOut);
}


void TM1637_6D::buildFloatDispArray(float floatdisplay, int8_t outListDispData[6], int8_t outListDispPointData[6])
{
  int8_t i = 0;
   
  memcpy(outListDispData, zeroListDisp, sizeof(zeroListDisp));
  memcpy(outListDispPointData, zeroListDispPoint, sizeof(zeroListDispPoint));

  // String for converting millis value to seperate characters in the string
  String floatstring;  
  
  // if the integer is bigger than 6 characters, display an error(dashes)
  if(floatdisplay >= 999999.5 || floatdisplay <= -99999.5)
  {
	displayError();
  }
  else
  {  
    // Calculate how many digits we have before the decimal point
    if(floatdisplay < 0) { floatstring =  String(floatdisplay*-1.0, 1);} // with one "dummy" decimal
    else { floatstring =  String(floatdisplay, 1);} // with one "dummy" decimal
	for(i = 0; i < floatstring.length(); i++) 
    {
      // check how many digits we have before the decimal point
      if(floatstring[i] == '.') break;
    }
    uint8_t numberofdigits = i;
    uint8_t decimal_point_add = 0; // used to skip the decimal point in the string
	
	if(floatdisplay < 0)
	{
      floatstring =  String((floatdisplay*-1.0), 5-numberofdigits); // convertering the inverted integer to a string
	  decimal_point_add = 0;
      for(i = 0; i < floatstring.length(); i++) 
      {
        // number "0" in ASCII is 48 in decimal. If we substract the character value
        // with 48, we will get the actual number it is representing as a character
	    outListDispData[i] = floatstring[floatstring.length()-i-1-decimal_point_add] - 48;
        if(floatstring[floatstring.length()-i-2] == '.')
        {
          outListDispPointData[i+1] = 0x80; // set decimal point
		  decimal_point_add = 1;
        }
        
      }
      outListDispData[5] = 11;// adding the dash/minus later for this negative integer
	}
	else
	{
      floatstring =  String(floatdisplay, 6-numberofdigits); // convertering integer to a string
      decimal_point_add = 0;

      for(i = 0; i < (floatstring.length() - decimal_point_add); i++) 
      {
        
        // number "0" in ASCII is 48 in decimal. If we substract the character value
        // with 48, we will get the actual number it is representing as a character
	    outListDispData[i] = floatstring[floatstring.length()-i-1-decimal_point_add] - 48;
        if(floatstring[floatstring.length()-i-2] == '.')
        {
          outListDispPointData[i+1] = 0x80; // set decimal point
		  decimal_point_add = 1;
        }
      }
	}
	if(decimal_point_add == 0) outListDispPointData[0] = 0x80;
  }
}

void TM1637_6D::clearDisplay(void)
{
  int8_t tempListDisp[6] = {10,10,10,10,10,10}; // fill array with blank characters(10th)
  int8_t tempListDispPoint[6] = {0x00,0x00,0x00,0x00,0x00,0x00}; // don't show any poinst
  display(tempListDisp, tempListDispPoint);
}
//To take effect the next time it displays.
void TM1637_6D::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

void TM1637_6D::coding(int8_t DispData[], int8_t DispPointData[], int8_t SegDataOut[])
{
  for(uint8_t i = 0;i < 6;i ++)
  {
	SegDataOut[i] = coding(DispData[i], DispPointData[i]);
  }
}
int8_t TM1637_6D::coding(int8_t DispData, int8_t DispPointData)
{
  if(DispData > 11 || DispData < 0) DispData = 11;
  if(DispData == 0x7f) DispData = 0x00 + DispPointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + DispPointData;
  
  return DispData;
}


void TM1637_6D::bitDelay(void)
{
	delayMicroseconds(50);
}

