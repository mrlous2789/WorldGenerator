#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Cell.h"


class Reader
{
public:
	Reader();

	bool ReadFile(std::string filename);

	void PrintData();

	void PrintDataByID(int id);
	std::vector<Cell> cells;
private:
	bool reading = true;

	

	std::pair<int, int>FindFirstAndLast(std::string line);
	std::string GetProperty(std::string line);
	int ConvertToInt(std::string property);
	float ConvertToDouble(std::string property);


};