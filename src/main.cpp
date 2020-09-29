#include <Arduino.h>
#include <jtag.h>

#define BAUD 9600

void print_commands() {
    Serial.println("Command list:");  

    Serial.println("d - get ID CODE in hex;");  
    Serial.println("1 - turn on LED;");  
    Serial.println("0 - turn off LED;");  
    Serial.println("b - get state of button in pin 29;");  
    Serial.println("i - print command list.");  
}

void setup() {
  JTAG.begin();
  Serial.begin(BAUD);

  Serial.println("Welcome to JTAG controller!\n\n");  
  print_commands();
}

void loop() {
    if(Serial.available()){
        switch(Serial.read()) {
            case 'd':
                Serial.println("Getting ID CODE...");
                Serial.print("ID CODE is 0x");
                Serial.print(JTAG.getID(), HEX);
                Serial.println(".");
                break;
            case '1':
                Serial.println("Turning on LED...");
                JTAG.setLED(HIGH);
                Serial.println("LED is on!");
                break;
            case '0':
                Serial.println("Turning off LED...");
                JTAG.setLED(LOW);
                Serial.println("LED is off!");
                break;
            case 'b':
                Serial.println("Checking button state...");
                Serial.print("Button is ");
                Serial.print(JTAG.checkButton() ? "not pressed" : "pressed");
                Serial.println("!");
                break;
            case 'i':
                print_commands();
                break;
            default:
                Serial.println("Command not available!");
        }
    }
}