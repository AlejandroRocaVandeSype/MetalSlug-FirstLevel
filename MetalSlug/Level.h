#pragma once
#include "utils.h"

class SVGParser;
class Texture;
class Sprite;
class Avatar;
class Level final
{
public:

	Level();
	~Level();
	Level(const Level& level) = delete;
	Level(Level&& level) = delete;
	Level& operator=(const Level& level) = delete;
	Level& operator=(Level&& level) = delete;


	void DrawBackground() const;
	void DrawForeground() const;
	void Update(float elapsedSec, const Rectf& ActorShape);

	// Collisions with the Level
	void HandleCollision(Rectf& actorShape, Vector2f& velocity) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f& velocity) const;


	Rectf GetBoundaries() const;
	std::vector <Point2f> GetVertices() const;

private:

	// SPRITES
	Sprite* m_pLevelLakeTexture;
	Sprite* m_pLevelTexture;
	
	// Sprites and drawing
	const int m_NrOfPaths;					// Total number of paths from the SVG File
	const int m_Rows;

	// Boundaries of the level
	Rectf m_Boundaries;
	float m_UpperBoundary;						// Upper limit for the level
	float m_BotBoundary;
	const float m_UpperBoundaryValue;
	const float m_BoundariesChangePos;			 // Position from where boundaries will change

	std::vector <std::vector<Point2f>> m_Vertices;
	std::vector<std::vector<Point2f>> m_TransformedVertices;
	SVGParser* m_pLevelSVG;

	
	
	// PRIVATE FUNCTIONS 

	void InitSprites();
	void TransformSVGVertices();

	// Collisions with the Level
	void HorizCollision(Rectf& actorShape, Vector2f& velocity) const;
	void VerticalCollision(Rectf& actorShape, Vector2f& velocity) const;

	void UpdateLevelBoundaries();

};

