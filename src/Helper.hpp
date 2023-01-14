/**
 * @file Helper.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief small functions to support the "runSystem" function in main.cpp
 * @version 0.1
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __HELPER_HPP__
#define __HELPER_HPP__
#include <iostream>
#include <filesystem>
#include "HashWorker.hpp"
#include <thread>
#include <math.h>
#include <fstream>

namespace helper
{
    /**
     * @brief Get the file size rounded up to be dividable by segmentSize
     *
     * @param fsize overall file size in bytes
     * @param segmentSize size of a segment in bytes
     * @return unsigned int fsize rounded up to be dividable by segmentSize
     */
    inline unsigned int filledFileSize(const unsigned int fsize, const unsigned int segmentSize)
    {
        int mod = fsize % segmentSize;
        return fsize + segmentSize - mod;
    };
    /**
     * @brief Create a vector of file positions to provide this information to the independent threads
     *
     * @param fsize overall filesize rounded up to segmentSize by @see filledFileSize
     * @param nthread number of threads running in parallel @see getNumberOfThreads
     * @param segmentSize size of every segment
     * @param startPos vector containing start position in input file for every thread
     */
    void createStartPos(const unsigned int fsize, const unsigned int nthread, const unsigned int segmentSize, std::vector<unsigned int> &startPos);
    /**
     * @brief Get the Number of Threads optimized for running the checksum operations
     *
     * @param fsize overall filesize rounded up to segmentSize by @see filledFileSize
     * @param nThreadsAvailable number of available threads/processors
     * @param segmentSize size of every segment
     * @return unsigned int number of parallel threads to be generated
     */
    inline unsigned int getNumberOfThreads(const unsigned int fsize, const unsigned int nThreadsAvailable, const unsigned int segmentSize)
    {
        if (static_cast<int>(fsize / segmentSize) < nThreadsAvailable)
        {
            return static_cast<int>(fsize / segmentSize);
        }
        // One Core reservated for os, gives a little speed up
        return nThreadsAvailable - 1;
    };
};

#endif