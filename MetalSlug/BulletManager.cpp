#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "utils.h"
#include "GameObject.h"
#include "Enemy.h"
#include <iostream>


BulletManager::BulletManager()
	: m_pBullets{ }
	, m_VerticesLevel{ }
	, m_TotalPoints{ }
{

}



BulletManager::~BulletManager()
{

	for (Bullet* ptr : m_pBullets)
	{
		delete ptr;
	}

	m_pBullets.clear();  

}

void BulletManager::Draw() const
{
	for (Bullet* ptr : m_pBullets)
	{
		if (ptr->GetIsActive())
		{
			ptr->Draw();
		}
	}

}
void BulletManager::Update(float elapsedSec, const Avatar* avatar)
{
	for (Bullet* ptr : m_pBullets)
	{
		if (ptr->GetIsActive())
		{
			ptr->Update(elapsedSec, avatar);
		}
		
	}

	CheckHitLevel();
}


void BulletManager::AddBullet(Point2f& startPos)
{

	m_pBullets.push_back( new Bullet(startPos) );

}

void BulletManager::ActivateBullet()
{
	
	for (Bullet* ptr : m_pBullets)
	{
		if (!ptr->GetIsActive())
		{
			// We activate first bullet in the vector thats is not already activated
			ptr->SetIsActive(true);
			break;  
		}

	}

}

void BulletManager::CheckHitLevel()
{
	int randomHit{};
	for (Bullet* ptr : m_pBullets)
	{
		if (ptr->GetIsActive())
		{
			RandomCollision(randomHit);

			Point2f ray{ ptr->GetTopShape().left,  ptr->GetTopShape().bottom + ptr->GetTopShape().height / 2};
			Point2f rayEnd{ ptr->GetTopShape().left + ptr->GetTopShape().width + float(randomHit),  ptr->GetTopShape().bottom + ptr->GetBotShape().height / 2};

			utils::HitInfo hitInfo{};

			if (utils::Raycast(m_VerticesLevel, ray, rayEnd, hitInfo))
			{
				if (rayEnd.x >= hitInfo.intersectPoint.x)
				{
					ptr->Hit();
				}
			}
		}
	}
}

// Check collision of the bullet with the enemies if they are activated
void BulletManager::CheckHitGameObjects(std::vector<GameObject*> gameObjects)
{
	for (Bullet* bulletPtr : m_pBullets)
	{
		if (bulletPtr->GetIsActive() && !bulletPtr->GetIsHit())
		{
			for (GameObject* pGameObject : gameObjects)
			{
				// Object active and not dying
				if (pGameObject->GetIsActive() && !pGameObject->GetIsDying())  
				{
					if (pGameObject->GetType() == GameObject::Type::boss)
					{
						if (CheckHit(bulletPtr, pGameObject, true))
						{
							break;  // Dont check for the rest of GameObjects
						}
					}
					else if(pGameObject->GetType() != GameObject::Type::item)
					{
						if (CheckHit(bulletPtr, pGameObject, false))
						{
							break;  
						}
					}

				}
			}
		}
	}

}

bool BulletManager::CheckHit(Bullet* bullet, GameObject* gameObject, bool isBoss)
{
	float XFit{ };
	if (isBoss)
	{
		XFit = 85.f;  // We adjust the bounding boxes of the Boss
	}

	// X axis
	if ( (bullet->GetTopShape().left) > gameObject->GetBotShape().left + XFit 
		&& bullet->GetTopShape().left < ( ( gameObject->GetBotShape().left + XFit ) +
			gameObject->GetBotShape().width ) )
	{
		// Y axis
		if (bullet->GetTopShape().bottom > gameObject->GetBotShape().bottom &&
			bullet->GetTopShape().bottom < (gameObject->GetBotShape().bottom +
				gameObject->GetBotShape().height))
		{

			bullet->Hit();
			gameObject->Hit();
			AddHitPoints(gameObject->GetPointsGameObject());

			return true;  // Bullet hit a GameObject

		}
	}

	return false;  // No hit
}

void BulletManager::AddHitPoints(const unsigned int points)
{
	m_TotalPoints += points;
}

void BulletManager::AddPoints(const unsigned int points)
{
	m_TotalPoints += points;
}

// Calculate a random value for the hit collision of the bullet
void BulletManager::RandomCollision(int& random)
{
	random = (rand() % 30) - 30; // Rand [-30, -1]
	random += (rand() % 10) + 1; // Rand [-30, 10]
}

void BulletManager::SetVerticesLevel(std::vector<Point2f> vertices)
{
	m_VerticesLevel = vertices;
}


int BulletManager::GetTotalPoints() const
{
	return m_TotalPoints;
}