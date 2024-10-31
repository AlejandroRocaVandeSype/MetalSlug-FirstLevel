#pragma once
#include "Enemy.h"

class Avatar;
class Level;
class GameObjectManager final
{

public:

	GameObjectManager(SoundManager* sounds);
	~GameObjectManager();
	GameObjectManager(const GameObjectManager& gameObjectManager) = delete;
	GameObjectManager(GameObjectManager&& GameObjectManager) = delete;
	GameObjectManager& operator=(const GameObjectManager& GameObjectManager) = delete;
	GameObjectManager& operator=(GameObjectManager&& GameObjectManager) = delete;


	void Draw() const;
	void DrawBoss() const;
	void Update(float elapsedSec, Avatar* avatar, const Level* level, const Rectf& cameraPos);

	void AddGameObject(const Point2f& startPos, const GameObject::Type& type, bool comingFromRight);


	std::vector <GameObject*> GetGameObjects() const;
	int GetTotalPrisoners() const;


private:

	std::vector <GameObject*> m_pGameObjects;
	int m_TotalNrHelicopters;
	int m_TotalNrPrisoners;
	int m_TotalNrItems;
	Rectf m_CameraPos;
	const float m_ActivateGameObjectDist;

	// Sounds for the GameObjects
	SoundManager* m_pSoundManager;
	SoundEffect* m_pHelixEffect;

	void ActivateHelicopter();
	void RemoveGameObject();
	void DecrementTotalNrGameObject(const GameObject::Type& type);
	void AvatarReleasePrisoner(const Rectf& avatarShape);
	void GrabItem(Avatar* avatar);
	Point2f SetStartPosItem(const Rectf& prisonerShape);
	void CheckPosCamera(const Rectf& cameraPos);
	void CheckBulletsBoss(Avatar* avatar);
	


};

