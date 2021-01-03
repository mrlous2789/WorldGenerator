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


	void WorldGenerator::Generate(int numSites)
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

		GenerateHeight();
		GenerateNations();
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

	void WorldGenerator::GenerateHeight()
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

		int numOfPoints = 15;
		for (int i = 0; i < numOfPoints; i++)
		{
			if (i > 0)
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
						cells[cellQueue.front()->neighbors[i]].height = cellQueue.front()->height;
					}
				}
				cellQueue.pop();
				if (i > 1)
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
					cellQueue.front()->height = 0;
					cellQueue.front()->added = false;
				}
				cellQueue.pop();
			}
		}

	}
	void WorldGenerator::GenerateNations()
	{
		int numOfNations = 12;

		std::vector<std::queue<Cell*>> nationsQueue;

		int cellCount = 0;

		for (int i = 0; i < numOfNations; i++)
		{
			std::queue<Cell*> temp;
			int randIndex = rand() % cells.size();
			while (cells[randIndex].height <= 0)
			{
				randIndex = rand() % cells.size();
			}
			temp.push(&cells[randIndex]);
			if (temp.front()->height > 0)
			{
				temp.front()->state = i + 1;
			}
			temp.front()->capital = true;
			temp.front()->nation = true;
			nationsQueue.push_back(temp);
			cellCount++;
		}
		while (cellCount < cells.size())
		{
			for (int i = 0; i < nationsQueue.size(); i++)
			{
				if (!nationsQueue[i].empty())
				{
					for (int j = 0; j < nationsQueue[i].front()->neighbors.size(); j++)
					{
						if (!cells[nationsQueue[i].front()->neighbors[j]].nation)
						{
							nationsQueue[i].push(&cells[nationsQueue[i].front()->neighbors[j]]);
							cells[nationsQueue[i].front()->neighbors[j]].nation = true;
							cellCount++;
						}
					}
					if (nationsQueue[i].front()->height > 0)
					{
						nationsQueue[i].front()->state = i + 1;
					}

					nationsQueue[i].pop();
				}
			}
		}
		
	}

}