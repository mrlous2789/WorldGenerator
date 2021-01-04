#include "WorldGenerator.h"
namespace Mer
{
	WorldGenerator::WorldGenerator()
	{

	}

	std::vector<mygal::Vector2<double>> WorldGenerator::GenerateSites(int numSites)
	{
		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine generator(seed);
		std::uniform_real_distribution<double> distribution(-1.0f, 1.0f);

		std::vector<mygal::Vector2<double>> points;

		for (int i = 0; i < numSites; i++)
		{
			//sites.push_back(glm::vec2{ distribution(generator), distribution(generator) });
			points.push_back({ distribution(generator), distribution(generator) });
		}

		return points;
	}


	void WorldGenerator::Generate(int numSites, int numOfHighIslands, int numOfLowIslands, int numOfNations, int numOfCultures, int numOfReligions)
	{
		cells.clear();
		std::vector<mygal::Vector2<double>> points = GenerateSites(numSites);
		
		auto diagram = GenerateDiagram(points);

		for (size_t i = 0; i < 4; i++)
		{
			points = diagram.computeLloydRelaxation();
			diagram = GenerateDiagram(points);
		}

		auto triangulation = diagram.computeTriangulation();

		


		int id = 0;

		for (const auto& site : diagram.getSites())
		{
			auto center = site.point;
			auto face = site.face;
			auto halfedge = face->outerComponent;
			if (halfedge == nullptr)
				continue;

			while (halfedge->prev != nullptr)
			{
				halfedge = halfedge->prev;
				if (halfedge == face->outerComponent)
				{
					break;
				}
			}

			auto start = halfedge;

			Cell temp = Cell();
			temp.id = site.index;
			temp.height = -1;
			if (center.x < 0.5 && center.x > -0.5 && center.y < 0.5 && center.y > -0.5)
			{
				temp.suitable = true;
			}
			for (const auto& j : triangulation.getNeighbors(site.index))
			{
				temp.neighbors.push_back(j);
			}


			while (halfedge != nullptr)
			{
				if (halfedge->origin != nullptr && halfedge->destination != nullptr)
				{
					temp.AddCoords(halfedge->origin->point.x, halfedge->origin->point.y);
				}
				halfedge = halfedge->next;
				if (halfedge == start)
					break;
			}
			cells.push_back(temp);
		}

		GenerateHeight(numOfHighIslands,numOfLowIslands);
		GenerateNations(numOfNations);
		GenerateCultures(numOfCultures);
		GenerateReligions(numOfReligions);
	}
	template<typename T>
	mygal::Diagram<T> WorldGenerator::GenerateDiagram(const std::vector<mygal::Vector2<T>>& points)
	{
		auto algorithm = mygal::FortuneAlgorithm<double>(points);
		algorithm.construct();
		algorithm.bound(mygal::Box<double>{-1, -1, 1, 1});
		auto diagram = algorithm.getDiagram();
		diagram.intersect(mygal::Box<double>{-1, -1, 1, 1});
		return diagram;
	}

