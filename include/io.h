/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   header file IO functions
 */

#pragma once

#include <vector>
#include <cstdint>
#include <string>

#define IMAGE_T std::vector<uint8_t>
#define DEFAULT_SIZE 512*512

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define M_NONE       0
#define M_COMPRESS   1
#define M_DECOMPRESS 2

namespace io {
    typedef struct{
        int         mode;
        bool        rle;
        bool        adaptive;
        const char* input; 
        const char* output;
        int         width;
    } params_t;

    /**
     * Load binary file to vecor 
     * @param file name of file to load
     * @return poiter to vector
     */
    IMAGE_T* load(const char* file);
    
    /**
     * Convert byte vector to string of bits
     * @param content vector of bytes
     * @return std::string of bits
     */
    std::string toBin(IMAGE_T* content);

    /**
     * Print help to stdout
     * @param argv argv of program
     */
    void printHelp(char** argv);

    /**
     * Parse arguments from cli 
     * @param argc cout of arguments
     * @param argv array of arguments
     * @param params poiter to output params
     */
    void args(int argc, char** argv, params_t *params);

    /**
     * Save byte array to file
     * @param img byte array to save
     * @param file name of file
     */
    void save(IMAGE_T* img, const char* file);

    /**
     * Save bits stream to file
     * file is paded by 0 to bytes
     * @param binary string of bits to save
     * @param file name of file
     */
    void saveBin(std::string binary, const char* file);
}