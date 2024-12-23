#include "Receiver.hpp"

Reciever::Reciever(B15F & drv, CRC crc){
    this->b15 = drv;
    this->USED_CRC_INSTANCE = crc;
}
