#pragma once
#include <vector>

class Bullet;
class Avatar;
class Enemy;
class GameObject;
class BulletManager final
{
public:

	BulletManager();
	BulletManager(const BulletManager& bulletManager) = delete;
	BulletManager(BulletManager&& bulletManager) = delete;
	BulletManager& operator=(const BulletManager& bulletManager) = delete;
	BulletManager& operator=(BulletManager&& bulletManager) = delete;
	~BulletManager();

	void Update(float elapsedSec, const Avatar* avatar);
	void Draw() const;

	void AddBullet(Point2f& startPos);
	void ActivateBullet();

	void CheckHitLevel();
	void CheckHitGameObjects(std::vector<GameObject*> gameObjects);

	int GetTotalPoints() const;

	void SetVerticesLevel(std::vector<Point2f> vertices);
	void AddPoints(const unsigned int points);


private:

	std::vector<Bullet*> m_pBullets;
	std::vector<Point2f> m_VerticesLevel;

	unsigned int m_TotalPoints;					// record of the player total points

	void RandomCollision(int& random);
	void AddHitPoints(const unsigned int hitPoints);
	bool CheckHit(Bullet* bullet, GameObject* gameObject, bool isBoss);
	

};

