#pragma once

#include "Blit3D.h"
#include <string>
#include <vector>

#define backgroundWidth 1920
#define backgroundHeight 1080

/**
* This class represents a birds and its behaviour.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/01/2023
*/
class Bird
{
private:
	/**
	* The screen width
	*/
	const float WIDTH = backgroundWidth;
	/**
	* The screen height
	*/
	const float HEIGHT = backgroundHeight;
	/**
	* The bird max speed
	*/
	const float MAX_SPEED = 100.f;
	/**
	* The max amount a bird can turn
	*/
	const float MAX_TURN_SPEED = 50.f;
	/**
	* The separation WEIGHT
	*/
	const float SEPARATION_WEIGHT = 1.5f;
	/**
	* The alignment WEIGHT
	*/
	const float ALIGNMENT_WEIGHT = 1.f;
	/**
	* The cohesion WEIGHT
	*/
	const float COHESION_WEIGHT = 1.f;
	/**
	* The desired separation
	*/
	const float SEPARATION = 100.0f;
	/**
	* The distance to start aligning close by birds
	*/
	const float ALIGN_DISTANCE = 200.0f;
	/**
	* The distance to start approaching close by birds
	*/
	const float COHESION_DISTANCE = 200.0f;
	/**
	* The sprite array that represents the bird graphically.
	*/
	std::vector<Sprite*> spriteList;
	/**
	* The 2D vector that represents the birds's direction and speed.
	*/
	glm::vec2 velocity;
	/**
	* The 2D vector that represents the birds's acceleration.
	*/
	glm::vec2 acceleration;
	/**
	* The 2D vector that represents the birds's position.
	*/
	glm::vec2 position;
	/**
	* A float representing the birds's angle.
	*/
	float angle;
	/**
	* A float representing the birds's size.
	*/
	float radius;
	/**
	* A float representing the birds's size's projection.
	*/
	float radiusOrtho;
	/**
	* The timer to end thrusting after the button is pressed.
	*/
	float thrustTimer = 0;
	/**
	* The number of frame to draw.
	*/
	int frameNumber = 0;
	/**
	* Indicates whether the birds is turning left or not.
	*/
	bool turningLeft = false;
	/**
	* Indicates whether the birds is rutning right or not.
	*/
	bool turningRight = false;
public:
	/**
	* Bird object constructor method.
	* @param glm::vec2 The birds's position
	* @param float The birds's radius.
	* @param float The birds's angle.
	* @return An instance of the birds class.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	Bird(glm::vec2, float, float);
	/**
	* Gets the birds's velocity
	* @return The birds's velocity.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	glm::vec2 GetVelocity();
	/**
	* Sets the birds's velocity to a given 2D vector.
	* @param glm::vec2 The birds's new velocity.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	void SetVelocity(glm::vec2);
	/**
	* Sets the birds's frame number.
	* @param frameNumber The frame Number.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	void SetFrameNumber(int frameNumber);
	/**
	* Sets the birds's position to a given 2D vector.
	* @param glm::vec2 The birds's new position.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	void SetPosition(glm::vec2);
	/**
	* Gets the birds's position
	* @return The birds's position.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	glm::vec2 GetPosition();
	/**
	* Gets the birds's radius.
	* @return The birds's radius.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	float GetRadius();
	/**
	* Gets the closest position of an element in the 9 possible places to this object.
	* @param other The position of the object.
	* @return The closest version of the position in the 9 quadrants.
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	glm::vec2 GetClosest(glm::vec2 other);
	/**
	* Calculates distance from the center of this birds to a position.
	* @param glm::vec2 The position to calculate the distance with another object.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	float Distance(glm::vec2);
	/**
	* Adds a sprite to the sprite list.
	* @param Sprite* The sprite to add to the list.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	void Bird::AddSprite(Sprite* newSprite);
	/**
	* Updates the birds's values after certain time period.
	* @param seconds The time period that has occurred since last uptade.
	* @return Returns true.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	bool Update(float seconds);
	/**
	* Draws the birds on the screen.
	* @author Dario Urdapilleta
	* @since 03/01/2023
	*/
	void Draw();
	/**
	* Destroys all the bird's components
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void Destroy();
	/**
	* Applies a force to the bird
	* @param force The forced applied to the acceleration.
	* @author Darren Reid
	*/
	void ApplyForce(glm::vec2 force);
	/**
	* Runs alld the flocking logic
	* @param birds The list of birds to watch out
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void Flock(std::vector<Bird*> birds);
	/**
	* Steers to a desired direction
	* @param seconds The time elapsed between last and this update.
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	void TurnToHeading(double seconds);
	/**
	* Separates this bird from other birds given the separation radius
	* @param birds The list of birds to separate from
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	glm::vec2 Separate(std::vector<Bird*> birds);
	/**
	* Aligns to birds close by
	* @param birds The list of birds to separate from
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	glm::vec2 Align(std::vector<Bird*> birds);
	/**
	* Gets closer to other birds.
	* @param birds The list of birds to separate from
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	glm::vec2 Cohesion(std::vector<Bird*> birds);
	/**
	* Seeks to reach a target vector
	* @param target The desired direction
	* @author Dario Urdapilleta
	* @since 03/06/2023
	*/
	glm::vec2 Seek(glm::vec2 target);
};