	void WorldGenerator::GenerateHeight(int numOfHighIslands, int numOfLowIslands)
	{
		std::srand(time(NULL));

		std::queue<Cell*> cellQueue;
		int heightMax = 9000;

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine highIslandGen(seed);
		std::uniform_real_distribution<float> highIslandDis(0.6f,1.0f);

		seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine lowIslandGen(seed);
		std::uniform_real_distribution<float> lowIslandDis(0.2f, 1.0f);

		int numOfPoints = numOfHighIslands + numOfLowIslands;
		for (int i = 0; i < numOfPoints; i++)
		{
			if (i >= numOfHighIslands)
			{
				heightMax = 4000;
			}
			float heightMultiplier = 1.0f;
			int heightIncrement = 10;
			int startIndex = rand() % cells.size();
			while (!cells[startIndex].suitable)
			{
				startIndex = rand() % cells.size();
			}
			cellQueue.push(&cells[startIndex]);
			cellQueue.front()->added = true;
			cellQueue.front()->height = heightMax;

			while (!cellQueue.empty() && cellQueue.front()->height >= 10)
			{
				cellQueue.front()->height *= heightMultiplier;
				cellQueue.front()->edited = true;

				for (int i = 0; i < cellQueue.front()->neighbors.size(); i++)
				{
					if (!cells[cellQueue.front()->neighbors[i]].added)
					{
						cellQueue.push(&cells[cellQueue.front()->neighbors[i]]);
						cells[cellQueue.front()->neighbors[i]].added = true;
						cells[cellQueue.front()->neighbors[i]].height += cellQueue.front()->height;
					}
				}
				cellQueue.pop();
				if (i >= numOfHighIslands)
				{
					heightMultiplier = lowIslandDis(lowIslandGen);
				}
				else
				{
					heightMultiplier = highIslandDis(highIslandGen);
				}
			}

			int size = cellQueue.size();

			for (int j = 0; j < size; j++)
			{
				if (!cellQueue.front()->edited)
				{
					cellQueue.front()->height = -1;
					cellQueue.front()->added = false;
				}
				cellQueue.pop();
			}
			for (int k = 0; k < cells.size(); k++)
			{
				if (cells[k].height > 0.0f)
				{
					cells[k].type = "land";
				}
				else
				{
					cells[k].type = "ocean";
				}
				cells[k].added = false;
			}
		}

	}
	void WorldGenerator::GenerateNations(int numOfNations)
	{
		std::vector<std::queue<Cell*>> nationsQueue;

		int cellCount = 0;
		
		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfNations; i++)
		{
			std::queue<Cell*> temp;
			int randIndex = rand() % cells.size();
			while (cells[randIndex].type == "ocean")
			{
				randIndex = rand() % cells.size();
			}
			temp.push(&cells[randIndex]);
			temp.front()->state = i;

			temp.front()->capital = true;
			temp.front()->nation = true;
			nationsQueue.push_back(temp);
			Nation tempNat;
			tempNat.id = i;
			tempNat.capitalID = temp.front()->id;
			tempNat.color[0] = colorDis(colorEngine);
			tempNat.color[1] = colorDis(colorEngine);
			tempNat.color[2] = colorDis(colorEngine);
			nations.push_back(tempNat);
			cellCount++;
		}
		while (!nationsQueue.empty())
		{
			for (int i = 0; i < nationsQueue.size(); i++)
			{
				if (!nationsQueue[i].empty())
				{
					for (int j = 0; j < nationsQueue[i].front()->neighbors.size(); j++)
					{
						if (!cells[nationsQueue[i].front()->neighbors[j]].nation && cells[nationsQueue[i].front()->neighbors[j]].type == "land")
						{
							cells[nationsQueue[i].front()->neighbors[j]].nation = true;
							cells[nationsQueue[i].front()->neighbors[j]].state = nationsQueue[i].front()->state;
							nationsQueue[i].push(&cells[nationsQueue[i].front()->neighbors[j]]);
						}
					}
					nationsQueue[i].pop();
				}
				else
				{
					nationsQueue.erase(nationsQueue.begin() + i);
				}
			}
		}
		
	}
	void WorldGenerator::GenerateCultures(int numOfCultures)
	{
		std::vector<std::queue<Cell*>> cultureQueue;

		int cellCount = 0;

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfCultures; i++)
		{
			std::queue<Cell*> temp;
			int randIndex = rand() % cells.size();
			while (cells[randIndex].type == "ocean")
			{
				randIndex = rand() % cells.size();
			}
			temp.push(&cells[randIndex]);
			temp.front()->culture = i;

			temp.front()->hasCulture = true;
			cultureQueue.push_back(temp);
			Culture tempCult;
			tempCult.id = i;
			tempCult.color[0] = colorDis(colorEngine);
			tempCult.color[1] = colorDis(colorEngine);
			tempCult.color[2] = colorDis(colorEngine);
			cultures.push_back(tempCult);
			cellCount++;
		}
		while (!cultureQueue.empty())
		{
			for (int i = 0; i < cultureQueue.size(); i++)
			{
				if (!cultureQueue[i].empty())
				{
					for (int j = 0; j < cultureQueue[i].front()->neighbors.size(); j++)
					{
						if (!cells[cultureQueue[i].front()->neighbors[j]].hasCulture && cells[cultureQueue[i].front()->neighbors[j]].type == "land")
						{
							cells[cultureQueue[i].front()->neighbors[j]].hasCulture = true;
							cells[cultureQueue[i].front()->neighbors[j]].culture = cultureQueue[i].front()->culture;
							cultureQueue[i].push(&cells[cultureQueue[i].front()->neighbors[j]]);
						}
					}
					cultureQueue[i].pop();
				}
				else
				{
					cultureQueue.erase(cultureQueue.begin() + i);
				}
			}
		}
	}
	void WorldGenerator::GenerateReligions(int numOfReligions)
	{
		std::vector<std::queue<Cell*>> religionQueue;

		int cellCount = 0;

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfReligions; i++)
		{
			std::queue<Cell*> temp;
			int randIndex = rand() % cells.size();
			while (cells[randIndex].type == "ocean")
			{
				randIndex = rand() % cells.size();
			}
			temp.push(&cells[randIndex]);
			temp.front()->religion = i;

			temp.front()->hasReligion = true;
			religionQueue.push_back(temp);
			Religion tempRel;
			tempRel.id = i;
			tempRel.color[0] = colorDis(colorEngine);
			tempRel.color[1] = colorDis(colorEngine);
			tempRel.color[2] = colorDis(colorEngine);
			religions.push_back(tempRel);
			cellCount++;
		}
		while (!religionQueue.empty())
		{
			for (int i = 0; i < religionQueue.size(); i++)
			{
				if (!religionQueue[i].empty())
				{
					for (int j = 0; j < religionQueue[i].front()->neighbors.size(); j++)
					{
						if (!cells[religionQueue[i].front()->neighbors[j]].hasReligion && cells[religionQueue[i].front()->neighbors[j]].type == "land")
						{
							cells[religionQueue[i].front()->neighbors[j]].hasReligion = true;
							cells[religionQueue[i].front()->neighbors[j]].religion = religionQueue[i].front()->religion;
							religionQueue[i].push(&cells[religionQueue[i].front()->neighbors[j]]);
						}
					}
					religionQueue[i].pop();
				}
				else
				{
					religionQueue.erase(religionQueue.begin() + i);
				}
			}
		}
	}
	int WorldGenerator::getNationCount()
	{
		return nations.size();
	}
	float WorldGenerator::getNationRed(int id)
	{
		return nations[id].color[0];
	}
	float WorldGenerator::getNationGreen(int id)
	{
		return nations[id].color[1];
	}
	float WorldGenerator::getNationBlue(int id)
	{
		return nations[id].color[2];
	}

	int WorldGenerator::getCultureCount()
	{
		return cultures.size();
	}
	float WorldGenerator::getCultureRed(int id)
	{
		return cultures[id].color[0];
	}
	float WorldGenerator::getCultureGreen(int id)
	{
		return cultures[id].color[1];
	}
	float WorldGenerator::getCultureBlue(int id)
	{
		return cultures[id].color[2];
	}

	int WorldGenerator::getReligionCount()
	{
		return religions.size();
	}
	float WorldGenerator::getReligionRed(int id)
	{
		return religions[id].color[0];
	}
	float WorldGenerator::getReligionGreen(int id)
	{
		return religions[id].color[1];
	}
	float WorldGenerator::getReligionBlue(int id)
	{
		return religions[id].color[2];
	}
	Cell* WorldGenerator::getCellAtCoords(double xpos, double ypos)
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
				else
				{

				}
			}
			if (counter % 2 != 0)
			{
				return &cells[i];
			}
		}

		return nullptr;
		
	}
	bool WorldGenerator::Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2)
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
}