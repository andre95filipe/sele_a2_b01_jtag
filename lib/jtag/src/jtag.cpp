#include <jtag.h>

void JTAGPort::begin() {
    DDRB |= _BV(TMS) |
            _BV(TCK) |
            _BV(TDO);

    DDRB &= ~_BV(TDI);

    PORTB &= ~(_BV(TMS) |
               _BV(TCK) |
               _BV(TDI) |
               _BV(TDO));

    this->reset();
}

void JTAGPort::reset() {
    for(int i = 0; i < 5; i++) {
        PORTB |= _BV(TMS);
        this->clockPulse();
    }
}

void JTAGPort::clockPulse() {
    PORTB |= _BV(TCK);
    delayMicroseconds(3);
    PORTB &= ~_BV(TCK);
    delayMicroseconds(3);
}

void JTAGPort::sendBits(uint8_t tms, uint8_t tdo) {
    PORTB = tms ? PORTB | _BV(TMS) : PORTB & ~_BV(TMS);
    PORTB = tdo ? PORTB | _BV(TDO) : PORTB & ~_BV(TDO);
    this->clockPulse();
}

void JTAGPort::sendBits(uint8_t tms, uint8_t tdo, uint8_t size) {
    for(int i = 0; i < size; i++) 
        this->sendBits((tms & _BV(i)), (tdo & _BV(i)));
}

void JTAGPort::setDR() {
    //this->reset();
    this->sendBits(DR_SELECT_TMS, DR_SELECT_TDO, DR_SELECT_SIZE);
}

void JTAGPort::setIR() {
    this->reset();
    this->sendBits(IR_SELECT_TMS, IR_SELECT_TDO, IR_SELECT_SIZE);
}

void JTAGPort::sendData(uint8_t tdo_pos, uint8_t value) {
    uint8_t i = 0;

    this->setDR();
    
    while(i < BOUNDARY_SIZE - 1) {
        if(i == _INPUT(tdo_pos)) 
            this->sendBits(0, 0);
        else if(i == _OUTPUT(tdo_pos)) 
            this->sendBits(0, value);
        else if(i == _CONTROL(tdo_pos)) 
            this->sendBits(0, 1);
        else if(i == 129)
            this->sendBits(0, 1);
        else
            this->sendBits(0, 0);
        i++;
    }

    this->sendBits(EXIT_SHIFT_TMS, EXIT_SHIFT_TDO, EXIT_SHIFT_SIZE);

}

void JTAGPort::sendInst(uint8_t instruction) {
    this->setIR();

    this->sendBits(IR_UPDATE_TMS, instruction, IR_UPDATE_SIZE);
    
    this->sendBits(EXIT_SHIFT_TMS >> 1, EXIT_SHIFT_TDO, EXIT_SHIFT_SIZE - 1);
}

uint8_t JTAGPort::readData(uint8_t tdo_pos) {
    uint8_t value = 0;

    this->setDR();

    for(int i = 0; i < tdo_pos; i++)
        this->sendBits(0, 0);

    value = (PINB & _BV(TDI)) ? 1 : 0;
    
    this->sendBits(EXIT_SHIFT_TMS, EXIT_SHIFT_TDO, EXIT_SHIFT_SIZE);
    
    return value;
}

uint32_t JTAGPort::readData() { 
    uint32_t value = (PINB & _BV(TDI) ? 1 : 0); 
    this->sendBits(0, 0);
    return value;
}

uint32_t JTAGPort::getID() {
    uint32_t value = 0;

    this->sendInst(IDCODE_TDO);

    this->setDR();

    for(int i = 0; i < IDCODE_SIZE; i++)
        value |= (this->readData() << i);

    this->sendBits(EXIT_SHIFT_TMS, EXIT_SHIFT_TDO, EXIT_SHIFT_SIZE);
    
    return value;
}

void JTAGPort::setLED(uint8_t value) {

    this->sendInst(EXTERNAL_TEST_TDO);
    
    this->sendData(LED_PIN, value);
}

uint8_t JTAGPort::checkButton() {
    uint8_t value = 0;

    this->sendInst(SAMPLE_PRELOAD_TDO);
    
    value = this->readData(BUTTON_PIN);

    return value;
}