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
        std::cerr << "Fehler: Bitte '-m' -> main oder '-s' -> side als Argument übergeben.\n";
        return 1;
    }
    CRC crcInstance = CRC();

    if(strcmp(argv[1], "-m") == 0){
        MainSender sender = MainSender(crcInstance);
        uint16_t blocksToSend = sender.createDataBlocks();
        sender.sendStartOfTransmitting();
        for(uint16_t currentBlock = 0; currentBlock < blocksToSend; currentBlock++){
            sender.sendDataBlock(currentBlock);
            if(!sender.verifyReadBlock()){
                sender.sendNAKN();
            }else sender.sendAKN();
            sender.delay(5000);
            if(!sender.checkAKN()) currentBlock--;
            if(currentBlock + 1 == blocksToSend) sender.sendEndOfTransmitting();
            else sender.sendNoEndOfTransmitting();
            sender.delay(5000);
        }
        // nur lesen und wenn der andere auch EOT sendet ist übertragung fertig  
       
    }
    else if(strcmp(argv[1], "-s") == 0){
        SideSender sender = SideSender(crcInstance);
        uint16_t blocksToSend = sender.createDataBlocks();
        while(!sender.waitForMainSender()){}
        for(uint16_t currentBlock = 0; currentBlock < blocksToSend; currentBlock++){
            sender.sendDataBlock(currentBlock);
            if(!sender.verifyReadBlock()){
                sender.sendNAKN();
            }else sender.sendAKN();
            sender.delay(5000);
            if(!sender.checkAKN()) currentBlock--;
            if(currentBlock + 1 == blocksToSend) sender.sendEndOfTransmitting();
            else sender.sendNoEndOfTransmitting();
            sender.delay(5000);            
        }
        // nur lesen und wenn der andere auch EOT sendet ist übertragung fertig 

    }

    return 0;
}