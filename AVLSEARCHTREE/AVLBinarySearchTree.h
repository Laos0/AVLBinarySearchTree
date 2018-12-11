#ifndef _BST_
#define _BST_
#include<string>
#include<iostream>
using namespace std;
////class defintion for tree nodes
class node {
public:
	node() {
		parent = nullptr;
		left = nullptr;
		right = nullptr;
		_k = int();
		_v = string();
	}
	/////left and right, and parent
	node* parent;
	node* left;
	node* right;
	bool IsExternal() {
		return (left == nullptr && right == nullptr);
	}
	bool IsInternal() {
		return !IsExternal();
	}
	bool IsRoot() {
		return (parent == nullptr);
	}
	////key and value
	int _k;
	string _v;
};

/////define for BST tree class
class BinarySearchTree {
public:
	BinarySearchTree() {

	}
	void displayTree() {
		// Need to implement: display all nodes in the BST
		cout << _Root->_k;
	}
	int getSize() {
		return this->_Size;
	}
	bool empty() {
		return this->_Size == 0;
	}
	node* getRoot() {
		return this->_Root;
	}
	////search for node function
	node* ToSearch(int k, node* N) {///N is always root at the beginning
		if (N->IsExternal()) {///not finding any nodes in tree that has k
			return N;
		}
		if (k < N->_k) {
			return ToSearch(k, N->left);
		}
		else if (k == N->_k) {
			return N;
		}
		else { // if k > N->k
			return ToSearch(k, N->right);
		}
	}
	///insert: building the tree
	node* ToInsert(int k, string v, node* N) {//N always starts from root node
		node* w;
		if (empty()) {///the node is root
			node* root = new node;
			this->_Root = root;
			w = root;
			InsertAtExternal(this->_Root, k, v);
		}
		else {
			w = ToSearch(k, N);///search for appropriate node
			if (w->IsInternal()) {////if such node already exists in the tree
				return ToInsert(k, v, w->left);
			}
			////insert the new node at an external node on the tree
			this->InsertAtExternal(w, k, v);
		}
		//return the new node
		this->_Size++;
		return w;
	}
	///to delete node from tree
	void deleteNode(int n) {
		node* newN = ToSearch(n, _Root);
		if (newN->_k != 0) {
			cout << "node found";
			// only for deleting the root
			if (newN->IsRoot()) {
				if (newN->right->_k == 0 && newN->left->_k == 0) {
					_Root->_k = 0;
					//_Root = nullptr;
				}
				else if (newN->right->_k != 0) {
					if (newN->left->_k != 0) {
						node* leftMostNode = getMostLeftChild(newN->right);
						leftMostNode->left = newN->left;
						newN->left->parent = newN->right;
					}
					_Root = newN->right;
					_Root->parent = nullptr;
					
					cout << "Root Node deleted and replaced by right";
				}
				else {
					_Root = newN->left;
					cout << "Root Node deleted and replaced by left";
				}
			}
			else { // for deleting an other nodes that are not the root
				if (newN->right->_k == 0 && newN->left->_k == 0) {
					node* nullNode = new node();
					nullNode->_k = 0;
					if (isleftSideOfParent(newN)) {
						nullNode->parent = newN->parent->left;
						newN->parent->left = nullNode;

					}
					else {
						nullNode->parent = newN->parent->right;
						newN->parent->right = nullNode;
					}
					// If anyt problem, switch to pointers
					//nullNode._v = "p";
				}
				else if(newN->right->_k != 0) {
					// checking the right first before the left
					newN->parent->right = newN->right;
					newN->right->parent = newN->parent;
					if (newN->left->_k != 0) {
						node* leftMostNode = getMostLeftChild(newN->right);
						leftMostNode->left = newN->left;
						newN->left->parent = newN->right;
					}
				}
				else {
					newN->parent->right = newN->left;
				}
			}
			ReBalance(_Root);
		}
		else {
			cout << "node not found";
		}

	}

	bool isleftSideOfParent(node* node) {
		if (node->_k > node->parent->_k) {
			return false;
		}
		else {
			return true;
		}
	}

	node* getMostLeftChild(node* n) {
		if (n->left->_k != 0) {
			return getMostLeftChild(n->left);
		}
		else {
			return n;
		}
	}

	////////////////////////functions for AVL tree
	///////1. rebalance
	/*
	input: the new node inserted to the tree
	output: void, postcondition: Tree is rebalanced if not
	*/
	void ReBalance(node* v) {
		node* z = v;
		while (!z->IsRoot()) {
			z = z->parent;
			if (!this->IsBalanced(z)) {
				node* x = tallGrandchild(z);
				this->Restructure(x);
			}
		}
	}
	////////2. BInsert - insertion with rebalancing
	node* BInsert(int key, string value) {
		node* v = ToInsert(key, value, this->getRoot());
		ReBalance(v);
		return v;
	}

