
#include "huffman.h"


void huffmanEncode(char* inpFile, char* huffFile)
{
	huffman h(inpFile, huffFile);
	h.create_pq();
	h.create_huffman_tree();
	h.calculate_huffman_codes();
	h.coding_save();
}