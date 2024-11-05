/*
CRC-Summe = (Daten-Block * 2^Grad(Generatorpolynom)) mod Generatorpolynom
CRC-Länge enspricht Grad des Generatorpolynom

Datenblock mit CRC = (Daten-Block * 2^Grad(Generatorpolynom) + CRC-Summe;

Fehlererkennung dann mittels CRC = (Datenblock mit CRC) mod Generatorpolynom = 0 ?

Beispiel:
CRC-Summe Berechnung:
D = 10111001
GP = x^4 + x + 1 => 10011
n = Grad(GP) = 4
D * 2^4 = 10111001_0000
CRC-Summe = (D * 2^n) mod GP
CRC-Summe = (10111001_0000) mod 10011 = 1001
gerechnet:
    101110010000 / 10011
XOR 10011
    0010000
XOR   10011
      00011100  
XOR      10011
         011110
XOR       10011
          011010
XOR        10011
           01001 -> Rest => CRC-Summe 1001 so lang wie Grad von GP

Datenblock mit CRC = (Daten-Block * 2^Grad(Generatorpolynom) + CRC-Summe
Datenblock mit CRC = 10111001_1001

Fehlererkennung:
    101110011001 / 10011
XOR 10011
    0010000
XOR   10011
      00011110
XOR      10011
         011010
XOR       10011
          010011 
XOR        10011 
           00000 Rest = 0 Übertragung ohne Fehler           
*/

#include <iostream>
#include <string>
#include <bitset>

template <std::size_t DataSize, std::size_t GeneratorSize, std::size_t CRCSize>
class CRC_Sum {
private:
    static const std::bitset<GeneratorSize> generator;

    std::bitset<DataSize + CRCSize> xorBitsets(const std::bitset<DataSize + CRCSize>& data, const std::bitset<GeneratorSize>& generatorPolynom, int shift) {
        std::bitset<DataSize + CRCSize> result = data;
        for (int i = 0; i < generatorPolynom.size(); i++) {
            if (generatorPolynom.test(i)) {
                result.set(i + shift, !result.test(i + shift));
            }
        }
        return result;
    }

public:

    std::bitset<CRCSize> calculateCRC(const std::bitset<DataSize>& data) {
        std::bitset<DataSize + CRCSize> extendedData = data.to_ullong(); 
        extendedData <<= CRCSize;
        std::bitset<DataSize + CRCSize> remainder = extendedData;
        for (int i = DataSize + CRCSize -1 ; i >= GeneratorSize - 1; i--) {
            if (remainder.test(i)) {
                remainder = xorBitsets(remainder, generator, i - (GeneratorSize - 1));
            }
        }
        return std::bitset<CRCSize>(remainder.to_ullong());
    }

    std::bitset<DataSize + CRCSize> createDataBlockWithCRC(const std::bitset<DataSize>& data) {
        std::bitset<CRCSize> crc = calculateCRC(data);

        std::cout << "Original-Data: "; displayBits(data);
        std::bitset<DataSize + CRCSize> dataWithCRC = data.to_ullong();
        dataWithCRC <<= CRCSize;
        std::cout << "Original_Data: "; displayBits(dataWithCRC);
        dataWithCRC |= crc.to_ullong();

        return dataWithCRC;
    }

    bool verifyDataBlock(const std::bitset<DataSize + CRCSize>& dataWithCRC) {
        std::bitset<DataSize + CRCSize> remainder = dataWithCRC;
        std::cout << "Original Data: ";
        displayBits(remainder);
        for (int i = DataSize + CRCSize -1 ; i >= GeneratorSize-1; i--) {
            if (remainder.test(i)) {
                remainder = xorBitsets(remainder, generator, i - (GeneratorSize - 1));
            }
        }
        std::cout << "Original Test: ";
        displayBits(remainder);
        return remainder.none(); // true, wenn keine Bits gesetzt sind
    }

    template <std::size_t N>
    void displayBits(const std::bitset<N>& bits) {
    for (int i = bits.size() - 1; i >= 0; i--) {
        std::cout << bits[i];
    }
    std::cout << std::endl;
}
};

//funktioniert nur bis 32 Bit Datenblock
const u_long d = 64;
const u_long g = 17;
const u_long c = 16;

template<>
const std::bitset<g> CRC_Sum<d, g, c>::generator = 0b10001000000100001; //CRC-CCITT (CRC-16)

int main() {
    CRC_Sum<d, g, c> crcCalculator;
    
    std::bitset<d> data("1011100110111001101110011011100110111001101110011011100110111001"); // Beispiel-Daten
    std::cout << "Original Data: ";
    crcCalculator.displayBits(data);

    std::bitset<c> crc = crcCalculator.calculateCRC(data);
    std::cout << "CRC: ";
    crcCalculator.displayBits(crc);

    std::bitset<d + c> dataWithCRC = crcCalculator.createDataBlockWithCRC(data);
    std::cout << "OriginalCData: ";
    crcCalculator.displayBits(dataWithCRC);

    bool isValid = crcCalculator.verifyDataBlock(dataWithCRC);
    std::cout << "Data Block Verification: " << (isValid ? "No Errors" : "Errors Detected") << std::endl;

    return 0;
}
