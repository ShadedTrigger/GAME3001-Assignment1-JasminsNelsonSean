#include "SpaceShip.h""
#include "Game.h"
#include "Util.h"
#include "SoundManager.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance()->load("../Assets/textures/spaceship.png", "spaceship");

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SPACE_SHIP);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
	setState("Seek");

	SoundManager::Instance().load("../Assets/audio/byebye.mp3", "byebye", SOUND_SFX);
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	TextureManager::Instance()->draw("spaceship",
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * 60.0f);
}

void SpaceShip::update()
{
	m_Move();
	if (((getTransform()->position.x >= 800 && getTransform()->position.x <= 802)
		|| (getTransform()->position.y >= 600 && getTransform()->position.y <= 602)
		|| (getTransform()->position.x <= 0 && getTransform()->position.x >= -2)
		|| (getTransform()->position.y <= 0 && getTransform()->position.y >= -2))
		&& getState() == "Flee")
	{
		SoundManager::Instance().playSound("byebye", 0, -1);
	}
}

void SpaceShip::clean()
{
}

void SpaceShip::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 SpaceShip::getOrientation()
{
	return m_orientation;
}

void SpaceShip::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

void SpaceShip::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

void SpaceShip::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

std::string SpaceShip::getState()
{
	return currentState;
}

float SpaceShip::getRotation() const
{
	return m_rotationAngle;
}

void SpaceShip::setState(const std::string state)
{
	currentState = state;
}

void SpaceShip::m_Move()
{
	const auto deltaTime = TheGame::Instance()->getDeltaTime();

	if (getState() == "Flee")
	{
		// direction with magnitude
		m_targetDirection = -(m_destination - getTransform()->position);
	}
	else
	{
		// direction with magnitude
		m_targetDirection = m_destination - getTransform()->position;
	}
	

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	const auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	const auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime) + 
		0.5f * getRigidBody()->acceleration * (deltaTime);

	// Clamp m_maxSpeed depending on distance to target
	if (m_destination.x - getTransform()->position.x <= 10.0f && m_destination.y - getTransform()->position.y <= 10.0f && getState() == "Arrival")
	{
		m_maxSpeed = Util::clamp(m_maxSpeed, 0.0f, 0.0f);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	}
	else if (m_destination.x - getTransform()->position.x <= 75.0f && m_destination.y - getTransform()->position.y <= 75.0f && getState() == "Arrival")
	{
		m_maxSpeed = Util::clamp(m_maxSpeed, 2.0f, 2.0f);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	}
	else if (m_destination.x - getTransform()->position.x <= 150.0f && m_destination.y - getTransform()->position.y <= 150.0f && getState() == "Arrival")
	{
		m_maxSpeed = Util::clamp(m_maxSpeed, 4.0f, 4.0f);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	}
	else
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}