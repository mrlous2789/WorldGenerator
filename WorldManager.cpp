#include "WorldManager.h"
namespace Mer
{
	WorldManager::WorldManager()
	{

	}
	bool WorldManager::Generate(int numOfSites, int numOfHighIslands, int numOfLowIslands, int numOfNations, int numOfCultures, int numOfReligions)
	{
		cells = wg.Generate(numOfSites);
		wg.GenerateHeight(numOfHighIslands, numOfLowIslands, &cells);
		nations = wg.GenerateNations(numOfNations, &cells);
		cultures = wg.GenerateCultures(numOfCultures, &cells);
		religions = wg.GenerateReligions(numOfCultures, &cells);
		return true;
	}
	bool WorldManager::LoadFromFile(std::string cellFile, std::string riverFile,std::string nationFile, std::string cultureFile, std::string religionFile)
	{
		cells = reader.ReadCellFile(cellFile);
		rivers = reader.ReadRiversFile(riverFile);
		nations = reader.ReadNationFile(nationFile);
		cultures = reader.ReadCutlureFile(cultureFile);
		religions = reader.ReadReligionFile(religionFile);

		if (cells.empty())
			return false;
		else
			return true;
	}
	Cell* WorldManager::getCellAtCoords(double xpos, double ypos)
	{
		float height = 0;

		for (int i = 0; i < cells.size(); i++)
		{
			int counter = 0;
			for (int j = 0, k = cells[i].coords.size() - 1; j < cells[i].coords.size(); k = j++)
			{
				if (Intersects(xpos, ypos, cells[i].coords[j].x, cells[i].coords[j].y, cells[i].coords[k].x, cells[i].coords[k].y))
				{
					counter++;
				}
			}
			if (counter % 2 != 0)
			{
				return &cells[i];
			}
		}
		//if cell is not found e.g. mouse if off screen return a nullptr
		return nullptr;
	}
	bool WorldManager::Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2)
	{
		
		if ((mouseY <= edgeY1) != (mouseY <= edgeY2))
		{
			if (mouseX <= (edgeX2 - edgeX1) * (mouseY - edgeY1) / (edgeY2 - edgeY1) + edgeX1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	Nation WorldManager::getNationById(int id)
	{
		Nation nat;
		nat.id = -1;
		nat.capitalId = -1;

		if (id >= 0 && id < nations.size())
			if (nations[id].id == id)
				return nations[id];

		for (int i = 0; i < nations.size(); i++)
			if (nations[i].id == id)
				return nations[i];

		return nat;
	}
	Culture WorldManager::getCultureById(int id)
	{
		Culture cult;
		cult.id = -1;
		if (id >= 0 && id < cultures.size())
			if (cultures[id].id == id)
				return cultures[id];

		for (int i = 0; i < cultures.size(); i++)
			if (cultures[i].id == id)
				return cultures[i];


		return cult;
	}
	Religion WorldManager::getReligionById(int id)
	{
		Religion rel;
		rel.id = -1;

		if (id >= 0 && id < religions.size())
			if (religions[id].id == id)
				return religions[id];

		for (int i = 0; i < religions.size(); i++)
			if (religions[i].id == id)
				return religions[i];

		
		return rel;
	}

	//function called by the state
	void WorldManager::SaveMap(std::string mapname)
	{
		saveWorld(mapname);
		saveRivers(mapname);
		saveNations(mapname);
		saveCultures(mapname);
		saveReligions(mapname);
	}
	//save world writes in a geojson format user can decide the name
	void WorldManager::saveWorld(std::string mapname)
	{
		std::ofstream file;
		std::string filename = ".\\OutputFiles\\" + mapname + ".geojson";

		file.open(filename);
		file.clear();

		file << "{ \"type\": \"FeatureCollection\", \"features\": [\n";



		for (int i = 0; i < cells.size(); i++)
		{
			file << "{\n";
			file << "    " << "\"type\": \"Feature\",\n";

			file << "    " << "\"geometry\": { \"type\": \"Polygon\", \"coordinates\": [[";

			for (int j = 0; j < cells[i].coords.size(); j++)
			{
				file << "[";
				file << cells[i].coords[j].x; file << ","; file << cells[i].coords[j].y;
				if (j == (cells[i].coords.size() - 1))
				{
					file << "]";
				}
				else
				{
					file << "],";
				}
				
			}
			file << "]] },\n";

			file << "    " << "\"properties\": {\n";
			file << "        " << "\"id\": "<< "\"" << cells[i].id << "\",\n";
			file << "        " << "\"height\": " << "\"" << cells[i].height << "\",\n";
			file << "        " << "\"biome\": " << "\"" << cells[i].biome << "\",\n";
			file << "        " << "\"type\": " << "\"" << cells[i].type << "\",\n";
			file << "        " << "\"population\": " << "\"" << cells[i].population << "\",\n";
			file << "        " << "\"state\": " << "\"" << cells[i].state << "\",\n";
			file << "        " << "\"province\": " << "\"" << cells[i].province << "\",\n";
			file << "        " << "\"culture\": " << "\"" << cells[i].culture << "\",\n";
			file << "        " << "\"religion\": " << "\"" << cells[i].religion << "\",\n";
			file << "        " << "\"neighbors\": [";
			for (int k = 0; k < cells[i].neighbors.size(); k++)
			{
				file << cells[i].neighbors[k];
				if (k != cells[i].neighbors.size() - 1)
					file << ",";

			}
			file << "]\n";
			file << "    }\n";
			if (i == cells.size() - 1)
				file << "}\n";
			else
				file << "},\n";

		}
		file << "]}";

		file.close();
	}
	void WorldManager::saveRivers(std::string mapname)
	{
		std::ofstream file;
		std::string filename = ".\\OutputFiles\\" + mapname + "_rivers.geojson";

		file.open(filename);
		file.clear();

		file << "{ \"type\": \"FeatureCollection\", \"features\": [\n";

		for (int i = 0; i < rivers.size(); i++)
		{
			file << "{\n";
			file << "    " << "\"type\": \"Feature\",\n";

			file << "    " << "\"geometry\": { \"type\": \"LineString\", \"coordinates\": [[";

			for (int j = 0; j < rivers[i].coords.size(); j++)
			{
				file << "[";
				file << rivers[i].coords[j].x; file << ","; file << rivers[i].coords[j].y;
				if (j == (rivers[i].coords.size() - 1))
				{
					file << "]";
				}
				else
				{
					file << "],";
				}

			}
			file << "]] },\n";

			file << "    " << "\"properties\": {\n";
			file << "        " << "\"id\": " << "\"" << rivers[i].id << "\",\n";
			file << "        " << "\"width\": " << "\"" << rivers[i].width << "\",\n";
			file << "        " << "\"increment\": " << "\"" << rivers[i].increment << "\",\n";
			file << "    }\n";
			if (i == rivers.size() - 1)
				file << "}\n";
			else
				file << "},\n";

		}
	}
	//everthing else has a custom text format to make reading them easy
	void WorldManager::saveNations(std::string mapname)
	{
		std::ofstream file;
		std::string filename = ".\\OutputFiles\\" + mapname + "_nations.mer";
		file.open(filename);
		file.clear();
		file << "Nations\n";
		file << "{\n";
		for (int i = 0; i < nations.size(); i++)
		{
			file << "    {\n";
			file << "        " <<"\"id\": " << "\"" << nations[i].id << "\"\n";
			file << "        " << "\"capital\": " << "\"" << nations[i].capitalId << "\"\n";
			file << "        " << "\"red\": " << "\"" << nations[i].colour[0] << "\"\n";
			file << "        " << "\"green\": " << "\"" << nations[i].colour[1] << "\"\n";
			file << "        " << "\"blue\": " << "\"" << nations[i].colour[2] << "\"\n";
			file << "    }\n";
		}
		file << "}";
		file.close();
	}
	void WorldManager::saveCultures(std::string mapname)
	{
		std::ofstream file;
		std::string filename = ".\\OutputFiles\\" + mapname + "_cultures.mer";
		file.open(filename);
		file.clear();
		file << "Cultures\n";
		file << "{\n";
		for (int i = 0; i < cultures.size(); i++)
		{
			file << "    {\n";
			file << "        " << "\"id\": " << "\"" << cultures[i].id << "\"\n";
			file << "        " << "\"red\": " << "\"" << cultures[i].colour[0] << "\"\n";
			file << "        " << "\"green\": " << "\"" << cultures[i].colour[1] << "\"\n";
			file << "        " << "\"blue\": " << "\"" << cultures[i].colour[2] << "\"\n";
			file << "    }\n";
		}
		file << "}";
		file.close();
	}
	void WorldManager::saveReligions(std::string mapname)
	{
		std::ofstream file;
		std::string filename = ".\\OutputFiles\\" + mapname + "_religions.mer";
		file.open(filename);
		file.clear();
		file << "Religions\n";
		file << "{\n";
		for (int i = 0; i < religions.size(); i++)
		{
			file << "    {\n";
			file << "        " << "\"id\": " << "\"" << religions[i].id << "\"\n";
			file << "        " << "\"red\": " << "\"" << religions[i].colour[0] << "\"\n";
			file << "        " << "\"green\": " << "\"" << religions[i].colour[1] << "\"\n";
			file << "        " << "\"blue\": " << "\"" << religions[i].colour[2] << "\"\n";
			file << "    }\n";
		}
		file << "}";
		file.close();
	}

	//deletes self explanatory
	void WorldManager::deleteNation(int index)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			if (cells[i].state == nations[index].id)
			{
				cells[i].state = -1;
			}
		}

		nations.erase(nations.begin() + index);
	}
	void WorldManager::deleteCulture(int index)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			if (cells[i].culture == cultures[index].id)
			{
				cells[i].culture = -1;
			}
		}

		cultures.erase(cultures.begin() + index);
	}
	void WorldManager::deleteReligion(int index)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			if (cells[i].religion == religions[index].id)
			{
				cells[i].religion = -1;
			}
		}

		religions.erase(religions.begin() + index);
	}
	//adds self explanatory
	void WorldManager::addNation()
	{
		Nation temp;
		if (nations.empty())
			temp.id = 0;
		else
			temp.id = nations.back().id + 1;

		temp.capitalId = -1;
		temp.colour[0] = 0.3f;
		temp.colour[1] = 0.3f;
		temp.colour[2] = 0.3f;
		nations.push_back(temp);
	}
	void WorldManager::addCulture()
	{
		Culture temp;
		if (cultures.empty())
			temp.id = 0;
		else
			temp.id = cultures.back().id + 1;

		temp.colour[0] = 0.3f;
		temp.colour[1] = 0.3f;
		temp.colour[2] = 0.3f;
		cultures.push_back(temp);
	}
	void WorldManager::addReligion()
	{
		Religion temp;
		if (religions.empty())
			temp.id = 0;
		else
			temp.id = religions.back().id + 1;

		temp.colour[0] = 0.3f;
		temp.colour[1] = 0.3f;
		temp.colour[2] = 0.3f;
		religions.push_back(temp);
	}
}