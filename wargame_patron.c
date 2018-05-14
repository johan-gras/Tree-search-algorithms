#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NB_LIGNES 10
#define NB_COLONNES 10
#define INT_MAX2 100000
#define INT_MIN2 (-100000)

#define PROF 4

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

//#define DEBUG

typedef struct pion_s
{
	int couleur;
	int valeur;
}Pion;

typedef struct List_Debug {
    int l1, l2, c1, c2, joueur, eval;
    //Pion *plateauDeJeu;
    struct List_Debug * next;
} List_Debug;

List_Debug * Get_List_Debug(){
	List_Debug *new = NULL;
	new = malloc(sizeof(List_Debug));
	if(new == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
	new->next = NULL;
	new->l1 = 123;
	new->l2 = 123;
	new->c1 = 123;
	new->c2 = 123;
	//new->plateauDeJeu = NULL;
	//new->plateauDeJeu = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
	//if(new->plateauDeJeu  == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
	return new;
}

void free_list (List_Debug *list){
	if (list != NULL){
		free_list(list->next);
		//free(list->plateauDeJeu);
		free (list);
	}
}


Pion *plateauDeJeu;
int direction[8][2] = {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};

int nodes_explored = 0;
int ia_call = 0;

void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);



int f_convert_char2int(char c)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(c>='A' && c<='Z')
		return (int)(c-'A');
	if(c>='a' && c<='z')
		return (int)(c-'a');
	return -1;
}

char f_convert_int2char(int i)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	return (char)i+'A';
}

Pion *f_init_plateau()
{
	int i, j;
	Pion *plateau=NULL;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	plateau = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
	if(plateau == NULL)
	{
		printf("error: unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			plateau[i*NB_COLONNES+j].couleur = 0;
			plateau[i*NB_COLONNES+j].valeur = 0;
		}
	}

	plateau[9*NB_COLONNES+5].couleur = 1;
	plateau[9*NB_COLONNES+5].valeur = 1;

	plateau[9*NB_COLONNES+6].couleur = 1;
	plateau[9*NB_COLONNES+6].valeur = 2;

	plateau[9*NB_COLONNES+7].couleur = 1;
	plateau[9*NB_COLONNES+7].valeur = 3;

	plateau[9*NB_COLONNES+8].couleur = 1;
	plateau[9*NB_COLONNES+8].valeur = 2;

	plateau[9*NB_COLONNES+9].couleur = 1;
	plateau[9*NB_COLONNES+9].valeur = 1;

	plateau[8*NB_COLONNES+0].couleur = 1;
	plateau[8*NB_COLONNES+0].valeur = 1;

	plateau[8*NB_COLONNES+1].couleur = 1;
	plateau[8*NB_COLONNES+1].valeur = 3;

	plateau[8*NB_COLONNES+2].couleur = 1;
	plateau[8*NB_COLONNES+2].valeur = 3;

	plateau[8*NB_COLONNES+3].couleur = 1;
	plateau[8*NB_COLONNES+3].valeur = 1;

	plateau[8*NB_COLONNES+6].couleur = 1;
	plateau[8*NB_COLONNES+6].valeur = 1;

	plateau[8*NB_COLONNES+7].couleur = 1;
	plateau[8*NB_COLONNES+7].valeur = 1;

	plateau[8*NB_COLONNES+8].couleur = 1;
	plateau[8*NB_COLONNES+8].valeur = 1;

	plateau[7*NB_COLONNES+1].couleur = 1;
	plateau[7*NB_COLONNES+1].valeur = 1;

	plateau[7*NB_COLONNES+2].couleur = 1;
	plateau[7*NB_COLONNES+2].valeur = 1;

	plateau[2*NB_COLONNES+7].couleur = -1;
	plateau[2*NB_COLONNES+7].valeur = 1;

	plateau[2*NB_COLONNES+8].couleur = -1;
	plateau[2*NB_COLONNES+8].valeur = 1;

	plateau[1*NB_COLONNES+1].couleur = -1;
	plateau[1*NB_COLONNES+1].valeur = 1;

	plateau[1*NB_COLONNES+2].couleur = -1;
	plateau[1*NB_COLONNES+2].valeur = 1;

	plateau[1*NB_COLONNES+3].couleur = -1;
	plateau[1*NB_COLONNES+3].valeur = 1;

	plateau[1*NB_COLONNES+6].couleur = -1;
	plateau[1*NB_COLONNES+6].valeur = 1;

	plateau[1*NB_COLONNES+7].couleur = -1;
	plateau[1*NB_COLONNES+7].valeur = 3;

	plateau[1*NB_COLONNES+8].couleur = -1;
	plateau[1*NB_COLONNES+8].valeur = 3;

	plateau[1*NB_COLONNES+9].couleur = -1;
	plateau[1*NB_COLONNES+9].valeur = 1;

	plateau[0*NB_COLONNES+0].couleur = -1;
	plateau[0*NB_COLONNES+0].valeur = 1;

	plateau[0*NB_COLONNES+1].couleur = -1;
	plateau[0*NB_COLONNES+1].valeur = 2;

	plateau[0*NB_COLONNES+2].couleur = -1;
	plateau[0*NB_COLONNES+2].valeur = 3;

	plateau[0*NB_COLONNES+3].couleur = -1;
	plateau[0*NB_COLONNES+3].valeur = 2;

	plateau[0*NB_COLONNES+4].couleur = -1;
	plateau[0*NB_COLONNES+4].valeur = 1;

#ifdef DEBUG
printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

return plateau;
}

