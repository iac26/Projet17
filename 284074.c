/**
 * \file		284074.c
 * \version		1.6
 * \date		2017-12-01
 * \author		Iacopo Sprenger (284074)
 * \brief		Programme pour le projet du cours CS-111(c)
 *              
 */

// *******************************************************************
// 		inclusion de fichiers en-tête avec la directives include

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// *******************************************************************
//			   Symboles définis avec la directive define
#define NB_PARAM_PERS 		5
#define MIN_WORLD_SIZE 		1
#define MIN_NB_PERS			0
#define MIN_NB_SIM 			0
#define COORDS 				2
#define NB_POSSIBLE_TARGETS 9
#define NB_POSSIBLE_MOVES 	8
#define MAX_CYCLES 			200
#define MODIFIER_SIZE 		9
#define PREMIER_ELEMENT 	1
#define VRAI 				1
#define FAUX 				0


//             Symboles définies avec l'instruction enum
enum{X, Y, BUT_X, BUT_Y, ETAT};
enum{N, C, V, I};

// *******************************************************************
//						Functions declaration

// ########   ne pas modifier ces déclarations de fonctions    #######

/**
 * \brief	Si nbSim n'est pas strictement positif
 * \param   nbSim		la valeur invalide de nbSim
 */
static void erreur_nbSim(int nbSim);

/**
 * \brief	Si la taille du monde n'est pas strictement supérieure à 1
 * \param 	n	   		la valeur invalide
 */
static void erreur_taille_monde(int n);

/**
 * \brief	Si le nombre de personnes n'est pas dans [2, n*n]
 * \param 	nbP		la valeur invalide
 * \param 	n		la taille du monde
 */

static void erreur_nbP(int nbP, int n);

/**
 * \brief	Si un indice de position ou de but n'est pas dans [0, n-1]
 * \param 	indice	             la première valeur d'indice invalide
 * \param 	indicePersonne		 l'indice de la personne dans [0, nbP-1]
 */

static void erreur_indice_ligne_colonne(int indice, int indicePersonne);

/**
 * \brief	Si deux personnes ont la même position
 * \param 	indiceP_A	l'indice de la premiere personne dans [0, nbP-1]
 * \param 	indiceP_B	l'indice de la seconde  personne dans [0, nbP-1]
 */
static void erreur_superposition(int indiceP_A, int indiceP_B);

/**
 * \brief	Lecture des parametres verbose et affichage
 */
static int lecture_affichage(void);

/**
 * \brief	Lecture du parametre nbSim (nombre de simulations)
 */
static int lecture_nbSim(void);

/**
 * \brief	Lecture du parametre worldSize (taille du monde)
 */
static int lecture_worldSize(void);

/**
 * \brief	Lecture du parametre nbPers (nombre de personnes)
 * \param	worldSize	taille du monde
 */
static int lecture_nbPers(int worldSize);

/**
 * \brief	Lecture de la position et du but de chaque personne
 * \param	worldSize	taille du monde
 * \param	nbPers		nombre de personnes
 * \param 	people		tableau dans lequel seront stoquees les personnes
 */
static int lecture_personnes(int worldSize, int nbPers,
							 int people[nbPers][NB_PARAM_PERS]);

/**
 * \brief	gere le rebouclement si une personne sort du monde 
 * \param	worldSize	taille du monde
 * \param 	pos			La coordonnee a gerer
 */
static int rebouclement(int worldSize, int pos);

/**
 * \brief	affiche la grille du monde
 * \param	worldSize	taille du monde
 * \param 	len			longeur du tableau people
 * \param 	people		tableau contenant toutes les personnes et leurs parametres
 */
static void print_world(int worldSize, int len, int people[len][NB_PARAM_PERS]);

/**
 * \brief	teste la contamination d'une personne avec ses voisins
 * \param	modifier	tableau des modificateurs de direction
 * \param	worldSize	taille du monde
 * \param 	len			longeur du tableau people
 * \param 	people		tableau contenant toutes les personnes et leurs parametres 
 * \param 	index		indice de la personne a tester
 */

