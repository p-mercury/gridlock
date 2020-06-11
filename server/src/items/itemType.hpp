#ifndef itemtype_h
#define itemtype_h

#include <stdexcept>
#include <vector>
#include "constants.hpp"

#include "item.hpp"

#include "itemType.hpp"

class ItemType{
private:
public:
	virtual unsigned int getName();
	virtual unsigned int getMass();
};

#endif