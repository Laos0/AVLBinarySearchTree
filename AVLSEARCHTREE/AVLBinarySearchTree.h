#ifndef _BST_
#define _BST_
#include<string>
#include<iostream>
#include<vector>
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
	//// personal method to call isBalance, since it is private
	node* getUnbalancedNode(node* N) {
		if (N == nullptr) {
			return N;
		}
		else {
			if (N->parent == nullptr) {
				if (IsBalanced(N)) {
					return nullptr;
				}
				else {
					return N;
				}
			}
			else {
				if (IsBalanced(N)) {
					getUnbalancedNode(N->parent);
				}
				else { // if N is unbalanced return N or itself
					return N;
				}
			}
		}
	}

	node* callTallGrandChild(node* N) {
		return tallGrandchild(N);
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
	////////////////////////////////////////////to delete node from tree//////////////////////////////
	node* deleteNode(int n) {
		if (this->getSize() > 0) {
			node* v = ToSearch(n, this->getRoot());//find the node to delete
			if (v->IsExternal()) {
				cout << "such node can not be found to be deleted from the tree" << endl;
			}
			else {
				node* w; //node that is possibly external and child of v
				if (v->left->IsExternal()) {///case1 if v's left child is external
					w = v->left;
				}
				else if (v->right->IsExternal()) {////case2 if v's right child is external
					w = v->right;
				}
				else {////case3 if no children of v is external
					w = v->right; ///we can also to left if we want
					do {
						w = w->left;///find the left most child with smallest key larger than v.key
					} while (w->IsInternal());
					node* u = w->parent;///u is node to replace the node to be deleted
					v->_k = u->_k;
					v->_v = u->_v;
				}
				////reduce number of nodes in tree
				this->_Size--;

				node* pN = v->parent;
				/////delete the node from the tree, using auxiliary function
				this->RemoveAboveExternal(w);
				return pN;

			}
		}
		else {
			cout << "tree is empty, nothing to be deleted" << endl;
			return nullptr;
		}

	}


	void RemoveAboveExternal(node* N) {///remove the passing parameter node
		if (N->IsExternal()) {
			///1. get sibling node
			node* sib;
			node* parent = N->parent;
			if (parent->left == N) {///if N is left child
				sib = parent->right;
			}
			else {
				sib = parent->left;
			}
			///2.replace parent with sibling
			////2.1 if parent is root: that means no grandparents
			if (parent->IsRoot()) {
				this->_Root = sib;
				sib->parent = nullptr;
			}
			else {////2.2 if parent is not root: that means there is grand parent
				node* Gparent = parent->parent;
				//need to figure out if parent is grandparent's left or right child
				if (parent == Gparent->left) {//if left
					Gparent->left = sib;
					sib->parent = Gparent;
				}
				else {
					Gparent->right = sib;
					sib->parent = Gparent;
				}
			}

			////delete the nodes after all the operations
			delete N;
			delete parent;
		}
		else {
			cout << "RemoveAboveExternal(): should be external node";
		}
	}

	////// ----------------------------- END OF DELETE -------------------------------------

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


	// ----------------------------------------- DRAW THE TREE -------------------------------------------
		///draw the tree
	void DrawTree() {
		///1. create array large enough for drawing the tree
		int col = this->_Size;
		int row = height(this->_Root) + 1;
		vector<vector<string>> Grid(row, vector<string>(col, "NULL"));
		///2. inorder traversal tree and assign coordinates
		int counter = 0;
		AssignCoordinates(this->_Root, Grid, counter);
		///3. draw the tree using the Grid
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (Grid[i][j] == "NULL") {
					cout << "  ";
				}
				else {
					cout << Grid[i][j] << " ";
				}
			}
			cout << endl;
		}
	}

	///Assign Coordinates
	void AssignCoordinates(node* N, vector<vector<string>>& Grid, int& counter) {
		if (N != nullptr) {
			if (N->_k != 0) {
				AssignCoordinates(N->left, Grid, counter);
				int col = counter;
				int row = GetDepth(N);
				Grid[row][col] = to_string(N->_k);
				counter++;
				AssignCoordinates(N->right, Grid, counter);
			}
		}
	}

	////depth-number of nodes above
	int GetDepth(node* N) {
		if (N->IsRoot()) {
			return 0;
		}
		else {
			return 1 + GetDepth(N->parent);
		}
	}
	// -------------------------------------------------- END OF DRAW TREE ---------------------------------------

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
			// if height of N's left and right has a difference of 1 or less then it is less
			// balanced
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