static void test_contamination(int modifier[MODIFIER_SIZE][COORDS], int worldSize,
							   int len, int people[len][NB_PARAM_PERS], int index);
										
/**
 * \brief	teste l'incubation d'une personne
 * \param 	person		tableau contenant les parametres d'une seule personne
 */
static void test_incubation(int person[NB_PARAM_PERS]);

static int test_fin(int len, int people[len][NB_PARAM_PERS]);

/**
 * \brief	teste di une personne a atteint son but
 * \param 	person		tableau contenant les parametres d'une seule personne
 */
static int but_check(int person[NB_PARAM_PERS]);

/**
 * \brief	calcule un nouveau but pour une personne
 * \param	worldSize	taille du monde
 * \param 	person		tableau contenant les parametres d'une seule personne
 */
static void new_but(int worldSize, int person[NB_PARAM_PERS]);

/**
 * \brief	calcule le meilleur mouvement pour atteindre le but
 * \param	next_move	tableau contenant le prochain deplacement
 * \param	worldSize	taille du monde
 * \param 	person		tableau contenant les parametres d'une seule personne
 */
static void move_find(int next_move[COORDS], int worldSize,
					 int person[NB_PARAM_PERS]);
/**
 * \brief	teste si un deplacement est possible
 * \param	next_move	tableau contenant le prochain deplacement
 * \param	worldSize	taille du monde
 * \param 	len			longeur du tableau people
 * \param 	people		tableau contenant les personnes et leurs parametres 
 * \param 	index		indice de la personne a tester
 */
static int move_possible(int next_move[COORDS], int worldSize, int len,
						 int people[len][NB_PARAM_PERS], int index);

/**
 * \brief	effectue le mouvement d'une personne
 * \param	modifier	tableau des modificateurs de direction
 * \param	worldSize	taille du monde
 * \param 	person		tableau contenant les parametres d'une seule personne
 */
static void move(int next_move[COORDS], int worldSize, int person[NB_PARAM_PERS]);

/**
 * \brief	teste si un deplacement est possible et en trouve un nouveau
 * \param	next_move	tableau contenant le prochain deplacement
 * \param	modifier	tableau des modificateurs de direction
 * \param	worldSize	taille du monde
 * \param 	len			longeur du tableau people
 * \param 	people		tableau contenant les personnes et leurs parametres 
 * \param 	index		indice de la personne a tester
 */
static void blocage(int next_move[COORDS], int modifier[MODIFIER_SIZE][COORDS],
					int worldSize, int len, int people[len][NB_PARAM_PERS],
					int index);
/**
 * \brief	trie un tableau de longeur len
 * \param 	len			longeur du tableau a trier
 * \param 	tab			tableau a trier
 */
static void sort(int len, int tab[len]);

/**
 * \brief	effectue une simulation
 * \param	modifier		tableau des modificateurs de direction
 * \param	affichage		booleen determinant l'affichage de la grille
 * \param	worldSize		taille du monde
 * \param	nbPers			nombre de personnes
 * \param 	people_original	tableau contenant toutes les personnes
 * \param 	blocker			limite du nombre de personnes
 */
static int simulation(int modifier[MODIFIER_SIZE][COORDS], int affichage,
					  int worldSize, int nbPers,
					  int people_original[nbPers][NB_PARAM_PERS], int blocker);

/**
 * \brief	calcule la mediane des valeurs d'un tableau
 * \param 	len			longeur du tableau
 * \param 	tab			tableau duquel il faut calculer la mediane des membres
 */
static double median(int len, int tab[len]);

// *******************************************************************
//						Global variable

/** variable booléenne déterminant l'affichage des messages d'invitation */
static int	verbose;

// *******************************************************************

// ############################ END ##################################

//						       MAIN

