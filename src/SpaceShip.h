#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "TextureManager.h"

class SpaceShip final : public DisplayObject
{
public:
	// constructor(s)
	SpaceShip();

	// destructor
	~SpaceShip();

	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(const glm::vec2 destination);
	void setMaxSpeed(const float speed);
	glm::vec2 getOrientation();
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	std::string getState();
	void setState(std::string state);

private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	std::string currentState;

	// private function
	void m_Move();
};

#endif // defined (__SPACE_SHIP__)
