#pragma once
#include "Sender.hpp"

class MainSender : public Sender{

public:
    MainSender(CRC);
    void sendStartOfTransmitting();
};