#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Cell.h"
#include "Nation.h"
#include "Culture.h"
#include "Religion.h"

namespace Mer
{
	class Reader
	{
	public:
		Reader();



		std::vector<Cell> ReadCellFile(std::string filename);
		std::vector<Nation> ReadNationFile(std::string filename);
		std::vector<Culture> ReadCutlureFile(std::string filename);
		std::vector<Religion> ReadReligionFile(std::string filename);

		//std::vector<Cell> cells;
	private:
		bool reading = true;



		std::pair<int, int>FindFirstAndLast(std::string line);
		std::string GetProperty(std::string line);
		int ConvertToInt(std::string property);
		float ConvertToFloat(std::string property);

		float highestX = 0.0f;
		float lowestX = 0.0f;
		float highestY = -10.0f;//is -10.0f because one of my test files didint have any coords above 0
		float lowestY = 0.0f;

		void NormaliseCells(std::vector<Cell>* cells);

		void FindLowestAndHightest(std::vector<Cell> cells);

	};
}
