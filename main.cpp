/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: sagg2
 *
 * Created on 24 de noviembre de 2024, 06:26 PM
 */

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <algorithm>
#include <map>

#define CANT_CARTAS_DECK 20
#define TOTAL_CARTAS 231
#define NITERACIONES 5
#define TSELECCION 0.5
#define TMUTCON 0.5
#define NIND 40

#define Prom_acciones 2

using namespace std;

// Headers, luego elimino :v
void generaPoblacionInicial(vector<vector<int>> &poblacion);
void muestraPoblacion(vector<vector<int>> &poblacion);
void agregaInicial(vector<vector<int>>& poblacion);
void cargaruleta(vector<int> &supervivencia,int *ruleta);
void calculasupervivencia(vector<vector<int>> &poblacion,
    vector<int> &supervivencia);
void seleccion(vector<vector<int>> &padres,vector<vector<int>> &poblacion);
double calculaFitness(vector<int>& mazo);
int lineasInconclusasCalcular(vector<int> mazo);
int binarySearch(int num, int p, int r, vector<int> &array);
vector<int> cantTiposCalcular(vector<int>& mazo);
int evaluaAcciones(vector<int> &mano);
double accionesPromedioCalcular(vector<int>& mazo);
void asignaMano(vector<int>& mazo,vector<int>& mano);
int compacta(vector<int>cromo);
void generarPoblacion(vector<vector<int>> &poblacion);
bool compara(vector<int>a,vector<int>b);
void muestraMejor(vector<vector<int>> poblacion);
void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> &padres);
bool aberracion(vector<int>& crom);
void muestraInfoDeck(vector<int> deck);
void muestraDeck(vector<int> deck);
int analizaLineas(vector<int> mazo,int& lineasIncompletas,int& lineasParciales, int& lineasCompletas);
void analizoLineaBasico(vector<int>& mazo,int& lineasIncompletas,int& lineasParciales, int& lineasCompletas,int actual);
void analizoLineaParcial(vector<int>& mazo,int& lineasIncompletas,int& lineasParciales,int& actual);

enum ELEMENTO{
	PLANTA,
	FUEGO,
	AGUA,
	ELECTRICO,
	PSIQUICO,
	LUCHA,
	SINIESTRO,
	METAL,
	DRAGON,
	NORMAL,
};

enum CARTA_TIPO{
	POKEMON,
	ITEM,
	PARTIDARIO
};

enum FASE{
	BASICO,
	FASE_1,
	FASE_2
};

enum TIPO_ENTRENADOR{
	DISRUPCION = 1,
	SOPORTE = 2,
	MOTOR_ROBO = 3
};

