#pragma once

#include "GameObject.h"

class Avatar;
class Level;
class SoundManager;
class SoundEffect;
class Item final : public GameObject
{

public:

	enum class ItemType {

		ammo = 0,
		card = 1

	};


	Item(const Point2f& startPos, SoundManager* soundManager);
	~Item();
	Item(const Item & item) = delete;
	Item(Item && item) = delete;
	Item& operator=(const Item & item) = delete;
	Item& operator=(Item && item) = delete;

	virtual void Draw() const override;
	virtual void Hit() override;
	virtual void CheckGameState() override;

	void Update(float elapsedSec, const Level* level);

	void GrabItem(Avatar* avatar);

private:

	ItemType m_ItemType;

	// Sounds
	SoundEffect* m_pGrabItemSound;

	virtual void Initialize() override;
	void SelectTypeItem();

};

