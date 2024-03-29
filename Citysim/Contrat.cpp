#include "stdafx.h"
#include "Contrat.h"

#include "City.h"

Contrat::Contrat() :
	timeLeft(1),
	villeA(NULL),
	villeB(NULL)
{
	for (int i = 0; i < MERCHANDISES_LINES; i++)
	{
		for (int j = 0; j < MERCHANDISES_COLUMNS; j++)
		{
			marchandises[i][j] = 0;
		}
	}
}

Contrat::Contrat(int duration, float arg_cost, City* desserve, City* client, float food, float energia):
	timeLeft(duration),
	villeA(desserve),
	villeB(client)
{
	marchandises[0][1] = food;
	if (food < 0)
	{
		marchandises[0][0] = -1;
		marchandises[0][2] = 1;
	}
	else
	{
		marchandises[0][0] = 1;
		marchandises[0][2] = -1;
	}
	marchandises[1][1] = energia;
	if (energia < 0)
	{
		marchandises[1][0] = -1;
		marchandises[1][2] = 1;
	}
	else
	{
		marchandises[1][0] = 1;
		marchandises[1][2] = -1;
	}
	marchandises[2][1] = arg_cost;
	if (arg_cost < 0)
	{
		marchandises[2][0] = -1;
		marchandises[2][2] = 1;
	}
	else
	{
		marchandises[2][0] = 1;
		marchandises[2][2] = -1;
	}
}

Contrat::~Contrat()
{
}

void Contrat::resolveContract()
{
	if (!isClosed() && villeA != NULL && villeB != NULL) // si toujours en cours
	{
		villeA->resolveContract(marchandises[0][0] * marchandises[0][1],
			marchandises[1][0] * marchandises[1][1],
			marchandises[2][0] * marchandises[2][1]);
		villeB->resolveContract(marchandises[0][2] * marchandises[0][1],
			marchandises[1][2] * marchandises[1][1],
			marchandises[2][2] * marchandises[2][1]);
		--timeLeft;
	}
}

bool Contrat::isClosed()
{
	if (timeLeft > 0) return false;
	return true;
}

void Contrat::set_client(City * client)
{
	if (villeB == NULL)	villeB = client;
}

bool Contrat::isThat(float arg_marchandises[][MERCHANDISES_LINES][MERCHANDISES_COLUMNS])
{
	for (int i = 0; i < MERCHANDISES_LINES; i++)
	{
		for (int j = 0; j < MERCHANDISES_COLUMNS; j++)
		{
			if (arg_marchandises[0][i][j] != marchandises[i][j]) return false;
		}
	}
	return true;
}

vector<vector<float>> Contrat::presentate()
{
	vector<vector<float>> temp;
	for (int i = 0; i < MERCHANDISES_LINES; i++)
	{
		temp.push_back(vector<float>());
		for(int j=0; j< MERCHANDISES_COLUMNS;j++)
		{
			temp[i].push_back(marchandises[i][j]);
		}
	}
	return temp;
}

City * Contrat::getCityA() const
{
	return villeA;
}

City * Contrat::getCityB() const
{
	return villeB;
}