void f_affiche_plateau(Pion *plateau)
{
	int i,j,k;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("%2c ",f_convert_int2char(k));
	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("-- ");
	printf("\n");
	for(i=NB_LIGNES-1; i>=0; i--)
	{
		printf("%2d ", i);
		for(j=0; j<NB_COLONNES; j++)
		{
			printf("|");
			switch(plateau[i*NB_COLONNES+j].couleur)
			{
			case -1:
				printf("%do",plateau[i*NB_COLONNES+j].valeur);
				break;
			case 1:
				printf("%dx",plateau[i*NB_COLONNES+j].valeur);
				break;
			default:
				printf("  ");
			}
		}
		printf("|\n    ");
		for(k=0; k<NB_COLONNES; k++)
			printf("-- ");
		printf("\n");
	}
	printf("    ");

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


int f_gagnant(Pion *plateau)
{
	int i, j, somme1=0, somme2=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	//Quelqu'un est-il arrive sur la ligne de l'autre
	for(i=0; i<NB_COLONNES; i++)
	{
		if(plateau[i].couleur == 1)
			return 1;
		if(plateau[(NB_LIGNES-1)*NB_COLONNES+i].couleur == -1)
			return -1;
	}

	//taille des armees
	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			if(plateau[i*NB_COLONNES+j].couleur == 1)
				somme1++;
			if(plateau[i*NB_COLONNES+j].couleur == -1)
				somme2++;
		}
	}
	if(somme1==0)
		return -1;
	if(somme2==0)
		return 1;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend comme argument la ligne et la colonne de la case
 * 	pour laquelle la bataille a lieu
 * Renvoie le couleur du gagnant
 * */
int f_bataille(int l, int c, Pion *plateau)
{
	int i, j, mini, maxi, minj, maxj;
	int somme=0;

#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	mini = l-1<0?0:l-1;
	maxi = l+1>NB_LIGNES-1?NB_LIGNES-1:l+1;
	minj = c-1<0?0:c-1;
	maxj = c+1>NB_COLONNES-1?NB_COLONNES-1:c+1;

	for(i=mini; i<=maxi; i++)
	{
		for(j=minj; j<=maxj; j++)
		{
			somme += plateau[i*NB_COLONNES+j].couleur*plateau[i*NB_COLONNES+j].valeur;
		}
	}
	somme -= plateau[l*NB_COLONNES+c].couleur*plateau[l*NB_COLONNES+c].valeur;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(somme < 0)
		return -1;
	if(somme > 0)
		return 1;

	return plateau[l*NB_COLONNES+c].couleur;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
	printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
	/* Erreur, hors du plateau */
	if(l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
			c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
		return 1;
	/* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
	if(plateau[l1*NB_COLONNES+c1].valeur == 0 || plateau[l1*NB_COLONNES+c1].couleur != couleur)
		return 1;
	/* Erreur, tentative de tir fratricide */
	if(plateau[l2*NB_COLONNES+c2].couleur == plateau[l1*NB_COLONNES+c1].couleur)
		return 1;

	if(l1-l2 >1 || l2-l1 >1 || c1-c2 >1 || c2-c1 >1 || (l1==l2 && c1==c2))
		return 1;
#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demand�e
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
	int gagnant=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	if(f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
		return 1;


	/* Cas ou il n'y a personne a l'arrivee */
	if(plateau[l2*NB_COLONNES+c2].valeur == 0)
	{
		plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
		plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
		plateau[l1*NB_COLONNES+c1].couleur = 0;
		plateau[l1*NB_COLONNES+c1].valeur = 0;
	}
	else
	{
		gagnant=f_bataille(l2, c2, plateau);
		/* victoire */
		if(gagnant == couleur)
		{
			plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
			plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
		/* defaite */
		else if(gagnant != 0)
		{
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}

//Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion* jeu, int joueur)
{
	int nbPion=0;
	int i, j;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				++nbPion;
			}
		}
	}
	return nbPion;
}

//Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion* jeu, int joueur)
{
	int i, j;
	int valeur=0;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				valeur += jeu[i * NB_COLONNES + j].valeur;
			}
		}
	}
	return valeur;
}

