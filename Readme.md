# Simple image compression
#### Project to BUT FIT KKO

Compression algoritm using adaptive scan, pixel diference model, RLE-0 and adaptive huffman coder.

### Building

Simple run make.

### Using

```
KKO project, simple image compress and decopress program.

Usage: ./huff_codec [-c] [-d] [-m] [-a] -i file -o file [-w width] [-h]

params:
  -c    compress mode
  -d    decompress mode
  -m    active model with RLE
  -a    active adaptive scan image mode
  -i <ifile>    set input file
  -o <ofile>    set output file
  -w <width_int>        set width of input file
  -h    print this and exit
```