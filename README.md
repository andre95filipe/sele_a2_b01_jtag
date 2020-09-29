# Electronic Systems - Sistemas Eletrónicos


## JTAG

### Description

* This system is based around the ChipKIT Uno32 board and its JTAG capabilities;
* Arduino programmed as a JTAG controller;
* Serial communication to receive JTAG controls;
* Functionality to get ID code, turn on/off LED and read a button's state.


### Firmware implementation

`JTAGPort::begin()`:
* Setup TMS, TCK, TDO and TDI pins;
* Reset JTAG state machine;
* Doesn't return.

`void JTAGPort::reset()`:
* set TMS pin to HIGH;
* Do 5 clock Cycles to reach reset state on Jtag state machine;
* Doesn't return.

`void JTAGPort::clockPulse()`:
* Set TCK pin to HIGH;
* Wait 3 microseconds;
* Set TCK pin to LOW;
* Wait 3 microseconds;
* Doesn't return.

`void JTAGPort::sendBits(uint8_t tms, uint8_t tdo)`:
* Set TMS pin to TMS;
* Set TDO pin to TDO;
* Do a clock cycle.
* Doesn't return;

`void JTAGPort::sendBits(uint8_t tms, uint8_t tdo, uint8_t size)`:
* Sends TMS and TDO bits up to position size.

`void JTAGPort::setDR()`:
* Set JTAG state machine to DATA SHIFT state.

`void JTAGPort::setIR()`:
* Reset JATG state machine;
* Set JTAG state machine to INTRUCTION SHIFT.

`void JTAGPort::sendInst(uint8_t instruction)`:
* Set JTAG state machine to INSTRUCTION SHIFT;
* Write chosen instruction;
* Exit into JTAG state machine ready state.

`void JTAGPort::sendData(uint8_t tdo_pos, uint8_t value)`:
* Set JTAG state machine to DATA SHIFT state;
* Bit shift 0 into chosen pin Input register;
* Bit shift value into chosen pin Output register;
* Bit shift 1 into chosen pin Control register;
* Bit shift 1 into Reset;
* Exit into JTAG state machine ready state.

`uint8_t JTAGPort::readData()`:
* Read from TDI pin;
* Shift data bit;
* return 1 if TDI pin is HIGH or 0 if LOW.

`uint8_t JTAGPort::readData(uint8_t tdo_pos)`:
* Set JTAG state machine to DATA SHIFT state;
* Send bits until reaching tdo_pos;
* Read TDI pin;
* Exit into JTAG state machine ready state;
* return 1 if TDI pin is HIGH or 0 if LOW.

`uint32_t JTAGPort::getID()`:
* Set intruction to IDCODE;
* Set JTAG state machine to DATA SHIFT state;
* Read TDI and bitshift readings into value;
* Exit into JTAG state machine ready state;
* return value.

`void JTAGPort::setLED(uint8_t value)`:
* Set Intruction to EXTERNAL TEST;
* Write 1 to LED pin register.

`uint8_t JTAGPort::checkButton()`:
* Set Instruction to SAMPLE AND PRELOAD;
* Read button pin register;
* return value read.


`void setup()`:
* Start JTAG library;
* Start Serial port;
* Print commands.


`void loop()`:
* Checks if theres any command available in the Serial port:
    *  If command 'd', get and print ID code in hexadecimal;
    *  If command '1', turn on LED;
    *  If command '0', turn off LED;
    *  If command 'b', check button state.


### Firmware test

* Clone this repository with `git clone https://git.fe.up.pt/up201808899/sele_a2_b01_jtag.git`;
* Compile the *main.cpp* file with PlatformIO to get *.hex* file;
* Flash the *.hex* file to the Arduino board.
* Run Putty with baud rate 9600;



