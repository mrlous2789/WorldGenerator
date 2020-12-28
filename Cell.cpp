#include "Cell.h"

Cell::Cell()
{

}
Cell::Cell(std::vector<glm::vec3> inCoords, int inId, int inHeight, int inBiome, std::string inType, int inPopulation, int inState, int inProvince, int inCulture, int inReligion, std::vector<int> inNeighbors)
{
	coords = inCoords;
	id = inId;
	height = inHeight;
	biome = inBiome;
	type = inType;
	population = inPopulation;
	state = inState;
	province = inProvince;
	culture = inCulture;
	religion = inReligion;
	neighbors = inNeighbors;
}

void Cell::PrintProperties()
{
	std::cout << "Number of Vertices: " << coords.size() << std::endl;
	std::cout << "Coordinates: ";
	for (int i = 0; i < coords.size(); i++)
	{
		std::cout << "[x= " << coords[i].x << ", y= " << coords[i].y << "] ";
	}
	std::cout << std::endl;
	std::cout << "ID: " << id << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Biome: " << biome << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Population: " << population << std::endl;
	std::cout << "State: " << state << std::endl;
	std::cout << "Province: " << province << std::endl;
	std::cout << "Culture: " << culture << std::endl;
	std::cout << "Religion: " << religion << std::endl;
	std::cout << "Neighbors: ";
	for (int i = 0; i < neighbors.size(); i++)
	{
		std::cout << neighbors[i] << ", ";
	}
	std::cout << std::endl;
}

void Cell::NormaliseCoords(float xDiff, float yDiff,float xEdge, float yEdge)
{
	for (int i = 0; i < coords.size(); i++)
	{
		coords[i].x = ((coords[i].x - xDiff) / xEdge) * -1;
		coords[i].y = ((coords[i].y - yDiff) / yEdge) * -1;
	}
}