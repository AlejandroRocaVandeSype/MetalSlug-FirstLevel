#pragma once
#include "GameObject.h"

class Level;
class Shield final : public GameObject
{
public:

	Shield( const Point2f& startPos );
	~Shield();
	Shield(const Shield & shield) = delete;
	Shield(Shield && shield) = delete;
	Shield& operator=(const Shield & shield) = delete;
	Shield& operator=(Shield && shield) = delete;


	virtual void Draw() const override;
	virtual void CheckGameState() override;
	virtual void Hit() override;
	
	void Update(float elapsedSec, const Level* level);

private:

	virtual void Initialize() override;

	const Point2f m_ImpactSpeed;				// Speed at which the shield will fly when it is destroyed
	bool m_Impact;							// Shield destroyed
}; 

