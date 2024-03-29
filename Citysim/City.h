#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
using namespace std;

#include "RandomName.h"
#include "Skill.h"
#include "pileContrats.h"
#include "Display.h"

// Définitions des retours d'erreurs
#define ATTRIBUTION_OK 0 // réussite d'attribution d'employés
#define ATTRIBUTION_ERROR 1 // erreur d'attribution d'employés

// Définitions des maximums d'employés
#define MAXIMUM_SETTLERS 15000

// Valeurs par défaut en début de partie
#define DEFAULT_POPULATION 100
#define DEFAULT_FOOD 1000
#define DEFAULT_ENERGY 2000
#define DEFAULT_WALLET 10000
#define DEFAULT_FARMERS 0
#define DEFAULT_TRADERS 0
#define DEFAULT_ENERGIZER 0

// Valeurs par défaut des coefficients
#define SALARY 2.5

// Prototype de classes
class Faction;
class Contrat;
class Grille;

class City
{
public:
	static int cityNumber;
	// Constructeurs
	City();
	City(string name,
		float arg_bonheur = 1,
		int arg_population = DEFAULT_POPULATION,
		float arg_nourriture = DEFAULT_FOOD,
		float arg_energie = DEFAULT_ENERGY,
		float arg_budget = DEFAULT_WALLET,
		int arg_color = DEFAULT_COLOR);
	void initiate(string arg_name = RandomName::generate(), int arg_color = DEFAULT_COLOR);
	// Destructeur
	virtual ~City();

	// Interface
	void presentation(); // fiche technique de la ville (caractéristiques)
	void print_ville_name(); // afficher nom de la ville
	void drawMyBuilding(int x, int y); // dessine la ville
	virtual void working(); // affichage des employés
	virtual void turn() = 0; // tour de jeu

	// Croissance de la ville
	void growth(); // appel des fonctions de croissance
	void energyGrowth(float coeffHappy); // croissance de l'énergie
	void foodGrowth(float coeffHappy); // croissance de la nourriture
	void populationGrowth(float coeffHappy); // croissance de la population
	void happinessGrowth(float coeffHappy); // croissance du bonheur
	float production(); // mesure de la production financière

	// Estimations et mesures
	float happinessPart(); // Part de population heureuse (coefficient)
	float salary(); // calcul du salaire total de la ville
	float realSalary(); // salaire réel de la ville

	// Gestion de la ville
	int generalSettingEmployees(int arg_farmers, int arg_traders, int arg_energizer); // setter pour tous les employés
	int setEmployees(int toSet, int &employee, int tree); // setter d'employés
	int set_Farmers(int toSet); // indiquer le nombre de fermiers actifs
	int set_Energize(int toSet); // indiquer le nombre de producteur d'énergie actifs
	int set_Traders(int toSet); // indiquer le nombre de traders actifs

	// Contrats
	void proposerContrat(int duration, float arg_cost, float arg_nourriture, float arg_energie); // configurer une proposition de contrat
	void resolveContract(float arg_food, float arg_energia, float arg_cost); // résolution de contrat, modification de la ressource concernée
	bool signerContrat(City* with, float marchandises[][3][3]); // envoyer une demande pour signer le contrat proposé par la ville "with"
	bool accorderContrat(City* with, float marchandises[][3][3]); // valider le contrat pour la ville cliente
	void contratsVoisine(City* voisine); // lister les contrats de la ville indiquées
	virtual void gererAccords() = 0; // signer accords avec les autres villes
	vector<vector<vector<float>>> get_Contracts(); // récupère la liste des contrats proposés par la ville
	vector<vector<vector<float>>> neighboorContracts(); // récupère la liste des contrats proposés par les villes voisines

	// Conquete
	bool acheterVille(City *achetee, float prix = 10); // fonction d'achat d'une ville
	void propositionRachat(Faction *arg_acheteur, float proposition); // fonction de proposition d'achat de la ville
	virtual void achatFinTour(); // fonction de fin de tour pour le rachat de la ville
	void estimate(); // estimation du prix de la ville

	// Accesseurs
	const string getName() const; // Accesseur de nom
	Faction* getFaction() const; // Accesseur de faction
	vector<City*> get_Neighbour() const; // Accesseur de voisins 
	float getFood() const;
	float getEnergy() const;
	float getWallet() const;
	float getPrice() const; // Accesseur de prix de vente
	Faction* getAcheteur() const; // Accesseur d'acheteur
	int getPopulation() const; // Accesseur de la population
	Grille* getGame() const; // accès grille
	pileContrats** getPile(); // accès pile

	// Setters
	void set_Coord(int posx, int posy); // setter de position
	void set_Faction(Faction *newFaction); // setter de la faction
	void set_Game(Grille *grid); // setter de game
	void setColor(int arg_color); // set la couleur

	// Fin de partie
	virtual void victory() = 0;
	virtual void defeat() = 0;

private:
	
	// Attributes
	string nom; // nom de la ville
	Faction *faction; // faction de la ville
	int id; // id de la ville
	int coord_x; // coordonnée de la ville en x
	int coord_y; // coordonnée de la ville en y

	int population; // population totale
	int farmers; // fermiers
	int energizer; // producteurs d'énergie
	int traders; // employés dans la finance

	float nourriture; // stock de nourriture
	float energie; // stock d'énergie
	float bonheur; // bonheur accumulé

	mutex negocie;
	float prix; // prix minimum de la ville (estimé en début de tour)
	Faction* acheteur; // faction proposant le prix le plus élevé pour la ville

	Grille *game;

	mutex contractActions;
	pileContrats *propositionsContrats;

	/*
	Skill skillFood; // arbre de compétences : nourriture
	Skill skillEnergy; // arbre de compétences : énergie
	Skill skillEconomy; // arbre de compétences : économie
	*/
};
