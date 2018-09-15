/*
 * oneTesla USB interrupter
 * Portions based on ATTiny85 MIDI HID device
 * All files licensed under the GNU GPL version 3.0
 * Copyright 2013 oneTesla LLC
 */

#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <util/delay_basic.h>


#include "usbdrv/usbdrv.h"
#include "constants.h"
#include "descriptor.h"
#include "util.h"

unsigned char cNote1 = 0;
unsigned char cVel1 = 0;
unsigned char cNote2 = 0;
unsigned char cVel2 = 0;
unsigned char mVol = 127;

unsigned int ot_cycles1 = 0;
unsigned int ot_cycles2 = 0;
unsigned int cFreq = 0, cPwr = 64;
unsigned char rstCnt = 0;

//in case your coil has an inverting input...
#define coilOn() outputOn()
#define coilOff() outputOff();

int main()
{
	initOutput();
	initUSB();
	
	if ((PINB & _BV(SWITCH)) >> SWITCH != 0) {
		cNote1 = 72;
		cVel1 = 127;
		mVol = 40;
		noteOn1();
	}
	
	sei();
	coilOff();
	
	// Endless loop
	for (;;) {
		wdt_reset();
		usbPoll();
	}
	return 0;
}

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{

	coilOn();
	unsigned long dLen = ot_cycles1 * mVol;
	dLen /= 128;
	if (dLen == 0) dLen = 1;
	_delay_loop_2(dLen);
	coilOff();
	
	TIFR &= ~(_BV(OCF1A));
	TCNT1 = 0;
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
	coilOn();
	unsigned long dLen = ot_cycles2 * mVol;
	dLen /= 128;
	if (dLen == 0) dLen = 1;
	_delay_loop_2(dLen);
	coilOff();
	
	TIFR &= ~(_BV(OCF0A));
	TCNT0 = 0;
}