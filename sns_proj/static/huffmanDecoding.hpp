
#include "huffman.h"


void huffmanDecode(string huffFile, string outFile)
{
	huffman h(huffFile, outFile);
	h.recreate_huffman_tree();
	h.decoding_save();
}
