#pragma once
#include <vector>
#include "CRC.hpp"

class DataBlock{
private:
    std::vector<unsigned char> header;
    std::vector<unsigned char> data;
    std::vector<unsigned char> crcSum;
    uint16_t blockNummer;
    static uint16_t DATA_BLOCK_NUMMER;
    std::vector<unsigned char> createHeader();

public:
    static const int MAX_LENGTH_DATA;

    DataBlock();  // Standardkonstruktor
    DataBlock(const std::vector<unsigned char>& data, CRC crc);  // Konstruktor mit Daten und CRC
    DataBlock(const DataBlock& other);  // Kopierkonstruktor
    std::vector<unsigned char> getFullBlock();
    uint16_t getBlockNummer();
};