int main(void) {
	int modifier[MODIFIER_SIZE][COORDS] = {
		{0,0},{0,-1},{0,1},{-1,0},{1,0},{1,-1},{1,1},{-1,1},{-1,-1}
	};
	srand(time(NULL));
	int affichage, nbSim, nbPers, worldSize;
	affichage = lecture_affichage();
	if (!(nbSim = lecture_nbSim()))
		return EXIT_FAILURE;
	if (!(worldSize = lecture_worldSize()))
		return EXIT_FAILURE;
	if (!(nbPers = lecture_nbPers(worldSize)))
		return EXIT_FAILURE;
	int people[nbPers][NB_PARAM_PERS];
	if(!lecture_personnes(worldSize, nbPers, people))
		return EXIT_FAILURE;
	int simulations[nbSim];
	for(int personnes = nbPers; personnes > 1; personnes--) {
		for(int contexte = 0; contexte < personnes-1; contexte++) {
			if (contexte != 0) {
				people[contexte][ETAT] = V;
			}
			for(int sim = 0; sim < nbSim; sim++){
				simulations[sim] = simulation(modifier, affichage,
											  worldSize, nbPers, people, personnes);
				if (verbose)
					printf("# simulation finie!\n");
			}
			double valeur_mediane = median(nbSim, simulations);
			double densite = (double)personnes/(worldSize*worldSize);
			double taux_vaccination = (double)contexte/(double)personnes;
			if (verbose)
				printf("# toutes les simulations pour un contexte finies!\n");
			printf("%-10g %-10g %-10g\n", densite, taux_vaccination, valeur_mediane);
		}
		if (verbose)
			printf("# touts les contextes pour une densité finies!\n");
		printf("\n");
		for(int i = PREMIER_ELEMENT; i < personnes-1; i++)
			people[i][ETAT] = N;
	}	
	return EXIT_SUCCESS;
}


// *******************************************************************

// ################## Ne pas modifier ces fonctions ##################
//====================================================================//
//																	  //
//				***		 Fonctions d'erreurs   	***					  //
//																	  //
//					  /!\ NE PAS MODIFIER /!\						  //
//																	  //
//====================================================================//

static void erreur_nbSim(int nbSim)
{
	printf("nbSim (=%d) ne valide pas nbSim > 0 !\n", nbSim);
}

static void erreur_taille_monde(int n)
{
	printf("n (=%d) ne valide pas n > 1 !\n", n);
}

static void erreur_nbP(int nbP, int n)
{
	printf("nbP (=%d) ne valide pas nbP > 1 ET nbP <= %d !\n", nbP, n*n);
}

static void erreur_indice_ligne_colonne(int indice, int indicePersonne)
{
	printf(	"indice incorrect %d de ligne ou colonne de la personne d'indice %d !\n", 
			indice, indicePersonne);
}

static void erreur_superposition(int indiceP_A, int indiceP_B)
{
	// s'assure d'avoir les indices A et B dans l'ordre croissant
	if(indiceP_B > indiceP_A) 
	{
		int temp = indiceP_A;
		indiceP_A = indiceP_B;
		indiceP_B = temp;
	}
	
	printf("les personnes d'indices %d et %d se superposent !\n", 
		   indiceP_A, indiceP_B);
}

static int lecture_affichage(void) {
	int affichage;
	scanf("%d", &verbose);
	if (verbose)
		printf("# afficher la grille ? \n");
	scanf("%d", &affichage);
	return affichage;
}

static int lecture_nbSim(void) {
	int nbSim;
	if (verbose)
		printf("# nombre de simulations ? \n");
	scanf("%d", &nbSim);
	if (!(nbSim > MIN_NB_SIM)) {
		erreur_nbSim(nbSim);
		return FAUX;
	}
	return nbSim;
}

static int lecture_worldSize(void) {
	int worldSize;
	if (verbose)
		printf("# taille du monde ? \n");
	scanf("%d", &worldSize);
	if (!(worldSize > MIN_WORLD_SIZE)) {
		erreur_taille_monde(worldSize);
		return FAUX;
	}
	return worldSize;
}

static int lecture_nbPers(int worldSize) {
	int nbPers;
	if (verbose)
		printf("# nombre de personnes ? \n");
	scanf("%d", &nbPers);
	if (!((nbPers > MIN_NB_PERS) && (nbPers <= (worldSize*worldSize)))) {
		erreur_nbP(nbPers, worldSize);
		return FAUX;
	}
	return nbPers;
}
	
