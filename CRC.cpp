#include "CRC.hpp"  // Header-Datei einbinden
#include <vector>

// Definition der statischen Variablen
const uint32_t CRC::POLYNOMIAL = 0x11021;  // Generatorpolynom (x^16 + x^12 + x^5 + 1) CRC-16-CCITT False
std::vector<uint16_t> CRC::crcTable;  // Definition der CRC-Tabelle

// Konstruktor
CRC::CRC() {
    if (crcTable.empty()) {
        initializeCRCTable();
    }
}

// Statische Methode zur Initialisierung der CRC-Tabelle
void CRC::initializeCRCTable() {
    crcTable.resize(256);
    for (int i = 0; i < 256; ++i) {
        uint16_t crc = i << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc = crc << 1;
            }
        }
        crcTable[i] = crc;
    }
}

// Berechnet den CRC-16-Wert für die gegebenen Daten
uint16_t CRC::calculateCRC16(const std::vector<unsigned char>& data) {
    uint16_t crc = 0xFFFF; // Initialwert für CRC (alle Einsen, weil CRC-16-CCITT False)

    for (unsigned char byte : data) {
        uint8_t tableIndex = ((crc >> 8) ^ byte) & 0xFF;
        crc = (crc << 8) ^ crcTable[tableIndex];
    }

    return crc;
}

// Verifiziert die Daten mit einem CRC-Wert
bool CRC::verifyDataWithCRC(const std::vector<unsigned char>& dataWithCRC) {
    uint16_t crc = calculateCRC16(dataWithCRC);
    return (crc == 0);
}
