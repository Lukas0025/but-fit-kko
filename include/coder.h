/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   header file with coder 
 */
#pragma once
#include "huffman.h"
#include "scan.h"

namespace coder {
    /**
     * Encode blocks using adaptive huffman tree
     * @param blocks blocks to encode to binary string
     * @param width  width of encoding image
     * @param size   number of pixels in image
     * @return std::string of bits
     */
    std::string encode(scan::block_vector_t* blocks, unsigned width, unsigned size);

    /**
     * Decode bits string to blocks
     * @param binary          string of bits to decode
     * @param blocks_per_line number of blocks per line readed from file if adaptive scan [OUT]
     * @param useRle          use RLE to decode stream
     * @return scan::block_vector_t*
     */
    scan::block_vector_t* decode(std::string binary, unsigned &blocks_per_line, bool useRle);
}