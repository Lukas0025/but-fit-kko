/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file with transformations 
 */

#include "transform.h"
#include <cstdio>
#include <cmath> 

namespace transform {
    void use_model(scan::block_vector_t* blocks) {
        for (auto &block : *blocks) {
            auto data = block.data;

            for (unsigned i = data->size() - 1; i >= 1 ; i--) {
                data->at(i) = (uint8_t) (data->at(i) - data->at(i - 1));
            }
        }
    }

    void unuse_model(scan::block_vector_t* blocks) {
        for (auto &block : *blocks) {
            auto data = block.data;

            for (unsigned i = 1; i < data->size(); i++) {
                data->at(i) = (uint8_t) (data->at(i) + data->at(i - 1));
            }
        }
    }

    unsigned countBits(unsigned number) {     
        return (int)log2(number)+1; 
    } 

    void rle0(scan::block_vector_t* blocks) {
        for (auto &block : *blocks) {
            auto data = block.data;

            unsigned count = 0;

            block.rle = new std::vector<int>();
            block.rle->reserve(data->size());

            for (unsigned i = 0; i < data->size(); i++) {
                if (data->at(i) != 0) {

                    if (count > 0) {
                        count += 1;
                        auto neededLen = countBits(count);

                        for (unsigned j = 0; j < neededLen - 1; j++) {
                            block.rle->push_back(count & 0b1 ? RLE0_SYMB : 0);
                            count = count >> 1;
                        }
                    }

                    block.rle->push_back(data->at(i));

                    count = 0;
                } else {
                    count += 1;
                }
            }

            if (count > 0) {
                count += 1;
                auto neededLen = countBits(count);

                for (unsigned j = 0; j < neededLen - 1; j++) {
                    block.rle->push_back(count & 0b1 ? RLE0_SYMB : 0);
                    count = count >> 1;
                }
            }
        }
    }

    void ActiveUnRle0(IMAGE_T* img, int inchar) {

        static uint64_t level;

        if (img->size() == 0) level = 1;

        if (inchar == 0) {
            for (unsigned i = 0; i < level; i++) img->push_back(0);
            level = level << 1;
        } else if (inchar == RLE0_SYMB) {
            for (unsigned i = 0; i < (level << 1); i++) img->push_back(0);
            level = level << 1;
        } else {
            img->push_back(inchar);
            level = 1;
        }
    }
}