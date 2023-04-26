#include "Bird.h"
#include <cmath>

Bird::Bird(glm::vec2 newPosition, float newRadius, float newAngle)
{
	this->frameNumber = 0;
	this->angle = newAngle;
	this->position = newPosition;
	// Set the initial velocity
	this->velocity = glm::vec2(glm::cos(this->angle), glm::sin(this->angle));
	this->acceleration = { 0,0 };
	this->radius = newRadius;
	// Set the radious used to scale the image
	this->radiusOrtho = newRadius / 30 / sqrt(2);
	
}

glm::vec2 Bird::GetVelocity()
{
	return this->velocity;
}

void Bird::SetVelocity(glm::vec2 newVelocity)
{
	this->velocity = newVelocity;
}

void Bird::SetFrameNumber(int frameNumber)
{
	this->frameNumber = frameNumber;
}

void Bird::SetPosition(glm::vec2 newPosition)
{
	this->position = newPosition;
}

glm::vec2 Bird::GetPosition()
{
	return this->position;
}

float Bird::GetRadius()
{
	return this->radius;
}

glm::vec2 Bird::GetClosest(glm::vec2 other)
{
	glm::vec2 minPosition = other;
	float minDistance = 4000, distance;
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			distance = sqrt(pow(this->position.x - (other.x + (WIDTH * x)), 2) + pow(this->position.y - (other.y + (HEIGHT * y)), 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				minPosition = vec2(other.x + (WIDTH * x), other.y + (HEIGHT * y));
			}
		}
	}
	return minPosition;
}

float Bird::Distance(glm::vec2 otherPosition)
{
	float distance = sqrt(pow(this->position.x - otherPosition.x, 2) + pow(this->position.y - otherPosition.y, 2));
	return distance;
}

void Bird::AddSprite(Sprite* newSprite)
{
	this->spriteList.push_back(newSprite);
}

bool Bird::Update(float seconds)
{
	// Steer to the calculated acceleration, this changes the next angle
	this->TurnToHeading(seconds);

	// From the angle, calculate the vector with an acceleration force
	float radians = angle * (M_PI / 180);
	glm::vec2 facing;
	facing.x = std::cos(radians);
	facing.y = std::sin(radians);

	facing *= seconds * 500.f;

	// Add this vector to the velocity
	this->velocity += facing;

	//check if over max speed
	if (glm::distance(this->velocity, vec2(0.f, 0.f)) > MAX_SPEED)
	{
		this->velocity = glm::normalize(this->velocity) * MAX_SPEED;
	}

	// Handle the animation
	this->thrustTimer += seconds;

	//animation timing
	if (this->thrustTimer >= 1.f/ 8.f)
	{
		//change frames
		this->frameNumber++;
		if (this->frameNumber > 5)
			this->frameNumber = 0;

		this->thrustTimer = 0.f;
	}

	//update position
	this->position += this->velocity * seconds;

	//bounds check position
	if (this->position.x < 0) this->position.x += 1920.f;
	if (this->position.x > 1920) this->position.x -= 1920.f;
	if (this->position.y < 0) this->position.y += 1080.f;
	if (this->position.y > 1080) this->position.y -= 1080.f;

	//velocity could potentialy get very small:we should
	//cap it to zero when it gets really close,
	//but that is a more advanced topic that
	//requires techniques like std::isnan()
	float length = glm::length(this->velocity);
	if (std::isnan(length) || length < 0.00001f)
		this->velocity = glm::vec2(0, 0);

	// Reset the acceleration
	this->acceleration *= 0.f;

	return true;
}

void Bird::Draw()
{	
	//change ship angle because my graphics face "up", not "right"
	this->spriteList[this->frameNumber]->angle = angle - 90;
	//draw main ship sprite
	this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y, this->radiusOrtho, this->radiusOrtho);

	//redraw if too close to an edge
	//left
	if (this->position.x < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x + 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//right
	if (this->position.x > 1920.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x - 1920.f, this->position.y, this->radiusOrtho, this->radiusOrtho);
	//down
	if (this->position.y < this->radius + 10.f) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y + 1080.f, this->radiusOrtho, this->radiusOrtho);
	//up
	if (this->position.y > 1080.f - (this->radius + 10.f)) this->spriteList[this->frameNumber]->Blit(this->position.x, this->position.y - 1080.f, this->radiusOrtho, this->radiusOrtho);
	//copies for 4 diagonal corners
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x - backgroundWidth, this->position.y + backgroundHeight, this->radiusOrtho, this->radiusOrtho);
	this->spriteList[this->frameNumber]->Blit(this->position.x + backgroundWidth, this->position.y - backgroundHeight, this->radiusOrtho, this->radiusOrtho);
}

void Bird::Destroy()
{
	// Nothing to do
}

void Bird::ApplyForce(glm::vec2 force)
{
	this->acceleration += force;
}

