#pragma once
#include "Sender.hpp"

class MainSender : public Sender{
public:
    MainSender(CRC);
    void sendStartOfTransmitting();
    void writeToB15(int) override;
    void sendAKN() override;
    void sendNAKN() override;
    bool checkAKN() override;
    void sendEndOfTransmitting() override;
    void sendNoEndOfTransmitting() override;
};