#pragma once
#include <string>
#include "Texture.h"

class Sprite final
{
public:

	Sprite( const std::string& filename, int nrCols = 1, int nrRows = 1, float frameSec = 0, float scale = g_Scale );
	Sprite(const Sprite& spr1);
	Sprite(Sprite&& spr1);
	~Sprite( );
	Sprite& operator=(const Sprite& spr1);
	Sprite& operator= (Sprite&& spr1);

	void Update( float elapsedSec, bool repeat );
	void Draw( ) const;

	void UpdateLeftSrcRect( );
	void UpdateBottomSrcRect( );
	void UpdateValues(int cols, int rows, int frames, float frameSec,
		float width, float height, float spriteSheetTop, float scale = 2.7f);

	void FlipSprite() const;
	void ScaleSprite(float value) const;
	void RotateSprite(float angle) const;
	void ResetActFrame();
	void ResetSprite();
	void ChangeFrame( int frame );

	// Getters
	float GetFrameWidth( ) const;
	float GetFrameHeight( ) const;
	Rectf GetSrcRect( ) const;
	Rectf& GetDstRect( );
	Rectf GetHitBox() const;
	int GetActFrame( ) const;
	int GetTotalFrames() const;
	Texture* GetTexture( ) const;
	bool GetAnimationFinish( ) const;


	// Setters
	void SetDstRect(float x, float y, float width, float height);
	void SetDstRect(float width, float height);
	void SetLeftDstRect(float left);
	void SetBottomDstRect(float bottom);
	void SetSrcRect(float y, float width, float height);
	void SetSrcRect(float y);
	void ResetAnimationFinish(bool reset);
	void SetActive(bool active);
	void SetActFrame(int frame);


private:

	std::string m_Path;
	Texture *m_pTexture;
	Rectf m_SrcRect;
	Rectf m_DstRect;
	int m_Cols;
	int m_Rows;
	int m_ActFrame;
	int m_Frames;
	float m_FrameSec;
	float m_AccuSec; 
	float m_SpriteSheetTop;
	float m_Width;
	float m_Height;
	float m_Scale;
	bool m_IsFinish;		// Animation finished or not
	bool m_IsActive;		// To know if the sprite should draw and update
	
};


