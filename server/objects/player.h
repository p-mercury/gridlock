#ifndef player_h
#define player_h

#include <stdexcept>
#include "constants.h"

#include "object.h"

class Player:public object{
	public:
		std::string getToken(){
			return token;
		}

		std::string getName(){
			return name;
		}

		std::vector<Container> getConatiners(){
			return container;
		}

		void addContainer(std::vector<Container> _conatiners){
			containers
		}

	private:
		std::string token;
		std::string name;
		std::vector<Container> conatiners;

};

#endif