#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <cerrno>
#include <sys/select.h>

// Funktion zum Öffnen und Konfigurieren des seriellen Ports
int openSerialPort(const char* portName) {
    int serialPort = open("/dev/cu.usbserial-1130", O_RDWR | O_NOCTTY | O_NDELAY);  // O_NDELAY macht den Port nicht blockierend
    if (serialPort < 0) {
        std::cerr << "Fehler beim Öffnen des Ports: " << strerror(errno) << std::endl;
        return -1;
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Fehler beim Abrufen der Port-Konfiguration: " << strerror(errno) << std::endl;
        close(serialPort);
        return -1;
    }

    cfsetispeed(&tty, B9600);  // Baudrate für Eingabe
    cfsetospeed(&tty, B9600);  // Baudrate für Ausgabe
    tty.c_cflag &= ~PARENB;    // Keine Parität
    tty.c_cflag &= ~CSTOPB;    // 1 Stopbit
    tty.c_cflag &= ~CSIZE;     // Lösche Bitmasken
    tty.c_cflag |= CS8;        // 8 Datenbits
    tty.c_cflag &= ~CRTSCTS;   // Kein Hardware-Flow-Control
    tty.c_cflag |= CREAD | CLOCAL; // Aktiviert Lesen und ignoriert DCD (Carrier Detect)

    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        std::cerr << "Fehler beim Setzen der Port-Konfiguration: " << strerror(errno) << std::endl;
        close(serialPort);
        return -1;
    }

    return serialPort;
}

// Funktion zum Asynchronen Lesen des seriellen Ports ohne Threads (nur mit select)
void readSerialData(int serialPort) {
    fd_set readfds;  // Datei-Deskriptoren für Leseoperationen
    char buffer[256];

    while (true) {
        FD_ZERO(&readfds);          // Setzt die Datei-Deskriptoren zurück
        FD_SET(serialPort, &readfds);  // Fügt den seriellen Port hinzu

        // Timeout für select() - wartet maximal 1 Sekunde
        struct timeval timeout = { 1, 0 };  // 1 Sekunde

        int ret = select(serialPort + 1, &readfds, NULL, NULL, &timeout);
        
        if (ret > 0) {
            // Wenn Daten verfügbar sind, wird der Lesevorgang ausgeführt
            int bytesRead = read(serialPort, buffer, sizeof(buffer));
            if (bytesRead > 0) {
                // Gebe die rohen Daten aus (ohne Start- und Stopbits)
                for (int i = 0; i < bytesRead; i++) {
                    std::cout << buffer[i]; // Nur die eigentlichen Nutzdaten
                }
                std::cout << std::flush;  // Verhindert unnötige Zeilenumbrüche
            } else if (bytesRead < 0) {
                std::cerr << "Fehler beim Lesen der Daten: " << strerror(errno) << std::endl;
            }
        } else if (ret == 0) {
            // Timeout - keine Daten vorhanden, einfach weitermachen
            // Hier könnte man eine weitere Funktion ausführen oder eine Statusanzeige machen
            // Zum Beispiel: std::cout << "Keine neuen Daten...\n";
        } else {
            // Fehler bei select()
            std::cerr << "Fehler bei select(): " << strerror(errno) << std::endl;
        }
    }
}

int main() {
    const char* portName = "/dev/tty.usbserial-130";  // Anpassen auf deinen Portnamen
    int serialPort = openSerialPort(portName);
    if (serialPort < 0) {
        return -1;
    }

    // Starte das kontinuierliche Lesen der seriellen Daten
    readSerialData(serialPort);

    close(serialPort);  // Seriellen Port nach Beendigung des Programms schließen
    return 0;
}
