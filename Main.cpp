#include <iostream>
#include <b15f/b15f.h>

//auf board scshreiben und von board direkt wieder lesen klappt
//von arudino lesen klappt noch nicht

void schreibe(B15F & drv, char c){
    std::cout << "Schreibe: " << c << std::endl;
    drv.setRegister(&PORTA, c);
}

void lese(B15F & drv){
    std::cout << "Lese: " << ((char) drv.getRegister(&PINA)) << std::endl; 
}

int main(){
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0xff); //bunutzt gerade noch alle 8 bits
    char c = 'a';
    while(true){
       schreibe(drv, c);
       drv.delay_ms(100);
       lese(drv);
       c++;
    }

    return 0;
}
