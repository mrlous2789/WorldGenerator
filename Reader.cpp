#include "Reader.h"

namespace Mer
{
	Reader::Reader()
	{

	}
	std::vector<Cell> Reader::ReadCellFile(std::string filename)
	{
		std::vector<Cell> cells;
		std::fstream file;
		file.open(filename);
		cells.clear();


		if (file)//if file is open read file
		{
			std::string line;
			std::cout << "File: " + filename + " opened" << std::endl;

			//temp properties
			int id = 0, height = 0, biome = 0, population = 0, state = 0, province = 0, culture = 0, religion = 0;
			std::string type;
			std::vector<int> neighbors;//temp neighbors vector
			std::vector<glm::vec3> coords;//temp coords vectore

			while (std::getline(file, line))
			{
				if (line.find("\"geometry\":") != std::string::npos)
				{
					int start = line.find_first_of('[');//find the start of the coordinates
					std::string coord = "";
					float x = 0.0f, y = 0.0f, z = 0.0f;//z will always be 0 as the maps are 2d
					bool xcoord = true;//toggles for if the its searching for xcoord or not
					for (int i = start; i < line.size(); i++)
					{
						if (line.at(i) == '[' || line.at(i) == ']')//if a [ or ] do nothing
						{

						}
						else if (line.at(i) == ',')// a ',' seperates each coord so when appears save coord string to variable then empty it
						{
							if (xcoord)
							{
								x = ConvertToFloat(coord);
								xcoord = false;
								coord = "";
							}
							else//if its the y coord then we already have the x coord so save onto temp coords vector
							{
								y = ConvertToFloat(coord);
								coords.push_back({ x,y,z });
								xcoord = true;
								coord = "";
							}
						}
						else//build up coord
						{
							coord += line.at(i);
						}
					}
				}															//
				else if (line.find("\"id\":") != std::string::npos) { id = ConvertToInt(GetProperty(line)); }							//all these state if statements 																		
				else if (line.find("\"height\":") != std::string::npos) { height = ConvertToInt(GetProperty(line)); }					//are pretty self explanatory 
				else if (line.find("\"biome\":") != std::string::npos) { biome = ConvertToInt(GetProperty(line)); }						//if the attribute is on this line basically
				else if (line.find("\"type\":") != std::string::npos) { type = GetProperty(line); }										//get the value basically
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
				else if (line == "}," || line == "]}")//save to cells vector
				{
					cells.push_back(Cell(coords, id, height, biome, type, population, state, province, culture, religion, neighbors));//add cell to cells vector
					neighbors.clear();
					coords.clear();
				}
			}

			FindLowestAndHightest(cells);//find the highest and lowest coords

			if (highestX > 1 || highestY > 1 || lowestX < -1 || lowestY < -1)//if the cells coords go out of bounds normalise them 
			{
				NormaliseCells(&cells);
			}



			FindLowestAndHightest(cells);

			file.close();
			return cells;
		}
		else//return error
		{
			std::cout << "File: " + filename + " could not be opened" << std::endl;
			return cells;
		}

	}

	std::pair<int, int> Reader::FindFirstAndLast(std::string line)//finding the position of the data in string
	{
		int last = line.find_last_of("\"");
		int first = 0;

		for (int i = (last - 1); i > 0; i--)//go backwards through string to find other "
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
	float Reader::ConvertToFloat(std::string line)
	{
		try//try converting data to float
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
	void Reader::NormaliseCells(std::vector<Cell>* cells)
	{
		float xDiff = (lowestX + highestX) / 2;//xdiff and ydiff are the furthest
		float yDiff = (lowestY + highestY) / 2;//out coords after its centered

		float xEdge = highestX + xDiff;//this isnt needed but im not removing it
		float yEdge = highestY + yDiff;//because everything works as is

		if (std::abs(lowestX) > std::abs(highestX))//xEdge and yEdge are the values of the furthest out coords they will always be positive
		{
			xEdge = lowestX - xDiff;
		}
		else
		{
			xEdge = highestX - xDiff;
		}
		if (std::abs(lowestY) > std::abs(highestY))
		{
			yEdge = lowestY - yDiff;
		}
		else
		{
			yEdge = highestY - yDiff;
		}

		//see NormaliseCoords for how these variables are used

		for (int i = 0; i < cells->size(); i++)
		{
			cells->at(i).NormaliseCoords(xDiff, yDiff, xEdge, yEdge);
		}
	}
	void Reader::FindLowestAndHightest(std::vector<Cell> cells)
	{
		lowestX = 0.0f;
		lowestY = 0.0f;
		highestX = 0.0f;
		highestY = -10.0f;

		//just loops through all cells and find the values code is self explanatory

		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[i].coords.size(); j++)
			{
				if (cells[i].coords[j].x > highestX)
				{
					highestX = cells[i].coords[j].x;
				}
				if (cells[i].coords[j].x < lowestX)
				{
					lowestX = cells[i].coords[j].x;
				}
				if (cells[i].coords[j].y > highestY)
				{
					highestY = cells[i].coords[j].y;
				}
				if (cells[i].coords[j].y < lowestY)
				{
					lowestY = cells[i].coords[j].y;
				}
			}
		}
	}


