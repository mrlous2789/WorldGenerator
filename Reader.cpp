#include "Reader.h"


Reader::Reader()
{

}

bool Reader::ReadFile(std::string filename)
{
	std::fstream file;
	file.open(filename);

	if (file)//if file is open read file
	{
		std::string line;
		std::cout << "File: " + filename + " opened" << std::endl;

		//temp properties
		int id = 0, height = 0, biome = 0, population = 0, state = 0, province = 0, culture = 0, religion = 0;
		std::string type;
		std::vector<int> neighbors;//temp neighbors vector
		std::vector<glm::vec2> coords;//temp coords vectore

		while (std::getline(file, line))
		{
			if (line.find("\"geometry\":") != std::string::npos)
			{
				int start = line.find_first_of('[');//find the start of the coordinates
				std::string coord = "";
				float x = 0.0f, y = 0.0f;
				bool xcoord = true;
				for (int i = start; i < line.size(); i++)
				{
					if (line.at(i) == '[' || line.at(i) == ']')//if a [ or ] do nothing
					{

					}
					else if (line.at(i) == ',')// a ',' seperates each coord so when appears save coord string to variable then empty it
					{
						if (xcoord)
						{
							x = ConvertToDouble(coord);
							xcoord = false;
							coord = "";
						}
						else//if its the y coord then we already have the x coord so save onto temp coords vector
						{
							y = ConvertToDouble(coord);
							coords.push_back({ x,y });
							xcoord = true;
							coord = "";
						}
					}
					else//build up coord
					{
						coord += line.at(i);
					}
				}
			}
			else if (line.find("\"id\":") != std::string::npos) { id = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"height\":") != std::string::npos) { height = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"biome\":") != std::string::npos) { biome = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"type\":") != std::string::npos) { type = GetProperty(line); }
			else if (line.find("\"population\":") != std::string::npos) { population = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"state\":") != std::string::npos) { state = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"province\":") != std::string::npos) { province = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"culture\":") != std::string::npos) { culture = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"religion\":") != std::string::npos) { religion = ConvertToInt(GetProperty(line)); }
			else if (line.find("\"neighbors\":") != std::string::npos)
			{
				int start = line.find_first_of('[');//find start of neighbors ids
				std::string neighbor = "";
				for (int i = start; i < line.size(); i++)
				{
					if (line.at(i) == ',' || line.at(i) == ']')// a ',' seperates each id so save id to neighbors vector and a ']' is for the last id
					{
						neighbors.push_back(ConvertToInt(neighbor));
						neighbor = "";
					}
					else if (line.at(i) == '[')//do nothing
					{

					}
					else//build up id
					{
						neighbor += line.at(i);
					}
				}
			}
			else if (line == "},")//save to cells vector
			{
				cells.push_back(Cell(coords, id, height, biome, type, population, state, province, culture, religion, neighbors));//add cell to cells vector
				neighbors.clear();
				coords.clear();
			}


		}
		std::cout << "Done" << std::endl;

		file.close();
		return true;
	}
	else//return error
	{
		std::cout << "File: " + filename + " could not be opened" << std::endl;
		return false;
	}

}

std::pair<int, int> Reader::FindFirstAndLast(std::string line)//finding the position of the data in string
{
	int last = line.find_last_of("\"");
	int first = 0;

	for (int i = (last - 1); i > 0; i--)
	{
		if (line.at(i) == '\"')
		{
			first = i + 1;
			break;
		}
	}
	return std::pair<int, int>(first, last);
}

std::string Reader::GetProperty(std::string line)//isolate the data that we actually want essentially just removing ""
{
	std::pair<int, int> firstLast = FindFirstAndLast(line);
	std::string propertyString = line.substr(firstLast.first, firstLast.second - firstLast.first);
	return propertyString;
}

int Reader::ConvertToInt(std::string line)
{
	try//try converting data to integer
	{
		int prop = std::stoi(line);
		return prop;
	}
	catch (const std::exception&)
	{
		std::cout << "Error converting: " << line << " to int" << std::endl;
		return 0;
	}
}
float Reader::ConvertToDouble(std::string line)
{
	try//try converting data to double
	{
		double prop = std::stof(line);
		return prop;
	}
	catch (const std::exception&)
	{
		std::cout << "Error converting: " << line << " to double" << std::endl;
		return 0.0f;
	}
}
void Reader::PrintData()
{
	std::cout << "Cell count: " << cells.size() << std::endl;
	cells[5].PrintProperties();
	cells[19].PrintProperties();
	cells[456].PrintProperties();
}
void Reader::PrintDataByID(int id)//cells are sorted and ids increment by one every time so finding data is very quick and simple
{
	if (id <= cells.size())
	{
		if (cells[id].id == id)
		{
			cells[id].PrintProperties();
		}
	}
	else
	{
		std::cout << "Doesnt exist" << std::endl;
	}
}