static int lecture_personnes(int worldSize, int nbPers,
							int people[nbPers][NB_PARAM_PERS]){
	int i, j;
	if (verbose)
		printf("# entrez les personnes : \n");
	for (i = 0; i < nbPers; i++) {
		scanf(	"%d %d %d %d",
				&people[i][X], &people[i][Y],
				&people[i][BUT_X], &people[i][BUT_Y]);
		people[i][ETAT] = N;
		for (j = 0; j < NB_PARAM_PERS; j++)
			if (!(people[i][j] < worldSize)) {
				erreur_indice_ligne_colonne(people[i][j], i);
				return FAUX;
			}
		for (j = 0; j < i; j++)
			if ((people[i][X] == people[j][X]) && (people[i][Y] == people[j][Y])) {
				erreur_superposition(j, i);
				return FAUX;
			}
	}
	people[0][ETAT] = C;
	return VRAI;
}

static int rebouclement(int worldSize, int pos){
	if (pos < 0)
		pos += worldSize;
	if (pos >= worldSize)
		pos -= worldSize;
	return pos;
}

static void print_world(int worldSize, int len, int people[len][NB_PARAM_PERS]) {
	char world[worldSize][worldSize];
	int i, j, k;
	for (i = 0; i < worldSize; i++)
		for (j = 0; j < worldSize; j++)
			world[i][j] = ' ';		
	for (i = 0; i < len; i++) {
			world[people[i][BUT_X]][people[i][BUT_Y]] = 'b';
		}
	for (i = 0; i < len; i++) {
			if (people[i][ETAT]==N) world[people[i][X]][people[i][Y]] = 'N';
			if (people[i][ETAT]==C) world[people[i][X]][people[i][Y]] = 'C';
			if (people[i][ETAT]==V) world[people[i][X]][people[i][Y]] = 'V';
			if (people[i][ETAT]==I) world[people[i][X]][people[i][Y]] = 'C';
		}
	for (i = 0; i < worldSize; i++) {
		printf("#");
		for (k = 0; k < worldSize; k++)
			printf("--");
		printf("-\n");
		printf("#");
		for (j = 0; j < worldSize; j++) {
			printf(":%c", world[i][j]);
		}
		printf(":\n");
	}
	printf("#");
	for (k = 0; k < worldSize; k++)
		printf("--");
	printf("-\n#\n");
}

static void test_contamination(int modifier[MODIFIER_SIZE][COORDS], 
							   int worldSize, int len, 
							   int people[len][NB_PARAM_PERS], int index){
	int i = index;
	for (int j = 0; j < len; j++) {
		for (int k = PREMIER_ELEMENT; k < MODIFIER_SIZE; k++) {
			if ((rebouclement(worldSize, people[i][X] + modifier[k][X]) == 
				 people[j][X]) &&
				(rebouclement(worldSize, people[i][Y] + modifier[k][Y]) ==
				 people[j][Y])) {
				if ((people[i][ETAT] == N) && (people[j][ETAT] == C))
					people[i][ETAT] = I;
				if ((people[i][ETAT] == C) && (people[j][ETAT] == N))
					people[j][ETAT] = I;
			}
		}
	}
}

static void test_incubation(int person[NB_PARAM_PERS]){
	if (person[ETAT] == I)
		person[ETAT] = C;
}

static int test_fin(int len, int people[len][NB_PARAM_PERS]){
	for (int i = 0; i < len; i++) {
		if (people[i][ETAT] == N)
			return VRAI;
	}
	return FAUX;
}

int but_check(int person[NB_PARAM_PERS]) {
	if ((person[X] == person[BUT_X]) && (person[Y] == person[BUT_Y])) {
		return VRAI;
	} else
		return FAUX;
}

static void new_but(int worldSize, int person[NB_PARAM_PERS]) {
	int new_target[COORDS];
	do {
		new_target[X] = rand() % worldSize;
		new_target[Y] = rand() % worldSize;
	}while((new_target[Y] == person[BUT_Y]) && (new_target[X] == person[BUT_X]));
		person[BUT_X] = new_target[X];
		person[BUT_Y] = new_target[Y];
}

