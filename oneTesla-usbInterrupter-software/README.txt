This is the source code for the oneTesla USB interrupter. The compiled firmware runs on ATTiny85 devices and presents itself transparently to the host computer as a USB HID MIDI device. It features an entirely software implementation of the USB stack via Obdev's V-USB, as well as a very efficient MIDI-to-DRSSTC engine written in fixed point math.

You will need an AVR-GCC to compile it; we've tested it under WinAVR but it should compile on other platforms as well.

All contents licensed under the GNU GPL 3.0