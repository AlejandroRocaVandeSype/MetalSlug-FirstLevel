#pragma once
#include "GameObject.h"

class Camera final
{
public:

	Camera(float width, float height);
	~Camera() = default;
	Camera(const Camera& level) = delete;
	Camera(Camera&& gameObject) = delete;
	Camera& operator=(const Camera& level) = delete;
	Camera& operator=(Camera&& camera) = delete;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target, const GameObject::GameStage& gameStage) ;


	const Rectf GetCameraPos() const;

private:

	const float m_AvatarWidth;
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	Point2f m_CameraPos;
	float m_PreviousXPos;
	

	void Clamp();
	void Track(const Rectf target, const GameObject::GameStage& gameStage);
};

