/**
 * KKO project - image compression program using adaptive huffman coding
 * @author Lukas Plevac <xpleva07>
 * @date   8.5.2024
 * @file   implementation file with huffman coder 
 */


#include "huffman.h"
#include "transform.h"
#include <bitset>

#define POP_FRONT(vec) {vec.front() = vec.back(); vec.pop_back();}

namespace huffman {
	inline node_t* nodeFactory(int value, int weight, int order, node_t* left, node_t* right, node_t* parent, bool isNYT) {
		auto node = new node_t();

		node->value  = value;
		node->weight = weight;
		node->order  = order;
		node->left   = left;
		node->right  = right;
		node->parent = parent;
		node->isNYT  = isNYT;

		return node;
	}

	tree::tree() {
		this->root = nodeFactory(-1, 0, 512, NULL, NULL, NULL, true);

		this->NYTNode = this->root;
	}

	std::string tree::encode(int value) {
		auto valNode = this->get(value, this->root);

		if (valNode != NULL) {
			std::string result = this->pathTo(this->root, valNode, "");
			this->update(value);
			return result;
		}

		std::string out = this->pathTo(this->root, this->NYTNode, "");
		out            += std::bitset<9>(value).to_string();

		this->update(value);

		return out;
	}

	int tree::decode(std::string &binary, unsigned &pos) {
		int     result = -1;
		node_t *node   = this->root;

		if (binary.length() == 0) return -1;

		while (result == -1) {
			if (node->value != -1) {
				result = node->value;
				this->update(node->value);
			} else if (node->isNYT) {

				std::string numberArr = "";

				#pragma unroll
				for (unsigned i = 0; i < 9; i++) {
					numberArr += binary[pos];
					pos++;
				}

				int number = std::stoi(numberArr, NULL, 2);
				result = number;
				this->update(number);

			} else if(binary[pos] == '0') {
				node = node->left;
				pos++;
			} else {
				node = node->right;
				pos++;
			}
		}

		return result;
	}

	std::string tree::pathTo(node_t *node, node_t *target, std::string currentPath) {
		if (node == target)  return currentPath;
		if (node == NULL)    return "";
		
		std::string left =         this->pathTo(node->left,  target, currentPath + "0");

		return left != "" ? left : this->pathTo(node->right, target, currentPath + "1");
	}

	void tree::update(int symbol) {
		node_t *leafToIncrement = NULL;
		node_t *symbolNode      = this->get(symbol, this->root);
		
		if (symbolNode == NULL) {
			int currentMinOrder = NYTNode->order;

			NYTNode->isNYT      = false;
			NYTNode->left       = nodeFactory(-1,     0, currentMinOrder - 2, NULL, NULL, NYTNode, true);
			NYTNode->right      = nodeFactory(symbol, 0, currentMinOrder - 1, NULL, NULL, NYTNode, false);
			
			symbolNode          = NYTNode->right;
			NYTNode             = NYTNode->left;
		}
			
		this->SlideAndIncrement(symbolNode);	
	}

	node_t* tree::get(int symbol, node_t *node) {

		if (node == NULL || node->value == symbol)  {
			return node;
		}

		auto *leftResult = this->get(symbol, node->left);
		
		return leftResult == NULL ? this->get(symbol, node->right) : leftResult;
	}

	void tree::SlideAndIncrement(node_t *node) {
		if (node == NULL) return;

		node_t *blockLeader = node;
		this->leader(this->root, blockLeader);

		if (blockLeader != node) this->swap(blockLeader, node);

		node->weight++;
		this->SlideAndIncrement(node->parent);
	}

	void tree::leader(node_t *node, node_t *&nodeMax) {
		if (node == nullptr || nodeMax == root) return;
		
		if (node->weight == nodeMax->weight && node != nodeMax->parent && node->order > nodeMax->order) {
			nodeMax = node;
		}
		
		this->leader(node->left,  nodeMax);
		this->leader(node->right, nodeMax);
	}
	
	void tree::swap(node_t *first, node_t *second) {
		if (first->parent == NULL   || second->parent == NULL)  return;
		if (first->parent == second || second->parent == first) return;
		
		auto *&firstRef  = first->parent->left  == first  ? first->parent->left  : first->parent->right;
		auto *&secondRef = second->parent->left == second ? second->parent->left : second->parent->right;
		
		std::swap(firstRef, secondRef);
		std::swap(firstRef->parent, secondRef->parent);
		std::swap(firstRef->order,  secondRef->order);
	}

}