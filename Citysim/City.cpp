#include "stdafx.h"

#include "Faction.h"
#include "City.h"
#include "RandomName.h"
#include <iostream>
using namespace std;

int City::cityNumber = 0;


City::City():
	bonheur(1),
	population(DEFAULT_POPULATION),
	nourriture(DEFAULT_FOOD),
	energie(DEFAULT_ENERGY),
	budget(DEFAULT_WALLET),
	farmers(DEFAULT_FARMERS),
	energizer(DEFAULT_ENERGIZER),
	traders(DEFAULT_TRADERS),
	acheteur(NULL),
	prix(1)
{
	initiate();
	nom = RandomName::generate();
	faction = new Faction(nom, this);
	id = City::cityNumber;
	++City::cityNumber;
}

City::City(int arg_posx, int arg_posy):
	bonheur(1),
	population(DEFAULT_POPULATION),
	nourriture(DEFAULT_FOOD),
	energie(DEFAULT_ENERGY),
	budget(DEFAULT_WALLET),
	farmers(DEFAULT_FARMERS),
	energizer(DEFAULT_ENERGIZER),
	traders(DEFAULT_TRADERS),
	acheteur(NULL),
	prix(1)
{
	initiate(arg_posx, arg_posy);
}

City::City(string name, int arg_posx, int arg_posy, float arg_bonheur, int arg_population, float arg_nourriture, float arg_energie, float arg_budget):
	bonheur(1),
	population(DEFAULT_POPULATION),
	nourriture(DEFAULT_FOOD),
	energie(DEFAULT_ENERGY),
	budget(DEFAULT_WALLET),
	farmers(DEFAULT_FARMERS),
	energizer(DEFAULT_ENERGIZER),
	traders(DEFAULT_TRADERS),
	acheteur(NULL),
	prix(1)
{
	initiate(arg_posx, arg_posy, name);
	population = arg_population;
	nourriture = arg_nourriture;
	energie = arg_energie;
	budget = arg_budget;
	bonheur = arg_bonheur;
}

void City::initiate(int posx, int posy, string arg_name)
{
	id = City::cityNumber;
	++City::cityNumber;
	nom = arg_name;
	faction = new Faction(nom, this);
	coord_x = posx;
	coord_y = posy;
}


City::~City()
{
}

// Affichages
void City::presentation()
{
	cout << "[" << faction->getName() << "] " << nom << "\n" << endl;
	cout << "Prix : " << prix << endl;
	cout << "Population : " << population << endl;
	cout << "Nourriture : " << nourriture << endl;
	cout << "Energie : " << energie << endl;
	cout << "Budget : " << budget << endl;
	cout << endl;
}

void City::working()
{
	int totalEmployes(energizer + farmers + traders);
	cout << "Employes : " << totalEmployes << "/" << population << endl;
	cout << "Fermiers : " << farmers << endl;
	cout << "Traders : " << traders << endl;
	cout << "Ingenieurs : " << energizer << endl;
	cout << endl;
}

//Croissance
void City::growth()
{
	float coeffHappy = happinessPart();
	energyGrowth(coeffHappy);
	foodGrowth(coeffHappy);
	happinessGrowth(coeffHappy);
	budgetGrowth();
	populationGrowth(coeffHappy);
}

void City::energyGrowth(float coeffHappy)
{
	float coefficient(1); // coefficient bonus
	float quantity(50); // energie produite par habitant
	float consommation(20); // energie consommee par habitant
	// Production en fonction du bonheur de la population
	energie += (energizer*coefficient*quantity*coeffHappy)+(energizer*coefficient*(1- coeffHappy))-population*consommation;
	if (energie < 0) energie = 0;
}

void City::foodGrowth(float coeffHappy)
{
	float coefficient(1); // coefficient bonus
	float quantity(30); // nourriture produite par habitant
	float consommation(10); // nourriture consommee par habitant
	// Production en fonction du bonheur de la population
	nourriture += (farmers*coefficient*quantity*coeffHappy)+(farmers*coefficient*(1- coeffHappy))-population*consommation;
	if (nourriture < 0) nourriture = 0;
}

void City::populationGrowth(float coeffHappy)
{
	float coefficient(1); // coefficient bonus
	float coeffSad(1 / bonheur); // coefficient de population malheureuse disparaissant
	float coeffWorker(1);
	int newPopulation(0),travailleurs(0);
	int happy(coeffHappy*population); // habitants heureux
	int sad(population - happy); // population dont les besoins ne sont pas tous satisfaits
	newPopulation = population+(happy/2)-sad*coeffSad; // population restante

	travailleurs = farmers + energizer + traders; // total de travailleurs

	if (travailleurs > newPopulation && newPopulation > 0) // ajustement des travailleurs � la population restante si inf�rieure
	{
		coeffWorker = (newPopulation / population);
		energizer *= coeffWorker;
		farmers *= coeffWorker;
		traders *= coeffWorker;
	}
	population = newPopulation;
}

