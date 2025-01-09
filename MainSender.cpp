#include "MainSender.hpp"

MainSender::MainSender(CRC crc) : Sender(crc, true) {}

void MainSender::sendStartOfTransmitting(){
    this->b15.setRegister(&PORTA, 0b00001000);
    this->b15.delay_ms(10);
    this->b15.setRegister(&PORTA, 0b00000000);
}
