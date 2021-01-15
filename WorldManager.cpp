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
	bool WorldManager::LoadFromFile(std::string cellFile, std::string nationFile, std::string cultureFile, std::string religionFile)
	{
		cells = reader.ReadCellFile(cellFile);
		nations = reader.ReadNationFile(nationFile);
		cultures = reader.ReadCutlureFile(cultureFile);
		religions = reader.ReadReligionFile(religionFile);
		if (cells.empty())
			return false;
		else if (nations.empty())
			return false;
		else if (cultures.empty())
			return false;
		else if (religions.empty())
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
		{
			if (nations[id].id == id)
			{
				return nations[id];
			}
		}
		else
		{
			for (int i = 0; i < nations.size(); i++)
			{
				if (nations[i].id == id)
				{
					return nations[i];
				}
			}
		}
		return nat;
	}
	Culture WorldManager::getCultureById(int id)
	{
		Culture cult;
		cult.id = -1;
		if (id >= 0 && id < cultures.size())
		{
			if (cultures[id].id == id)
			{
				return cultures[id];
			}
		}
		else
		{
			for (int i = 0; i < cultures.size(); i++)
			{
				if (cultures[i].id == id)
				{
					return cultures[i];
				}
			}
		}
		return cult;
	}
	Religion WorldManager::getReligionById(int id)
	{
		Religion rel;
		rel.id = -1;

		if (id >= 0 && id < religions.size())
		{
			if (religions[id].id == id)
			{
				return religions[id];
			}
		}
		else
		{
			for (int i = 0; i < religions.size(); i++)
			{
				if (religions[i].id == id)
				{
					return religions[i];
				}
			}
		}
		return rel;
	}

	void WorldManager::SaveMap()
	{
		saveWorld();
		saveNations();
		saveCultures();
		saveReligions();
	}
	void WorldManager::saveWorld()
	{
		std::ofstream file;
		file.open(".\\OutputFiles\\testOutputFile.geojson");
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
	void WorldManager::saveNations()
	{
		std::ofstream file;
		file.open(".\\OutputFiles\\testOutputFile_nations.mer");
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
	void WorldManager::saveCultures()
	{
		std::ofstream file;
		
		file.open(".\\OutputFiles\\testOutputFile_cultures.mer");
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
	void WorldManager::saveReligions()
	{
		std::ofstream file;
		file.open(".\\OutputFiles\\testOutputFile_religions.mer");
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
}