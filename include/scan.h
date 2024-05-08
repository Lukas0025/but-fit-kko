/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   header file with scanner
 */

#pragma once

#include <vector>
#include <cstdint>
#include "io.h"

#define SCAN_HORIZONTAL 0
#define SCAN_VERTICAL   1
#define MAX_BLOCK_SIZE  512

namespace scan {
    typedef struct{
        int                     mode;
        int                     compress;
        IMAGE_T*                data;
        std::vector<int>*       rle;
    } block_t;

    typedef std::vector<block_t> block_vector_t;

    /**
     * Do static scan on image
     * @param img image to scan
     * @return vector of blocks
     */
    block_vector_t* img_static(IMAGE_T* img);

    /**
     * Do adaptive scan on image using 16x16 blocks
     * @param img image to scan
     * @return vector of blocks
     */
    block_vector_t* img_adaptive(IMAGE_T* img, int width);

    /**
     * Concatenate blocks back to one image
     * @param blocks array of blocks
     * @param blocks_per_line number of blocks per one line
     * @return vector of bytes
     */
    IMAGE_T* unblock(block_vector_t* blocks, int blocks_per_line);
}