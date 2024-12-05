#pragma once

enum class ControlCharacter : unsigned char {
    START = 0x02,    
    END = 0x03,     
    ACK = 0x06,      
    NAK = 0x15,      
    ESC = 0x1B       
};
