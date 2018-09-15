Dual Resonant Solid State Tesla Coil and Solid State Tesla Coil PCB pack.
Copyright (C) 2018 by Daniel L. Marks, licensed under the Created Commons CC-BY-SA 4.0.

These are PCBs for use in DRSSTC/SSTC devices.  They are projects using
the free and open source Kicad software (kicad-pcb.org) version 4.0.7, so these projects
may be modified using this software.  Each file includes the following:

1.  Kicad project, so it can be modified or improved
2.  PDF Schematic
3.  Bill of materials, to refer to when assembling
4.  PNG circuit board picture, to refer to when assembling
5.  Gerber files

The projects, except for the interrupters, are 100 by 100 mm boards with 90 by 90 mm
mounting hole spacing.  These boards may be stacked using M3 spacers. Board of this 
size can often be fabricated for $5 (without shipping) from PCB manufacturers such as
Seeedstudio.  You can save on shipping per board if you have several of
these made at a time, for example, the driver, interrupter, and a bridge board.  

The projects are as follows:

ud27c - a through hole version of the Universal Tesla coil driver, version 2.7C.
          This is based on a design originally by Steve Ward and refined by others including
          Eric Goodchild and Gao Guangyan.  It has substituted the through-hole parts 
          IRF540N/IRF9540N complementary MOSFET pair for the FDD8424H dual MOSFET part.
          It includes phase lead, unlike the Universal Driver v1.3.

ud29 - an experimental version of ud27c that includes skip pulse functionality.  Pulse skip is
          enabled or disabled using a jumper, so it may be used as a normal DRSSTC controller.
		  The board has been built and worked for my coil.
		  
PSoc5-power - a through-hole version of the Universal Driver 3 by Steve Ward and Jens Kerrinnes
          based on the PSoC5 system-on-chip. This includes the functionality for temperature,
		  bus voltage, and bus current sensing.	 This is experimental.  It uses the CYC8KIT-059
		  dev board plugged into the PCB using pin header sockets.

interrupter - a DRSSTC interrupter based on Steve Ward's burst mode interrupter.  It
          includes a place for the IFE97 LED optical fiber transmission unit.  The ability
          to include an external pulse trigger signal has been included for use with audio
          sources.  Also includes a spot on the board for a OneTesla 
		  ATTINY85-based MIDI interrupter which may be flashed with the OneTesla code through
		  the ICSP interface.

lineinterrupter - an experimental interrupter that interrupts on the peak of the AC cycle.
           Interrupting only at the peak of the AC cycle is intended to improve the power factor
		   by minimizing the inrush of current into the bus capacitors as the magnitude of
		   the voltage increases.  
		  
NOTE: You need to flash the included firmware on your ATTINY85 for the MIDI interrupter to work.
Instructions are in the folder "oneTesla-usbInterrupter-software".
		  
half-bridge-sstc - a half-bridge/full-bridge driver designed by myself
          that can either self-oscillate, or take an extetrnal antenna input from a
          Solid State Tesla Coil.  It includes overcurrent and undervoltage protection.
	  
half-bridge-transistor - a layout for a two TO-247 transistor half-bridge that includes
          an area for the capacitor divider network.  Note the polarity on the gates indicated
          on the silkscreening always indicates the negative and positive voltage side of
          the gate; wire the phases of the gates accordingly (oppositely in this case).  This
          convention is followed for all of the layouts.  Space is included for both
          supply bypass/DC Link capacitors and snubber capacitors.
	  
full-bridge-transistor - a layout for a four TO-247 transistor full-bridge.  Space is included
          for supply bypass DC/Link capacitors and snubber capacitors.  The two high-side gates
          should have their polarities wired oppositely, the two low-side gates should
          have their polarities wired oppositely, and the high and low-side gates of each half-bridge
          should be have their polarities wired oppositely.
	  
half-bridge-multi - a layout for an eight TO-247 transistor half-bridge, with four transistors
          of each side of the half-bridge.  The transistors should have a positive temperature
          coefficient to share current more equally.  The high and low-side gates should have their
          polarities wired oppositely.
	  
full-bridge-multi - a layout for an eight TO-247 transistor full-bridge, with two transistors on
          each side of each half bridge.  The two high-side gates should have their 
          polarities wired oppositely, the two low-side gates should have their polarities
          wired oppositely, and the high and low-side gates of each half-bridge
	      should be have their polarities wired oppositely.
		  

