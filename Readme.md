# CRC File Hasher

A small application to create a crc checksum for every block of a large file. Using multithreading to optimize the runtime.

Needs boost minimum 1.6.0 installed.

Tested under Ubuntu 20.04 and 22.04: ![Build and Test](https://github.com/mual-de/crc_filehash_example/actions/workflows/build_cpp.yml/badge.svg)

Run with `crc_fileHash <inputfile> <outputfile>`