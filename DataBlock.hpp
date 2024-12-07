#pragma once
#include <vector>
#include "ControlCharacter.hpp"
#include "CRC.hpp"

class DataBlock {
private:
    std::vector<unsigned char> header;
    std::vector<unsigned char> data;
    std::vector<unsigned char> crcSum;
    int16_t blockNummer;
    static int16_t DATA_BLOCK_NUMMER;

    // Methode zum Erstellen des Headers
    std::vector<unsigned char> createHeader();

public:
    // Konstruktoren
    DataBlock();  // Standardkonstruktor
    DataBlock(const std::vector<unsigned char>& data, CRC crc);  // Konstruktor mit Daten und CRC
    DataBlock(const DataBlock& other);  // Kopierkonstruktor

    // Öffentliche Methoden
    std::vector<unsigned char> getFullDataBlock();
    uint16_t getBlockNummer();
};
