/*****************************************
* UW User ID: zwalford
* Submitted for ECE 250
* Semester of Submission:  (Winter) 2018
*
* By submitting this file, I affirm that
* I am the author of all modifications to
* the provided code.
*****************************************/

#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include "Exception.h"
#include "ece250.h"
#include <cassert>
#include <algorithm>

template <typename Type>
class Search_tree {

private:
	class Node {
	public:
		Type node_value;
		int tree_height;

		// The left and right sub-trees
		Node *left_tree;
		Node *right_tree;

		// Hint as to how you can create your iterator
		// Point to the previous and next nodes in linear order
		Node *previous_node;
		Node *next_node;

		// Member functions
		Node(Type const & = Type());

		void update_height();

		int height() const;
		bool is_leaf() const;
		Node *front();
		Node *back();
		Node *find(Type const &obj);

		void clear();
		bool insert(Type const &obj, Node *&to_this);
		bool erase(Type const &obj, Node *&to_this);
	};

	Node *root_node;
	int tree_size;

	// Hint as to how to start your linked list of the nodes in order 
	Node *front_sentinel;
	Node *back_sentinel;

public:
	class Iterator {
	private:
		Search_tree * containing_tree;
		Node *current_node;
		bool is_end;

		// The constructor is private so that only the search tree can create an iterator
		Iterator(Search_tree *tree, Node *starting_node);

	public:
		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Type operator*() const;
		Iterator &operator++();
		Iterator &operator--();
		bool operator==(Iterator const &rhs) const;
		bool operator!=(Iterator const &rhs) const;

		// Make the search tree a friend so that it can call the constructor
		friend class Search_tree;
	};

	// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
	Search_tree();
	~Search_tree();

	bool empty() const;
	int size() const;
	int height() const;

	Type front() const;
	Type back() const;

	Iterator begin();
	Iterator end();
	Iterator rbegin();
	Iterator rend();
	Iterator find(Type const &);

	void clear();
	bool insert(Type const &);
	bool erase(Type const &);

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Search_tree<T> const &);
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree() :
	root_node(nullptr),
	tree_size(0),
	front_sentinel(new Search_tree::Node(Type())),
	back_sentinel(new Search_tree::Node(Type())) {
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
	clear();  // might as well use it...
	delete front_sentinel;
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return (root_node == nullptr);
}

