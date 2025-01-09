#include <iostream>
#include <chrono>
#include <thread>

void correctedDelay(int delay_ms) {
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < delay_ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Kleine Schritte
        end = std::chrono::steady_clock::now();
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Verstrichene Zeit: " << elapsed.count() << " ms\n";
}

int main() {
    correctedDelay(10); // Soll 10 ms dauern
    return 0;
}