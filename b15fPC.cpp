#include <iostream>
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
        uint16_t currentBlock = 0;
        uint16_t blocksToSend = sender.createDataBlocks();
        std::vector<bool> blockReceived(blocksToSend, false); 
        std::vector<bool> blockSendedSuccessfully(blocksToSend, false); 
        
        while(currentBlock < blocksToSend){
            sender.sendStartOfTransmitting();
            sender.sendDataBlock(currentBlock);
            if(!sender.verifyReadBlock()){
                sender.sendNAKN();
            } 
            else{
                sender.sendAKN();
                blockReceived[currentBlock] = true;
            }
            sender.delay(5000);
            if(sender.checkAKN()) blockSendedSuccessfully[currentBlock] = true;
            if(blockSendedSuccessfully[currentBlock] && blockReceived[currentBlock]) currentBlock++;
            sender.delay(5000);
        }

        sender.printReceivedData();
       
    }
    else if(strcmp(argv[1], "-s") == 0){
        SideSender sender = SideSender(crcInstance);
        uint16_t currentBlock = 0;
        uint16_t blocksToSend = sender.createDataBlocks();
        std::vector<bool> blockReceived(blocksToSend, false); 
        std::vector<bool> blockSendedSuccessfully(blocksToSend, false);
        
        while(currentBlock < blocksToSend){
            while(!sender.waitForMainSender()){}
            sender.sendDataBlock(currentBlock);
            if(!sender.verifyReadBlock()){
                sender.sendNAKN();
            } 
            else{
                sender.sendAKN();
                blockReceived[currentBlock] = true;
            }
            sender.delay(5000);
            if(sender.checkAKN()) blockSendedSuccessfully[currentBlock] = true;
            if(blockSendedSuccessfully[currentBlock] && blockReceived[currentBlock]) currentBlock++;
            sender.delay(5000);           
        }

        sender.printReceivedData();

    }

    return 0;
}