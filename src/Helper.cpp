#include "Helper.hpp"

void helper::createStartPos(const unsigned int fsize, const unsigned int nthread, const unsigned int segmentSize, std::vector<unsigned int> &startPos)
{
    int mod = (fsize / segmentSize) % nthread;
    int blocks = static_cast<int>(std::round((fsize / segmentSize) / nthread));
    startPos.push_back(0);
    for (int i = 1; i < nthread; i++)
    {
        if (mod == 0)
        {

            startPos.push_back(startPos[i - 1] + blocks * segmentSize);
        }
        else
        {
            startPos.push_back(startPos[i - 1] + (blocks + 1) * segmentSize);
            mod--;
        }
    }
}