void City::happinessGrowth(float coeffHappy)
{
	float coefficient(1); // coefficient bonus
	int happynessPop(coeffHappy*population); // population comblee
	int sadnessPop(population - happynessPop); // population en manque
	bonheur += (happynessPop - sadnessPop)*coefficient; // calcul du bonheur selon la population heureuse et malheureuse
	if (bonheur <= 0) bonheur = 1;
}

void City::budgetGrowth()
{
	float salaire(2.5),coeffTraders(25.0);
	int travailleurs = farmers + energizer + traders;
	budget += traders*coeffTraders - salaire*travailleurs;
	if (budget < 0) budget = 0;
}

// Estimations et mesures
float City::happinessPart()
{
	float foodCons(10), energyCons(20); // besoins par habitant selon la ressource

	if (nourriture <= 0 || energie <= 0) return 0.0;

	float happyFood = (nourriture / foodCons) / population; // part de population dont les besoins en nourriture sont satisfaits
	if (happyFood > 1.0) happyFood = 1.0; // controle du pourcentage de population satisfaite en nourriture

	float happyEnergy = (energie / energyCons) / population; // part de population dont les besoins en energie sont satisfaits
	if (happyEnergy > 1.0) happyEnergy = 1.0; // controle du pourcentage de population satisfaite en �nergie

	return happyFood*happyEnergy; // part de population dont tous les besoins sont satisfaits
}

// Achat/Vente de villes
void City::acheterVille(City *achetee, float prix)
{
	if (achetee->getFaction() != faction)
	{
		achetee->propositionRachat(faction, prix); // proposition de rachat de la ville
	}
}

void City::propositionRachat(Faction * arg_acheteur, float proposition)
{
	negocie.lock(); // on emp�che d'autres n�gociations simultan�es

	if (proposition > prix)
	{
		acheteur = arg_acheteur;
	}

	negocie.unlock(); // on d�verrouille
}

void City::achatFinTour()
{
	if (acheteur != NULL)
	{
		acheteur->acheterVille(this, prix);
		acheteur = NULL;
	}
}

void City::estimate()
{
	prix = budget + bonheur - (1-happinessPart())*population;
}

// Accesseurs
const string City::getName() const
{
	return nom;
}

Faction* City::getFaction() const
{
	return faction;
}

vector<City*> City::get_Neighbour() const
{
	int sizex = worldMap->size();
	int sizey = worldMap->begin()->size();
	vector<City*> voisines; // vector contenant les villes voisines : Nord, Ouest, Est, Sud
	City* temp(NULL);
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			if (i*i != j*j)
			{
				if ( (0 <= coord_x + i && coord_x + i < sizex) && (0 <= coord_y + j && coord_y + j < sizey) )
				{
					temp = worldMap[0][coord_x + i][coord_y + j];
					if (temp->getFaction() != faction) voisines.push_back(temp); // on ajoute la ville aux voisines
				}
			}
		}
	}
	return voisines;
}

float City::getFood() const
{
	return nourriture;
}

float City::getEnergy() const
{
	return energie;
}

float City::getWallet() const
{
	return budget;
}

float City::getPrice() const
{
	return prix;
}

Faction * City::getAcheteur() const
{
	return acheteur;
}

void City::set_Coord(int posx, int posy)
{
	coord_x = posx;
	coord_y = posy;
}

void City::set_Map(vector<vector<City*>>* arg_map)
{
	worldMap = arg_map;
}

void City::set_Faction(Faction * newFaction)
{
	faction = newFaction;
}


// Setters
int City::set_Farmers(int toSet)
{
	int freePopulation(population-energizer-traders);
	int maximum(100);
	if(toSet < 0 || toSet > maximum || toSet > freePopulation) return ATTRIBUTION_ERROR;
	farmers = toSet;
	return ATTRIBUTION_OK;
}

int City::set_Energize(int toSet)
{
	int freePopulation(population-farmers-traders);
	int maximum(100);
	if(toSet < 0 || toSet > maximum || toSet > freePopulation) return ATTRIBUTION_ERROR;
	energizer = toSet;
	return ATTRIBUTION_OK;
}

int City::set_Traders(int toSet)
{
	int freePopulation(population-farmers-energizer);
	int maximum(100);
	if(toSet < 0 || toSet > maximum || toSet > freePopulation) return ATTRIBUTION_ERROR;
	traders = toSet;
	return ATTRIBUTION_OK;
}