#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>
#include <set>

#include "DataBlock.hpp"
#include "CRC.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"

int main(int argc, char* argv[]) {
    if(argc < 2){
        std::cerr << "Fehler: Bitte '-s' -> sender oder '-r' -> receiver als Argument übergeben.\n";
        return 1;
    }
    B15F& drv = B15F::getInstance();
    CRC crcInstance = CRC();
    drv.setRegister(&DDRA, 0x0f);
    //drv.setRegister(&PORTA, 0x0f);
    if(strcmp(argv[1], "-s") == 0){
        Sender sender = Sender(drv, crcInstance);
        sender.createDataBlocks();
        do{
            sender.send();
            sender.checkAKNFromReceiver();
        }while(sender.addBlocksForAdditionalSending());     
    }
    else if(strcmp(argv[1], "-r") == 0){
        Reciever reciever = Reciever(drv, crcInstance);
    }


    drv.setRegister(&PORTA, 0);
    return 0;
}