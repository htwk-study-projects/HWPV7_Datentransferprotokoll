#include <iostream>
//#include <b15f/b15f.h>
#include <map>
#include <vector>
#include <set>

#include "CRC.hpp"
#include "MainSender.hpp"
#include "SideSender.hpp"


int main(int argc, char* argv[]) {
    if(argc < 2){
        std::cerr << "Fehler: Bitte '-s' -> sender oder '-r' -> receiver als Argument übergeben.\n";
        return 1;
    }
    CRC crcInstance = CRC();

    if(strcmp(argv[1], "-s") == 0){
        MainSender sender = MainSender(crcInstance);
        uint16_t blocksToSend = sender.createDataBlocks();
        sender.sendStartOfTransmitting();
        for(uint16_t currentBlock = 0; currentBlock <= blocksToSend; currentBlock++){
            sender.sendDataBlock(currentBlock);
            if(sender.verifyReadBlock()){
                //sendNAKN();
            }
            //sendAKN();
            sender.checkAKN();
            //currentBlock -1 rechnen bei NAKN
        }           

        sender.sendEndOfTransmitting();
    }
    else if(strcmp(argv[1], "-r") == 0){
        SideSender sender = SideSender(crcInstance);
        uint16_t blocksToSend = sender.createDataBlocks();
        while(!sender.waitForMainSender()){}
        for(uint16_t currentBlock = 0; currentBlock <= blocksToSend; currentBlock++){
            sender.sendDataBlock(currentBlock);
            if(sender.verifyReadBlock()){
                //sendNAKN();
            }
            //sendAKN()
            sender.checkAKN();
            //currentBlock -1 rechnen bei NAKN
        }
        sender.sendEndOfTransmitting();

    }

    return 0;
}