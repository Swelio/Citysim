#include "stdafx.h"
#include <algorithm>
#include <thread>
using namespace std;
#include "Faction.h"

int Faction::factionNumber = 0;

Faction::Faction():
	budget(0),
	id(factionNumber)
{
	++factionNumber;
}

Faction::Faction(string arg_name, City* capitale):
	budget(0),
	id(factionNumber)
{
	name = arg_name;
	cities.push_back(capitale);
	++factionNumber;
}


Faction::~Faction()
{
}

void Faction::update()
{
	budget = 0;
	for each (City* town in cities)
	{
		budget += town->getWallet();
	}
}

void Faction::acheterVille(City *vendue, float prix)
{
	budget -= prix;
	vendue->getFaction()->vendreVille(vendue, prix);
	vendue->set_Faction(this);
	cities.push_back(vendue);
}

void Faction::vendreVille(City * vendue, float prix)
{
	cities.erase(remove(cities.begin(), cities.end(), vendue), cities.end());
	budget += prix;
}

const string Faction::getName() const
{
	return name;
}

const int Faction::getCitiesLenght() const
{
	return cities.size();
}

vector<City*> Faction::getNeighbourhood()
{
	vector<City*> neighbour;
	for each (City* town in cities)
	{
		for each (City* voisine in town->get_Neighbour())
		{
			neighbour.push_back(voisine);
		}
	}
	return neighbour;
}

float Faction::getBudget() const
{
	return budget;
}

void Faction::setBudget(float arg_budget)
{
	budget = arg_budget;
}

void Faction::budgetGrowing(City* setBy)
{
	inSetting.lock();
	setBy->budgetGrowth();
	inSetting.unlock();
}

void Faction::getVictory()
{

	for each (City* town in cities)
	{
		town->victory();
	}
}
