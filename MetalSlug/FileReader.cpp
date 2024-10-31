#include "pch.h"
#include "FileReader.h"
#include "GameObjectManager.h"
#include <iostream>
#include "sstream"


FileReader::FileReader(std::string& filename)
	: m_Filename{filename}
{

}


void FileReader::CreateGameObjects(GameObjectManager* gameObjectManager, char delim)
{
	std::ifstream is(m_Filename);
	std::string line{};

	if (!is)
	{
		std::cerr << "Error opening the file " << m_Filename << std::endl;
	}

	while (std::getline(is, line, delim))  // Get characters until delim
	{
		CreateGameObject(line, gameObjectManager);
	}

}

// Create a GameObject from a string 
void FileReader::CreateGameObject(std::string& gameObject, GameObjectManager* gameObjectManager)
{
	GameObject::Type typeGameObject{};
	bool isFound{ false };

	if (gameObject.find("Helicopter") != std::string::npos)
	{
		// Helicopter
		typeGameObject = GameObject::Type::helicopter;
		isFound = true;
	}
	else if (gameObject.find("Soldier") != std::string::npos)
	{
		// Normal Soldier
		typeGameObject = GameObject::Type::soldier;
		isFound = true;
	}
	else if (gameObject.find("Shield") != std::string::npos)
	{
		// Soldier with shield
		typeGameObject = GameObject::Type::soldierShield;
		isFound = true;
	}
	else if (gameObject.find("Prisoner") != std::string::npos)
	{
		// Prisoner
		typeGameObject = GameObject::Type::prisoner;
		isFound = true;
	}
	else if (gameObject.find("Boss") != std::string::npos)
	{
		// Boss
		typeGameObject = GameObject::Type::boss;
		isFound = true;
	}

	if (isFound)
	{
		Point2f startPos{ ToPoint2f(GetAttributeValue("Position", gameObject)) };
		bool isComingFromRight{ ToBool(GetAttributeValue("ComingFromRight", gameObject)) };

		gameObjectManager->AddGameObject(Point2f{ startPos.x * g_Scale, startPos.y }, typeGameObject, isComingFromRight);
	}

}

// Get the value from the attribute
std::string FileReader::GetAttributeValue(const std::string& attrName, const std::string& element) const
{
	std::string attribute;
	std::string findAttr{ attrName };
	std::stringstream valuesStream;
	char delim{ '"' };
	const int lineOffset{ 2 };						// To get the line starting in the correct pos (avoiding the "=")

	size_t found{ element.find(findAttr) };				// Find the attribute name in the string
	valuesStream.str(element.substr(found + attrName.length() + lineOffset)); // Get a string starting with the pos with the value(s)

	std::getline(valuesStream, attribute, delim);		// Get all characters from the beggining until the delim

	return attribute;
}

// Convert from string to Point2f
Point2f FileReader::ToPoint2f(const std::string& point2fStr) const
{
	std::stringstream point2fStream;

	point2fStream.str(point2fStr);  // Copy content of the string into the stringstream

	std::string pointValue{ };
	char delim{ ',' };
	Point2f point;

	std::getline(point2fStream, pointValue, delim);   // Get a line from the stream object (stops when find delim char)
													  // and store it into pointValue string
	point.x = std::stof(pointValue);

	std::getline(point2fStream, pointValue);		// Continues from where it was

	point.y = std::stof(pointValue);

	return point;
}

// Convert from string to bool
bool FileReader::ToBool(const std::string& boolStr) const
{
	std::string value{ boolStr.substr(0) };  // Get from first pos to the end of the string
	bool boolean;

	if (value == "false")
	{
		boolean = 0;
	}
	else
	{
		boolean = 1;
	}

	return boolean;
}