	void print2DUtil(node *root, int space)
	{
		// Base case 
		if (root == NULL)
			return;

		// Increase distance between levels 
		space += 10;

		// Process right child first 
		print2DUtil(root->right, space);

		// Print current node after space 
		// count 
		printf("\n");
		for (int i = 10; i < space; i++)
			printf(" ");
		printf("%d\n", root->_k);

		// Process left child 
		print2DUtil(root->left, space);
	}

	// Wrapper over print2DUtil() 
	void print2D(node *root)
	{
		// Pass initial space count as 0 
		print2DUtil(root, 0);
	}

private:
	node* _Root;
	int _Size;
	/////1.auxiliary function for insert
	void InsertAtExternal(node* v, int key, string value) {
		if (v->IsExternal()) {
			////assign key and value to the node
			v->_k = key;
			v->_v = value;
			/////create children of v, so it becomes internal node
			node* left = new node;
			node* right = new node;
			v->left = left;
			v->right = right;
			left->parent = v;
			right->parent = v;
		}
		else {////if not external
			cout << "this function should insert new node on an external node";
		}
	}


	/*
	auxiliary functions needed for AVL tree rebalancing
	*/
	///1. height
	int height(node* N) {
		if (N != nullptr) {
			if (N->IsExternal()) {
				return 0;
			}
			else {
				int left = height(N->left);
				int right = height(N->right);
				if (left >= right) {
					return left + 1;
				}
				else {
					return right + 1;
				}
			}
		}
		else {
			cout << "node this null" << endl;
		}
	}
	////2. Isbalance-check if node is balanced or not, difference between heights of left and right can not exceed 1
	bool IsBalanced(node* N) {
		if (N->IsExternal()) {
			return true;
		}
		else {
			int bal = height(N->left) - height(N->right);
			return ((-1 <= bal) && (bal <= 1));
		}
	}
	////3. tallGrandchild: find the node x, which is grandchild of z, child of y, from z
	/*
	z is first unbalanced node from bottom up
	y is child of z with greater height
	x is child of y with greater height
	*/
	node* tallGrandchild(node* z) {
		node* left = z->left;
		node* right = z->right;
		if (height(left) >= height(right)) {///left is y
			node* y = left;
			if (height(y->left) >= height(y->right)) {
				return y->left;
			}
			else {
				return y->right;
			}
		}
		else {/////right is y
			node* y = right;
			if (height(y->left) >= height(y->right)) {
				return y->left;
			}
			else {
				return y->right;
			}
		}
	}
	////4.restructure
	/*
	input: a node x that has parent y and grandparent z
	output: void, afterward condition: tree T will be balanced AVL T
	*/
	void Restructure(node* x) {
		////1. find y z
		node* y = x->parent;
		node* z = y->parent;
		////2. find their inorder-there can only 4 situations(reference from book)
		node* a = nullptr; node* b = nullptr; node* c = nullptr;
		if (z->right == y && y->right == x) {
			a = z; b = y; c = x;
		}
		else if (z->left == y && y->left == x) {
			c = z; b = y; a = x;
		}
		else if (z->right == y && y->left == x) {
			a = z; b = x; c = y;
		}
		else if (z->left == y && y->right == x) {
			a = y; b = x; c = z;
		}
		////3. find the subtrees not directed from z y x (similarly only 4 cases, reference from book)
		node* T0 = nullptr; node* T1 = nullptr;
		node* T2 = nullptr; node* T3 = nullptr;
		if (a == z && b == y && c == x) {
			T0 = a->left; T1 = b->left; T2 = c->left; T3 = c->right;
		}
		else if (a == x && b == y && c == z) {
			T0 = a->left; T1 = a->right; T2 = b->right; T3 = c->right;
		}
		else if (a == z && b == x && c == y) {
			T0 = a->left; T1 = b->left; T2 = b->right; T3 = c->right;
		}
		else if (a == y && b == x && c == z) {
			T0 = a->left; T1 = b->left; T2 = b->right; T3 = c->right;
		}
		////4.1 replace the subtree rooted at z with a new subtree rooted at b
		///4.1.1 if z is not root
		if (!z->IsRoot()) {
			if (z->parent->left == z) {
				z->parent->left = b;
			}
			else {
				z->parent->right = b;
			}
			b->parent = z->parent;
		}
		///4.1.2 if z is root
		else {
			b->parent = nullptr;
			this->_Root = b;
		}
		////4.2.1 set a be left child of b
		b->left = a;
		a->parent = b;
		a->left = nullptr;
		a->right = nullptr;
		////4.2.2 let c be right child of b
		b->right = c;
		c->parent = b;
		c->left = nullptr;
		c->right = nullptr;
		/////4.3.1 let T0, T1 be left and right subtrees of a, respectively
		a->left = T0;
		if (T0 != nullptr) {
			T0->parent = a;
		}
		a->right = T1;
		if (T1 != nullptr) {
			T1->parent = a;
		}
		/////4.3.2 let T2, T3 be left and right subtrees of c, respectively
		c->left = T2;
		if (T2 != nullptr) {
			T2->parent = c;
		}
		c->right = T3;
		if (T3 != nullptr) {
			T3->parent = c;
		}
	}
};
#endif