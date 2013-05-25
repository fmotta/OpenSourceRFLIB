/*
OpenSourceRF.h (formerly SoftwareSerial.h) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

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

NOTE: I bear no responsibility for function nor usefulness of this code.
It is not implied nor stated to be functional.


*/

#ifndef OpenSourceRF_h
#define OpenSourceRF_h
#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#define ESN_LEN 4
typedef struct OSRF_pkt {
  unsigned char button;
  unsigned char esn[ESN_LEN];
} OSRF_pkt;

// OpenSourceRF specific constants
#ifndef ESN_LEN
#define ESN_LEN 4
#endif
#define LEFT_BUTTON 0x40
#define CENTER_BUTTON 0x20
#define RIGHT_BUTTON 0x10
#define LEFT_CENTER_BUTTON (LEFT_BUTTON | CENTER_BUTTON)
#define RIGHT_CENTER_BUTTON (RIGHT_BUTTON | CENTER_BUTTON)
#define LEFT_RIGHT (LEFT_BUTTON | RIGHT_BUTTON)
#define LEFT_CENTER_RIGHT (LEFT_BUTTON | CENTER_BUTTON | RIGHT_BUTTON)
#define REPEAT_KEY 0xFFFFFFF0

// library interface description
class OpenSourceRF : public Stream
{
  private:
    // per object data
    uint8_t _receivePin;
    uint8_t _receiveBitMask;
    volatile uint8_t *_receivePortRegister;
    uint8_t _transmitBitMask;
    volatile uint8_t *_transmitPortRegister;

    uint16_t _rx_delay_centering;
    uint16_t _rx_delay_intrabit;
    uint16_t _rx_delay_stopbit;
    uint16_t _tx_delay;

    uint16_t _buffer_overflow:1;
    uint16_t _inverse_logic:1;

    // static data
    static char _receive_buffer[_SS_MAX_RX_BUFF]; 
    static volatile uint8_t _receive_buffer_tail;
    static volatile uint8_t _receive_buffer_head;
    static OpenSourceRF *active_object;
    static int _inpDelayRate;

    // private methods
    void recv();
    uint8_t rx_pin_read();
    void tx_pin_write(uint8_t pin_state);
    void setTX(uint8_t transmitPin);
    void setRX(uint8_t receivePin);

    // private static method for timing
    static inline void tunedDelay(uint16_t delay);

  public:
    static unsigned char _lastKey;
    /*
     * Constructor. 
     * This library expects full 
     */

    // public methods
    OpenSourceRF(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    OpenSourceRF();
    ~OpenSourceRF();

    void begin(long speed);
    bool listen();
    void end();
    bool isListening() { return this == active_object; }
    bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
    int peek();
    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    using Print::write;
    
    void setDelay(int delayRate);

	void getPacket(OSRF_pkt *rv);
	unsigned int getButtonPress();
	void clearLastKey();

    // public only for easy access by interrupt handlers
    static inline void handle_interrupt();
};

#endif
