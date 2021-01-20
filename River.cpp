#include "River.h"
namespace Mer
{
	River::River()
	{

	}
	River::River(std::vector<glm::vec3> coords, int id, float width, float increment)
	{
		this->coords = coords;
		this->id = id;
		this->width = width;
		this->increment = increment;
	}
	void River::AddCoords(float x, float y)
	{
		this->coords.push_back({ x, y, 1.0f });
	}
	void River::NormaliseCoords(float xDiff, float yDiff, float xEdge, float yEdge)
	{
		for (int i = 0; i < coords.size(); i++)
		{
			coords[i].x = ((coords[i].x - xDiff) / xEdge) * -1;//all cells are scaled using the same numbers
			coords[i].y = ((coords[i].y - yDiff) / yEdge) * -1;//the diff variables are used to centre the map
															   //the edge variables are used to scale down the coords to all fit in -1 -> 1	
		}
	}
}