#pragma once
#include "Sender.hpp"

class SideSender: public Sender{

public:
    SideSender(CRC crc);
    bool waitForMainSender();
};