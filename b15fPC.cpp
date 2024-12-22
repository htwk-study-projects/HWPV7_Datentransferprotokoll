#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>
#include <set>

#include "DataBlock.hpp"
#include "CRC.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"

//IDEE: Über ARGS* in main bestimmen ob eine Sender Instanz oder eine Empfänger Instanz erstellt wird
// sollte umgesetzt sein
int main(int argc, char* argv[]) {
    if(argc < 2){
        std::cerr << "Fehler: Bitte '-s' -> sender oder '-r' -> receiver als Argument übergeben.\n";
        return 1;
    }
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    //drv.setRegister(&PORTA, 0x0f);
    if(strcmp(argv[1], "-s") == 0){
        Sender sender = Sender(drv);
        sender.createDataBlocks();
        sender.send();
    }
    else if(strcmp(argv[1], "-r") == 0){
        Reciever reciever = Reciever();
    }


    drv.setRegister(&PORTA, 0);
    return 0;
}