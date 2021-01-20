#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
namespace Mer
{
	class River
	{
	public:
		River();
		River(std::vector<glm::vec3> coords, int id, float width, float increment);
		
		void AddCoords(float x, float y);
		void NormaliseCoords(float xDiff, float yDiff, float xEdge, float yEdge);

		std::vector<glm::vec3> coords;
		int id = -1;
		float width = 0.0f;
		float increment = 0.0f;

	};
}

