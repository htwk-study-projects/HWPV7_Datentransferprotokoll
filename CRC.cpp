#include "CRC.hpp"
#include <vector>
#include <iostream>

const uint32_t CRC::POLYNOMIAL = 0x11021;  // Generatorpolynom (x^16 + x^12 + x^5 + 1) CRC-16-CCITT False
std::vector<uint16_t> CRC::crcTable;

// Konstruktor
CRC::CRC() {
    if (crcTable.empty()) {
        initializeCRCTable();
    }
}

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

uint16_t CRC::calculateCRC16(const std::vector<unsigned char>& data) {
    uint16_t crc = 0xFFFF; // Initialwert für CRC (alle Einsen, weil CRC-16-CCITT False)

    for (unsigned char byte : data) {
        uint8_t tableIndex = ((crc >> 8) ^ byte) & 0xFF;
        crc = (crc << 8) ^ crcTable[tableIndex];
    }

    return crc;
}

bool CRC::verifyDataWithCRC(const std::vector<unsigned char>& dataWithCRC) {
    uint16_t crc = calculateCRC16(dataWithCRC);
    return (crc == 0);
}
std::vector<unsigned char> hexStringToByteVector(const std::string& hexString) {
    std::vector<unsigned char> result;
    for (size_t i = 0; i < hexString.length(); i += 2) {
        unsigned char byte = std::stoi(hexString.substr(i, 2), nullptr, 16);
        result.push_back(byte);
    }
    return result;
}

/*
int main() {
    // Erstelle eine Instanz der CRC-Klasse
    CRC crc;

    // Eingabe im Hex-Format (Beispiel: "48 65 6C 6C 6F" -> "Hallo")
    std::string hexInput = "021B000048616C6C6F3429"; // Entspricht "Hallo" in Hex

    // Wandle den Hex-String in einen Vektor von unsigned char um
    std::vector<unsigned char> data = hexStringToByteVector(hexInput);

    // Berechnet die CRC-16 Prüfziffer
    uint16_t crcResult = crc.calculateCRC16(data);

    // Gibt das Ergebnis im Hex-Format aus
    std::cout << "CRC-16 (CCITT False) für Hex '"
              << hexInput << "': 0x"
              << std::hex << std::uppercase << crcResult << std::endl;

    return 0;
}*/