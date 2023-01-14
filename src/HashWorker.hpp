/**
 * @file HashWorker.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A worker thread to calculates crc checksums on a bunch of blocks in an input file.
 * @version 0.1
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __HASH_WORKER_HPP__
#define __HASH_WORKER_HPP__
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <vector>
#include <thread>
namespace fs = boost::filesystem;

class HashWorker
{
public:
    /**
     * @brief Construct a new Hash Worker object
     *
     * @param path2file path to input file (opened read only)
     * @param startPos start position in file to work on
     * @param endPos  stop position in file to work on
     * @param segmentSize size of every block to calculate crc
     */
    HashWorker(std::string path2file, unsigned int startPos, unsigned int endPos, unsigned int segmentSize, int threadID);
    ~HashWorker(){};
    /**
     * @brief Get a shared pointer to the vector containing the checksums
     *
     * @return std::shared_ptr<std::vector<unsigned int>>
     */
    inline std::shared_ptr<std::vector<unsigned int>> getResults() { return this->m_results; };
    /**
     * @brief Starts worker thread.
     *
     */
    inline void startWorker() { this->m_workerThread = std::thread(&HashWorker::runWorker, this); };
    /**
     * @brief Wait until worker thread is joined
     *
     */
    inline void stopWorker() { this->m_workerThread.join(); };
    /**
     * @brief Check if internal thread is joinable
     *
     * @return true
     * @return false
     */
    inline bool isJoinable() { return this->m_workerThread.joinable(); };

private:
    /**
     * @brief Unique ptr to a predefined array of segment size to read file bufferwise.
     * 
     */
    std::unique_ptr<char[]> m_segmentStorage;
    /**
     * @brief input filestream to m_fileStream
     * 
     */
    std::unique_ptr<fs::ifstream> m_fileStream;
    /**
     * @brief vector of calculated results.
     * 
     */
    std::shared_ptr<std::vector<unsigned int>> m_results;
    const int m_threadId;
    const unsigned int m_segmentSize;
    const unsigned int m_startPos;
    const unsigned int m_stopPos;
    unsigned int m_steps;
    unsigned long int m_fileLength;
    /**
     * @brief Internal thread
     * 
     */
    std::thread m_workerThread;
    /**
     * @brief Read a new segment at the given position
     * 
     * @param position 
     */
    void readFileSegment(int position);
    /**
     * @brief runs the calculation thread until all blocks have been processed
     * 
     */
    void runWorker();
};

#endif
