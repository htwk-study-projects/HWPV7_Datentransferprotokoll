#include <iostream>
#include <b15f/b15f.h>

void schreibe(B15F & drv, int data) {
    drv.setRegister(&PORTA, data | 0b00001000);
    drv.delay_ms(10);
    drv.setRegister(&PORTA, 0x00);
    std::bitset<3> bin(data);
    std::cout << bin << std::endl;
}



void sendeDaten(B15F & drv) {
    unsigned int bitStream = 0;
    int bitCount = 0;
    char currentChar;
    while (std::cin.get(currentChar)) {
        for (int j = 7; j >= 0; j--) {
            bitStream = (bitStream << 1) | ((currentChar >> j) & 0x01);
            bitCount++;
            if (bitCount == 3) {
                schreibe(drv, bitStream & 0x07);
                bitStream = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitStream = bitStream << (3 - bitCount); // Verschiebe die verbleibenden Bits nach links, um Nullen hinzuzufügen
        schreibe(drv, bitStream & 0x07); //
    }
}

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    sendeDaten(drv);
    drv.setRegister(&PORTA, 0);
    std::cout<<std::endl;
}