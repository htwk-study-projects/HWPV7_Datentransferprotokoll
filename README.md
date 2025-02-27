# HWPV7 Datentransferprotokoll

## Überblick
HWPV7 ist ein spezialisiertes Datentransferprotokoll für die Kommunikation zwischen zwei Computern über B15 Boards.

## Verwendete Programmiersprache
C++

## Funktionsweise
Das Protokoll ermöglicht einen synchronen, voll-duplexen Datenaustausch zwischen zwei über B15 Boards verbundenen Computern. 

Dabei werden die 8-Bit-Register der Boards genutzt:
- 4 Bits zum Senden
- 4 Bits zum Empfangen
- Zusätzlich eine Clock-Leitung pro Richtung
- Effektiv können so 3 Bits parallel übertragen werden

## Paketstruktur
Die Daten werden in maximal 132 Byte große Pakete aufgeteilt:
- **2 Byte Header**
  - Enthält die Blocknummer
- **128 Byte Daten**
- **2 Byte 16-Bit-CRC** (Fehlerprüfung)

Falls das Paket kleiner ist und nicht restlos durch 3 teilbar, werden Padding-Bytes angehängt.

## Fehlerkontrolle
- Pakete werden so lange erneut gesendet, bis beide Seiten ein korrektes Empfangsbestätigungssignal erhalten.
- Eine vollständige Datenübertragung ist nur möglich, wenn die Datenblockanzahl und die Größe des letzten Blocks auf beiden Seiten übereinstimmen.

## Master-Slave Prinzip
Das Protokoll basiert auf einer **quasi Master-Slave Beziehung**:
- **Main Sender**: Initiator der Datenübertragung
- **Side Sender**: Wartet auf das Startsignal des Main Senders, bevor die Kommunikation beginnt

---

# HWPV7 Data Transfer Protocol

## Overview
HWPV7 is a specialized data transfer protocol for communication between two computers via B15 boards.

## Programming Language
C++

## Functionality
The protocol enables synchronous, full-duplex data exchange between two computers connected via B15 boards.

The 8-bit registers of the boards are used as follows:
- 4 bits for sending
- 4 bits for receiving
- One clock line per direction
- Effectively, 3 bits can be transmitted in parallel

## Packet Structure
Data is transmitted in packets of up to 132 bytes:
- **2-byte header**
  - Contains the block number
- **128-byte data section**
- **2-byte 16-bit CRC** (error checking)

If the packet is smaller and not divisible by 3, padding bytes are added.

## Error Control
- Packets are resent until both sides receive a correct acknowledgment signal.
- A complete data transfer is only possible if both sides have the same number of data blocks and the same size for the last block.

## Master-Slave Principle
The protocol is based on a **quasi master-slave relationship**:
- **Main Sender**: Initiates the data transfer
- **Side Sender**: Waits for the start signal from the Main Sender before starting communication

