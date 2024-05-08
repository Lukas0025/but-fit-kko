echo "file;huffman;huffman+rle;adaptive+huffman;ataptive+huffman+rle"

for file in data/* ; do \
	./huff_codec -c -i ${file} -o compresed1.out -w 512 && \
	./huff_codec -c -i ${file} -o compresed2.out -w 512 -m && \
	./huff_codec -c -i ${file} -o compresed3.out -w 512 -a && \
	./huff_codec -c -i ${file} -o compresed4.out -w 512 -m -a && \
	echo ${file}";"$(($(stat -c%s "${file}") * 100 / $(stat -c%s compresed1.out)))";"$(($(stat -c%s "${file}")  * 100  / $(stat -c%s compresed2.out)))";"$(($(stat -c%s "${file}") * 100  / $(stat -c%s compresed3.out)))";"$(($(stat -c%s "${file}") * 100  / $(stat -c%s compresed4.out))); \
done

rm -rf compresed1.out
rm -rf compresed2.out
rm -rf compresed3.out
rm -rf compresed4.out