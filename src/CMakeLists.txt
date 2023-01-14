project(CRC_FileHashApp)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost 1.6.0 REQUIRED COMPONENTS filesystem)
find_package(Threads REQUIRED)

include_directories( ${Boost_INCLUDE_DIR} )
add_executable(crc_fileHash main.cpp HashWorker.cpp Helper.cpp)
target_link_libraries(crc_fileHash PRIVATE Boost::filesystem Threads::Threads)