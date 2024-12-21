#include "DataBlock.hpp"

// Initialisierung der statischen Variablen
int16_t DataBlock::DATA_BLOCK_NUMMER = 0;
const int DataBlock::MAX_LENGTH_DATA = 128;

// Konstruktor ohne Parameter
DataBlock::DataBlock() {
    this->header = {}; // Leere Liste für header
    this->data = {};   // Leere Liste für data
    this->crcSum = {}; // Leere Liste für crcSum
    this->blockNummer = -1;
}

// Konstruktor mit Daten und CRC
DataBlock::DataBlock(const std::vector<unsigned char>& data, CRC crc) {
    this->header = createHeader();
    this->data = data;

    // Header und Daten zusammenführen
    std::vector<unsigned char> headerAndData;
    headerAndData.insert(headerAndData.end(), header.begin(), header.end());
    headerAndData.insert(headerAndData.end(), data.begin(), data.end());

    // CRC berechnen
    std::vector<unsigned char> computedCrc;
    uint16_t crcResult = crc.calculateCRC16(headerAndData);
    computedCrc.push_back((crcResult >> 8) & 0xff);
    computedCrc.push_back(crcResult & 0xff);
    this->crcSum = computedCrc;
}

// Kopierkonstruktor
DataBlock::DataBlock(const DataBlock& other) {
    this->header = other.header;
    this->data = other.data;
    this->crcSum = other.crcSum;
    this->blockNummer = other.blockNummer;
}

// Methode zum Erstellen des vollständigen Datenblocks
std::vector<unsigned char> DataBlock::getFullDataBlock() {
    std::vector<unsigned char> fullBlock;
    //fullBlock.insert(fullBlock.end(), header.begin(), header.end());
    fullBlock.insert(fullBlock.end(), data.begin(), data.end());
    //fullBlock.insert(fullBlock.end(), crcSum.begin(), crcSum.end());
    //fullBlock.insert(fullBlock.end(), static_cast<char>(ControlCharacter::END));  // End-Byte hinzufügen
    return fullBlock;
}

// Methode zum Abrufen der Blocknummer
uint16_t DataBlock::getBlockNummer() {
    return this->blockNummer;
}

// Private Methode zum Erstellen des Headers
std::vector<unsigned char> DataBlock::createHeader() {
    std::vector<unsigned char> headerForCurrentBlock;
    headerForCurrentBlock.push_back(static_cast<unsigned char>(ControlCharacter::START));
    headerForCurrentBlock.push_back(static_cast<unsigned char>(ControlCharacter::ESC));
    headerForCurrentBlock.push_back(((DATA_BLOCK_NUMMER >> 8) & 0xff));
    headerForCurrentBlock.push_back((DATA_BLOCK_NUMMER & 0xff));
    this->blockNummer = DATA_BLOCK_NUMMER;
    DATA_BLOCK_NUMMER++;  // Die Blocknummer für den nächsten Datenblock erhöhen
    return headerForCurrentBlock;
}
