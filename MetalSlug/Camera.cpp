#include "pch.h"
#include "Camera.h"


Camera::Camera(float width, float height)
	:m_Width(width)
	, m_Height(height)
	, m_LevelBoundaries{ 0, 0, width, height }
	, m_CameraPos{ }
	, m_PreviousXPos{ }
	, m_AvatarWidth { 35.f }
{


}

// Change the camera position to the new position calculated by Clamp and Track
void Camera::Transform(const Rectf& target, const GameObject::GameStage& gameStage)
{

	Track(target, gameStage);

	Clamp();

	glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0.f);
}

// Correct camera pos so it doesn't leave the boundaries
void Camera::Clamp()
{
	if (m_LevelBoundaries.left > m_CameraPos.x)
	{
		m_CameraPos.x = 0;
	}

	if ((m_LevelBoundaries.left + m_LevelBoundaries.width) < (m_CameraPos.x + m_Width))
	{
		m_CameraPos.x = m_LevelBoundaries.width - m_Width;
	}


	if ((m_LevelBoundaries.bottom + m_LevelBoundaries.height) < (m_CameraPos.y + m_Height))
	{
		m_CameraPos.y = m_LevelBoundaries.height - m_Height;
	}



	if (m_LevelBoundaries.bottom > m_CameraPos.y)
	{
		m_CameraPos.y = m_LevelBoundaries.bottom;

	}

}

// Keep track of the pos of the Avatar
// Camera is center in X pos but not in Y in order to not exit the boundaries of the level
void Camera::Track(const Rectf target, const GameObject::GameStage& gameStage)
{

	if (gameStage == GameObject::GameStage::moving)
	{
		// Only move camera when State of the game is moving
		m_CameraPos.x = (target.left + m_AvatarWidth / 2) - (m_Width / 2);
		m_CameraPos.y = (target.bottom + target.height / 2) - (m_Height / 4.5f);

	}
	
	
}


void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

const Rectf Camera::GetCameraPos() const
{
	return Rectf{m_CameraPos.x, m_CameraPos.y, m_Width, m_Height};
}
