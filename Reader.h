#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Cell.h"

namespace Mer
{
	class Reader
	{
	public:
		Reader();

		std::vector<Cell> ReadCellFile(std::string filename);

		//std::vector<Cell> cells;
	private:
		bool reading = true;



		std::pair<int, int>FindFirstAndLast(std::string line);
		std::string GetProperty(std::string line);
		int ConvertToInt(std::string property);
		float ConvertToDouble(std::string property);

		float highestX = 0.0f;
		float lowestX = 0.0f;
		float highestY = -10.0f;
		float lowestY = 0.0f;

		void NormaliseCells(std::vector<Cell> cells);

		void FindLowestAndHightest(std::vector<Cell> cells);

	};
}
