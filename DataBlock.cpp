#include <vector>
#include <stdexcept>
#include "ControlCharacter.hpp"

class DataBlock {
    private:
        std::vector<char> header;
        std::vector<char> data;
        std::vector<char> crcSum;
        static int16_t DATA_BLOCK_NUMMER;

        std::vector<char> createHeader(){
            std::vector<char> headerForCurrentBlock;
            headerForCurrentBlock.push_back(static_cast<char>(ControlCharacter::START));
            headerForCurrentBlock.push_back(static_cast<char>(ControlCharacter::ESC));
            headerForCurrentBlock.push_back(((DATA_BLOCK_NUMMER >> 8) & 0xff));
            headerForCurrentBlock.push_back((DATA_BLOCK_NUMMER & 0xff));
            DATA_BLOCK_NUMMER++;
            return headerForCurrentBlock;
        }

    public:

        DataBlock(const std::vector<char>& data, const std::vector<char>& crcSum) {
            this->header = createHeader();
            this->data = data;
            this->crcSum = crcSum;
        }

       std::vector<char> getFullDataBlock() {
            std::vector<char> fullBlock;
            fullBlock.insert(fullBlock.end(), header.begin(), header.end());
            fullBlock.insert(fullBlock.end(), data.begin(), data.end());
            fullBlock.insert(fullBlock.end(), crcSum.begin(), crcSum.end());
            fullBlock.insert(fullBlock.end(), static_cast<char>(ControlCharacter::END));
            return fullBlock;
        }
        
        
};

int16_t DataBlock::DATA_BLOCK_NUMMER = 0;
