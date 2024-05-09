huff_codec:
	g++ src/* -I include -o huff_codec -O3

debug:
	g++ -g src/* -I include -o huff_codec

clean:
	rm -rf huff_codec

test: huff_codec
	for file in data/* ; do \
        echo "Testing" $${file} && \
		echo "" && \
		./huff_codec -c -i $${file} -o compresed.out -w 512 && \
		./huff_codec -d -i compresed.out -o decompress.raw && \
		cmp -l decompress.raw $${file} || exit 1 && \
		echo "-m" && \
		./huff_codec -c -i $${file} -o compresed.out -w 512 -m && \
		./huff_codec -d -i compresed.out -o decompress.raw -m && \
		cmp -l decompress.raw $${file} || exit 1 && \
		echo "-a" && \
		./huff_codec -c -i $${file} -o compresed.out -w 512 -a && \
		./huff_codec -d -i compresed.out -o decompress.raw -a && \
		cmp -l decompress.raw $${file} || exit 1 && \
		echo "-m -a" && \
		./huff_codec -c -i $${file} -o compresed.out -w 512 -m -a && \
		./huff_codec -d -i compresed.out -o decompress.raw -m -a && \
		cmp -l decompress.raw $${file} || exit 1; \
    done
	rm -rf compresed.out
	rm -rf decompress.raw
