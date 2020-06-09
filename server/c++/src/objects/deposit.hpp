#ifndef deposit_h
#define deposit_h

#include <stdexcept>
#include <vector>
#include "constants.hpp"

#include "deposit.hpp"

class Deposit{
private:
	//Material *material;
	//How much of the material is present
	unsigned int volume;
	//How accisible is the material
	unsigned int accessibility;
public:
	Deposit(/*Material * _material,*/unsigned int _volume,unsigned int _accessibility);
	//Material * getMaterial();
	unsigned int getVolume();
	unsigned int getAccessibility();
};

#endif