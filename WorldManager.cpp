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
	bool WorldManager::LoadFromFile(std::string cellFile)
	{
		cells = reader.ReadCellFile(cellFile);
		if (cells.empty())
		{
			return false;
		}
		else
		{
			return true;
		}
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

}