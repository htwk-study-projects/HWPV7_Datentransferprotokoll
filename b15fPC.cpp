#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>
#include <set>

#include "DataBlock.hpp"
#include "CRC.hpp"
#include "Sender.hpp"

//IDEE: Über ARGS* in main bestimmen ob eine Sender Instanz oder eine Empfänger Instanz erstellt wird

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    //drv.setRegister(&PORTA, 0x0f);
    
    Sender sender = Sender(drv);

    sender.createDataBlocks();
    sender.send();

    drv.setRegister(&PORTA, 0);
    return 0;
}