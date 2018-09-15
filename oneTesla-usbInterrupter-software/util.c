#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>


#include "usbdrv/usbdrv.h"
#include "constants.h"
#include "util.h"
#include "shared.h"

void initOutput()
{
	DDRB = _BV(OUTPUT);
	DDRB &= ~(_BV(SWITCH));
}

void outputOn()
{
	PORTB |= _BV(OUTPUT);
}

void outputOff()
{
	PORTB &= ~(_BV(OUTPUT));
}
unsigned int midiFreq(unsigned char n) {
	if (n < 36 || n > 94) return 0;
	float bfreq = base[n % 12];
	unsigned int mult = pow2[(n - 36) / 12];
	return (unsigned int) (bfreq * mult);
}

void noteOn1() {
	TCNT1 = 0;
	unsigned int freq = midiFreq(cNote1);
	if (freq == 0) return;
	unsigned long period_us = 1000000UL / freq;
	unsigned long on_time = period_us / 32;
	on_time = on_time * cVel1 / 127;
	if (on_time > 100) on_time = 100;
	ot_cycles1 = 4 * on_time + on_time / 8;
	period_us -= on_time;
	
	unsigned long p_cycles = 16 * period_us + (period_us / 2);

	if (p_cycles / 32 < 0x100) {
		TCCR1 = _BV(CS11) | _BV(CS12);				// 1:32
		p_cycles /= 32;
	} else if (p_cycles / 64 < 0x100) {
		TCCR1 = _BV(CS10) | _BV(CS11) | _BV(CS12);	// 1:64
		p_cycles /= 64;
	} else if (p_cycles / 256 < 0x100) {
		TCCR1 = _BV(CS10) | _BV(CS13);				// 1:256
		p_cycles /= 256;
	} else if (p_cycles / 1024 < 0x100) {
		TCCR1 = _BV(CS10) | _BV(CS11) | _BV(CS13);	// 1:1024
		p_cycles /= 1024;
	} else {
		return;
	}
	
	OCR1A = p_cycles;
	TIMSK |= _BV(OCIE1A);
}

void noteOn2() {
	TCNT0 = 0;
	unsigned int freq = midiFreq(cNote2);
	if (freq == 0) return;
	unsigned long period_us = 1000000UL / freq;
	unsigned long on_time = period_us / 32;
	on_time = on_time * cVel2 / 127;
	if (on_time > 100) on_time = 100;
	ot_cycles2 = 4 * on_time + on_time / 8;
	period_us -= on_time;
	
	unsigned long p_cycles = 16 * period_us + (period_us / 2);

	if (p_cycles / 64 < 0x100) {
		TCCR0B = _BV(CS00) | _BV(CS01);				// 1:64
		p_cycles /= 64;
	} else if (p_cycles / 256 < 0x100) {
		TCCR0B = _BV(CS02);				// 1:256
		p_cycles /= 256;
	} else if (p_cycles / 1024 < 0x100) {
		TCCR0B = _BV(CS02) | _BV(CS00);	// 1:1024
		p_cycles /= 1024;
	} else {
		return;
	}
	
	OCR0A = p_cycles;
	TIMSK |= _BV(OCIE0A);
}

void noteOff1() {
	TIMSK &= ~(_BV(OCIE1A));
	cNote1 = 0;
}

void noteOff2() {
	TIMSK &= ~(_BV(OCIE0A));
	cNote2 = 0;
}

void initUSB() {
	cli();
	
	uchar uCalVal, i;

	do {} while (!eeprom_is_ready());
	uCalVal = eeprom_read_byte(0);
	if (uCalVal != 0xff) OSCCAL = uCalVal;
	
	wdt_enable(WDTO_2S);

	usbInit();
	
	usbDeviceDisconnect();  // enforce re-enumeration, do this while interrupts are disabled!
	i = 0;
	while (--i) {             // fake USB disconnect for > 250 ms
		wdt_reset();
		_delay_ms(1);
    }
    usbDeviceConnect();
}
