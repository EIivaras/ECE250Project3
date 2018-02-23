#include <iostream>
#include "Search_tree.h"

int main() {
	Search_tree<int> tree;

	// For testing iterator

	for (int i = 1; i <= 30; ++i) {
		std::cout << tree.insert((83 * i) % 30);
	}

	std::cout << std::endl;

	for (int i = 1; i <= 30; ++i) {
		std::cout << tree.insert((97 * i) % 30);
	}

	std::cout << std::endl;

	std::cout << "Front:   " << tree.front() << std::endl;
	std::cout << "Back:    " << tree.back() << std::endl;
	std::cout << "Height:  " << tree.height() << " <- this will change for an AVL tree" << std::endl;

	std::cout << "Front to Back --> ";

	for (Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	std::cout << "Now to erase nodes with values on the interval of [14, 21] inside the tree!" << std::endl;

	for (int i = 14; i < 22; ++i) {
		std::cout << tree.erase(i);
	}

	std::cout << std::endl;

	std::cout << "Front to Back --> ";

	for (Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	std::cout << "Back to Front --> ";

	for (Search_tree<int>::Iterator itr = tree.rbegin(); itr != tree.rend(); --itr) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	tree.clear();

	std::cout << "After Clearing --> ";


	for (Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	return 0;
}