
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

class CRC_Sum {
private:
    static const std::bitset<5> generator; // Generatorpolynom (x^4 + x + 1 => 10011)

    // Funktion für die XOR-Operation zwischen zwei bitsets
    std::bitset<12> xorBitsets(const std::bitset<12>& a, const std::bitset<5>& b, int shift) {
        std::bitset<12> result = a;
        for (int i = 0; i < b.size(); i++) {
            if (b.test(i)) {
                result.set(i + shift, !result.test(i + shift));
            }
        }
        return result;
    }

public:
    // Funktion zur Berechnung der CRC-Prüfziffer
    std::bitset<4> calculateCRC(const std::bitset<8>& data) {
        std::bitset<12> extendedData = std::bitset<12>(data.to_ulong() << 4); // D * 2^4
        std::bitset<12> remainder = extendedData;

        // Durchführung der Division
        for (int i = 11; i >= 4; i--) {
            if (remainder.test(i)) {
                remainder = xorBitsets(remainder, generator, i - 4);
            }
        }

        // Rückgabe des CRC-Rests als 4-Bit-Wert
        return std::bitset<4>(remainder.to_ulong());
    }

    // Funktion zum Erstellen des Datenblocks mit CRC
    std::bitset<12> createDataBlockWithCRC(const std::bitset<8>& data) {
        std::bitset<4> crc = calculateCRC(data);

        // Kombinieren der Daten und CRC
        std::bitset<12> dataWithCRC = (data.to_ulong() << 4); // Daten um 4 Bits nach links schieben
        dataWithCRC |= crc.to_ulong(); // CRC an die letzten 4 Bits anfügen

        return dataWithCRC; // Rückgabe des kombinierten Datenblocks mit CRC
    }


    // Funktion zur Überprüfung des Datenblocks
    bool verifyDataBlock(const std::bitset<12>& dataWithCRC) {
        std::bitset<12> remainder = dataWithCRC;

        // Durchführung der Division
        for (int i = 11; i >= 4; i--) {
            if (remainder.test(i)) {
                remainder = xorBitsets(remainder, generator, i - 4);
            }
        }

        // Überprüfung, ob der Rest null ist
        return remainder.none(); // true, wenn keine Bits gesetzt sind
    }

    // Funktion zur Anzeige der Bits in einem bitset
    template <std::size_t N>
    void displayBits(const std::bitset<N>& bits) {
    for (int i = bits.size() - 1; i >= 0; i--) {
        std::cout << bits[i];
    }
    std::cout << std::endl;
}
};

// Definition des Generatorpolynoms (10011)
const std::bitset<5> CRC_Sum::generator = 0b10011;

int main() {
    CRC_Sum crcCalculator;
    
    std::bitset<8> data("10111000"); // Beispiel-Daten
    std::cout << "Original Data: ";
    crcCalculator.displayBits(data);

    std::bitset<4> crc = crcCalculator.calculateCRC(data);
    std::cout << "CRC: ";
    crcCalculator.displayBits(crc);

    std::bitset<12> dataWithCRC = crcCalculator.createDataBlockWithCRC(data);
    std::cout << "Data Block with CRC: ";
    crcCalculator.displayBits(dataWithCRC);

    bool isValid = crcCalculator.verifyDataBlock(dataWithCRC);
    std::cout << "Data Block Verification: " << (isValid ? "No Errors" : "Errors Detected") << std::endl;

    return 0;
}