template <typename Type>
int Search_tree<Type>::size() const {
	return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
	return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if (empty()) {
		throw underflow();
	}

	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if (empty()) {
		throw underflow();
	}

	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
	return empty() ? Iterator(this, back_sentinel) : Iterator(this, root_node->front());
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator(this, back_sentinel);
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	return empty() ? Iterator(this, front_sentinel) : Iterator(this, root_node->back());
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator(this, front_sentinel);
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find(Type const &obj) {
	if (empty()) {
		return Iterator(this, back_sentinel);
	}

	typename Search_tree<Type>::Node *search_result = root_node->find(obj);

	if (search_result == nullptr) {
		return Iterator(this, back_sentinel);
	}
	else {
		return Iterator(this, search_result);
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if (!empty()) {
		root_node->clear();
		root_node = nullptr;
		tree_size = 0;
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert(Type const &obj) {
	if (empty()) { // If the search tree is empty (root_node == nullptr) 
		root_node = new Search_tree::Node(obj); // Create a new root node
		tree_size = 1;

		// When putting our root node into the tree, as it is the only node in the tree we have to configure it to point to
		// the front and back sentinels, and have those sentinels point to it
		root_node->next_node = back_sentinel;
		root_node->previous_node = front_sentinel;
		back_sentinel->previous_node = root_node;
		front_sentinel->next_node = root_node;

		return true;
	}
	else if (root_node->insert(obj, root_node)) { // Otherwise, call the NODE insert function
		++tree_size;
		return true;
	}
	else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::erase(Type const &obj) {
	if (!empty() && root_node->erase(obj, root_node)) {
		--tree_size;
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node(Type const &obj) :
	node_value(obj),
	left_tree(nullptr),
	right_tree(nullptr),
	next_node(nullptr),
	previous_node(nullptr),
	tree_height(0) {
	// does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
	tree_height = std::max(left_tree->height(), right_tree->height()) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	return (this == nullptr) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return ((left_tree == nullptr) && (right_tree == nullptr));
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	return (left_tree == nullptr) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	return (right_tree == nullptr) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find(Type const &obj) {
	if (obj == node_value) {
		return this;
	}
	else if (obj < node_value) {
		return (left_tree == nullptr) ? nullptr : left_tree->find(obj);
	}
	else {
		return (right_tree == nullptr) ? nullptr : right_tree->find(obj);
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if (left_tree != nullptr) {
		left_tree->clear();
	}

	if (right_tree != nullptr) {
		right_tree->clear();
	}

	delete this;
}

template <typename Type>
bool Search_tree<Type>::Node::insert(Type const &obj, Node *&to_this) {
	if (obj < node_value) { // If our incoming value is smaller than our current node's value
		if (left_tree == nullptr) { // If the left search tree doesn't exist
			left_tree = new Search_tree<Type>::Node(obj); // Create a node to start the left search tree and have left_tree point to the new node
			update_height();

			left_tree->next_node = this;
			left_tree->previous_node = this->previous_node;
			this->previous_node->next_node = left_tree;
			this->previous_node = left_tree;
			
			return true;
		}
		else {
			if (left_tree->insert(obj, left_tree)) { // Otherwise, recursively call insert until we get to the right spot in the tree
				update_height();
				return true;
			}
			else { // TODO: Could not be inserted - perhaps a duplicate?
				return false;
			}
		}
	}
	else if (obj > node_value) { // Same operations for the right sub tree as for the left, except if incoming value is larger than current node's value
		if (right_tree == nullptr) {
			right_tree = new Search_tree<Type>::Node(obj);
			update_height();

			right_tree->previous_node = this;
			right_tree->next_node = this->next_node;
			this->next_node->previous_node = right_tree;
			this->next_node = right_tree;

			return true;
		}
		else {
			if (right_tree->insert(obj, right_tree)) {
				update_height();
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::Node::erase(Type const &obj, Node *&to_this) {
	if (obj < node_value) { // If the value of the object is less than the value of the current node
		if (left_tree == nullptr) { // But we're already at the smallest node
			return false; // Return false
		}
		else {
			if (left_tree->erase(obj, left_tree)) { // If we're not already at the smallest node, we'll keep looking
				update_height();
				return true;
			}

			return false;
		}
	}
	else if (obj > node_value) { // If the value of the object is greater than the value of the current node
		if (right_tree == nullptr) { // But we're already at the largest node
			return false; // Return false
		}
		else {
			if (right_tree->erase(obj, right_tree)) { // If we're not already at the largest node, we'll keep looking
				update_height();
				return true;
			}

			return false;
		}
	}
	else { // If the value being tested is not less than the current value or greater than the current value
		assert(obj == node_value); // To be absolutely sure, will terminate function call if evaluates false

		// For the iterator
		// Update the two nodes that point to the node that's about to be erased, and make them point to each other instead
		this->previous_node->next_node = this->next_node;
		this->next_node->previous_node = this->previous_node;

		if (is_leaf()) { // If the node is a leaf node, we're fine
			to_this = nullptr;
			delete this;
		}
		else if (left_tree == nullptr) { // If the node's left tree is null (so it's a 2-part linked list)
			to_this = right_tree; // We can just make the current node the right_tree (reduces height of sub-tree)
			delete this; // Deletes current node
		}
		else if (right_tree == nullptr) { // If the node's right tree is null (so it's a 2-part linked list)
			to_this = left_tree; // We can just make the current node the left_tree (reduces height of sub-tree)
			delete this; // Deletes current node
		}
		else { // Otherwise, we're in the middle of the tree. We have work to do.
			node_value = right_tree->front()->node_value; // Replaces the value of the node we wanted to erase with the value of the node at 
			                                              // the front of the right tree
			                                              // This means that everything in the right tree will still be greater than the node
			                                              // whose value we just replaced
			right_tree->erase(node_value, right_tree); // Erase the node at the front of the tree whose vaue we just replicated
			update_height(); // And update the height
		}

		return true;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator(Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node) :
	containing_tree(tree),
	current_node(starting_node) {
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing
	if (current_node->next_node == nullptr) {
		return *this;
	}
	// Your implementation here, do not change the return value
	current_node = current_node->next_node;
	return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing

	if (current_node->previous_node == nullptr) {
		return *this;
	}

	// Your implementation here, do not change the return value
	current_node = current_node->previous_node;
	return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==(typename Search_tree<Type>::Iterator const &rhs) const {
	// This is done for you...
	return (current_node == rhs.current_node);
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=(typename Search_tree<Type>::Iterator const &rhs) const {
	// This is done for you...
	return (current_node != rhs.current_node);
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<(std::ostream &out, Search_tree<T> const &list) {
	out << "not yet implemented";

	return out;
}

#endif