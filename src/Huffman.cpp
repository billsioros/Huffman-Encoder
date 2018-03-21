// This is the Huffman.cpp File:

#include "Huffman.h"
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

const std::string Tree::Node::search(const unsigned char ascii) const
{
	static string path;

	if (this->c == ascii) {
		const string mask = path;
		path.clear();
		return mask;
	}

	if (l != NULL) {
		path += '0';
		const string rv = l->search(ascii);
		if (!rv.empty())
			return rv;
		path = path.substr(0, path.size() - 1);
	}

	if (r != NULL) {
		path += '1';
		const string rv = r->search(ascii);
		if (!rv.empty())
			return rv;
		path = path.substr(0, path.size() - 1);
	}

	return "";
}

const Tree::Node* Tree::Huffman(const float* const f)
{
	priority_queue<Node*, vector<Node*>, Node::compare> Q;
	unsigned int n = 0;
	for (unsigned int i = 0; i < 256; i++) {
		if (f[i] > 0.0f) {
			Q.push(new Node(f[i], NULL, NULL, (char) i));
			n++;
		}
	}

	for (unsigned int i = 0; i < n - 1; i++) {
		const Node * const l = Q.top(); Q.pop();
		const Node * const r = Q.top(); Q.pop();
		const float f = l->f + r->f;

		Q.push(new Node(f, l, r));
	}

	const Node* const root = Q.top(); Q.pop();
	return root;
}

Tree::Tree(const float* const f) : root(NULL)
{
	root = Huffman(f);
}

Tree::~Tree()
{
	delete root;
}

const std::string Tree::search(const unsigned char ascii) const
{
	return (root != NULL ? root->search(ascii) : "");
}