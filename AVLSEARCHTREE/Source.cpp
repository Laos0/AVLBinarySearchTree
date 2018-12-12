#include "AVLBinarySearchTree.h"
using namespace std;

int main() {

	BinarySearchTree MyBST;

	while (true)
	{
		cout << "enter 1 to insert:" << endl;
		cout << "enter 2 to search: " << endl;
		cout << "enter 3 to delete: " << endl;
		cout << "enter -999 to stop: " << endl;
		cout << endl;
		int input;
		cin >> input;

		if (input == -999) {
			break;
		}
		switch (input) {
			case 1: {///insert
				int key;
				string value;
				cout << "please enter ID and Name:"; // with a space between them when entering
				cin >> key >> value;
				MyBST.BInsert(key, value);
				//MyBST.displayTree();
				//MyBST.print2D(MyBST.getRoot());
				MyBST.DrawTree();
				cout << endl;
				break;
			}
			case 2: {/////search
				int key;
				cout << "please enter ID to search:";
				cin >> key;
				node* v = MyBST.ToSearch(key, MyBST.getRoot());
				if (v->IsInternal()) {
					cout << "ID: " << v->_k << ",  " << "Name: " << v->_v << endl;
				}
				else {
					cout << "record no exist" << endl;
				}
				//MyBST.print2D(MyBST.getRoot());
				MyBST.DrawTree();
				break;
			}
			case 3: { // Deleting
				int key;
				cout << "please enter ID to delete";
				cin >> key;
				node* v = MyBST.ToSearch(key, MyBST.getRoot());
				if (v->IsInternal()) {
					MyBST.deleteNode(key);
				}
				else {
					cout << "record not exist" << endl;
				}
				//MyBST.print2D(MyBST.getRoot());
				MyBST.DrawTree();
				break;
			}
			default: {
				cout << "you entered wrong option" << endl;
			}
		}
	}

	return 0;
}