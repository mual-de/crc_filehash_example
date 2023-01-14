#include "HashWorker.hpp"
#include <fstream>
#include <boost/crc.hpp>
#include <cmath>
#include <iostream>

using namespace boost::filesystem;
using namespace std;

HashWorker::HashWorker(string path2file, unsigned int startPos, unsigned int stopPos, unsigned int segmentSize, int threadID) : m_segmentSize(segmentSize), m_startPos(startPos), m_stopPos(stopPos), m_threadId(threadID)
{
    if (startPos > stopPos)
    {
        throw "endPos have to be bigger then startPos!";
    }
    this->m_segmentStorage = make_unique<char[]>(segmentSize);
    this->m_fileStream = make_unique<fs::ifstream>(path2file, fs::ifstream::in);
    this->m_fileLength = file_size(path2file);
    this->m_steps = static_cast<int>(ceil((stopPos - startPos) / segmentSize));
    this->m_results = make_shared<vector<unsigned int>>(m_steps);
}

void HashWorker::readFileSegment(int position)
{
    this->m_fileStream->seekg(position, ios::beg);
    if (position + this->m_segmentSize - 1 < this->m_fileLength)
    {
        this->m_fileStream->read(this->m_segmentStorage.get(), m_segmentSize);
    }
    else
    {
        this->m_fileStream->read(this->m_segmentStorage.get(), this->m_fileLength - position - 1);
        int openSize = (position + m_segmentSize) - m_fileLength;
        for (int i = openSize; i < m_segmentSize; i++)
        {
            this->m_segmentStorage[i] = 0x00;
        }
    }
}

void HashWorker::runWorker()
{
    boost::crc_32_type result;
    int position = this->m_startPos;
    for (int i = 0; i < this->m_steps; i++)
    {
        this->readFileSegment(position);
        position += this->m_segmentSize;
        result.process_bytes(this->m_segmentStorage.get(), this->m_segmentSize);
        this->m_results->at(i) = result.checksum();
    }

}