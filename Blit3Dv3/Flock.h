#pragma once
#include <vector>
#include "Bird.h"
#include "RandomGenerator.h"

/**
* This class represents a flock.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/06/2023
*/
class Flock
{
private:
	/**
	* The sprite array that represents the bird graphically.
	*/
	std::vector<Sprite*> spriteList;
	/**
	* The list of flock's irds
	*/
	std::vector<Bird*> birds;
	/**
	* The random number generator
	*/
	RandomGenerator* random;
public:
	/**
	* Creates a Flock
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	Flock();
	/**
	* Creates a Bird
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void MakeBird();
	/**
	* Adds a bird sprite for creating birds.
	* @param sprite The sprite to add
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void AddSprite(Sprite* sprite);
	/**
	* Updates the Flock
	* @param seconds The time elapsed between the this and the last update.
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void Update(double seconds);
	/**
	* Draws the flock
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void Draw();
	/**
	* Destroys the Flock
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void Destroy();
};