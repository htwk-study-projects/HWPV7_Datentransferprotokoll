#include <iostream>
#include <b15f/b15f.h>

// Übertragen von abwechselnd 0 und 1
int main(){
    B15f& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x01); //Bit 7-1 Eingabe, Bit 0 Ausgabe
    while(true){
        //Ausgabe
        std::cout << "Schreibe 0";
        drv.setRegister(&PORTA, 0); // ungeschicktes Alternieren
        std::cout << "Schreibe 1";
        drv.setRegister(&PORTA, 1);
    }

    return 0;
}

/* Asynchrones Lesen von PortA anderes PC mit seinem Board
int main(){
    B15f& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x00); //Bit 7-1 Eingabe, Bit 0 Ausgabe
    while(true){
    //Lesen
        std::cout << ((int) drv.getRegister(&PINA) << endl; 
        drv.delay_ms(10);
    }

    return 0;
}*/