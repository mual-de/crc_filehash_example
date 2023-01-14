/**
 * @file main.cpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A small application to create crc checksums for every block of a file and store this information in an output file.
 * @version 0.1
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 *
 * Block size is defined by SEGMENT_SIZE parameter.
 */
#include <iostream>
#include <filesystem>
#include <thread>
#include <math.h>
#include <fstream>

#include "HashWorker.hpp"
#include "Helper.hpp"

// 1MB definition
#define SEGMENT_SIZE 1000000

using namespace std;
/**
 * @brief Write all checksums to output file.
 *
 * @param stream filestream to output file
 * @param values vector containing resulting crc checksums (ordered)
 */
void storeExport(ofstream &stream, shared_ptr<vector<unsigned int>> values)
{
    for (auto it = values->begin(); it != values->end(); ++it)
    {
        stream << *it << endl;
    }
}

/**
 * @brief Run process on input file and store calculated crc checksums in output file
 *
 * @param input path and filename of input file
 * @param output path and filename of output file
 */
void runSystem(string input, string output)
{
    if (!filesystem::exists(input))
    {
        throw invalid_argument("Input File doesn't exist!");
    }
    // Overwrite exisiting output file without asking.
    ofstream stream(output);

    unsigned int nthreads = thread::hardware_concurrency();
    unsigned int fsize = filesystem::file_size(input);

    if (fsize == 0)
    {
        throw invalid_argument("Input File is empty!");
    }

    unsigned int filledFSize = helper::filledFileSize(fsize, SEGMENT_SIZE);
    nthreads = helper::getNumberOfThreads(filledFSize, nthreads, SEGMENT_SIZE);

    vector<unique_ptr<HashWorker>> workers(nthreads);
    vector<unsigned int> startPos;

    helper::createStartPos(filledFSize, nthreads, SEGMENT_SIZE, startPos);

    for (int i = 0; i < nthreads; i++)
    {
        workers.at(i) = make_unique<HashWorker>(input, startPos[i], (i != nthreads - 1) ? startPos[i + 1] : filledFSize, SEGMENT_SIZE, i);
    }
    for (auto &worker : workers)
    {
        worker->startWorker();
    }
    for (auto &worker : workers)
    {
        worker->stopWorker();
        storeExport(stream, worker->getResults());
    }
    stream.close();
}

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        throw invalid_argument("Undefined number of parameters! Only input and output are expected!");
    }
    runSystem(string(argv[1]), string(argv[2]));
}