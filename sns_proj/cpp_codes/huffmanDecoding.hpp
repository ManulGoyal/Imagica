
#include "huffman.h"


void huffmanDecode(char* huffFile, char* outFile)
{
	huffman h(huffFile, outFile);
	h.recreate_huffman_tree();
	h.decoding_save();
}
