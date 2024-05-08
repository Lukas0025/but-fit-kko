/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file with IO operations
 */


#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <bitset>

extern char* optarg;

namespace io {
    void args(int argc, char** argv, params_t *params) {
        
        params->mode     = M_NONE;
        params->rle      = false;
        params->adaptive = false; 
        params->input    = NULL;
        params->output   = NULL;
        params->width    = 0;

        int          opt;

        while ((opt = getopt(argc, argv, "cdmai:o:w:h")) != -1) {
            switch (opt) {
                case 'h':
                    printHelp(argv);
                    exit(EXIT_SUCCESS);
                case 'c':
                    params->mode     = M_COMPRESS;
                    break;
                case 'd':
                    params->mode     = M_DECOMPRESS;
                    break;
                case 'm':
                    params->rle      = true;
                    break;
                case 'a':
                    params->adaptive = true;
                    break;
                case 'i':
                    params->input    = optarg;
                    break;
                case 'o':
                    params->output   = optarg;
                    break;
                case 'w':
                    params->width    = atoi(optarg);
                    
                    if (params->width <= 0) {
                        fprintf(stderr, "width must be >= 1\n");
                        exit(EXIT_FAILURE);
                    }

                    break;

                default:
                    fprintf(stderr, "Usage: %s [-c] [-d] [-m] [-a] -i file -o file [-w width] [-h]\n", argv[0]);
                    exit(EXIT_FAILURE);
            }
        }

        if (params->input == NULL || params->output == NULL) {
            fprintf(stderr, "Must set input/output files\n");
            exit(EXIT_FAILURE);
        }

        if (params->mode == M_COMPRESS && params->width == 0) {
            fprintf(stderr, "When compressing width must be set\n");
            exit(EXIT_FAILURE);
        }
    }

    void printHelp(char** argv) {
        printf("KKO project, simple image compress and decopress program.\n\n");
        printf("Usage: %s [-c] [-d] [-m] [-a] -i file -o file [-w width] [-h]\n\n", argv[0]);
        printf("params:\n");
        printf("  -c\tcompress mode\n");
        printf("  -d\tdecompress mode\n");
        printf("  -m\tactive model with RLE\n");
        printf("  -a\tactive adaptive scan image mode\n");
        printf("  -i <ifile>\tset input file\n");
        printf("  -o <ofile>\tset output file\n");
        printf("  -w <width_int>\tset width of input file\n");
        printf("  -h\tprint this and exit\n");
    }

    IMAGE_T* load(const char* file) {
        try {
            // now load image
            std::ifstream inputFile(file, std::ios_base::binary);

            // Determine the length of the file by seeking
            // to the end of the file, reading the value of the
            // position indicator, and then seeking back to the beginning.
            inputFile.seekg(0, std::ios_base::end);
            auto length = inputFile.tellg();
            inputFile.seekg(0, std::ios_base::beg);

            // Make a buffer of the exact size of the file and read the data into it.
            auto img = new IMAGE_T(length);

            inputFile.read(reinterpret_cast<char*>(img->data()), length);

            inputFile.close();

            return img;
        } catch (...) {
            fprintf(stderr, "Cant open input file\n");
            exit(EXIT_FAILURE);
        }

        return NULL;
    }

    std::string toBin(IMAGE_T* content) {
        std::string out;

        for (int byte = 0; byte < content->size(); byte++) {
            out += std::bitset<8>(content->at(byte)).to_string();
        }

        return out;
    }

    void save(IMAGE_T* img, const char* file) {
        try {
            std::ofstream outputFile(file, std::ios::binary);
            outputFile.write((char*)img->data(), img->size());
            outputFile.close();
        } catch (...) {
            fprintf(stderr, "Cant open output file\n");
            exit(EXIT_FAILURE);
        }
    }

    void saveBin(std::string binary, const char* file) {
        //pad binary with zeros to byte
        for (unsigned i = 0; i < (8 - binary.size() % 8); i++) {
            binary.push_back('0');
        }

        try {
            std::ofstream outputFile(file, std::ios::binary);
            
            unsigned pos = 0;

            while (pos < binary.size()) {
                uint8_t byte = 0;

                for (unsigned i = 0; i < 8; i++) {
                    byte  = byte << 1;
                    byte |= binary[pos] == '1' ? 0b1 : 0b0;
                    pos++;
                }

                outputFile.write((char*) &byte, 1);
            }
            
            outputFile.close();
        } catch (...) {
            fprintf(stderr, "Cant open output file\n");
            exit(EXIT_FAILURE);
        }
    }
}