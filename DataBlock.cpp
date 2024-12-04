#include <vector>
#include <stdexcept>
#include "ControlCharacter.hpp"
#include "CRC.cpp"

class DataBlock {
    private:
        std::vector<char> header;
        std::vector<char> data;
        std::vector<char> crcSum;
        int16_t blockNummer;
        static int16_t DATA_BLOCK_NUMMER;

        std::vector<char> createHeader(){
            std::vector<char> headerForCurrentBlock;
            headerForCurrentBlock.push_back(static_cast<char>(ControlCharacter::START));
            headerForCurrentBlock.push_back(static_cast<char>(ControlCharacter::ESC));
            headerForCurrentBlock.push_back(((DATA_BLOCK_NUMMER >> 8) & 0xff));
            headerForCurrentBlock.push_back((DATA_BLOCK_NUMMER & 0xff));
            this->blockNummer = DATA_BLOCK_NUMMER;
            DATA_BLOCK_NUMMER++;
            return headerForCurrentBlock;
        }

    public:

        DataBlock(const std::vector<char>& data, CRC crc) {
            this->header = createHeader();
            this->data = data;

        }

       std::vector<char> getFullDataBlock() {
            std::vector<char> fullBlock;
            fullBlock.insert(fullBlock.end(), header.begin(), header.end());
            fullBlock.insert(fullBlock.end(), data.begin(), data.end());
            fullBlock.insert(fullBlock.end(), crcSum.begin(), crcSum.end());
            fullBlock.insert(fullBlock.end(), static_cast<char>(ControlCharacter::END));
            return fullBlock;
        }
        
        void setCrcSum(const std::vector<char>& crc){
            this->crcSum = crc;
        }
        
        uint16_t getBlockNummer(){
            return this->blockNummer;
        }
};

int16_t DataBlock::DATA_BLOCK_NUMMER = 0;
