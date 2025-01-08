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
    CRC crcInstance = CRC();

    if(strcmp(argv[1], "-s") == 0){
        Sender sender = Sender(crcInstance);
        sender.createDataBlocks();
        do{
            sender.send();
            sender.checkAKNFromReceiver();
        }while(sender.addBlocksForAdditionalSending());
        sender.sendEndOfTransmitting();
    }
    else if(strcmp(argv[1], "-r") == 0){
        Receiver receiver = Receiver(crcInstance);
        //ein lesender und ein verarbeitender Thread, 
        //Lesender soll solange arbeiten bis alles gelesen und alle akn gesendet wurden
        //Verarbeitender bis alle blocks erfolgreich da sind und diese zusammengesetzt wurden

        receiver.read();   //soll dan in einen thread
    }

    return 0;
}