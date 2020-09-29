#include <Arduino.h>

#define IR_SELECT_TMS  0b00110
#define IR_SELECT_TDO  0b00000
#define IR_SELECT_SIZE 5

#define DR_SELECT_TMS  0b0010
#define DR_SELECT_TDO  0b0000
#define DR_SELECT_SIZE 4

#define EXIT_SHIFT_TMS  0b011
#define EXIT_SHIFT_TDO  0b000
#define EXIT_SHIFT_SIZE 3

#define _INPUT(bit)     (bit + 0)
#define _OUTPUT(bit)    (bit + 1)
#define _CONTROL(bit)   (bit + 2)


// Instruction codes
#define IDCODE_TDO          0b00001
#define SAMPLE_PRELOAD_TDO  0b00010
#define EXTERNAL_TEST_TDO   0b00110

#define IR_UPDATE_TMS   0b10000
#define IR_UPDATE_SIZE  5

#define BOUNDARY_SIZE  148

#define IDCODE_SIZE   32

#define TMS PORTB0
#define TCK PORTB1
#define TDI PORTB2
#define TDO PORTB3

#define LED_PIN     18
#define BUTTON_PIN  3


class JTAGPort {
    private:
        // Low level methods
        void reset();
        void clockPulse();
        void sendBits(uint8_t tms, uint8_t tdo);
        void sendBits(uint8_t tms, uint8_t tdo, uint8_t size);
        void setDR();
        void setIR();
        void sendData(uint8_t tdo, uint8_t value);
        void sendInst(uint8_t instruction);
        uint8_t readData(uint8_t tdo_pos);
        uint32_t readData();
    public:
        // High level methods
        void begin();
        uint32_t getID();
        void setLED(uint8_t value);
        uint8_t checkButton();
};

extern JTAGPort JTAG;