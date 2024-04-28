#include <iostream>
#include <cstdlib> // For rand() and srand()
#include "SbusParser.h"

int main ()
{
    SbusParser sbusParser;

    std::vector<uint16_t> writtingChannels(18);
    for (auto &channel : writtingChannels)
    {
        channel = rand() % 2048;
    }

    // Last two channels are booleans.
    writtingChannels.at(16) = rand() & 1;
    writtingChannels.at(17) = rand() & 1;

    // Set channels to a buffer.
    uint8_t buffer[25];
    sbusParser.setChannels(buffer, writtingChannels);

    // Get channels back to anothe vector to compare.
    std::vector<uint16_t> readingChannels;
    sbusParser.getChannels(buffer, readingChannels);

    // Compare them.
    bool isPassed {true};
    for (int i = 0; i < writtingChannels.size(); ++i)
    {
        if (writtingChannels.at(i) != readingChannels.at(i))
        {
            std::cout << "Channel " << i << " is different." << std::endl;
            std::cout << "Writting : " << writtingChannels[i] << "   |  Reading : " << readingChannels[i] << std::endl;
            isPassed = false;
        }
    }

    if(isPassed)
    {
        std::cout << "Writting and reading channels OK." << std::endl;
    }

    return 0;
}