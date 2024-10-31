#include "pch.h"
#include "GameObjectManager.h"
#include "Helicopter.h"
#include "Soldier.h"
#include "SoldierShield.h"
#include "Prisoner.h"
#include "Avatar.h"
#include "Item.h"
#include "utils.h"
#include "Boss.h"
#include "SoundEffect.h"
#include <iostream>



GameObjectManager::GameObjectManager(SoundManager* sounds)
	:m_pGameObjects{ }
	, m_TotalNrHelicopters{ }
	, m_TotalNrPrisoners{ }
	, m_TotalNrItems{ }
	, m_ActivateGameObjectDist { 45.f }
	, m_pSoundManager { sounds }
	, m_pHelixEffect { }
{

}


GameObjectManager::~GameObjectManager()
{
	if (!m_pGameObjects.empty())
	{
		for (GameObject* pGameObject : m_pGameObjects)
		{
			delete pGameObject;
		}

		m_pGameObjects.clear();
	}
}



void GameObjectManager::Draw() const
{
	if (!m_pGameObjects.empty())
	{
		for (GameObject* pGameObject : m_pGameObjects)
		{
			if (pGameObject->GetIsActive() && pGameObject->GetType() != GameObject::Type::boss)
			{
				pGameObject->Draw();
			}
		}
	}
}

// In order to draw the boss in front of the level
void GameObjectManager::DrawBoss() const
{
	if (!m_pGameObjects.empty())
	{
		for (GameObject* pGameObject : m_pGameObjects)
		{
			if (pGameObject->GetType() == GameObject::Type::boss && pGameObject->GetIsActive())
			{
				pGameObject->Draw();
			}
		}
	}
	
}

void GameObjectManager::Update(float elapsedSec, Avatar* avatar, const Level* level, const Rectf& cameraPos)
{
	if (!m_pGameObjects.empty())
	{
		CheckPosCamera(cameraPos);

		for (GameObject* pGameObject : m_pGameObjects)
		{
			if (pGameObject->GetIsActive())
			{
				switch (pGameObject->GetType())
				{
					case GameObject::Type::helicopter:
					case GameObject::Type::soldier:
					case GameObject::Type::soldierShield:
					case GameObject::Type::prisoner:
					case GameObject::Type::boss:
					{
						Enemy* pEnemy{ dynamic_cast<Enemy*>(pGameObject) };
						if (pEnemy != nullptr)
						{
							pEnemy->Update(elapsedSec, avatar, level);
						}

						break;
					}
					case GameObject::Type::item:
					{
						Item* pItem{ dynamic_cast<Item*>(pGameObject) };
						if (pItem != nullptr)
						{
							pItem->Update(elapsedSec, level);
						}
						break;
					}
				}
			}
	
		}

		
		if (m_TotalNrHelicopters > 0)   // Still helicopters left
		{
			ActivateHelicopter();
		}

		if (m_TotalNrPrisoners > 0)   // Still prisoners left
		{
			AvatarReleasePrisoner(avatar->GetHitBox());
		}

		if (m_TotalNrItems > 0)
		{
			GrabItem(avatar);
		}

		CheckBulletsBoss(avatar);

		// Erase GameObject from the vector if it is Dead
		RemoveGameObject();
	}

}

// Check if any of the bullets from the Boss hit the avatar
void GameObjectManager::CheckBulletsBoss(Avatar* avatar)
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		if (pGameObject->GetType() == GameObject::Type::boss)
		{
			if (pGameObject->GetIsActive())
			{
				Boss* pBoss{ dynamic_cast<Boss*>(pGameObject) };
				if (pBoss != nullptr)
				{
					pBoss->CheckHitBullet(avatar);
				}
			}
		}
	}
}
// Activate the gameObject depending on the distance of the camera
void GameObjectManager::CheckPosCamera(const Rectf& cameraPos)
{

	for (GameObject* pGameObject : m_pGameObjects)
	{
		if (pGameObject->GetType() != GameObject::Type::item
				&& pGameObject->GetType() != GameObject::Type::helicopter)
		{
			if (!pGameObject->GetIsActive())  // Not activated yet
			{
				if (pGameObject->GetIsComingFromRight())
				{
					// GameObject appears from the right of the window
					if (utils::GetDistanceX(pGameObject->GetBotShape().left, cameraPos.left + cameraPos.width)
						< m_ActivateGameObjectDist)
					{
						// Activate GameObject
						pGameObject->SetIsActive(true);
					}
					else
					{
						if (pGameObject->GetBotShape().left > cameraPos.left &&
							pGameObject->GetBotShape().left < cameraPos.left + cameraPos.width)
						{
							// GameObject already inside window 
							pGameObject->SetIsActive(true);
						}
					}
				}
				else
				{
					// GameObject appears from the left of the window
					if (pGameObject->GetBotShape().left < cameraPos.left)
					{
						if (utils::GetDistanceX(pGameObject->GetBotShape().left, cameraPos.left)
							> m_ActivateGameObjectDist)
						{
							
							// Activate GameObject
							pGameObject->SetIsActive(true);
						}
					}
				}
				
			}
		}

	}


}

