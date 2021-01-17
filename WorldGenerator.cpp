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
		std::uniform_real_distribution<double> distribution(-1.0f, 1.0f);//has better randomness then rand()

		std::vector<mygal::Vector2<double>> points;

		//adds randomly generated sites up to the amount wanted
		for (int i = 0; i < numSites; i++)
		{
			points.push_back({ distribution(generator), distribution(generator) });
		}

		return points;
	}


	std::vector<Cell> WorldGenerator::Generate(int numSites)
	{
		std::vector<Cell> cells;
		std::vector<mygal::Vector2<double>> points = GenerateSites(numSites);
		
		auto diagram = GenerateDiagram(points);

		for (size_t i = 0; i < 4; i++)
		{
			points = diagram.computeLloydRelaxation();//lloyds relaxation finds the centre of each vornoio shape to make cells look more regular when computed again
			diagram = GenerateDiagram(points);
		}

		//voronoi diagrams and delaunay triangulations are dual graphs
		//for me this means that the triangulation will show which cells are next to each other
		auto triangulation = diagram.computeTriangulation();

		


		

		for (const auto& site : diagram.getSites())//for each site
		{
			auto center = site.point;
			auto face = site.face;//face contains the coords that make up the cell
			auto halfedge = face->outerComponent;
			if (halfedge == nullptr)
				continue;

			while (halfedge->prev != nullptr)//while there are still edges find first edge
			{
				halfedge = halfedge->prev;
				if (halfedge == face->outerComponent)
				{
					break;
				}
			}

			auto start = halfedge; //take the start edge to no when to stop

			Cell temp = Cell();
			temp.id = site.index;//id = site.index as the triangualtion as the same ids for each cell
			temp.height = -1;
			if (center.x < 0.5 && center.x > -0.5 && center.y < 0.5 && center.y > -0.5)//if the site is close to the middle of the screen
			{																		   //it suitable for land to start to be generated on it
				temp.suitable = true;												   //i do this to so land doesnt always go off the screen
			}
			for (const auto& j : triangulation.getNeighbors(site.index))//for each neighbor add to cells neighbors
			{
				temp.neighbors.push_back(j);
			}


			while (halfedge != nullptr)														//loop through all edges
			{																				//
				if (halfedge->origin != nullptr && halfedge->destination != nullptr)		//if a complete edge
				{																			//i only need the origin as a cell used in the world manager
					temp.AddCoords(halfedge->origin->point.x, halfedge->origin->point.y);   //are complete loops so using destination would result in
				}																			//duplicate points
				halfedge = halfedge->next;													//go to next edge
				if (halfedge == start)														//stop if weve been all the way round
					break;
			}
			cells.push_back(temp);//add cell to cells vector
		}

		return cells;//return cells to world manager
	}
	template<typename T>
	mygal::Diagram<T> WorldGenerator::GenerateDiagram(const std::vector<mygal::Vector2<T>>& points)//computes a diagram using MyGal
	{
		auto algorithm = mygal::FortuneAlgorithm<double>(points);
		algorithm.construct();
		algorithm.bound(mygal::Box<double>{-1, -1, 1, 1});
		auto diagram = algorithm.getDiagram();
		diagram.intersect(mygal::Box<double>{-1, -1, 1, 1});
		return diagram;
	}

	void WorldGenerator::GenerateHeight(int numOfHighIslands, int numOfLowIslands, std::vector<Cell>* cells)
	{																											
		std::srand(time(NULL));//randomness doesnt matter too much here so rand is good enough															

		std::queue<Cell*> cellQueue;//queue of cells pointers that will have their height adjusted
		int heightMax = 9000;//starting height of chosen cell for a high island

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();	
		std::default_random_engine highIslandGen(seed);
		std::uniform_real_distribution<float> highIslandDis(0.6f,1.0f);//used to change height of cells for a high island

		seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine lowIslandGen(seed);
		std::uniform_real_distribution<float> lowIslandDis(0.2f, 1.0f);//used to change height of cells for a low island has a wider range giving a quicker fall off

		int numOfPoints = numOfHighIslands + numOfLowIslands;//number of total islands
		for (int i = 0; i < numOfPoints; i++)
		{
			if (i >= numOfHighIslands)//if all high islands done now do low islands
			{
				heightMax = 4000;
			}
			float heightMultiplier = 1.0f;//height multiplier is 1 for the first cell chosen
			int startIndex = rand() % cells->size();//get a random cell
			while (!cells->at(startIndex).suitable)//if cell isnt suitable get another one until it is
			{
				startIndex = rand() % cells->size();
			}
			
			cellQueue.push(&cells->at(startIndex));//and cell to the queue
			cellQueue.front()->added = true;
			cellQueue.front()->height = heightMax;

			while (!cellQueue.empty() && cellQueue.front()->height >= 10)//if height of front cell gets too low stop
			{
				cellQueue.front()->height *= heightMultiplier;//adjust height of cell at front of queue
				cellQueue.front()->edited = true;//show that the cells has been edited so we dont add it back to the queue 
												 //dont think this is actually necessary but I didnt remove it when I was coding this 
												 //so im not removing it now

				for (int i = 0; i < cellQueue.front()->neighbors.size(); i++)//add neighbors to the queue
				{
					if (!cells->at(cellQueue.front()->neighbors[i]).added)//if they havent alreadt been added add them
					{
						cellQueue.push(&cells->at(cellQueue.front()->neighbors[i]));
						cells->at(cellQueue.front()->neighbors[i]).added = true;//signify its been added
						cells->at(cellQueue.front()->neighbors[i]).height += cellQueue.front()->height;//cell height = height of the cell that has just been adjusted
					}
				}
				cellQueue.pop();
				if (i >= numOfHighIslands)//if a low island use lowisland random gen else use high island random gen
				{
					heightMultiplier = lowIslandDis(lowIslandGen);
				}
				else
				{
					heightMultiplier = highIslandDis(highIslandGen);
				}
			}

			int size = cellQueue.size();//rest of cells that didnt have their height changed

			for (int j = 0; j < size; j++)//remove them all from cell queue
			{
				if (!cellQueue.front()->edited)
				{
					cellQueue.front()->height = -1;
					cellQueue.front()->added = false;//change their added status so that they can be added again
				}
				cellQueue.pop();
			}
		}
		for (int k = 0; k < cells->size(); k++)
		{
			//set cell type based on height
			if (cells->at(k).height > 0.0f)
			{
				cells->at(k).type = "land";
			}
			else
			{
				cells->at(k).type = "ocean";
			}
			cells->at(k).added = false;
		}
	}

	//nations, cultures and religions all generate in the same way. they start at a point and spread out 
	//radially they dont spread to cells that have already been given a generated nation, religion or culture 
	//depending on what is being generated they also dont spread over water 
	std::vector<Nation> WorldGenerator::GenerateNations(int numOfNations, std::vector<Cell>* cells)
	{
		std::vector<Nation> nations;
		std::vector<std::queue<Cell*>> nationsQueue;

		int cellCount = 0;
		
		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfNations; i++)
		{
			std::queue<Cell*> temp;

			int randIndex = rand() % cells->size();
			while (cells->at(randIndex).type == "ocean")
			{
				randIndex = rand() % cells->size();
			}
			temp.push(&cells->at(randIndex));
			temp.front()->state = i;

			temp.front()->capital = true;
			temp.front()->nation = true;
			nationsQueue.push_back(temp);
			Nation tempNat;
			tempNat.id = i;
			tempNat.capitalId = temp.front()->id;
			tempNat.colour[0] = colorDis(colorEngine);
			tempNat.colour[1] = colorDis(colorEngine);
			tempNat.colour[2] = colorDis(colorEngine);
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
						if (!cells->at(nationsQueue[i].front()->neighbors[j]).nation && cells->at(nationsQueue[i].front()->neighbors[j]).type == "land")
						{
							cells->at(nationsQueue[i].front()->neighbors[j]).nation = true;
							cells->at(nationsQueue[i].front()->neighbors[j]).state = nationsQueue[i].front()->state;
							nationsQueue[i].push(&cells->at(nationsQueue[i].front()->neighbors[j]));
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
		return nations;
	}
	std::vector<Culture> WorldGenerator::GenerateCultures(int numOfCultures, std::vector<Cell>* cells)
	{
		std::vector<Culture> cultures;
		std::vector<std::queue<Cell*>> cultureQueue;

		int cellCount = 0;

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfCultures; i++)
		{
			std::queue<Cell*> temp;
      
			int randIndex = rand() % cells->size();
			while (cells->at(randIndex).type == "ocean")
			{
				randIndex = rand() % cells->size();
			}
			temp.push(&cells->at(randIndex));
			temp.front()->culture = i;

			temp.front()->hasCulture = true;
			cultureQueue.push_back(temp);
			Culture tempCult;
			tempCult.id = i;
			tempCult.colour[0] = colorDis(colorEngine);
			tempCult.colour[1] = colorDis(colorEngine);
			tempCult.colour[2] = colorDis(colorEngine);
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
						if (!cells->at(cultureQueue[i].front()->neighbors[j]).hasCulture && cells->at(cultureQueue[i].front()->neighbors[j]).type == "land")
						{
							cells->at(cultureQueue[i].front()->neighbors[j]).hasCulture = true;
							cells->at(cultureQueue[i].front()->neighbors[j]).culture = cultureQueue[i].front()->culture;
							cultureQueue[i].push(&cells->at(cultureQueue[i].front()->neighbors[j]));
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

		return cultures;
	}
	std::vector<Religion> WorldGenerator::GenerateReligions(int numOfReligions, std::vector<Cell>* cells)
	{
		std::vector<Religion> religions;
		std::vector<std::queue<Cell*>> religionQueue;

		int cellCount = 0;

		uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine colorEngine(seed);
		std::uniform_real_distribution<float> colorDis(0.01f, 1.0f);

		for (int i = 0; i < numOfReligions; i++)
		{
			std::queue<Cell*> temp;

			int randIndex = rand() % cells->size();
			while (cells->at(randIndex).type == "ocean")
			{
				randIndex = rand() % cells->size();
			}
			temp.push(&cells->at(randIndex));
			temp.front()->religion = i;

			temp.front()->hasReligion = true;
			religionQueue.push_back(temp);
			Religion tempRel;
			tempRel.id = i;
			tempRel.colour[0] = colorDis(colorEngine);
			tempRel.colour[1] = colorDis(colorEngine);
			tempRel.colour[2] = colorDis(colorEngine);
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

						if (!cells->at(religionQueue[i].front()->neighbors[j]).hasReligion && cells->at(religionQueue[i].front()->neighbors[j]).type == "land")
						{
							cells->at(religionQueue[i].front()->neighbors[j]).hasReligion = true;
							cells->at(religionQueue[i].front()->neighbors[j]).religion = religionQueue[i].front()->religion;
							religionQueue[i].push(&cells->at(religionQueue[i].front()->neighbors[j]));
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
		return religions;
	}
}
