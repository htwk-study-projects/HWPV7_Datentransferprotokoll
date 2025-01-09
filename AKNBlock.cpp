#include "AKNBlock.hpp"

AKNBlock::AKNBlock(unsigned char c, uint16_t i){
    this->controlChar = c;
    this->blockNummer = i;
}

std::vector<unsigned char> AKNBlock::getFullBlock(){
    std::vector<unsigned char> fullBlock;
    fullBlock.insert(fullBlock.end(), this->controlChar);
    fullBlock.insert(fullBlock.end(), (this->blockNummer >> 8) & 0xff);
    fullBlock.insert(fullBlock.end(), this->blockNummer & 0xff);
    return fullBlock;
}