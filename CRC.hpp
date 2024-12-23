#pragma once
#include <vector>
#include <cstdint>

class CRC {
private:
    static const uint32_t POLYNOMIAL;           // Generatorpolynom (x^16 + x^12 + x^5 + 1) CRC-16-CCITT False
    static std::vector<uint16_t> crcTable;     // Statische Variable für die CRC-Tabelle

public:
    // Konstruktor
    CRC();

    // Statische Methode, um die CRC-Tabelle zu initialisieren
    static void initializeCRCTable();

    // Berechnet den CRC-16-Wert für die übergebenen Daten
    uint16_t calculateCRC16(const std::vector<unsigned char>& data);

    // Verifiziert die Daten mit einem CRC-Wert
    bool verifyDataWithCRC(const std::vector<unsigned char>& dataWithCRC);
};