//Calcul de la distance qu'il reste � parcourir
int f_distance(Pion* jeu, int joueur)
{
	int distance = -1;
	int i = 0, j = ((joueur == -1) ? NB_LIGNES-1 : 0);
		do {
			do {
				if (jeu[j * NB_COLONNES + i].couleur == joueur)
					distance = (joueur == -1) ? NB_LIGNES - 1 - j : j;
				i++;
			} while (distance == -1 && i < NB_COLONNES);
			j = j + joueur;
			i = 0;
		} while (distance == -1 && j >= 0 && j < NB_LIGNES);

	return distance;
}

//Calcul de la distance qu'il reste � parcourir
float f_distance_moyenne(Pion* jeu, int joueur)
{
	int nbF = 0, sommeF = 0, nbE = 0, sommeE = 0;
	int i, j;

	for (j = 0 ; j < NB_LIGNES ; j++){
		for (i = 0 ; i < NB_COLONNES ; i++){

			if (jeu[j * NB_COLONNES + i].couleur == joueur){
				sommeF += (joueur == -1) ? NB_LIGNES - 1 - j : j;
				nbF++;
			}
			else if (jeu[j * NB_COLONNES + i].couleur == -joueur){
				sommeE += (-joueur == -1) ? NB_LIGNES - 1 - j : j;
				nbE++;
			}
		}
	}

	return ((float)sommeF/(float)nbF) - ((float)sommeE/(float)nbE);
}


//fonction d'�valuation
int f_eval(Pion* jeu, int joueur, int depth)
{
    nodes_explored++;
    int win = f_gagnant(jeu);
	if (win == joueur)
        return INT_MAX2 - 20 + depth;

    if (win == -joueur)
        return INT_MIN2 + 20 - depth;

	return (NB_LIGNES-1)*1000 - (int)(f_distance_moyenne(jeu, joueur) * 1000);
}

//copie du plateau
void f_copie_plateau(Pion* source, Pion* destination)
{
	int i, j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
			destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
		}
	}
}

//mise a zero du plateau
Pion* f_raz_plateau()
{
	Pion* jeu = NULL;
	int i, j;
	jeu = (Pion *) malloc(NB_LIGNES * NB_COLONNES * sizeof (Pion));
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			jeu[i * NB_COLONNES + j].couleur = 0;
			jeu[i * NB_COLONNES + j].valeur = 0;
		}
	}
	return jeu;
}

//Fonction alpha beta

