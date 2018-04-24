/*
 * TM1637.h
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

#ifndef TM1637_6D_h
#define TM1637_6D_h
#include <inttypes.h>
#include <Arduino.h>
//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   0x80
#define POINT_OFF  0x00
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

class TM1637_6D
{
  public:
    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;
    boolean _PointFlag;     //_PointFlag=1:the clock point on
    TM1637_6D(uint8_t, uint8_t);
    void init(void);        //To clear the display
    int  writeByte(int8_t wr_data);//write 8bit data to tm1637
    void start(void);//send start bits
    void stop(void); //send stop bits
    void display(int8_t DispData[],int8_t DispPointData[]);
    void display(uint8_t BitAddr,int8_t DispData,int8_t DispPointData);
    void displayCodedByte(int8_t SegData[]);
    void displayCodedByte(uint8_t BitAddr,int8_t SegData);
    void displayError();
    void buildIntegerDispArray(int32_t intdisplay, bool leading_zeros, int8_t outListDispData[6], int8_t outListDispPointData[6]);
    void displayInteger(int32_t intdisplay, bool leading_zeros);
	void buildIntegerCodedByteArray(int32_t intdisplay, bool leading_zeros, int8_t SegDataOut[6]);
    void displayFloat(float floatdisplay);
	void buildFloatCodedByteArray(float floatdisplay, int8_t SegDataOut[6]);
    void buildFloatDispArray(float floatdisplay, int8_t outDispData[6], int8_t outDispPointData[6]);
    void clearDisplay(void);
    void set(uint8_t = BRIGHT_TYPICAL,uint8_t = 0x40,uint8_t = 0xc0);//To take effect the next time it displays.
    void coding(int8_t DispData[],int8_t DispPointData[], int8_t SegDataOut[]);
    int8_t coding(int8_t DispData,int8_t DispPointData);
    void bitDelay(void);
  private:
    uint8_t Clkpin;
    uint8_t Datapin;
};
#endif
