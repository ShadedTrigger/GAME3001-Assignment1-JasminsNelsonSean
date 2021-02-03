#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	CollisionManager::AABBCheck(m_pSpaceShip, m_pTarget);
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	// Seeking
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		// Reset in case switching from other behaviour
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set angle to 0 deg
		m_pSpaceShip->setTurnRate(10.0f);
		m_pSpaceShip->setAccelerationRate(10.0f);
		m_pSpaceShip->setMaxSpeed(10.0f);

		m_pSpaceShip->setState("Seek");
		m_pSpaceShip->setEnabled(true);
		m_pTarget->setEnabled(true);
		m_pSpaceShip->getTransform()->position = glm::vec2(-50.0f, 200.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pTarget->getTransform()->position = glm::vec2(400.0f, 300.0f);
	}

	// Fleeing
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		// Reset in case switching from other behaviour
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set angle to 0 deg
		m_pSpaceShip->setTurnRate(10.0f);
		m_pSpaceShip->setAccelerationRate(10.0f);
		m_pSpaceShip->setMaxSpeed(4.0f);

		m_pSpaceShip->setState("Flee");
		m_pSpaceShip->setEnabled(true);
		m_pTarget->setEnabled(true);
		m_pSpaceShip->getTransform()->position = glm::vec2(500.0f, 200.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	}

	// Arrival
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
	{
		// Reset in case switching from other behaviour
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set angle to 0 deg
		m_pSpaceShip->setTurnRate(10.0f);
		m_pSpaceShip->setAccelerationRate(10.0f);
		m_pSpaceShip->setMaxSpeed(7.5f);

		m_pSpaceShip->setState("Arrival");
		m_pSpaceShip->setEnabled(true);
		m_pTarget->setEnabled(true);
		m_pSpaceShip->getTransform()->position = glm::vec2(-50.0f, 200.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pTarget->getTransform()->position = glm::vec2(400.0f, 300.0f);
	}

	// Obstacle Avoidance
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4))
	{

	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Music
	SoundManager::Instance().load("../Assets/audio/TokyoDrift.ogg", "tokyodrift", SOUND_MUSIC);
	SoundManager::Instance().playMusic("tokyodrift");
	SoundManager::Instance().setMusicVolume(32);

	m_pSeekingLabel = new Label("Press 1 for Seeking", "Consolas", 20, { 0, 0, 0, 255 }, glm::vec2(400.0f, 20.0f));
	m_pSeekingLabel->setParent(this);
	addChild(m_pSeekingLabel);

	m_pFleeingLabel = new Label("Press 2 for Fleeing", "Consolas", 20, { 0, 0, 0, 255 }, glm::vec2(400.0f, 40.0f));
	m_pFleeingLabel->setParent(this);
	addChild(m_pFleeingLabel);

	m_pArrivalLabel = new Label("Press 3 for Arrival", "Consolas", 20, { 0, 0, 0, 255 }, glm::vec2(400.0f, 60.0f));
	m_pArrivalLabel->setParent(this);
	addChild(m_pArrivalLabel);

	m_pAvoidLabel = new Label("Press 4 for Obstacle Avoidance", "Consolas", 15, { 0, 0, 0, 255 }, glm::vec2(400.0f, 80.0f));
	m_pAvoidLabel->setParent(this);
	addChild(m_pAvoidLabel);

	// Create target, obstacle, and spaceship, but disabled.
	
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	m_pTarget->setEnabled(false);
	addChild(m_pTarget);
	
	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(500.0f, 300.0f);
	m_pObstacle->setEnabled(false);
	addChild(m_pObstacle);
	
	// Instantiate spaceship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->setEnabled(false);
	addChild(m_pSpaceShip);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Super Secret Settings Spectacle", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSpaceShip->getRotation();
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}

	if(ImGui::Button("Start"))
	{
		m_pSpaceShip->setEnabled(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set angle to 0 deg
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pSpaceShip->getRotation();
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
