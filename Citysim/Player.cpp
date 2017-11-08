#include "stdafx.h"
#include <iostream>
using namespace std;
#include "Player.h"
#include "Faction.h"

Player::Player():City::City()
{
}


Player::~Player()
{
}

void Player::turn()
{
	presentation();
	working();
	
	if (ask("Changer les employes (o/n) ?"))
	{
		setEmployes("de fermiers", &City::set_Farmers);
		setEmployes("de traders", &City::set_Traders);
		setEmployes("d'ingenieurs", &City::set_Energize);
	}
	working();

	buy();

	growth(); // croissance de la ville en fin de tour
}

void Player::setEmployes(string jobName, int (City::*setter)(int))
{
	int assigned(0);
	int attribution(ATTRIBUTION_OK);
	do
	{
		do
		{
			cout << "Saisir le nombre " << jobName << " (-1 pour ignorer) : " << endl;
			cin.clear();
			cin.ignore(INT_MAX,'\n');
			cin >> assigned;
		} while (cin.fail() || assigned < -1);
		if (assigned > -1)
		{
			attribution = (this->*setter)(assigned);
		}
		cout << endl;
	} while (attribution == ATTRIBUTION_ERROR);
}

bool Player::ask(string question)
{
	char choice;
	do
	{
		cout << question << endl;
		cin >> choice;
	} while (choice != 'o' && choice != 'n' && choice != 'O' && choice != 'N');
	return (choice == 'o' || choice == 'O');
}

void Player::buy()
{
	vector<City*> voisines;
	int i(0);
	int choice(0);
	int size(0);
	float price(0); // prix d'achat propos�
	if (ask("Acheter une ville (o/n) ?"))
	{
		voisines = getFaction()->getNeighbourhood();
		size = voisines.size();
		for each (City* town in voisines)
		{
			cout << "Index : " << i << " -> " << ends;
			town->presentation();
			cout << endl;
			++i;
		}
		do
		{
			cout << "Saisir l'index de la ville a acheter : " << ends;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cin >> choice;
		} while (cin.fail() || choice < 0 || choice > size-1);
		do
		{
			cout << "Prix propose : " << ends;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cin >> price;
		} while (cin.fail() || price < 1);
		acheterVille(voisines[choice], price);
		cout << "Proposition de rachat envoyee !" << endl;
	}
}

void Player::victory()
{
	cout << "Le monde vous appartient !" << endl;
	cout << "Victoire !" << endl;
}

void Player::achatFinTour()
{
	cout << "Vous avez �t� rachet� !" << endl;
	cout << "Partie termin�e" << endl;
	exit(EXIT_SUCCESS);
}