void GameObjectManager::AddGameObject(const Point2f& startPos, const GameObject::Type& type, bool comingFromRight)
{
	GameObject* gameObject{ };
	GameObject* item{ };

	switch (type)
	{
	case GameObject::Type::helicopter:

		gameObject = new Helicopter(startPos, m_pSoundManager);
		m_TotalNrHelicopters++;

		break;

	case GameObject::Type::soldier:

		gameObject = new Soldier(startPos, comingFromRight, m_pSoundManager);

		break;

	case GameObject::Type::soldierShield:

		gameObject = new SoldierShield(startPos, comingFromRight, m_pSoundManager);

		break;

	case GameObject::Type::boss:

		gameObject = new Boss(startPos, comingFromRight, m_pSoundManager);

		break;

	case GameObject::Type::prisoner:

		gameObject = new Prisoner(startPos, m_pSoundManager);

		m_TotalNrPrisoners++;

		// Create also an Item next to the position of the prisoner (doesn't draw it yet)
		Point2f starPosItem = SetStartPosItem(gameObject->GetBotShape());
		item = new Item(starPosItem, m_pSoundManager);
		m_TotalNrItems++;

		break;

	
	}

	m_pGameObjects.push_back(gameObject);

	if (type == GameObject::Type::prisoner)
	{
		// Item will be positioned in the vector right after the prisoner
		m_pGameObjects.push_back(item);
	}
}

// Free memory for those game objects that are dead
void GameObjectManager::RemoveGameObject()
{
	for (int i{ }; i < m_pGameObjects.size(); i++)
	{
		if (m_pGameObjects[i]->GetIsDead())   // Enemy dead
		{
			if (m_pGameObjects[i]->GetType() == GameObject::Type::helicopter)
			{
				// If helicopter is dead --> Stop his sound effect
				m_pHelixEffect->StopAll();
			}
	
			DecrementTotalNrGameObject(m_pGameObjects[i]->GetType());  // Reduce counter of gameObject
			delete m_pGameObjects[i];
			m_pGameObjects.erase(m_pGameObjects.begin() + i);
		}
	}
}


void GameObjectManager::GrabItem(Avatar* avatar)
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		if (pGameObject->GetType() == GameObject::Type::item)
		{
			if (pGameObject->GetIsActive())
			{
				Item* pItem{ dynamic_cast<Item*>(pGameObject) };
				if (pItem != nullptr)
				{
					pItem->GrabItem(avatar);
				}
			}
		}
	}
}



// Activate a helicopter if the the game is in the correct stage
void GameObjectManager::ActivateHelicopter()
{
	if (m_pGameObjects[0]->GetGameStageChanged())  // Stage changed
	{
		if (m_pGameObjects[0]->GetGameStage() == GameObject::GameStage::firstHeliFight
			|| m_pGameObjects[0]->GetGameStage() == GameObject::GameStage::secondHeliFight)
		{
			for (GameObject* pGameObject : m_pGameObjects)
			{
				if (pGameObject->GetType() == GameObject::Type::helicopter)
				{
					// Activate the helicopter
					pGameObject->SetIsActive(true);
					pGameObject->SetGameStageChanged(false);

					// Helix sound for the helicopter
					m_pHelixEffect = m_pSoundManager->GetEffect("Resources/Sounds/Helicopter.wav");
					m_pSoundManager->PlaySoundEffect(m_pHelixEffect, 100);
					break;
				}
			}
		}
	}
}

// Decrement the total Number of the gameobject according with his type
void GameObjectManager::DecrementTotalNrGameObject(const GameObject::Type& type)
{

	switch (type)
	{
		case GameObject::Type::helicopter:
			m_TotalNrHelicopters--;
			break;

		case GameObject::Type::prisoner:
			m_TotalNrPrisoners--;
			break;

		case GameObject::Type::item:
			m_TotalNrItems--;
			break;
	}
}

// Check if avatar overlaps with prisoner to release it
void GameObjectManager::AvatarReleasePrisoner(const Rectf& avatarShape)
{

	for (int i{ }; i < m_pGameObjects.size(); i++)
	{
		if (m_pGameObjects[i]->GetType() == GameObject::Type::prisoner)
		{
			if (m_pGameObjects[i]->GetIsActive())
			{
				Prisoner* pPrisoner{ dynamic_cast<Prisoner*>(m_pGameObjects[i]) };
				if (pPrisoner != nullptr)
				{
					pPrisoner->CheckAvatarRelease(avatarShape);

					if (pPrisoner->GetDropItem())
					{
						if ( m_pGameObjects[i + 1] != nullptr && (i +1) < m_pGameObjects.size() )
						{
							m_pGameObjects[i + 1]->SetIsActive(true);  // Activate the item to show
							pPrisoner->ItemDropped();				// Item Dropped
							
						}
						
					}
					
				}
			}
		}
	}

}

Point2f GameObjectManager::SetStartPosItem(const Rectf& prisonerShape)
{
	Point2f starPos{};

	starPos.x = prisonerShape.left - prisonerShape.width / 2.f;
	starPos.y = prisonerShape.bottom + prisonerShape.height / 3.f;

	return starPos;

}

std::vector <GameObject*> GameObjectManager::GetGameObjects() const
{
	return m_pGameObjects;
}

int GameObjectManager::GetTotalPrisoners() const
{
	return m_TotalNrPrisoners;
}