void Bird::Flock(std::vector<Bird*> birds)
{
	// Calculate the different adjustments
	glm::vec2 separation = this->Separate(birds);
	glm::vec2 align = this->Align(birds);
	glm::vec2 cohesion = this->Cohesion(birds);

	// Apply the forces
	this->ApplyForce(separation);
	this->ApplyForce(align);
	this->ApplyForce(cohesion);
}

void Bird::TurnToHeading(double seconds)
{
	//angle=atan2(-b*x+a*y,a*x+b*y)
	float difference = std::atan2f(-this->velocity.y * this->acceleration.x + this->velocity.x * this->acceleration.y, this->velocity.x * this->acceleration.x
		+ this->velocity.y * this->acceleration.y);
	difference = glm::degrees(difference);
	float amountAllowed = MAX_TURN_SPEED * seconds;

	if (abs(difference) <= amountAllowed)
	{
		//we are turning less than the amount allowed, so we can just
		//set our direction to the desired one
		angle += difference;
		//this->velocity = this->acceleration; // The velocity is getting updated somewhere else.
	}
	else
	{
		//we are trying to turn too far for the time passed,
		//so only change by the amount allowed in that time
		if (difference < 0) angle -= amountAllowed;
		else angle += amountAllowed;
		//this->velocity.x = cos(glm::radians(angle)); // The velocity is getting updated somewhere else.
		//this->velocity.y = sin(glm::radians(angle)); // The velocity is getting updated somewhere else.
	}
}

glm::vec2 Bird::Separate(std::vector<Bird*> birds)
{
	glm::vec2 steer = glm::vec2(0.f, 0.f);
	int count = 0;
	float distance;
	glm::vec2 closest;
	// Check for every bird in the system if this bird is too close
	for (auto bird : birds)
	{
		closest = this->GetClosest(bird->GetPosition());
		distance = this->Distance(closest);
		if ((distance > 0) && (distance < SEPARATION))
		{
			// Get the vector torwards the birds
			steer += glm::normalize(position - closest) / distance;
			count++;
		}
	}
	// Modularize the result vector by the amount of close by birds
	if (count > 0)
	{
		steer /= (float)count;
	}

	// Steer from the previous velocity and make sure it doesnt go over the limit.
	if (glm::distance(steer, vec2(0.f, 0.f)) > 0)
	{
		steer = glm::normalize(steer) * MAX_SPEED;
		steer -= velocity;
		if (steer.length() > MAX_SPEED)
		{
			steer = glm::normalize(steer) * MAX_SPEED;
		}
	}

	// Apply a weight to this steering behaviour
	steer *= SEPARATION_WEIGHT;

	return steer;
}

glm::vec2 Bird::Align(std::vector<Bird*> birds)
{
	glm::vec2 steer = glm::vec2(0.f, 0.f);
	int count = 0;
	float distance;
	glm::vec2 closest;
	// Check for every bird in the system if this bird is close enough
	for (auto bird : birds)
	{
		closest = this->GetClosest(bird->GetPosition());
		distance = this->Distance(closest);
		if ((distance > 0) && (distance < ALIGN_DISTANCE))
		{
			// Align to the bird
			steer += bird->GetVelocity();
			count++;
		}
	}

	// Modularize the result vector by the amount of close by birds
	if (count > 0)
	{
		steer /= (float)count;
		steer = glm::normalize(steer) * MAX_SPEED;
		steer -= velocity;
		if (steer.length() > MAX_SPEED)
		{
			steer = glm::normalize(steer) * MAX_SPEED;
		}

		// Apply a weight to this steering behaviour
		steer *= ALIGNMENT_WEIGHT;

		return steer;
	}
	else
	{
		return glm::vec2(0.f, 0.f);
	}
}

glm::vec2 Bird::Cohesion(std::vector<Bird*> birds)
{
	glm::vec2 steer = glm::vec2(0.f, 0.f);
	int count = 0;
	float distance;
	glm::vec2 closest;
	// Check for every bird in the system if this bird is close enough
	for (auto bird : birds)
	{
		closest = this->GetClosest(bird->GetPosition());
		distance = this->Distance(closest);
		if ((distance > 0) && (distance < COHESION_DISTANCE))
		{
			// Get the vector torwards the birds
			steer += closest;
			count++;
		}
	}

	// Modularize the result vector by the amount of close by birds
	if (count > 0)
	{
		steer /= (float)count;
		steer = this->Seek(steer);

		// Apply a weight to this steering behaviour
		steer *= COHESION_WEIGHT;

		return steer;
	}
	else
	{
		return glm::vec2(0.f, 0.f);
	}
}

glm::vec2 Bird::Seek(glm::vec2 target)
{
	// get the direction torwards the target
	glm::vec2 desired = glm::normalize(target - this->position) * MAX_SPEED;
	// Calculate the direction from the current velocity
	glm::vec2 steer = desired - this->velocity;
	// Make sure the result is not past the max speed
	if (steer.length() > MAX_SPEED)
	{
		steer = glm::normalize(steer) * MAX_SPEED;
	}
	return steer;
}