#include "Receiver.hpp"

#include <iostream>

Receiver::Receiver(CRC crc){
    this->USED_CRC_INSTANCE = crc;
    this->needToRead = true;
    this->b15.setRegister(&DDRA, 0x0f);
}

void Receiver::readWithSendAKN(AKNBlock akn){
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : akn.getFullAKNBlock()) {
        for (int j = 7; j >= 0; j--) {
            bitStream = (bitStream << 1) | ((currentChar >> j) & 0x01);
            bitCount++;
            if (bitCount == 3) {                                            //muss aufgrund der größe immer aufgehen hier
                contactB15(bitStream & 0x07, true);
                bitCount = 0;
            }
        }
    }

}

void Receiver::readWithoutSendAKN(){
    contactB15(0, false);
}

void Receiver::contactB15(int data, bool withSend){
    if(withSend){
        this->b15.setRegister(&PORTA, data | 0b00001000);
        this->b15.delay_ms(10);       
    }
    else{
        this->b15.delay_ms(15); 
    }
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b00010000){
        this->inputBuffer.push_back((currentInput >> 5));
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
    this->b15.delay_ms(15);
}

void Receiver::read(){
    std::cerr << "start reading"  << std::endl;
    while(needToRead){
        if(!AKNBlocks.empty()){
            AKNBlock akn = AKNBlocks.front();
            AKNBlocks.pop_front();
            std::cerr << "read with send akn" << std::endl;
            readWithSendAKN(akn);
        }
        else readWithoutSendAKN();
    }
    std::cerr << "stopped reading" << std::endl;
}

void Receiver::processReadBlocks(){

    //muss endzeichen abschneiden und verifikation durchführen

    //muss Headererkennung haben wenn verfikation passt
    //muss Datenextrahieren
}

bool Receiver::hasTransmissionStarted(){
    if(this->b15.getRegister(&PINA) == 0b00001111) return true;
    return false;
}
