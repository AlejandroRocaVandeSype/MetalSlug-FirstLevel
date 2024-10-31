#pragma once

#include "GameObject.h"

#include "Item.h"


class Texture;
class Level;
class BulletManager;
class Item;
class SoundEffect;
class Avatar final : public GameObject
{
public:
	
	// All animations --> To keep track of changing animations
	enum class Animations { 
		iddle, 
		movingRight, 
		movingLeft, 
		jumping, 
		crawlingIddle, 
		crawlingLeft, 
		crawlingRight, 
		crawlingShooting, 
		shooting, 
		pointingUp, 
		shootingUp,
		death,
		respawn
	};

	// Action State for the Top sprite
	enum class TopActionState {   
		iddle,
		crawling,		// Top sprite not being used for this state
		jumping,
		shooting,
		pointingUp,
		shootingUp,
		death,
		respawn
	};

	// Action State for the Bottom sprite
	enum class BotActionState {   
		iddle,
		crawling,		
		jumping,
		shooting,		// Only when crawling this state happens
		death,
		respawn
	};

	Avatar(SoundManager* sounds);
	Avatar(const Avatar& avatar) = delete;
	Avatar(Avatar&& avatar) = delete;
	Avatar& operator=(const Avatar& avatar) = delete;
	Avatar& operator=(Avatar&& avatar) = delete;
	~Avatar();

	virtual void Draw() const override;
	virtual void Hit() override;
	virtual void CheckGameState() override;

	void Shoot();
	void Update(float elapsedSeconds, const Level* level, const Rectf& cameraPos);
	void GrabItem(Item::ItemType itemType);

	// Getters
	const bool GetIsMovingRight() const;
	const Animations GetActiveAnimation() const;
	BulletManager* GetBullets();
	Rectf GetHitBox() const;
	int GetNrLifes() const;
	const bool GetAvatarAlive() const;
	unsigned int GetTotalPoints() const;

	void SetVerticesLevel(std::vector<Point2f> vertices);
	void AddItemPoints(const unsigned int points);

private:

	// Camera
	Rectf m_CameraPos;
	
	// BulletManager

	const int m_NrOfBullets;
	BulletManager* m_pBulletManager;


	// Sprite Related

	TopActionState m_TopActionState;				// Current action state for the Top sprite
	BotActionState m_BotActionState;
	Animations m_ActTopAnimation;
	Animations m_ActBotAnimation;
	Animations m_PrevTopAnimation;					// To keep track if the animation changed
	Animations m_PrevBotAnimation;
	float m_Offset;									// To recolocate the top Sprite when changing sprites

	// Physics variables
						
	const float m_SlowSpeed;
	const float m_JumpSpeed;
	const float m_NormalSpeed;
	
	// Booleans

	bool m_IsMovingRight;							// Moving right or not (for OpenGL)
	bool m_IsMoving;								// Indicates if character is moving or not (for sprites)				
	bool m_IsOnGround;								// Avatar is in the ground or not
	bool m_IsShooting;
	

	// Death
	int m_NrLifes;
	bool m_AreLifesLeft;
	bool m_IsImmortal;								// To know if the avatar should avoid all damage
	bool m_IsBlink;									// Drawing of the avatar needs to blink when Immortal
	const float m_MaxTimeRespawn;
	const float m_MaxTimeImmortal;
	float m_SecondsRespawn;
	float m_SecondsImmortal;
	int m_CountNrFrames;								//  Every certain number of frames we do a blink in drawing

	// Sounds

	SoundEffect* m_pDeathSound;
	SoundEffect* m_pPistolFire;				// Shoot pistol effect
	

	// PRIVATE functions

	virtual void Initialize() override;

	void Respawn(float elapsedSec);
	void AvoidDamage(float elapsedSec);
	void DoBlink(float elapsedSec);

	void CheckCameraBoundaries();

	// Sprite related
	void UpdateTopSrcRect();
	void UpdateBotSrcRect();
	void UpdateFrames(float elapsedSeconds);
	void ResetSprite(Sprite* sprite, bool topSprite);
	void CorrectTopSprite();

	// Checkers
	void CheckPreviousState();
	void CheckCrawling();
	void CheckShooting();
	void CheckFlipSprite() const;
	

	// Input and actions
	void HandleInput();
	void Move(float elapsedSec);
	void AvatarFalling(float elapsedSec);

	void DrawAvatar() const;

	void InitBulletManager();

};

