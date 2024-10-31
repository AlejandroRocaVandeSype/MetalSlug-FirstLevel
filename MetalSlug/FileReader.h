#pragma once

#include <fstream>

class GameObjectManager;
class FileReader final
{
public:

	FileReader(std::string& filename);
	~FileReader() = default;

	void CreateGameObjects(GameObjectManager* gameObjectManager, char delim);

private:

	std::string m_Filename;


	void CreateGameObject(std::string& gameObject, GameObjectManager* gameObjectManager);
	std::string GetAttributeValue(const std::string& name, const std::string& shape) const;
	Point2f ToPoint2f(const std::string& point) const;
	bool ToBool(const std::string& boolString) const;
	
};