typedef struct Carta{
    //general
	string nombre;
	
    int tipo_carta;     //CARTA_TIPO
    int elemento;       //ELEMENTO 
    
    //pokemon
	int fase;
	bool fase_final; // para pokemon
    vector<int> sigEvo;

	//partidario y objeto
	int tipo_entrenador; //TIPO_ENTRENADOR

    vector<int> sinergia;

	Carta(){};

	Carta(string _nombre, int _tipo_carta, int _elemento,
          int _fase = 0, bool _fase_final = false, vector<int> _sigEvo = {},
          int _tipo_entrenador = 0, vector<int> _sinergia = {})
        : nombre(_nombre), tipo_carta(_tipo_carta), elemento(_elemento),
          fase(_fase), fase_final(_fase_final), sigEvo(_sigEvo),
          tipo_entrenador(_tipo_entrenador), sinergia(_sinergia) {}
} Carta;
Carta pool[232] =  {Carta("este no es un pokemon",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{2},0,{}),
					Carta("Bulbasaur",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{2},0,{}),
					Carta("Ivysaur",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,false,{3,4},0,{}),
					Carta("Venusaur",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{}),
					Carta("Venusaur ex",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{}),
					Carta("Caterpie",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{6},0,{}),
					Carta("Metapod",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,false,{7},0,{}),
					Carta("Butterfree",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{-ELEMENTO::NORMAL}),
					Carta("Weedle",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{9},0,{}),
					Carta("Kakuna",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,false,{10},0,{}),
					Carta("Beedrill",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{}),
					Carta("Oddish",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{12},0,{}),
					Carta("Gloom",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,false,{13},0,{}),
					Carta("Vileplume",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{}),
					Carta("Paras",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{15},0,{}),
					Carta("Parasect",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Venonat",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{17},0,{}),
					Carta("Venomoth",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Bellsprout",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{19},0,{}),
					Carta("Weepinbell",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,false,{20},0,{}),
					Carta("Victreebel",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_2,true,{},0,{}),
					Carta("Exeggcute",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{22,23},0,{}),
					Carta("Exeggutor",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Exeggutor ex",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Tangela",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,true,{},0,{}),
					Carta("Scyther",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,true,{},0,{}),
					Carta("Pinsir",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,true,{},0,{}),
					Carta("Cottonee",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{28},0,{}),
					Carta("Whimsicott",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Petilil",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{30},0,{}),
					Carta("Lilligant",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{3,4,209}),
					Carta("Skiddo",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::BASICO,false,{32},0,{}),
					Carta("Gogoat",CARTA_TIPO::POKEMON,ELEMENTO::PLANTA,FASE::FASE_1,true,{},0,{}),
					Carta("Charmander",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{34},0,{}),
					Carta("Charmeleon",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,false,{35,36},0,{}),
					Carta("Charizard",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_2,true,{},0,{47}),
					Carta("Charizard ex",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_2,true,{},0,{}),
					Carta("Vulpix",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{38},0,{}),
					Carta("Ninetales",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Growlithe",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{40,41},0,{}),
					Carta("Arcanine",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Arcanine ex",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Ponyta",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{43},0,{}),
					Carta("Rapidash",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Magmar",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,true,{},0,{}),
					Carta("Flareon",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Moltres",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,true,{},0,{}),
					Carta("Moltres ex",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,true,{},0,{35,36,38,45,52}),
					Carta("Heatmor",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,true,{},0,{}),
					Carta("Salandit",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{50},0,{}),
					Carta("Salazzle",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Sizzlipede",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::BASICO,false,{52},0,{}),
					Carta("Centiskorch",CARTA_TIPO::POKEMON,ELEMENTO::FUEGO,FASE::FASE_1,true,{},0,{}),
					Carta("Squirtle",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{54},0,{}),
					Carta("Wartortle",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,false,{55,56},0,{}),
					Carta("Blastoise",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_2,true,{},0,{}),
					Carta("Blastoise ex",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_2,true,{},0,{}),
					Carta("Psyduck",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{58},0,{}),
					Carta("Golduck",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Poliwag",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{60},0,{}),
					Carta("Poliwhirl",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,false,{61},0,{}),
					Carta("Poliwrath",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_2,true,{},0,{}),
					Carta("Tentacool",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{63},0,{}),
					Carta("Tentacruel",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Seel",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{65},0,{}),
					Carta("Dewgong",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Shellder",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{67},0,{}),
					Carta("Cloyster",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Krabby",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{69},0,{}),
					Carta("Kingler",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Horsea",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{71},0,{}),
					Carta("Seadra",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Goldeen",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{73},0,{}),
					Carta("Seaking",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Staryu",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{75,76},0,{}),
					Carta("Starmie",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Starmie ex",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Magikarp",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{78},0,{}),
					Carta("Gyarados",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Lapras",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,true,{},0,{}),
					Carta("Vaporeon",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Omanyte",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,false,{82},0,{}),
					Carta("Omastar",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_2,false,{},0,{}),
					Carta("Articuno",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,true,{},0,{}),
					Carta("Articuno ex",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,true,{},0,{}),
					Carta("Ducklett",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{86},0,{}),
					Carta("Swanna",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Froakie",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{88},0,{}),
					Carta("Frogadier",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,false,{89},0,{28}),
					Carta("Greninja",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_2,true,{},0,{}),
					Carta("Pyukumuku",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,true,{},0,{}),
					Carta("Bruxish",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,true,{},0,{}),
					Carta("Snom",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::BASICO,false,{93},0,{}),
					Carta("Frosmoth",CARTA_TIPO::POKEMON,ELEMENTO::AGUA,FASE::FASE_1,true,{},0,{}),
					Carta("Pikachu",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{95},0,{}),
					Carta("Raichu",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Pikachu ex",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,true,{},0,{}),
					Carta("Magnemite",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{98},0,{}),
					Carta("Magneton",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Voltorb",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{100},0,{}),
					Carta("Electrode",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Electabuzz",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,true,{},0,{}),
					Carta("Jolteon",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Zapdos",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,true,{},0,{}),
					Carta("Zapdos ex",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,true,{},0,{}),
					Carta("Blitzle",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{106},0,{}),
					Carta("Zebstrika",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Tynamo",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{108},0,{}),
					Carta("Eelektrik",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,false,{109},0,{}),
					Carta("Eelektross",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_2,true,{},0,{}),
					Carta("Helioptile",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,false,{111},0,{}),
					Carta("Heliolisk",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::FASE_1,true,{},0,{}),
					Carta("Pincurchin",CARTA_TIPO::POKEMON,ELEMENTO::ELECTRICO,FASE::BASICO,true,{},0,{}),
					Carta("Clefairy",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{114},0,{}),
					Carta("Clefable",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,true,{},0,{}),
					Carta("Abra",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{116},0,{}),
					Carta("Kadabra",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,false,{117},0,{}),
					Carta("Alakazam",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_2,true,{},0,{}),
					Carta("Slowpoke",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{119},0,{}),
					Carta("Slowbro",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,true,{},0,{}),
					Carta("Gastly",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{121},0,{}),
					Carta("Haunter",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,false,{122,123},0,{}),
					Carta("Gengar",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_2,true,{},0,{}),
					Carta("Gengar ex",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_2,true,{},0,{}),
					Carta("Drowzee",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{125},0,{}),
					Carta("Hypno",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,true,{},0,{}),
					Carta("Mr. Mime",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,true,{},0,{}),
					Carta("Jynx",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,true,{},0,{}),
					Carta("Mewtwo",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,true,{},0,{}),
					Carta("Mewtwo ex",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,true,{},0,{}),
					Carta("Ralts",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{131},0,{}),
					Carta("Kirlia",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,false,{132},0,{}),
					Carta("Gardevoir",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_2,true,{},0,{117,122,128,129,136}),
					Carta("Woobat",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{134},0,{}),
					Carta("Swoobat",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,true,{},0,{}),
					Carta("Golett",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::BASICO,false,{136},0,{}),
					Carta("Golurk",CARTA_TIPO::POKEMON,ELEMENTO::PSIQUICO,FASE::FASE_1,true,{},0,{}),
					Carta("Sandshrew",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{138},0,{}),
					Carta("Sandslash",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Diglett",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{140},0,{}),
					Carta("Dugtrio",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Mankey",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{142},0,{}),
					Carta("Primeape",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Machop",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{144},0,{}),
					Carta("Machoke",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,false,{145,146},0,{}),
					Carta("Machamp",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_2,true,{},0,{}),
					Carta("Machamp ex",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_2,true,{},0,{}),
					Carta("Geodude",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{148},0,{}),
					Carta("Graveler",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,false,{149},0,{}),
					Carta("Golem",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_2,true,{},0,{}),
					Carta("Onix",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,true,{},0,{}),
					Carta("Cubone",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{152,153},0,{}),
					Carta("Marowak",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Marowak ex",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Hitmonlee",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,true,{},0,{}),
					Carta("Hitmonchan",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,true,{},0,{}),
					Carta("Rhyhorn",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{157},0,{}),
					Carta("Rhydon",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Kabuto",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,false,{159},0,{}),
					Carta("Kabutops",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_2,true,{},0,{}),
					Carta("Mienfoo",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{161},0,{}),
					Carta("Mienshao",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Clobbopus",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::BASICO,false,{163},0,{}),
					Carta("Grapploct",CARTA_TIPO::POKEMON,ELEMENTO::LUCHA,FASE::FASE_1,true,{},0,{}),
					Carta("Ekans",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{165},0,{}),
					Carta("Arbok",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,true,{},0,{}),
					Carta("Nidoran♀",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{167},0,{}),
					Carta("Nidorina",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,false,{168},0,{}),
					Carta("Nidoqueen",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_2,true,{},0,{}),
					Carta("Nidoran♂",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{170},0,{}),
					Carta("Nidorino",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,false,{171},0,{}),
					Carta("Nidoking",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_2,true,{},0,{175}),
					Carta("Zubat",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{173},0,{}),
					Carta("Golbat",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,true,{},0,{}),
					Carta("Grimer",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{175},0,{175}),
					Carta("Muk",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,true,{},0,{}),
					Carta("Koffing",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::BASICO,false,{177},0,{}),
					Carta("Weezing",CARTA_TIPO::POKEMON,ELEMENTO::SINIESTRO,FASE::FASE_1,true,{},0,{175}),
					Carta("Mawile",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::BASICO,true,{},0,{}),
					Carta("Pawniard",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::BASICO,false,{180},0,{}),
					Carta("Bisharp",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::FASE_1,true,{},0,{}),
					Carta("Meltan",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::BASICO,false,{182},0,{}),
					Carta("Melmetal",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::FASE_1,true,{},0,{}),
					Carta("Dratini",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::BASICO,false,{184},0,{}),
					Carta("Dragonair",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::FASE_1,false,{185},0,{}),
					Carta("Dragonite",CARTA_TIPO::POKEMON,ELEMENTO::METAL,FASE::FASE_2,true,{},0,{}),
					Carta("Pidgey",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{187},0,{}),
					Carta("Pidgeotto",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,false,{188},0,{}),
					Carta("Pidgeot",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_2,true,{},0,{}),
					Carta("Rattata",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{190},0,{}),
					Carta("Raticate",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Spearow",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{192},0,{}),
					Carta("Fearow",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Jigglypuff",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{194,195},0,{}),
					Carta("Wigglytuff",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Wigglytuff ex",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Meowth",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{197},0,{}),
					Carta("Persian",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Farfetch'd",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Doduo",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{200},0,{}),
					Carta("Dodrio",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Lickitung",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Chansey",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Kangaskhan",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Tauros",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Ditto",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Eevee",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{45,80,102},0,{}),
					Carta("Porygon",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Aerodactyl",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Snorlax",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,true,{},0,{}),
					Carta("Minccino",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{211},0,{}),
					Carta("Cinccino",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Wooloo",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::BASICO,false,{213},0,{}),
					Carta("Dubwool",CARTA_TIPO::POKEMON,ELEMENTO::NORMAL,FASE::FASE_1,true,{},0,{}),
					Carta("Helix Fossil",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{81},0,{}),
					Carta("Dome Fossil",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{158},0,{}),
					Carta("Old Amber",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{208},0,{}),
					Carta("Erika",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{-ELEMENTO::PLANTA}),
					Carta("Misty",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{-ELEMENTO::AGUA}),
					Carta("Blaine",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{38,43,44}),
					Carta("Koga",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{175,177}),
					Carta("Giovanni",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{-ELEMENTO::NORMAL}),
					Carta("Brock",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{149,150}),
					Carta("Sabrina",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::DISRUPCION,{-ELEMENTO::NORMAL}),
					Carta("Lt. Surge",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{95,100,101}),
					Carta("Investigacion de Profesores",CARTA_TIPO::PARTIDARIO,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::MOTOR_ROBO,{}),
					Carta("Pocion",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{}),
					Carta("Velocidad X",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{}),
					Carta("Periscopio",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{}),
					Carta("Pokedex",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::SOPORTE,{}),
					Carta("Poke Ball",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::MOTOR_ROBO,{}),
					Carta("Tarjeta Roja",CARTA_TIPO::ITEM,ELEMENTO::NORMAL,FASE::BASICO,false,{},TIPO_ENTRENADOR::DISRUPCION,{})
					};


int main(int argc, char const *argv[])
{	
	
	srand(time(NULL));	
	vector<vector<int>> poblacion(NIND);
	//ingresar un basico
	
	//agregar el basico a cada individuo de la poblacion
	//se escogeran 19 cartas aleatorias

	generaPoblacionInicial(poblacion);
	muestraPoblacion(poblacion);
	
	for(int n = 0;n < NITERACIONES;n++){
		cout << "entre" << endl;
		vector<vector<int>> padres;
		cout << "Generacion: " << n << endl;
		seleccion(padres,poblacion);
		
		mutacion(poblacion,padres);
		generarPoblacion(poblacion);
		muestraPoblacion(poblacion);
		muestraMejor(poblacion);
	} 

	// vector<int> temp = {1,93,230,143,165,180,196,170,3,126,178,38,12,208,57,131,52,191,227,171};
	// muestraInfoDeck(temp);
    return 0;
}

void muestraInfoDeck(vector<int> deck){
	sort(deck.begin(),deck.end());

	cout << "Fitness: " << calculaFitness(deck) << endl;
	muestraDeck(deck);
}

void muestraDeck(vector<int> deck){
	for(int n : deck){
		cout << n << " " << pool[n].nombre << endl;
	}
}

void muestraMejor(vector<vector<int>> poblacion){
    int mejor=0;
    for(int i=0;i<poblacion.size();i++)
        if(calculaFitness(poblacion[mejor])<calculaFitness(poblacion[i]))
            mejor=i;
    
    cout << endl<<"La mejor solucion es:" << calculaFitness(poblacion[mejor])<<endl;
    vector<int> aux = poblacion[mejor];
    sort(aux.begin(),aux.end());
    // for(int i=0;i<poblacion[mejor].size();i++){
    //     cout << left<<setw(5)<<aux[i] << "  "  << setw(15)<<pool[aux[i]].nombre;
    //     if(pool[aux[i]].tipo_carta==POKEMON){
    //         cout << setw(8)<<pool[aux[i]].fase;
    //     }
    //     cout<<endl; 
    // }
	muestraInfoDeck(aux);
    cout << endl << endl;
}

bool compara(vector<int>a,vector<int>b){
    int suma=0,sumb=0;
   
    for(int i=0;i<a.size();i++)
        suma+=calculaFitness(a);
    for(int i=0;i<b.size();i++)
        sumb+=calculaFitness(b);
    return suma>sumb;
}

void generarPoblacion(vector<vector<int>> &poblacion){
    
    sort(poblacion.begin(),poblacion.end(),compara);
    if(poblacion.size()>NIND){
        poblacion.erase(poblacion.begin()+NIND,poblacion.end());
    }
    
}

int compacta(vector<int>cromo){
    int num=0;
    for(int i=0;i<cromo.size();i++)
        num+=pow(2,i)*cromo[i];
    
    return num;  
}


void agregaInicial(vector<vector<int>>& poblacion){

	int id;
	while(1){
		cout << "Ingrese el identificador de un pokemon basico:";
		//cin >> id;
		id = 1;
		if(id<1 || id>232){
			cout << endl << "El identificador no es valido" << endl;
		}else{
			if(pool[id].fase!=FASE::BASICO){
				cout << endl << "El identificador no es de un pokemon basico" << endl;
			}else{
				cout << endl << "Su baraja se construirá usando a " << pool[id].nombre << endl;
				for(int n = 0; n < NIND; n++){ 
					vector<int> deck_posible(CANT_CARTAS_DECK); 
					deck_posible[0] = id;
					poblacion[n] = deck_posible;
				}
				break;
			}
		}
	}
	cout << "sali" << endl;

}

void imprimirVector(vector<int>imprimir){
    for(int i=0; i<imprimir.size();i++){
        cout<<setw(4)<<imprimir[i];
    }
    cout<<endl;
}

//void generaPoblacionInicial(vector<vector<int>> &poblacion) {
//    
//    agregaInicial(poblacion);
//    int i=0;
//    while (i < NIND) { // Luego cambio :v
//        int verificacion[232]{};
//        vector<int>vaux;
//        int j=1;
//        while (j < CANT_CARTAS_DECK) {
//            int cartaCandidata = (rand() % TOTAL_CARTAS) + 1; 
//            if(verificacion[cartaCandidata] < 2) {
//                    verificacion[cartaCandidata]++;
//                    vaux.push_back(cartaCandidata);
//                    j++;
//            }
//        }
//        if(!aberracion(vaux)){
//            for(int j=0;j<vaux.size();j++){
//                poblacion[i].push_back(vaux[j]);
//            }
//            imprimirVector(vaux);
//            imprimirVector(poblacion[i]);
//            i++;
//        }
//    }
//    
//}

void generaPoblacionInicial(vector<vector<int>> &poblacion) {
    agregaInicial(poblacion);
    int i=0;
    while (i < NIND){
        int verificacion[232]{};
        vector<int>vaux;
        for (int j = 1; j < CANT_CARTAS_DECK;) {
            int cartaCandidata = (rand() % TOTAL_CARTAS) + 1; 
            if(verificacion[cartaCandidata] < 2) {
                    verificacion[cartaCandidata]++;
                    vaux.push_back(cartaCandidata);
                    j++;
            }
        }
        if(!aberracion(vaux)){
            for(int j=0;j<vaux.size();j++){
                poblacion[i][j+1] = vaux[j];
            }
            imprimirVector(vaux);
            imprimirVector(poblacion[i]);
            i++;
			cout << i << endl;
        }
    }
	
}

void muestraPoblacion(vector<vector<int>> &poblacion){
	for(int i = 0; i < NIND; i++){
		cout << "Poblacion: " << i + 1 << endl;
		for(int j = 0; j < CANT_CARTAS_DECK; j++){
			cout << poblacion[i][j] << " ";
		}
		cout << " "  << calculaFitness(poblacion[i]);
		cout << endl;
	}
	cout << endl;
}

// 1 2 3 4 5
/*=> 0 0 1 1 2 => Combinación
int arreglo[20] = {49, 50, 97, 11, 50, 160, 71, 50, 9, 170, 56, 199, 29, 48, 52, 158, 37, 142, 168, 1}; */

void calculasupervivencia(vector<vector<int>> &poblacion,
    vector<int> &supervivencia){
    int suma=0;
    
    for(int i=0;i<poblacion.size();i++)
        suma+=calculaFitness(poblacion[i]);
    for(int i=0;i<poblacion.size();i++){
        int fit= round(100*(double)calculaFitness(poblacion[i])/suma);
        supervivencia.push_back(fit);
    }

}

// :v
void cargaruleta(vector<int> &supervivencia,int *ruleta){
    int ind=0;
    int cantInd=0;
    for(int i=0;i<supervivencia.size();i++)
        cantInd+=supervivencia[i];
    cout<<"Cant Ind Ruleta "<<cantInd<<endl;
    for(int i=0;i<supervivencia.size();i++)
        for(int j=0;j<supervivencia[i];j++)
            ruleta[ind++]=i;
}

void seleccion(vector<vector<int>> &padres,vector<vector<int>> &poblacion){
	int ruleta[100]{-1};
	vector<int>supervivencia;
    calculasupervivencia(poblacion,supervivencia);
    cargaruleta(supervivencia,ruleta);
    int nseleccionados= poblacion.size()*TSELECCION; // Parámetro de torneo.        
    for(int i=0;i<nseleccionados;i++){
        int ind=rand()%100;
        if(ruleta[ind]>-1)
            padres.push_back(poblacion[ruleta[ind]]);
            
    } 
} 


void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> &padres){
    int cont=0;
    int nmuta=round(padres[0].size()*TMUTCON);
    for(int i=0;i<padres.size();i++){
        while(cont<nmuta){
            int ind=rand()%padres[0].size();
        	padres[i][ind]=rand() % TOTAL_CARTAS + 1;
            
            cont++;
        }
        if(!aberracion(padres[i]))
            poblacion.push_back(padres[i]);     
    }
}

bool aberracion(vector<int>& crom){
    int arr[TOTAL_CARTAS]{};
    for(int id : crom){
            if(arr[id]>2) return true;
            else arr[id]++;
    }

    //if(lineasInconclusasCalcular(crom)>4) return true;
    //if(cantTiposCalcular(crom).size()>3) return true;
    
	return false;

}


vector<int> cantTiposCalcular(vector<int>& mazo){
	vector<int> tipos;
	for(int id: mazo){
		if(!tipos.empty())sort(tipos.begin(), tipos.end());
		if(pool[id].tipo_carta==CARTA_TIPO::POKEMON && (pool[id].elemento!=ELEMENTO::NORMAL || pool[id].elemento!=ELEMENTO::DRAGON)) {
			if(binarySearch(pool[id].elemento,0,tipos.size()-1,tipos)==-1)
				tipos.push_back(pool[id].elemento);
		}
	}
	return tipos;
}

int binarySearch(int num, int p, int r, vector<int> &array){
	if(p <= r){ // Se puede buscar
		int q = p + (r - p) / 2;
		//cout << "Indide: " << q << endl;
		if(num == array[q])return q;
		else{
			if(num <= array[q])return binarySearch(num, p, q - 1, array);
			else return binarySearch(num, q + 1, r, array);
		}
	}
	else return -1; // Se sobrepasa
}

int binarySearchAlt(int num, int p, int r, vector<int> &array){
	if(p <= r){ // Se puede buscar
		int q = p + (r - p) / 2;
		//cout << "Indide: " << pool[array[q]].nombre <<" " <<q << endl;
		if(num == array[q])return q;
		else{
			if(num > array[q])return binarySearchAlt(num, p, q - 1, array);
			else return binarySearchAlt(num, q + 1, r, array);
		}
	}
	else return -1; // Se sobrepasa
}

//int lineasInconclusasCalcular(vector<int> mazo){// necesito ordenar una copia 
//	sort(mazo.begin(),mazo.end());
//
//	int lineasInconclusas = 0;
//	vector<int> evoluciones;
//	for(int id : mazo){
//		if(pool[id].tipo_carta == CARTA_TIPO::POKEMON && pool[id].fase != FASE::BASICO) lineasInconclusas++;
//
//		if(pool[id].tipo_carta == CARTA_TIPO::POKEMON && pool[id].fase == FASE::BASICO){
//			evoluciones = pool[id].sigEvo;
//			if(!evoluciones.empty()){ // Solo en caso el pokemon cuente con evoluciones.
//				for(int i = 0; i < evoluciones.size(); i++){ // Recorre evoluciones.
//					if(!binarySearch(evoluciones[i], 0, mazo.size() - 1, mazo)){ // No se ha encontrado, rompe bucle.
//						lineasInconclusas++;
//						break;
//					}
//				}
//				// Bucle termina, evoluciones completas, no hay acción.
//			}
//		}
//	}
//
//	return lineasInconclusas;
//}

int buscaEvo(vector<int>& mazo, int actual){
	//cout << "-> Estoy buscando a ";
	for(int n : pool[actual].sigEvo)
		cout << pool[n].nombre << " ";
	cout << endl;
	muestraDeck(mazo);

	for(int n : pool[actual].sigEvo){
		int temp = binarySearchAlt(n,0,mazo.size(),mazo);
		if(temp != -1) return temp;
	}
	return -1;
}

bool buscaLinea(vector<int>& mazo, int& actual){

	while(1){
		if(pool[actual].fase_final) return true;
		int aux = buscaEvo(mazo,actual);
		//cout << "aux: " << aux << endl;
		if(aux == -1) return false;
		actual = aux;
		mazo.erase(mazo.begin()+aux);
	}

}

int lineasInconclusasCalcular(vector<int> mazo){// necesito ordenar una copia 
	sort(mazo.begin(),mazo.end(),[](int a, int b) {return a > b;});
	
	int lineasInconclusas = 0;
	//muestraDeck(mazo);
	//cout <<"--------------------------" << endl;
    int actual = mazo.back();
	mazo.pop_back();

	bool no_encontrado = false;

	while(mazo.size()){
		//cout << "->Actual: " << pool[actual].nombre << endl;
		if(pool[actual].tipo_carta == CARTA_TIPO::POKEMON){
			if(pool[actual].fase == FASE::BASICO){ //agregar indices de fosiles 
				if(pool[actual].fase_final){
					//cout << "-> Este pokemon no evoluciona" << endl;
					actual = mazo.back();
					mazo.pop_back();
				}else{
					if(!buscaLinea(mazo,actual)) {
						//cout << "-> No encontre la linea completa." << endl;
						actual = mazo.back();
						mazo.pop_back();
						lineasInconclusas++;
					}
				}
			}else{
				//cout << "-> Encontre primero una evolucion" << endl;
				actual = mazo.back();
				mazo.pop_back();
				lineasInconclusas++;
			}
		}else{
			//cout << "->Salto esta carta" << endl;
			actual = mazo.back();
			mazo.pop_back();
		}

		//muestraDeck(mazo);
		//cout << "L:" << lineasInconclusas << endl;
		//cout << "-----------------------------------" << endl;
	}
	return lineasInconclusas;
}

int analizaLineas(vector<int> mazo,int& lineasIncompletas,int& lineasParciales, int& lineasCompletas){
	sort(mazo.begin(),mazo.end(),[](int a,int b){return a > b;});

	int actual = mazo.back();
	mazo.pop_back();

	while(mazo.size()){
		if(pool[actual].tipo_carta == CARTA_TIPO::POKEMON){
			if(pool[actual].fase == FASE::BASICO){
				//cout << "-> Actual: " << pool[actual].nombre << endl;
				analizoLineaBasico(mazo,lineasIncompletas,lineasParciales,lineasCompletas,actual);
			}else if(pool[actual].fase == FASE::FASE_1){
				analizoLineaParcial(mazo,lineasIncompletas,lineasParciales,actual);
			}else lineasIncompletas++;
		}
		actual = mazo.back();
		mazo.pop_back();
	}
}

void analizoLineaBasico(vector<int>& mazo,int& lineasIncompletas,int& lineasParciales, int& lineasCompletas,int actual){
	
	if(pool[actual].fase_final){
		lineasCompletas++;
		return;
	}

	int fase1 = buscaEvo(mazo,actual);
	if(fase1 != -1){
		actual = mazo[fase1];
		mazo.erase(mazo.begin()+fase1);
		if(!pool[actual].fase_final){
			int fase2 = buscaEvo(mazo,actual);
			if(fase2 != -1){
				mazo.erase(mazo.begin()+fase2);
				lineasCompletas++;
			}else{
				lineasParciales++;
			}
		}else{
			lineasCompletas++;
			return;
		}
	}else{
		//cout << "-> No encontre el fase 1 asi que busco el fase 2" << endl;
		actual = pool[actual].sigEvo[0];// pasamos a la fase 1 para buscar si la fase 2 esta en el deck
		//cout << "-> Busco la evo de " << pool[actual].nombre << endl;
		if(!pool[actual].fase_final){ // si fase 1 es la fase final estamos ante una lineaIncompleta
			int fase2 = buscaEvo(mazo,actual);
			if(fase2!= -1){
				//cout << "-> Encontre a : " << pool[mazo[fase2]].nombre << endl;
				mazo.erase(mazo.begin()+fase2);
				lineasParciales++;
			}else{
				//cout << "-> No lo encontre" << endl;
				lineasIncompletas++;
			}
		}
	}
}

void analizoLineaParcial(vector<int>& mazo,int& lineasIncompletas,int& lineasParciales,int& actual){

	if(pool[actual].fase_final){
		lineasIncompletas++;
		return;
	}

	int fase2 = buscaEvo(mazo,actual);
	if(fase2 != -1){
		mazo.erase(mazo.begin()+fase2);
		lineasParciales++;
	}else{
		lineasIncompletas++;
	}

}

double accionesPromedioCalcular(vector<int>& mazo){
	double accionesTotales = 0;
	for(int n= 0; n<5; n++){
		vector<int> mano;
		asignaMano(mazo,mano);
		accionesTotales+=evaluaAcciones(mano);
	}
	return accionesTotales/5.0;
}

void asignaMano(vector<int>& mazo,vector<int>& mano){
	vector<bool> usado(mazo.size(), false);
	int randInd;
	for(int n = 0; n<5;n++){
		do{
			randInd = rand() % mazo.size();
		}while(usado[randInd]);
		usado[randInd] = true;
		mano.push_back(mazo[randInd]); 
	}
}

int evaluaAcciones(vector<int> &mano){

	int pokes_basicos = 0;
	int items = 0;
	int partidarios = 0;

	for(int id : mano){
		switch (pool[id].tipo_carta)
		{
		case (CARTA_TIPO::POKEMON):
			if(pool[id].fase == FASE::BASICO) pokes_basicos++;
			break;
		case (CARTA_TIPO::ITEM):
			items++;
			break;
		case (CARTA_TIPO::PARTIDARIO):
			partidarios++;
			break;
		default:
			break;
		}
	}

	if(pokes_basicos>4) pokes_basicos = 4;
	if(partidarios>1) partidarios = 1;

	return pokes_basicos+items+partidarios;
}

double calculaSinergia(vector<int> mazo){
	
	sort(mazo.begin(),mazo.end());

	int puntosSinergias = 0;
	for(int id : mazo){
		switch (pool[id].tipo_carta)
		{
		case (CARTA_TIPO::POKEMON):
			if(pool[id].sinergia.size()){
				if(pool[id].sinergia[0]>0){
					for(int idSinergia : pool[id].sinergia){
						if(binarySearch(idSinergia,0,mazo.size()-1,mazo)!=-1) puntosSinergias++;
					}
				}else{
					int tipo = -pool[id].sinergia[0];
					if(tipo == ELEMENTO::NORMAL) {
						puntosSinergias++;
					}else{
						vector<int> tiposDelDeck = cantTiposCalcular(mazo);
						if(binarySearch(tipo,0,mazo.size()-1,mazo)!=-1) puntosSinergias++;
					}
					
				}
			}
			break;
		case (CARTA_TIPO::ITEM):
			puntosSinergias+=pool[id].tipo_entrenador;
			break;
		case (CARTA_TIPO::PARTIDARIO):
			if(pool[id].sinergia.size()){	// los puntos de entrenador solo puede reclamarlos si cumple primero la sinergia 
				if(pool[id].sinergia[0]>0){
					int tempSinergia = 0;
					for(int idSinergia : pool[id].sinergia){
						if(binarySearch(idSinergia,0,mazo.size()-1,mazo)!=-1) tempSinergia++;
					}
					if(tempSinergia) tempSinergia+=pool[id].tipo_entrenador;
					puntosSinergias+=tempSinergia;
				}else{
					int tipo = -pool[id].sinergia[0];
					if(tipo == ELEMENTO::NORMAL) {
						puntosSinergias++;
						puntosSinergias+=pool[id].tipo_entrenador;
					}else{
						vector<int> tiposDelDeck = cantTiposCalcular(mazo);
						if(binarySearch(tipo,0,mazo.size()-1,mazo)!=-1) {
							puntosSinergias++;
							puntosSinergias+=pool[id].tipo_entrenador;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return puntosSinergias;
}

double calculaFitness(vector<int>& mazo){
	int cantTipos = cantTiposCalcular(mazo).size();
	//int lineasInconclusas = lineasInconclusasCalcular(mazo);
	double sinergia = calculaSinergia(mazo);
	double accionesPromedio = accionesPromedioCalcular(mazo);
	int lineasIncompletas = 0;
	int lineasCompletas = 0;
	int lineasParciales = 0;
	analizaLineas(mazo,lineasIncompletas,lineasParciales,lineasCompletas);

	cout << "S: " << sinergia << " A: " <<accionesPromedio << " T: " <<cantTipos << endl;
	cout << "Incompletas: " << lineasIncompletas << " Parciales: " << lineasParciales << " Completas: " << lineasCompletas << endl;

	//return  (sinergia*accionesPromedio)/(cantTipos*(lineasInconclusas != 0 ? lineasInconclusas : 1));
	return (sinergia+accionesPromedio+(2*lineasCompletas+1))+(lineasParciales+1)/(cantTipos+(lineasIncompletas+1));
}