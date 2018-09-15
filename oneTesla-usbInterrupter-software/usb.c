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
#include "descriptor.h"
#include "shared.h"

uchar usbFunctionDescriptor(usbRequest_t * rq)
{
	if (rq->wValue.bytes[1] == USBDESCR_DEVICE) {
		usbMsgPtr = (uchar *) deviceDescrMIDI;
		return sizeof(deviceDescrMIDI);
	}
	else {		/* must be config descriptor */
		usbMsgPtr = (uchar *) configDescrMIDI;
		return sizeof(configDescrMIDI);
	}
}

uchar usbFunctionSetup(uchar data[8])
{
	return 0xff;
}

uchar usbFunctionRead(uchar * data, uchar len)
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;

	return 7;
}

uchar usbFunctionWrite(uchar * data, uchar len)
{
	return 1;
}

void usbFunctionWriteOut(uchar * data, uchar len)
{
	uchar msgType = data[0] & 0x0f;
	if (msgType == MIDI_NOTE_ON) {
		if (data[1] != 0x90) return;
		if (data[3] == 0 && data[2] == cNote1) {
			noteOff1();
		} else if (data[3] == 0 && data[2] == cNote2) {
			noteOff2();		
		} else if (data[3] != 0) {
			if (cNote1 == 0) {
				noteOff1();
				cNote1 = data[2];
				cVel1 = data[3];
				noteOn1();
			} else if (cNote2 == 0)	{
				noteOff2();
				cNote2 = data[2];
				cVel2 = data[3];
				noteOn2();
			} else {
				unsigned char c2 = cNote2;
				unsigned char v2 = cVel2;
				noteOff1();
				noteOff2();
				cNote1 = c2;
				cVel1 = v2;
				cNote2 = data[2];
				cVel2 = data[3];
				noteOn1();
				noteOn2();
			}	
		}
		return;
	}
	if (msgType == MIDI_NOTE_OFF && data[2] == cNote1) {
		if (data[1] != 0x80) return;
		noteOff1();
		return;
	}
	if (msgType == MIDI_NOTE_OFF && data[2] == cNote2) {
		if (data[1] != 0x80) return;
		noteOff2();
		return;
	}
	if (msgType == MIDI_BYTE_MSG) {
		if (data[1] == 0xfc) {
			noteOff1();
			noteOff2();
		}
		return;
	}
	if (msgType == MIDI_CTRL_CHANGE) {
		if (data[2] == 0x78 || data[2] == 0x79 || data[2] == 0x7b || data[2] == 0x7c) {
			noteOff1();
			noteOff2();
			return;
		}
		if (data[2] == 0x07) {
			mVol = data[3];
			if (mVol < 1) mVol = 1;
			return;
		}
	}
}

static void calibrateOscillator(void)
{
	uchar step = 128;
	uchar trialValue = 0, optimumValue;
	int x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

	//do a binary search:
    do {
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    //proportional to current real frequency
        if(x < targetValue)             //frequency still too low
            trialValue += step;
        step >>= 1;
    } while (step > 0);

    //We have a precision of +/- 1 for optimum OSCCAL here
    //now do a neighborhood search for optimum value 
    optimumValue = trialValue;
    optimumDev = x; //this is certainly far away from optimum 
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++) {
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0) x = -x;
        if (x < optimumDev) {
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}

void hadUsbReset(void)
{
	if (rstCnt == 0) {
		rstCnt++;
		do {} while (!eeprom_is_ready());
		uchar uCalVal = eeprom_read_byte(0);
		cli();
		calibrateOscillator();
		sei();
		eeprom_write_byte(0, OSCCAL);
	}
}