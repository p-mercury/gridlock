#ifndef item_h
#define item_h

#include <stdexcept>
#include <string>
#include "constants.hpp"

#include "item.hpp"

class Item{
	private:
		void Item();
		int size;
		string Name;

	public:
		int getSize();
};

#endif