static void move_find(int next_move[COORDS], int worldSize,
					 int person[NB_PARAM_PERS]) {
	int dist_interne, dist_externe;
	dist_interne = abs(person[X] - person[BUT_X]);
	dist_externe = worldSize - dist_interne;
	next_move[X] = dist_interne == 0 ? 0 : (person[X] - person[BUT_X])/dist_interne;
	if (dist_interne < dist_externe)
		next_move[X] *= -1;
	dist_interne = abs(person[Y] - person[BUT_Y]);
	dist_externe = worldSize - dist_interne;
	next_move[Y] = dist_interne == 0 ? 0 : (person[Y] - person[BUT_Y])/dist_interne;
	if (dist_interne < dist_externe)
		next_move[Y] *= -1;
}

static int move_possible(int next_move[COORDS], int worldSize, int len,
						 int people[len][NB_PARAM_PERS], int index) {
	int i = index;
	if ((next_move[X] == 0) && (next_move[Y] == 0))
		return VRAI;
	for (int j = 0; j < len; j++) {
		if ((rebouclement(worldSize, people[i][X] + next_move[X]) == people[j][X]) &&
			(rebouclement(worldSize, people[i][Y] + next_move[Y]) == people[j][Y]))
			return FAUX;
	}
	return VRAI;
}

static void move(int next_move[COORDS], int worldSize, int person[NB_PARAM_PERS]) {
	person[X] = rebouclement(worldSize, person[X] += next_move[X]);
	person[Y] = rebouclement(worldSize, person[Y] += next_move[Y]);
}

static void blocage(int next_move[COORDS], int modifier[MODIFIER_SIZE][COORDS],
					 int worldSize, int len, int people[len][NB_PARAM_PERS],
					 int index) {
	int i = index;
	if(!move_possible(next_move, worldSize, len, people, i)) {
		int m = rand() % NB_POSSIBLE_MOVES;
		int mcount = 0;
		while(!move_possible(modifier[m], worldSize, len, people, i)) {
			m++;
			if (m > NB_POSSIBLE_MOVES)
				m -= NB_POSSIBLE_MOVES;
			if (mcount > NB_POSSIBLE_MOVES)
				m = 0;
				new_but(worldSize, people[i]);
			mcount++;
		}
		next_move[X] = modifier[m][X];
		next_move[Y] = modifier[m][Y];
	}
}

static void sort(int len, int tab[len]) {
	int tmp;
	int i,j;
	for (i = PREMIER_ELEMENT; i < len; i++) {
		tmp = tab[i];
		j = i;
		while ((j > 0) && (tab[j-1] > tmp)) {
			tab[j] = tab[j-1];
			j--;
		}
		tab[j] = tmp;
	}
}

static int simulation(int modifier[MODIFIER_SIZE][COORDS], int affichage,
					  int worldSize, int nbPers, 
					  int people_original[nbPers][NB_PARAM_PERS], int blocker) {
	int count = 0;
	int end;
	int i,j;
	int people[blocker][NB_PARAM_PERS];
	for (i = 0; i < blocker; i++)
			for (j = 0; j < NB_PARAM_PERS; j++)
				people[i][j] = people_original[i][j];
	for (i = 0; i < blocker; i++) {
		test_contamination(modifier, worldSize, blocker, people, i);
	}
	end = test_fin(blocker, people);
	int next_move[COORDS];
	while(end) {
			if (affichage) 
				print_world(worldSize, blocker, people);
			for(i = 0; i < blocker; i++) {
				test_incubation(people[i]);
				if(but_check(people[i]))
					new_but(worldSize, people[i]);
				move_find(next_move, worldSize, people[i]);
				blocage(next_move, modifier, worldSize, blocker, people, i);
				move(next_move, worldSize, people[i]);
				test_contamination(modifier, worldSize, blocker, people, i);
			}
			end = test_fin(blocker, people);
			if (count > MAX_CYCLES)
				end = FAUX; 
			count++;
		}
	if (affichage)
		print_world(worldSize, blocker, people);
	return count;
}

static double median(int len, int tab[len]){
	sort(len, tab);
	if ((len % 2) == 0)
		return (tab[len/2] + tab[(len/2)-1])/2.;
	else 
		return tab[len/2];
}
// ############################ END ##################################
