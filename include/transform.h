/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   header file with transformations 
 */

#pragma once

#include "scan.h"
#define RLE0_SYMB 256

namespace transform {
    /**
     * Use simple model X1 - X0
     * @param blocks to applay model
     */
    void use_model(scan::block_vector_t* blocks);

    /**
     * Reverse Use simple model X1 - X0
     * @param blocks to applay model
     */
    void unuse_model(scan::block_vector_t* blocks);

    /**
     * Use RLE-0 on blocks
     * @param blocks to applay
     */
    void rle0(scan::block_vector_t* blocks);

    /**
     * Decode RLE-0 and add output tobyte array
     * @param img byte array to add decoded symbols
     * @param inchar encoded char to decode
     * @post static varaible is updated to store information of current stream
     * @pre if decoding new block img must bee empty to triger stratic variable reset
     */
    void ActiveUnRle0(IMAGE_T* img, int inchar);
}