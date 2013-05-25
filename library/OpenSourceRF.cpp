/*
  OpenSourceRF.h - Using OpenSourceRF
  Just a guy
  
  History:
	20May13	- V0.0 initial version

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
*/

#include <OpenSourceRF.h>

void OpenSourceRF::getPacket() {
  int in = 0;  
  int i = 0;
  if (mySerial.available() > 0) {
    while ((mySerial.available() > 0) && i < 5) {
      in = mySerial.read();
      if (!i)
        rv->button = in;        
      else
        rv->esn[i-1] = in;
#if WORD_DEBUG
      Serial.write(": byte: ");
      Serial.print(in, HEX);
      Serial.write("\n\r");
#endif // WORD_DEBUG
      i++;
    }
  }
}

