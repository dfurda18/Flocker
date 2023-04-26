#include "Flock.h"

Flock::Flock()
{
	random = new RandomGenerator();
}

void Flock::MakeBird()
{
	// Create the bird
	Bird* bird = new Bird(glm::vec2(random->RandomInt(0, backgroundWidth), random->RandomInt(0, backgroundHeight)), 50.0f, random->RandomFloat(0.f, 360.f, 2));
	// Bird sprites
	for (int i = 0; i < 6; i++)
	{
		bird->AddSprite(this->spriteList[i]);
	}
	// Set the animation frame number
	bird->SetFrameNumber(random->RandomInt(0, this->spriteList.size() - 1));
	// Add the bird to the list
	this->birds.push_back(bird);
	

}

void Flock::AddSprite(Sprite* sprite)
{
	this->spriteList.push_back(sprite);
}

void Flock::Update(double seconds)
{
	// Calculate the flock behaviour
	for (auto bird : this->birds)
	{
		bird->Flock(this->birds);
	}
	// Update the birds
	for (auto bird : this->birds)
	{
		bird->Update(seconds);
	}
}

void Flock::Draw()
{
	for (auto bird : this->birds)
	{
		bird->Draw();
	}
}

void Flock::Destroy()
{
	// Destroy the birds
	for (auto bird : this->birds)
	{
		if (bird != NULL)
		{
			bird->Destroy();
			delete bird;
		}
	}
	// Destroy the random number generator
	if (random != NULL)
	{
		delete random;
	}
}

