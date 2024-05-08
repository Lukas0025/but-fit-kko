/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file with coder 
 */

#include "coder.h"
#include "transform.h"

#include <bitset>

#define CODER_IDETF    "10110101"
#define CODER_VERSION  "0"

namespace coder {
    std::string encode(scan::block_vector_t* blocks, unsigned width, unsigned size) {
        std::string binary = CODER_IDETF; //first 8 bits is idetifier next one bit is version
        binary            += CODER_VERSION;

        if (blocks->size() > 1) {
            binary += '1';
            binary += std::bitset<8>(width / 16).to_string(); //pocet bloku na jeden radek
            binary += std::bitset<8>((size / width) / 16).to_string(); //pocet bloku na jeden sloupec
        } else {
            binary += '0';
            binary += std::bitset<16>(size / 16).to_string(); // size
        }

        for (auto &block : *blocks) {

            //init huffman tree
            auto htree = huffman::tree();

            if (blocks->size() > 1) {
                binary += block.mode == SCAN_HORIZONTAL ? '1' : '0'; //scan type
            }

            //try compress using huffman
            std::string cbinary = "";
            if (block.rle != NULL) {
                auto data = block.rle;

                for (unsigned i = 0; i < data->size() ; i++) {
                    cbinary += htree.encode(data->at(i));
                }

            } else {
                auto data = block.data;
                for (unsigned i = 0; i < data->size() ; i++) {
                    cbinary += htree.encode(data->at(i));
                }
            }

            block.compress &= cbinary.length() < (block.data->size() * 8);

            binary += block.compress ? '1' : '0'; //compressed?

            if (block.compress) { // compressed is shorter
                binary += cbinary;
            } else { // usncompressed is shorter
                auto data = block.data;

                for (unsigned i = 0; i < data->size() ; i++) {
                    binary += std::bitset<8>(data->at(i)).to_string();
                }
            }
        }

        return binary;
    }

    scan::block_vector_t* decode(std::string binary, unsigned &blocks_per_line, bool useRle) {

        auto vec    = new scan::block_vector_t;

        //read first 8 bits bust be correct identifier
        #pragma unroll
        for (unsigned i = 0; i < 8; i++) {
            if (binary[i] != CODER_IDETF[i]) {
                fprintf(stderr, "ERROR: unsuported input file\n");
                exit(EXIT_FAILURE);
            }
        }

        if (binary[8] != CODER_VERSION[0]) {
            fprintf(stderr, "ERROR: unsuported version of coder used in compressed file\n");
            exit(EXIT_FAILURE);
        }

        auto is_adaptive_scan = binary[9] == '1';

        if (!is_adaptive_scan) {
            //init huffman tree
            auto htree = huffman::tree();

            unsigned toread = std::stoi(binary.substr(10, 16), NULL, 2) * 16;

            unsigned binPos = 26;

            scan::block_t block;

            block.mode     = SCAN_HORIZONTAL;
            block.compress = binary[binPos] == '1';
            block.data     = new IMAGE_T();
            block.rle      = NULL;

            binPos++;

            block.data->reserve(512*512);
            
            while(block.data->size() < toread) {
                if (!block.compress) { 
                    block.data->push_back(std::stoi(binary.substr(binPos, 8), NULL, 2));
                    binPos += 8;
                } else if (useRle) {
                    transform::ActiveUnRle0(block.data, htree.decode(binary, binPos));
                } else {
                    block.data->push_back(htree.decode(binary, binPos));
                }
            }

            vec->push_back(block);
        } else {
            //read length
            // 10th to 17th bit
            blocks_per_line = std::stoi(binary.substr(10, 8), NULL, 2);

            // 18th to 25th bit
            unsigned blocks_per_col = std::stoi(binary.substr(10, 8), NULL, 2);

            unsigned binPos = 26;

            for (unsigned blockId = 0; blockId < blocks_per_line * blocks_per_col; blockId++) {
                //init huffman tree
                auto htree  = huffman::tree();
                
                scan::block_t block;

                block.mode     = binary[binPos]     == '1' ? SCAN_HORIZONTAL : SCAN_VERTICAL;
                block.compress = binary[binPos + 1] == '1';
                block.data     = new IMAGE_T();
                block.rle      = NULL;

                block.data->reserve(16*16);

                binPos += 2; // skip block header
            
                while(block.data->size() < 16*16) {
                    if (!block.compress) {
                        block.data->push_back(std::stoi(binary.substr(binPos, 8), NULL, 2));
                        binPos += 8;
                    } else if (useRle) {
                        transform::ActiveUnRle0(block.data, htree.decode(binary, binPos));
                    } else {
                        block.data->push_back(htree.decode(binary, binPos));
                    }
                }

                vec->push_back(block);
            }
        }

        return vec;

    }
}