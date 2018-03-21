// This is the Huffman.h File:

#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <string>

class Tree
{
private:
	struct Node
	{
		struct compare
		{
			const bool operator()(Node* A, Node* B)
			{
				return A->f > B->f;
			}
		};

		const float f;
		const Node* l;
		const Node* r;
		const char  c;

		Node(const float F, const Node* const L, const Node* const R, const char C = '\0') : f(F), l(L), r(R), c(C) {}
		~Node() { delete l; delete r; }

		const std::string search(const unsigned char) const;
	};

	const Node* root;

	static const Node* const Huffman(const float* const);
public:
	Tree(const float* const);
	~Tree();

	const std::string search(const unsigned char) const;
};

#endif