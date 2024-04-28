#pragma once
#include <vector>

class SbusParser
{
public:

    void getChannels(uint8_t *buffer, std::vector<uint16_t> &channels);

    uint16_t getChannel(uint8_t *buffer, int channelId);

    void setChannels(uint8_t *buffer, std::vector<uint16_t> &channels);

    void setChannel(uint8_t *buffer, uint16_t data, int channelId);

private:

    bool m_failsafe {false};
    bool m_lostFrame {false};
};