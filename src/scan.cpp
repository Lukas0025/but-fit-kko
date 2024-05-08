/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file with scanner 
 */

#include "io.h"
#include "scan.h"

namespace scan {
    block_vector_t* img_static(IMAGE_T* img) {
        auto vec = new block_vector_t(1);

        block_t* block = &(vec->at(0));
        
        block->mode     = SCAN_HORIZONTAL;
        block->compress = true;
        block->data     = img;
        block->rle      = NULL;
        
        return vec;
    }

    IMAGE_T* scanHorizontal(unsigned sx, unsigned sy, unsigned width, IMAGE_T* img) {
        auto out = new IMAGE_T();
        out->reserve(16*16);

        for (unsigned y = sy; y < sy + 16; y++) {
            for (unsigned x = sx; x < sx + 16; x++) {
                out->push_back(img->at(x + y * width));
            }
        }

        return out;
    }

    IMAGE_T* scanVertical(unsigned sx, unsigned sy, unsigned width, IMAGE_T* img) {
        auto out = new IMAGE_T();
        out->reserve(16*16);

        for (unsigned x = sx; x < sx + 16; x++) {
            for (unsigned y = sy; y < sy + 16; y++) {
                out->push_back(img->at(x + y * width));
            }
        }

        return out;
    }


    unsigned horizontalScore(unsigned sx, unsigned sy, unsigned width, IMAGE_T* img) {
        unsigned last  = 0;
        unsigned score = 0;
        for (unsigned y = sy; y < sy + 16; y++) {
            for (unsigned x = sx; x < sx + 16; x++) {
                score += std::abs((int)last - (int)(img->at(x + y * width)));
                last = img->at(x + y * width);
            }
        }

        return score;
    }

    unsigned verticalScore(unsigned sx, unsigned sy, unsigned width, IMAGE_T* img) {
        unsigned last  = 0;
        unsigned score = 0;
        for (unsigned x = sx; x < sx + 16; x++) {
            for (unsigned y = sy; y < sy + 16; y++) {
                score += std::abs((int)last - (int)(img->at(x + y * width)));
                last = img->at(x + y * width);
            }
        }

        return score;
    }

    block_vector_t* img_adaptive(IMAGE_T* img, int width) {
        int height = img->size() / width;

        auto vec = new block_vector_t();

        //16x16 blocks
        for (unsigned bx = 0; bx < width / 16; bx++) {
            for (unsigned by = 0; by < height / 16; by++) {
                //copute scores
                auto vs = verticalScore  (bx * 16, by * 16, width, img);
                auto hs = horizontalScore(bx * 16, by * 16, width, img);

                block_t block;

                block.compress = true;

                if (vs < hs) { // do vertical scan because is beater
                    block.mode = SCAN_VERTICAL;
                    block.data = scanVertical(bx * 16, by * 16, width, img);
                    block.rle  = NULL;
                } else {       // do horizontal scan
                    block.mode = SCAN_HORIZONTAL;
                    block.data = scanHorizontal(bx * 16, by * 16, width, img);
                    block.rle  = NULL;
                }

                vec->push_back(block);
            }
        }

        return vec;
    }

    IMAGE_T* unblock(block_vector_t* blocks, int blocks_per_line) {

        unsigned width = blocks_per_line * 16;

        if (blocks->size() == 1) return blocks->at(0).data;

        auto img = new IMAGE_T(16 * 16 * blocks->size());

        unsigned block_id = 0;
        for (auto &block : *blocks) {
            auto data = block.data;

            auto sy = (block_id / blocks_per_line) * 16;
            auto sx = (block_id % blocks_per_line) * 16;

            for (unsigned i = 0; i < data->size(); i++) {
                auto x = i % 16;
                auto y = i / 16;

                img->at((sy + y) + (sx + x) * width) = block.mode == SCAN_HORIZONTAL ? data->at(y + 16 * x) : data->at(x + 16 * y);
            }

            block_id++;
        }

        return img;
    }
}