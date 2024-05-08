/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file of main "TOP"
 */


#include "io.h"
#include "scan.h"
#include "transform.h"
#include "huffman.h"
#include "coder.h"

int main(int argc, char **argv) {
    io::params_t params;
    io::args(argc, argv, &params);

    auto input  = io::load(params.input);

    if (params.mode == M_COMPRESS) {
        scan::block_vector_t* blocks;

        if (params.adaptive) {
            blocks = scan::img_adaptive(input, params.width);
        } else {
            blocks = scan::img_static(input);
        }

        if (params.rle) {
            transform::use_model(blocks);
            transform::rle0(blocks);
        }

        //binary string init
        auto binary = coder::encode(blocks, params.width, input->size());

        //save binary to file
        io::saveBin(binary, params.output);

    } else if (params.mode == M_DECOMPRESS) {
        unsigned blocks_per_line = 0;

        //load binary
        auto bin = io::toBin(input);

        //now HF to blocks
        auto blocks = coder::decode(bin, blocks_per_line, params.rle);

        //decompress blocks
        if (params.rle) transform::unuse_model(blocks);

        //concat blocks
        auto img = scan::unblock(blocks, blocks_per_line);

        //save
        io::save(img, params.output);
    }

    delete input;

    return 0;
}