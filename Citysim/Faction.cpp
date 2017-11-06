#include "stdafx.h"
#include "Faction.h"
#include <algorithm>
using namespace std;

int Faction::factionNumber = 0;

Faction::Faction():
	nourriture(0),
	energie(0),
	budget(0),
	id(factionNumber)
{
	++factionNumber;
}

Faction::Faction(string arg_name, City* capitale):
	nourriture(0),
	energie(0),
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
	nourriture = 0;
	energie = 0;
	budget = 0;
	for each (City* town in cities)
	{
		nourriture += town->getFood();
		energie += town->getEnergy();
		budget += town->getWallet();
	}
}

void Faction::acheterVille(City *vendue, float prix)
{
	budget -= prix;
	vendue->estAchetee(this);
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