	std::vector<Nation> Reader::ReadNationFile(std::string filename)//self explanatory
	{
		
		std::vector<Nation> nations;
		std::fstream file;
		file.open(filename);
		std::string line;
		int id = 0, capitalId = 0;
		float red = 0.0f, green = 0.0f, blue = 0.0f;
		while (std::getline(file,line))
		{
			if (line.find("id") != std::string::npos) { id = ConvertToInt(GetProperty(line)); }
			else if (line.find("capital") != std::string::npos) { capitalId = ConvertToInt(GetProperty(line)); }
			else if (line.find("red") != std::string::npos) { red = ConvertToFloat(GetProperty(line)); }
			else if (line.find("green") != std::string::npos) { green = ConvertToFloat(GetProperty(line)); }
			else if (line.find("blue") != std::string::npos) { blue = ConvertToFloat(GetProperty(line)); }
			else if (line == "    }")
			{
				Nation temp;
				temp.id = id;
				temp.capitalId = capitalId;
				temp.colour[0] = red; temp.colour[1] = green; temp.colour[2] = blue;
				nations.push_back(temp);
			}
		}
		file.close();
		return nations;
	}
	std::vector<Culture> Reader::ReadCutlureFile(std::string filename)//self explanatory
	{
		std::vector<Culture> cultures;
		std::fstream file;
		file.open(filename);
		std::string line;
		int id = 0;
		float red = 0.0f, green = 0.0f, blue = 0.0f;
		while (std::getline(file, line))
		{
			if (line.find("id") != std::string::npos) { id = ConvertToInt(GetProperty(line)); }
			else if (line.find("red") != std::string::npos) { red = ConvertToFloat(GetProperty(line)); }
			else if (line.find("green") != std::string::npos) { green = ConvertToFloat(GetProperty(line)); }
			else if (line.find("blue") != std::string::npos) { blue = ConvertToFloat(GetProperty(line)); }
			else if (line == "    }")
			{
				Culture temp;
				temp.id = id;
				temp.colour[0] = red; temp.colour[1] = green; temp.colour[2] = blue;
				cultures.push_back(temp);
			}
		}
		file.close();
		return cultures;
	}
	std::vector<Religion> Reader::ReadReligionFile(std::string filename)//self explanatory
	{
		std::vector<Religion> religions;
		std::fstream file;
		file.open(filename);
		std::string line;
		int id = 0;
		float red = 0.0f, green = 0.0f, blue = 0.0f;
		while (std::getline(file, line))
		{
			if (line.find("id") != std::string::npos) { id = ConvertToInt(GetProperty(line)); }
			else if (line.find("red") != std::string::npos) { red = ConvertToFloat(GetProperty(line)); }
			else if (line.find("green") != std::string::npos) { green = ConvertToFloat(GetProperty(line)); }
			else if (line.find("blue") != std::string::npos) { blue = ConvertToFloat(GetProperty(line)); }
			else if (line == "    }")
			{
				Religion temp;
				temp.id = id;
				temp.colour[0] = red; temp.colour[1] = green; temp.colour[2] = blue;
				religions.push_back(temp);
			}
		}
		file.close();
		return religions;
	}
}


