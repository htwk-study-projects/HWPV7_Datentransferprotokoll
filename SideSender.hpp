#pragma once
#include "Sender.hpp"

class SideSender: public Sender{
public:
    SideSender(CRC crc);
    bool waitForMainSender();
    void sendAKN() override;
    void sendNAKN() override;
    bool checkAKN() override;
    void writeToB15(int) override;
    void sendEndOfTransmitting() override;
};