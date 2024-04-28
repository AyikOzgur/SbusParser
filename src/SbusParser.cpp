#include "SbusParser.h"

#define HEADER 0x0F
#define FOOTER 0x00
#define FOOTER2 0x04
#define CH17_MASK 0x01
#define CH18_MASK 0x02
#define LOST_FRAME_MASK 0x04
#define FAILSAFE_MASK 0x08

void SbusParser::getChannels(uint8_t *buffer, std::vector<uint16_t> &channels)
{
    // Ensure the vector has enough space for 19 channels (0-18 index)
    if (channels.size() < 18) 
    {
        channels.resize(18);
    }

    channels[0]  = static_cast<int16_t>(buffer[1] | ((buffer[2] << 8) & 0x07FF));
    channels[1]  = static_cast<int16_t>((buffer[2] >> 3) | ((buffer[3] << 5) & 0x07FF));
    channels[2]  = static_cast<int16_t>((buffer[3] >> 6) | (buffer[4] << 2) | ((buffer[5] << 10) & 0x07FF));
    channels[3]  = static_cast<int16_t>((buffer[5] >> 1) | ((buffer[6] << 7) & 0x07FF));
    channels[4]  = static_cast<int16_t>((buffer[6] >> 4) | ((buffer[7] << 4) & 0x07FF));
    channels[5]  = static_cast<int16_t>((buffer[7] >> 7) | (buffer[8] << 1) | ((buffer[9] << 9) & 0x07FF));
    channels[6]  = static_cast<int16_t>((buffer[9] >> 2) | ((buffer[10] << 6) & 0x07FF));
    channels[7]  = static_cast<int16_t>((buffer[10] >> 5) | ((buffer[11] << 3) & 0x07FF));
    channels[8]  = static_cast<int16_t>(buffer[12] | ((buffer[13] << 8) & 0x07FF));
    channels[9]  = static_cast<int16_t>((buffer[13] >> 3) | ((buffer[14] << 5) & 0x07FF));
    channels[10] = static_cast<int16_t>((buffer[14] >> 6) | (buffer[15] << 2) | ((buffer[16] << 10) & 0x07FF));
    channels[11] = static_cast<int16_t>((buffer[16] >> 1) | ((buffer[17] << 7) & 0x07FF));
    channels[12] = static_cast<int16_t>((buffer[17] >> 4) | ((buffer[18] << 4) & 0x07FF));
    channels[13] = static_cast<int16_t>((buffer[18] >> 7) | (buffer[19] << 1) | ((buffer[20] << 9) & 0x07FF));
    channels[14] = static_cast<int16_t>((buffer[20] >> 2) | ((buffer[21] << 6) & 0x07FF));
    channels[15] = static_cast<int16_t>((buffer[21] >> 5) | ((buffer[22] << 3) & 0x07FF));

    // Chanelle 17 and 18 are bool. 0 or 1.
    channels[16] = (buffer[23] & CH17_MASK) ? 1 : 0;
    channels[17] = (buffer[23] & CH18_MASK) ? 1 : 0;

    m_lostFrame = buffer[23] & LOST_FRAME_MASK;
    m_failsafe = buffer[23] & FAILSAFE_MASK;
}

uint16_t SbusParser::getChannel(uint8_t *buffer, int channelId)
{
    // Read all channels
    std::vector<uint16_t> channels(18);
    getChannels(buffer, channels);

    return channels.at(channelId);
}

void SbusParser::setChannels(uint8_t *buffer, std::vector<uint16_t> &channells)
{
    buffer[0] = HEADER;
    buffer[1] = static_cast<uint8_t>((channells[0] & 0x07FF));
    buffer[2] = static_cast<uint8_t>((channells[0] & 0x07FF) >> 8 | (channells[1] & 0x07FF) << 3);
    buffer[3] = static_cast<uint8_t>((channells[1] & 0x07FF) >> 5 | (channells[2] & 0x07FF) << 6);
    buffer[4] = static_cast<uint8_t>((channells[2] & 0x07FF) >> 2);
    buffer[5] = static_cast<uint8_t>((channells[2] & 0x07FF) >> 10 | (channells[3] & 0x07FF) << 1);
    buffer[6] = static_cast<uint8_t>((channells[3] & 0x07FF) >> 7 | (channells[4] & 0x07FF) << 4);
    buffer[7] = static_cast<uint8_t>((channells[4] & 0x07FF) >> 4 | (channells[5] & 0x07FF) << 7);
    buffer[8] = static_cast<uint8_t>((channells[5] & 0x07FF) >> 1);
    buffer[9] = static_cast<uint8_t>((channells[5] & 0x07FF) >> 9 |(channells[6] & 0x07FF) << 2);
    buffer[10] = static_cast<uint8_t>((channells[6] & 0x07FF) >> 6 | (channells[7] & 0x07FF) << 5);
    buffer[11] = static_cast<uint8_t>((channells[7] & 0x07FF) >> 3);
    buffer[12] = static_cast<uint8_t>((channells[8] & 0x07FF));
    buffer[13] = static_cast<uint8_t>((channells[8] & 0x07FF) >> 8 | (channells[9]  & 0x07FF) << 3);
    buffer[14] = static_cast<uint8_t>((channells[9] & 0x07FF) >> 5 | (channells[10] & 0x07FF) << 6);
    buffer[15] = static_cast<uint8_t>((channells[10] & 0x07FF) >> 2);
    buffer[16] = static_cast<uint8_t>((channells[10] & 0x07FF) >> 10 | (channells[11] & 0x07FF) << 1);
    buffer[17] = static_cast<uint8_t>((channells[11] & 0x07FF) >> 7 | (channells[12] & 0x07FF) << 4);
    buffer[18] = static_cast<uint8_t>((channells[12] & 0x07FF) >> 4 | (channells[13] & 0x07FF) << 7);
    buffer[19] = static_cast<uint8_t>((channells[13] & 0x07FF) >> 1);
    buffer[20] = static_cast<uint8_t>((channells[13] & 0x07FF) >> 9  | (channells[14] & 0x07FF) << 2);
    buffer[21] = static_cast<uint8_t>((channells[14] & 0x07FF) >> 6  | (channells[15] & 0x07FF) << 5);
    buffer[22] = static_cast<uint8_t>((channells[15] & 0x07FF) >> 3);
    buffer[23] = 0x00 | (channells[16] * CH17_MASK) | (channells[17] * CH18_MASK) | 
                        (m_failsafe * FAILSAFE_MASK) |
                        (m_lostFrame * LOST_FRAME_MASK);
    buffer[24] = FOOTER;
}

void SbusParser::setChannel(uint8_t *buffer, uint16_t data, int channelId)
{
    // Get all current channels;
    std::vector<uint16_t> currentChannels(18);
    getChannels(buffer, currentChannels);

    // Change related channel.
    currentChannels.at(channelId) = data;

    // Write channels to buffer back.
    setChannels(buffer, currentChannels);
}