int f_alphabeta (Pion *node, int depth, int player, int A, int B, List_Debug *debug){
	if (depth == 0 || f_gagnant(node) != 0){
		return f_eval(node, player, depth);
	}

	int bestValue = INT_MIN2;
	int l, c, dir;

	for (l = 0 ; l < NB_LIGNES ; l++){
		for (c = 0 ; c < NB_COLONNES ; c++){
			for (dir = 0 ; dir < 8 ; dir++){
				if (f_test_mouvement(node, l, c, l + direction[dir][1], c + direction[dir][0], player) == 0){

					List_Debug *tempDebug = Get_List_Debug();

					Pion *newNode = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
					if(newNode == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
					f_copie_plateau(node, newNode);
					f_bouge_piece (newNode, l, c, l + direction[dir][1], c + direction[dir][0], player);

					int value = - f_alphabeta(newNode, depth-1, -player, -B, -A, tempDebug);

					if (value > bestValue){
						bestValue = value;
						debug->l1 = l;
						debug->c1 = c;
						debug->l2 = l + direction[dir][1];
						debug->c2 = c + direction[dir][0];
						debug->joueur = player;
						debug->eval = bestValue;
						free_list(debug->next);
						debug->next = tempDebug;

						if (bestValue > A){
							A = bestValue;

							if (A >= B){
								free (newNode);
								return bestValue;
							}
						}
					}
					else
						free_list (tempDebug);

					free (newNode);
				}
			}
		}
	}

	return bestValue;
}


/**
 * Calcule et joue le meilleur cout
 * */
void f_IA_alphabeta(int joueur)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    int depth = PROF;
	int A = INT_MIN2-1, B = INT_MAX2+1;
	int l1, c1, l2, c2;
	int l, c, dir;
	List_Debug *debug = NULL;

	for (l = 0 ; l < NB_LIGNES ; l++){
		for (c = 0 ; c < NB_COLONNES ; c++){
			for (dir = 0 ; dir < 8 ; dir++){
				if (f_test_mouvement(plateauDeJeu, l, c, l + direction[dir][1], c + direction[dir][0], joueur) == 0){

					List_Debug *tempDebug = Get_List_Debug();

					Pion *newNode = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
					if(newNode == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
					f_copie_plateau(plateauDeJeu, newNode);
					f_bouge_piece (newNode, l, c, l + direction[dir][1], c + direction[dir][0], joueur);

					int value = - f_alphabeta(newNode, depth-1, -joueur, -B, -A, tempDebug);
					if (value > A){
						A = value;
						l1 = l;
						c1 = c;
						l2 = l + direction[dir][1];
						c2 = c + direction[dir][0];
						free_list(debug);
						debug = tempDebug;
					}
					else
						free_list(tempDebug);

					free (newNode);
				}
			}
		}
	}

	int oldEval = f_eval(plateauDeJeu, joueur, depth);

	f_bouge_piece (plateauDeJeu, l1, c1, l2, c2, joueur);

	printf("\nJ%d ----- %c%d -> %c%d ----- %d -> %d -> %d\n",
		   ((joueur == 1) ? 1 : 2), f_convert_int2char(c1), l1, f_convert_int2char(c2), l2, oldEval, f_eval(plateauDeJeu, joueur, depth), A);
	//f_affiche_plateau(plateauDeJeu);

	/*for ( ; debug->c1 != 123 ; debug = debug->next){
		printf("J%d ----- %c%d -> %c%d ----- %d\n",
		   ((debug->joueur == 1) ? 1 : 2), f_convert_int2char(debug->c1), debug->l1, f_convert_int2char(debug->c2), debug->l2, debug->eval);
		//f_affiche_plateau(debug->plateauDeJeu);
	}*/

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


//Fonction negamax

int f_negamax (Pion *node, int depth, int player, List_Debug *debug){
	if (depth == 0 || f_gagnant(node) != 0){
		return f_eval(node, player, depth);
	}

	int bestValue = INT_MIN2;
	int l, c, dir;

	for (l = 0 ; l < NB_LIGNES ; l++){
		for (c = 0 ; c < NB_COLONNES ; c++){
			for (dir = 0 ; dir < 8 ; dir++){
				if (f_test_mouvement(node, l, c, l + direction[dir][1], c + direction[dir][0], player) == 0){

					List_Debug *tempDebug = Get_List_Debug();

					Pion *newNode = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
					if(newNode == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
					f_copie_plateau(node, newNode);
					f_bouge_piece (newNode, l, c, l + direction[dir][1], c + direction[dir][0], player);

					int value = - f_negamax(newNode, depth-1, -player, tempDebug);

					if (value > bestValue){
						bestValue = value;
						debug->l1 = l;
						debug->c1 = c;
						debug->l2 = l + direction[dir][1];
						debug->c2 = c + direction[dir][0];

						debug->joueur = player;
						debug->eval = bestValue;
						free_list(debug->next);
						debug->next = tempDebug;
					}
					else
						free_list (tempDebug);

					free (newNode);
				}
			}
		}
	}

	return bestValue;
}


/**
 * Calcule et joue le meilleur cout
 * */
void f_IA_negamax(int joueur)
{
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    int depth = PROF;
	int bestValue = INT_MIN2-1;
	int l1, c1, l2, c2;
	int l, c, dir;
	List_Debug *debug = NULL;

	for (l = 0 ; l < NB_LIGNES ; l++){
		for (c = 0 ; c < NB_COLONNES ; c++){
			for (dir = 0 ; dir < 8 ; dir++){
				if (f_test_mouvement(plateauDeJeu, l, c, l + direction[dir][1], c + direction[dir][0], joueur) == 0){

					List_Debug *tempDebug = Get_List_Debug();

					Pion *newNode = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
					if(newNode == NULL) { printf("error: unable to allocate memory\n"); exit(EXIT_FAILURE); }
					f_copie_plateau(plateauDeJeu, newNode);
					f_bouge_piece (newNode, l, c, l + direction[dir][1], c + direction[dir][0], joueur);

					int value = - f_negamax(newNode, depth-1, -joueur, tempDebug);
					if (value > bestValue){
						bestValue = value;
						l1 = l;
						c1 = c;
						l2 = l + direction[dir][1];
						c2 = c + direction[dir][0];
						free_list(debug);
						debug = tempDebug;
					}
					else
						free_list(tempDebug);

					free (newNode);
				}
			}
		}
	}

	int oldEval = f_eval(plateauDeJeu, joueur, depth);

	f_bouge_piece (plateauDeJeu, l1, c1, l2, c2, joueur);

	printf("\nJ%d ----- %c%d -> %c%d ----- %d -> %d -> %d\n",
		   ((joueur == 1) ? 1 : 2), f_convert_int2char(c1), l1, f_convert_int2char(c2), l2, oldEval, f_eval(plateauDeJeu, joueur, depth), bestValue);
	//f_affiche_plateau(plateauDeJeu);

/*	for ( ; debug->c1 != 123 ; debug = debug->next){
		printf("J%d ----- %c%d -> %c%d ----- %d\n",
		   ((debug->joueur == 1) ? 1 : 2), f_convert_int2char(debug->c1), debug->l1, f_convert_int2char(debug->c2), debug->l2, debug->eval);
		//f_affiche_plateau(debug->plateauDeJeu);
	}*/

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur)
{
	char c1, c2;
	char buffer[32];
	int l1, l2;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("joueur ");
	switch(joueur)
	{
	case -1:
		printf("o ");
		break;
	case 1:
		printf("x ");
		break;
	default:
		printf("inconnu ");
	}
	printf("joue:\n");
	while(1)
	{
		fgets(buffer, 32, stdin);
		if(sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4)
		{
			if(f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
				break;
		}
		fflush(stdin);
		printf("mauvais choix\n");
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int main(int argv, char *argc[])
{
	int fin = 0,mode=0 , ret, joueur = 1;
	char useless;
	printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
	scanf("%d",&mode);

	plateauDeJeu = f_init_plateau();
	while (!fin)
	{
		f_affiche_plateau(plateauDeJeu);

		if(mode==1)
		{
			if(joueur>0)
				f_humain(joueur);
			else
				f_IA_negamax(joueur);
		}
		else if(mode==2)
		{
			f_humain(joueur);
		}
		else
		{
			ia_call++;
			f_IA_negamax(joueur);
		}

		if ((ret = f_gagnant(plateauDeJeu)) != 0)
		{
			switch (ret)
			{
			case 1:
				f_affiche_plateau(plateauDeJeu);
				printf("joueur x gagne!\n");
				fin = 1;
				break;
			case -1:
				f_affiche_plateau(plateauDeJeu);
				printf("joueur o gagne!\n");
				fin = 1;
				break;
			}
		}
		joueur = -joueur;
	}

    printf("\nProfondeur %d, %d noeuds explores, %d appels a l'ia, en moyenne %d noeuds explores\n",
           PROF, nodes_explored, ia_call, nodes_explored/ia_call);

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

	return 0;
}

