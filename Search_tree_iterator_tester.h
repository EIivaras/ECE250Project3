/*************************************************
 * Search_tree_iterator_tester<Type>
 * A class for testing iterators used in search trees
 *
 * Author:  Gerald NDA stolen from Douglas Wilhelm Harder
 *
 * DO NOT EDIT THIS FILE
 *************************************************/

#ifndef SEARCH_TREE_ITERATOR_TESTER_H
#define SEARCH_TREE_ITERATOR_TESTER_H

#include "Exception.h"
#include "Tester.h"
#include "Search_tree.h"

#include <iostream>

template <typename Type>
class Search_tree_iterator_tester:public Tester< typename Search_tree<Type>::Iterator > {
	using Tester< typename Search_tree<Type>::Iterator >::object;
	using Tester< typename Search_tree<Type>::Iterator >::command;

	public:
		Search_tree_iterator_tester( typename Search_tree<Type>::Iterator *obj = nullptr );
		void process();
};

template <typename Type>
Search_tree_iterator_tester<Type>::Search_tree_iterator_tester( typename Search_tree<Type>::Iterator *obj ):
Tester< typename Search_tree<Type>::Iterator >( obj ) {
	// empty
}

/****************************************************
 * void process()
 *
 * ! DO NOT CALL DELETE ON THIS SUBTEST!
 * Process the current command.  For search tree iterators, these include:
 *
 *                 Member
 *   Flag         Function         Tests if ...
 * 
 *  Mutators
 *
 *   next          ++              go to the next node
 *   previous      --              go to the previous node
 *
 *  Accessors
 *
 *   value n       *               the stored value is n
 ****************************************************/

template <typename Type>
void Search_tree_iterator_tester<Type>::process() {
	if ( command == "next" ) {
		++(*object);
        std::cout << "Okay" << std::endl;
	} else if ( command == "value" ) {
		Type expected_value;
		std::cin >> expected_value;

		Type actual_value = **object;

		if ( expected_value == actual_value ) {
			std::cout << "Okay" << std::endl;
		} else {
			std::cout << ": Failure in value(): expecting '" << expected_value <<
				"' but got '" << actual_value << "'" << std::endl;
		}
	} else if ( command == "previous" ) {
		--(*object);
        std::cout << "Okay" << std::endl;
	} else {
		std::cout << command << ": Command not found." << std::endl;
	}
}

#endif
