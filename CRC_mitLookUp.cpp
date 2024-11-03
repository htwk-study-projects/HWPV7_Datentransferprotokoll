#include <iostream>
#include <bitset>
#include <array>

template <std::size_t DataSize, std::size_t GeneratorSize, std::size_t CRCSize>
class CRC_Sum {
private:
    static const std::bitset<GeneratorSize> generator;
    static std::array<uint16_t, 256> crcTable; // Statische Lookup-Tabelle
    static bool isInitialized;

    static void initializeCRCTable() {
        uint16_t polynomial = generator.to_ullong(); // Holen des Generatorpolynoms als ganzzahligen Wert
        for (int i = 0; i < 256; i++) {
            uint16_t crc = i;
            for (int j = 0; j < 8; j++) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ polynomial;
                } else {
                    crc >>= 1;
                }
            }
            crcTable[i] = crc;
        }
    }

public:
    CRC_Sum() {
        if (!isInitialized) {
            initializeCRCTable(); 
            isInitialized = true;
        }
    }

    std::bitset<CRCSize> calculateCRC(const std::bitset<DataSize>& data) {
        uint16_t crc = 0xFFFF; // Startwert

        // Iteration durch die Daten in Bytes
        for (int i = 0; i < DataSize; i += 8) {
            uint8_t byte = (data >> (DataSize - 8 - i)).to_ullong() & 0xFF; // Nächstes Byte
            crc = (crc >> 8) ^ crcTable[(crc ^ byte) & 0xFF]; // Lookup und Aktualisierung des CRC
        }

        return std::bitset<CRCSize>(crc);
    }

    std::bitset<DataSize + CRCSize> createDataBlockWithCRC(const std::bitset<DataSize>& data) {
        std::bitset<CRCSize> crc = calculateCRC(data);
        std::bitset<DataSize + CRCSize> dataWithCRC = (data.to_ullong() << CRCSize) | crc.to_ullong();
        return dataWithCRC;
    }

    bool verifyDataBlock(const std::bitset<DataSize + CRCSize>& dataWithCRC) {
        uint16_t crc = 0xFFFF;

        // Iteration durch die Daten in Bytes
        for (int i = 0; i < DataSize + CRCSize; i += 8) {
            uint8_t byte = (dataWithCRC >> (DataSize + CRCSize - 8 - i)).to_ullong() & 0xFF; 
            crc = (crc >> 8) ^ crcTable[(crc ^ byte) & 0xFF]; 
        }

        return crc == 0; // Rückgabe, ob kein Fehler aufgetreten ist
    }

    template <std::size_t N>
    void displayBits(const std::bitset<N>& bits) {
        for (int i = bits.size() - 1; i >= 0; i--) {
            std::cout << bits[i];
        }
        std::cout << std::endl;
    }
};

// Definition des Generatorpolynoms (z.B. CRC-16-CCITT)
template<>
const std::bitset<17> CRC_Sum<128, 17, 16>::generator = 0x11021; // 0b10001000000100001 in hex

// Statische Lookup-Tabelle
template<>
std::array<uint16_t, 256> CRC_Sum<128, 17, 16>::crcTable = {0}; // Initialisierung der Tabelle

template<>
bool CRC_Sum<128, 17, 16>::isInitialized = false;


int main() {
    CRC_Sum<128, 17, 16> crcCalculator;

    std::bitset<128> data("1100101011110001001110000000000000000000000000000000000000000000"); // Beispiel-Daten
    std::cout << "Original Data: ";
    crcCalculator.displayBits(data);

    std::bitset<16> crc = crcCalculator.calculateCRC(data);
    std::cout << "CRC: ";
    crcCalculator.displayBits(crc);

    std::bitset<144> dataWithCRC = crcCalculator.createDataBlockWithCRC(data);
    std::cout << "Data Block with CRC: ";
    crcCalculator.displayBits(dataWithCRC);
//0001110100110110
//0010001011001110
    bool isValid = crcCalculator.verifyDataBlock(dataWithCRC);
    std::cout << "Data Block Verification: " << (isValid ? "No Errors" : "Errors Detected") << std::endl;

    return 0;
}
