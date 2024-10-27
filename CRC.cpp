
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

class CRC_Sum {
private:

};