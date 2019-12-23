/*
  HalfDuplexSerial.cpp - Stream Wrapper for Raplh Doncaster's

  Wrapper by J.Sleeman (sparks@gogo.co.nz), Derived from TinySoftwareSerial

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified by MCUdude for the MicroCore
  https://github.com/MCUdude/MicroCore
*/

#include "Arduino.h"
#include "HalfDuplexSerial.h"


// Public Methods //////////////////////////////////////////////////////////////

/*
void HalfDuplexSerial::begin(const uint8_t baud)
{

}

void HalfDuplexSerial::end()
{
  // We can not end
}

void HalfDuplexSerial::flush()
{
  // Nothing to do here
}
*/

int HalfDuplexSerial::available(void)
{
  // There is never anything available, we have no buffer
  return 0;
}


int HalfDuplexSerial::peek(void)
{
  // We have no buffer, no peeking
  return -1;
}

int HalfDuplexSerial::read(void)
{
  #ifdef HALF_DUPLEX_SERIAL_DISABLE_READ
  return -1;
  #else
  return RxByteNBNegOneReturn();
  #endif
}

// This is the same as Serial.read() in that it is a
// non blocking read, returning -1 if no data was read
int HalfDuplexSerial::read_byte(void)
{
  return RxByteNBNegOneReturn();
}

char HalfDuplexSerial::read_char(void)
{
  return RxByteNBZeroReturn();
}

char HalfDuplexSerial::read_char_blocking(void)
{
  cli();
  return RxByte();
}

void HalfDuplexSerial::read_str(char buf[], uint8_t length)
{
  uint16_t t = 0xFFFF; // every time we attempt to read and fail, we decrement,
                       // when we hit zero time is up.  FIXME to use a more
                       // definite timing!
  uint8_t i = 0;
  buf[length - 1] = 0; // enforce null termination

  // Caution! There's no buffering in our serial routine, no interrupt driven
  // collection, so we need to have this loop pretty tight to avoid missed-bits
  // and corrupted bytes.
  //
  // The usefulness of this routine is questionable at best.
  //
  // Interrupts are disabled during the entire read
  uint8_t oldSREG = SREG;
  cli();
  do
  {
    while( (!(buf[i] = RxByteNBZeroReturn())) && --t);
  } while((++i < (length-1)) && t);
  SREG = oldSREG; // Put back interrupts again

  // i ends up as the index of the next character to read
  // this is at most length-1 so we can simply set i to be
  // null and we have our null-terminated string
  buf[i] = 0;
}


size_t HalfDuplexSerial::write(uint8_t ch)
{
  TxByte(ch);
  return 1;
}

// A non-virtual version, can be optimized out, always available
size_t HalfDuplexSerial::write_byte(uint8_t ch)
{
  TxByte(ch);
  return 1;
}


HalfDuplexSerial::operator bool()
{
  return true;
}

HalfDuplexSerial Serial;
