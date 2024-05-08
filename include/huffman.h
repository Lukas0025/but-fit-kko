/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   header file with humman encoder
 */

#pragma once
#include <string>

namespace huffman {
    
    typedef struct HuffmanNode {
        int value;      //real value of node
		int weight;     //how many times in file
		int order;

		struct HuffmanNode* left;
		struct HuffmanNode* right;
		struct HuffmanNode* parent;

		bool isNYT;
    } node_t;

    class tree {
        public:
        	tree();

            /**
             * Get encoding string of bits for value
             * @post tree is updated
             * @param value value to encode
             * @return string of bits encoding value
             */
	        std::string encode(int value);

            /**
             * Decode string of bits to value
             * @post tree is updated
             * @post pos is shifted no next not readed bit
             * @param binary string of bits encoding value
             * @param pos    position in string to decode
             * @return decoded value
             */
            int decode(std::string &binary, unsigned &pos);

            /**
             * Get path throw tree to traget
             * @param node node from we what path
             * @param traget node to we traveling
             * @param currentPath prefix of path (prefix + path)
             * @return path to node in string with 0 and 1 as LEFT/RIGHT
             */
	        std::string pathTo(node_t *node, node_t *target, std::string currentPath);

            /**
             * Update tree when symbol is used
             * @param symbol used symbol
             * @post tree was updated
             */
	        void update(int symbol);

            /**
             * Get NODE what reprezenting symbol
             * @param symbol symbol what we traing to find
             * @param node   root of (sub)tree where we finding symbol  
             * @return poiter to node
             */
            node_t* get(int symbol, node_t *node);

            /**
             * Increment weight and swap upper if can
             * @param node node to increment and go upper if can
             * @post tree was updated
             */
            void SlideAndIncrement(node_t *node);

            /**
             * Find node with biggext weight and order in subtree
             * @param node root of sub tree
             * @param nodeMax finded leader [OUT]
             */
            void leader(node_t *node, node_t *&nodeMax);

            /**
             * Swap two nodes between eatch other
             * @param first   fisrt node to swap
             * @param second  second node to swap
             */
            void swap(node_t *first, node_t *second);
        private:
            node_t *NYTNode;
            node_t *root;
    };
}