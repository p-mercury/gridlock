#include "node.hpp"
#include <boost/random.hpp>
#include <random>
#include <cmath>
#define MAX_CLUSTER_SIZE 5

Node::Node(
		const unsigned int _posX,
		const unsigned int _posY,
		unsigned int _seed,
        std::vector<std::shared_ptr<Material>> _materials):
		posX(_posX),
		posY(_posY){

	for(auto const& material:_materials){
		for(long x=posX-(MAX_CLUSTER_SIZE/2);x<posX+(MAX_CLUSTER_SIZE/2);x++){
			for(long y=posY-(MAX_CLUSTER_SIZE/2);y<posY+(MAX_CLUSTER_SIZE/2);y++){
				if(isClusterOrigin(x,y,_seed,material)){
					debris.push_back(std::make_shared<Deposit>(
						material,
						depositVolume(_posX,posY,_seed,x,y,material),
						20));
				}
			}
		}
	}

}
/**
 *
 * @param _posX
 * @param _posY
 * @param _seed current build adds a value associated to the material to the seed before being introduced in the function
 * @return  seed associated to the x, y positions and the seed for randomization
 */
inline unsigned long seedConcat(unsigned int _posX, unsigned int _posY, unsigned int _seed){
	//concat x and y positions numerically, x then y.
	unsigned int pwr = 1;
	while (_posY/pwr > 1 )
		pwr *= 10;
	unsigned long XYcat = _posX * pwr + _posY;
	pwr = 1;
	//numerically concatenate the seed and the XY conc.
	while (XYcat  / pwr > 0)
		pwr *= 10;
	return _seed * pwr + XYcat;
}


bool Node::isClusterOrigin(
    unsigned int _posX,
    unsigned int _posY,
   	unsigned int _seed,
    std::shared_ptr<Material> _material){
	std::string n = _material->getName();
	unsigned int sum = 0;
	for (int i = 0; i < n.size(); i++)
		sum += (n[i] * (i+1));
	//add a number associated to material name to seed
	long seed = seedConcat(_posX , _posY, _seed + sum);
	std::default_random_engine engine(seed);
	int max = MATERIAL_ABUNDANCE_CAP;
	std::uniform_int_distribution<int> distribution(0, max);
	//if our abundance is greater than the random number, this is a cluster
	if (_material->getAbundance() > distribution(engine) )
		return true;
	else return false;

}
inline int distance(int x1, int x2,  int y1, int y2){
	return abs(x1 - x2) + abs(y1 - y2);
}

unsigned int Node::depositVolume(
    unsigned int _posX,
    unsigned int _posY,
    unsigned int _seed,

    long _originPosX,
    long _originPosY,
    std::shared_ptr<Material> _material){
	long s = seedConcat(_posX, _posY, _seed);
	std::default_random_engine engine(s);
	int d = distance(_posX, _originPosX, _posY, _originPosY);

	std::uniform_int_distribution<int> distribution(0,d) ;
	std::uniform_int_distribution<int> decay(d, 15);
	int a = distribution(engine);
	//just some numbers I made up LUL, should give 100 for origin and less for everything else lmaoxd;
	int res = (a+10) *10 - (decay(engine)* d)*5;
	if (res < 0)
		return 0;
	return  res;

}

std::shared_ptr<OccupyingObject> Node::getOccupant(){

  return occupant;
}

void Node::setOccupant(
	std::shared_ptr<OccupyingObject> _occupant){

	occupant = _occupant;
}

std::vector<std::shared_ptr<NonOccupyingObject>> Node::getObjects(){

	return objects;
}

std::vector<std::shared_ptr<NonPhysicsObject>> Node::getDebris(){

	return debris;
}