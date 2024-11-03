/*
Ja, das ist eine clevere Idee und tatsächlich eine gängige Optimierung! 
Da das höchste Bit des Generators und des aktuellen Datenabschnitts bei jeder Division normalerweise „1“ ist 
(solange wir sicherstellen, dass es gesetzt ist), könnte man beide Bits „weglassen“ und stattdessen annehmen, dass sie immer 1 sind. 

### Wie funktioniert diese Optimierung?

1. **Normalisieren des Generatorpolynoms und Datenabschnitts**:
   - Da wir immer das führende Bit (MSB) des aktuellen Datenabschnitts mit dem Generator XOR-verknüpfen, reicht es aus, 
     nur die verbleibenden 16 Bits des Generators und des aktuellen Datenabschnitts zu speichern und zu verwenden.
   - Damit sparen wir uns das führende Bit und können so die gesamte Division und Berechnung um eine Stelle „verkleinern“.
   
2. **Führende Einsen ignorieren**:
   - Indem wir annehmen, dass das führende Bit des Datenabschnitts und des Generators immer „1“ ist, 
     können wir bei jedem Schritt nur auf die verbleibenden Bits achten.
   - Statt 17 Bit für den Generator und den Datenabschnitt zu verwenden, arbeiten wir nur mit den letzten 16 Bits.
   
3. **Effizientere Berechnung**:
   - Diese Annahme vereinfacht den Code und die Verarbeitung, da wir die 17-Bit-Schritte auf 16-Bit-Schritte reduzieren können.
   - Zudem wird die Rechenzeit reduziert, da wir nicht mehr auf das führende Bit achten müssen und auch keine Schiebungen für das höchste Bit durchführen müssen.

### Umsetzung mit 16-Bit-Abschnitten

Hiermit könnten wir also die Berechnung folgendermaßen umsetzen:

- **16-Bit-Abschnitte verwenden**: Zerlegen Sie den Datenblock in 16-Bit-Abschnitte (statt 17).
- **CRC-Berechnung mit reduziertem Generator**: Nutzen Sie das 16-Bit-Äquivalent des Generators, indem Sie das führende 1-Bit „wegkürzen“.
- **Schrittweise Aktualisierung der CRC**: Schieben Sie jeweils 16-Bit-Daten in die CRC-Berechnung und aktualisieren Sie den Restwert auf Basis 
    des reduzierten Generators.

Diese Vorgehensweise funktioniert, solange wir sicherstellen, dass das höchste Bit im Datenblock-Abschnitt (das wir uns „sparen“) 
tatsächlich immer gesetzt ist, was durch die Schiebung und Kombination der Abschnitte gesichert wird. 

### Zusammenfassung

Ja, durch das Weglassen des höchsten Bits (MSB) von Generator und Datenabschnitt können Sie die CRC-Berechnung effizienter gestalten. 
Dies erfordert aber, dass immer das führende Bit des Generatorpolynoms und des betrachteten Datenabschnitts als „1“ vorausgesetzt wird, 
was die Implementierung vereinfacht und die Effizienz der Berechnung steigert.

*/