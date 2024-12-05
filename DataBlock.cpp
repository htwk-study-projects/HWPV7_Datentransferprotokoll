#include <vector>
#include <stdexcept>
#include "ControlCharacter.hpp"
#include "CRC.cpp"

class DataBlock {
    private:
        std::vector<unsigned char> header;
        std::vector<unsigned char> data;
        std::vector<unsigned char> crcSum;
        int16_t blockNummer;
        static int16_t DATA_BLOCK_NUMMER;

        std::vector<unsigned char> createHeader(){
            std::vector<unsigned char> headerForCurrentBlock;
            headerForCurrentBlock.push_back(static_cast<unsigned char>(ControlCharacter::START));
            headerForCurrentBlock.push_back(static_cast<unsigned char>(ControlCharacter::ESC));
            headerForCurrentBlock.push_back(((DATA_BLOCK_NUMMER >> 8) & 0xff));
            headerForCurrentBlock.push_back((DATA_BLOCK_NUMMER & 0xff));
            this->blockNummer = DATA_BLOCK_NUMMER;
            DATA_BLOCK_NUMMER++;
            return headerForCurrentBlock;
        }

    public:

        DataBlock(const std::vector<unsigned char>& data, CRC crc) {
            this->header = createHeader();
            this->data = data;
            std::vector<unsigned char> headerAndData;
            headerAndData.insert(headerAndData.end(), header.begin(), header.end());
            headerAndData.insert(headerAndData.end(), data.begin(), data.end());
            std::vector<unsigned char> computedCrc;
            uint16_t crcResult = crc.calculateCRC16(headerAndData);
            computedCrc.push_back((crcResult >> 8) & 0xff);
            computedCrc.push_back(crcResult & 0xff);
            this->crcSum = computedCrc;
        }

       std::vector<unsigned char> getFullDataBlock() {
            std::vector<unsigned char> fullBlock;
            fullBlock.insert(fullBlock.end(), header.begin(), header.end());
            fullBlock.insert(fullBlock.end(), data.begin(), data.end());
            fullBlock.insert(fullBlock.end(), crcSum.begin(), crcSum.end());
            fullBlock.insert(fullBlock.end(), static_cast<char>(ControlCharacter::END));
            return fullBlock;
        }
        
        uint16_t getBlockNummer(){
            return this->blockNummer;
        }
};

int16_t DataBlock::DATA_BLOCK_NUMMER = 0;
