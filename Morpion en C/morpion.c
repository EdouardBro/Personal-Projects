#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>


void afficheGrille(int *grille){
	int k = 0;
	for(int i=0; i<3 ; i++){
		printf("%c",'|');
		for(int j=0; j<3 ; j++){
			//printf("%d", grille[i+j+k]);
			if(grille[i+j+k]==1) printf("%c",'X');
			else if(grille[i+j+k]==2) printf("%c",'O');
			else printf("%c",' ');
			printf("%c",'|');
		}
		printf("\n");
		k+=2;
	}
}


void joueurDebutant(int *grille, int pion){

	printf("%s\n","______________________________________");
	printf("            Joueur DEBUTANT\n");
	printf("%s\n","______________________________________");

	// Génère un nombre aléatoire entre 0 et 8
	int nb_aleatoire = 0 ;

	do
	{
		nb_aleatoire = rand()%9;
	}while(grille[nb_aleatoire] != 0);

	grille[nb_aleatoire]=pion;
	grille[9]=nb_aleatoire;

	afficheGrille(grille);


}

void joueurMalin(int *grille, int pion){

	printf("%s\n","______________________________________");
	printf("              Joueur MALIN\n");
	printf("%s\n","______________________________________");

	int jeuPrecedent = grille[9];
	int possibilite[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int nb_aleatoire = 0;
	int intervalle = 0;
	int caseChoisie = 0;


	do{
		switch(jeuPrecedent){
			case 0 :
				possibilite[0]=1;
				possibilite[1]=3;
				possibilite[2]=4;
				intervalle=3;
				break;

			case 1 :
				possibilite[0]=0;
				possibilite[1]=2;
				possibilite[2]=3;
				possibilite[3]=4;
				possibilite[4]=5;
				intervalle = 5;
				break;



			case 2 :
				possibilite[0]=1;
				possibilite[1]=4;
				possibilite[2]=5;
				intervalle = 3;
				break;
			case 3 :
				possibilite[0]=0;
				possibilite[1]=1;
				possibilite[2]=4;
				possibilite[3]=6;
				possibilite[4]=7;
				intervalle = 5;
				break;
			case 4 :
				possibilite[0]=0;
				possibilite[1]=1;
				possibilite[2]=2;
				possibilite[3]=3;
				possibilite[4]=5;
				possibilite[5]=6;
				possibilite[6]=7;
				possibilite[7]=8;
				intervalle = 8;
				break;
			case 5 :
				possibilite[0]=1;
				possibilite[1]=2;
				possibilite[2]=4;
				possibilite[3]=7;
				possibilite[4]=8;
				intervalle = 5;
				break;
			case 6 :
				possibilite[0]=3;
				possibilite[1]=4;
				possibilite[2]=7;
				intervalle = 3;
				break;
			case 7 :
				possibilite[0]=3;
				possibilite[1]=4;
				possibilite[2]=5;
				possibilite[3]=6;
				possibilite[4]=8;
				intervalle = 5;
				break;


			case 8 :
				possibilite[0]=4;
				possibilite[1]=5;
				possibilite[2]=7;
				intervalle = 3;
				break;
			default:
				possibilite[0]=0;
				possibilite[1]=1;
				possibilite[2]=2;
				possibilite[3]=3;
				possibilite[4]=4;
				possibilite[5]=5;
				possibilite[6]=6;
				possibilite[7]=7;
				possibilite[8]=8;
				intervalle = 9;
				break;
		}

		/* Si toutes les cases "possibilite" sont prises, alors possibilite = "grille" */
		int compteur=0;
		for(int i=0; i<intervalle; i++){
			if(grille[possibilite[i]]!=0){
				compteur++;
			}
		}

		if(compteur==intervalle){
				possibilite[0]=0;
				possibilite[1]=1;
				possibilite[2]=2;
				possibilite[3]=3;
				possibilite[4]=4;
				possibilite[5]=5;
				possibilite[6]=6;
				possibilite[7]=7;
				possibilite[8]=8;

				intervalle = 9;
		}
		

		nb_aleatoire=rand()%intervalle;
		//printf("Nombre aléatoire = %d \n",nb_aleatoire);
		//printf("Possibilité étudiée = %d \n",possibilite[nb_aleatoire]);
		caseChoisie = possibilite[nb_aleatoire];
	}while(grille[caseChoisie] != 0);

	grille[caseChoisie]=pion;
	grille[9]=caseChoisie;


	afficheGrille(grille);

}

void joueurTricheur(int *grille, int pion){

	printf("%s\n","______________________________________");
	printf("             Joueur TRICHEUR\n");
	printf("%s\n","______________________________________");

	// Génère un nombre aléatoire entre 0 et 8
	int nb_aleatoire = 0 ;
	do{
		nb_aleatoire = rand()%100; // On prend un intervalle de 100 pour pas avoir les mêmes positions que Débutant
		nb_aleatoire %=9;
	}while(grille[nb_aleatoire]==pion); //On veut tricher, on évite de rejouer sur sa propre case.
	

	grille[nb_aleatoire]=pion;
	grille[9]=nb_aleatoire;


	afficheGrille(grille);

}

void joueurPolytech(int *grille, int pion){

	printf("%s\n","______________________________________");
	printf("            Joueur POLYTECH\n");
	printf("%s\n","______________________________________");
	printf("•\n");
	printf("•\n");

	/* On montre les numéros de la grille : */

	printf("%s\n","|0|1|2|" );
	printf("%s\n","|3|4|5|" );
	printf("%s\n","|6|7|8|" );
	printf("•\n");


	int casePopo = 0;

	do{
	printf("Entrer un numéro de case correspondant à la grille au dessus ; cette case doit être libre !\n");
	scanf("%d",&casePopo);

	}while(grille[casePopo]!=0);

	grille[casePopo] = pion;
	grille[9] = casePopo;


	afficheGrille(grille);

}


void verifGagnant(int *grille){

	/* grille[12] peut prendre plusieurs valeurs :
	• 0 : la grille n'est pas encore remplie, le jeu continue
	• 1 : le joueur 1 a gagné (rempli 3 cases consécutives)
	• 2 : le joueur 2 a gagné (rempli 3 cases consécutives)
	• 3 : la grille est remplie mais personne n'a gagné
	*/

	/* Vérification première diagonale */
	if(grille[0]==grille[4] && grille[4]==grille[8] && grille[0]!=0){
		if(grille[0]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
		
	}

	/* Vérification geuxième diagonale */
	if(grille[2]==grille[4] && grille[4]==grille[6] && grille[2]!=0){
		if(grille[2]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}

	/* Vérification lignes */
	/* Ligne 1 */
	if(grille[0]==grille[1] && grille[1]==grille[2] && grille[0]!=0){
		if(grille[0]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}
	/* Ligne 2 */
	if(grille[3]==grille[4] && grille[4]==grille[5] && grille[3]!=0){
		if(grille[3]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}
	/* Ligne 3 */
	if(grille[6]==grille[7] && grille[7]==grille[8] && grille[6]!=0){
		if(grille[6]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}

	/* Vérification colonnes */
	/* Colonne 1 */
	if(grille[0]==grille[3] && grille[3]==grille[6] && grille[0]!=0){
		if(grille[0]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}
	/* Colonne 2 */
	if(grille[1]==grille[4] && grille[4]==grille[7] && grille[1]!=0){
		if(grille[1]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}
	/* Colonne 3 */
	if(grille[2]==grille[5] && grille[5]==grille[8] && grille[2]!=0){
		if(grille[2]==1){
			grille[12]=1;
		}
		else{
			grille[12]=2;
		}
	}


	/* Vérifie si la grille est remplie */

	int compteur=0;
	for(int i=0; i<9; i++)
	{
		if(grille[i]!=0){
			compteur+=1;
		}
	}

	if(compteur==9 && grille[12] == 0){
		grille[12] = 3;
	}


}



int main(int argc, char *argv[]){

	srand(time(NULL));
	pid_t pid;

	printf("%s\n","Les joueurs joueront avec 'X' et 'O'");
	printf("%s\n","______________________________________");

	/* Initialisation de la grille */
	/* On va utiliser un tableau à 13 cases (de 0 à 12) */
	/* Case 9 : case jouée précédemment */
	/* Case 10 : case de synchronisation (attente active) */
	/* Case 11 : quel joueur a joué */
	/* Case 12 : fin du jeu */
	int shmid = shmget(IPC_PRIVATE, 13*sizeof(int), IPC_CREAT|0700);
	int *grille = shmat(shmid, NULL, 0);

	if(shmid<0){
		printf("Erreur\n");
		exit(-1);
	}

	/*
	|X|O|X|
	|O|X|O|
	|X|O|X|
	*/


	/* On initie la grille */
	for(int i=0; i<10; i++){
		grille[i]=0;
	}
	grille[9] = -1; // dernier coup joué, personne n'a encore joué
	grille[10] = 0; // synchronisation des processus
	/* Synchronisation des processus : */
	// grille[10] = 1 => fils 1
	// grille[10] = 2 => fils 2
	// grille[10] = 3 => père
	grille[11] = 0; // contient le numéro du joueur qui a joué
	grille[12] = 0; //Jeu toujours actif quand elle vaut 0


	// création des joueurs :
	int joueur1 = 0;
	int joueur2 = 0;
	char pion1 = 1;
	char pion2 = 2;

	/* Menu */
	printf("______________________________________\n");
	printf("--------------------------------------\n");
	printf(".     .  ____   ____   ____     ____  .   .\n|\\   /| |    | |    \\ |    | | |    | |\\  |\n| \\ / | |    | |   _/ |  __/ | |    | | \\ |\n|  °  | |____| |   \\  |      | |____| |  \\|\n");
	printf("______________________________________\n");
	printf("--------------------------------------\n");


	printf("Choisir deux joueurs en entrant les bons numéros :\n");
	printf("'1' - Joueur débutant\n");
	printf("'2' - Joueur malin\n");
	printf("'3' - Joueur tricheur\n");
	printf("'4' - Joueur Polytech\n");
	printf("______________________________________\n");
	do{
		printf("%s\n","Entrer le joueur 1 :");
		scanf("%d",&joueur1);
	}while(joueur1<1 || joueur1>4);

	printf("Le joueur 1 est : %d et joue avec le pion 'X' \n",joueur1);

	printf("%s\n","______________________________________");
		do{
		printf("%s\n","Entrer le joueur 2 :");
		scanf("%d",&joueur2);
	}while(joueur2<1 || joueur2>4);

	printf("Le joueur 2 est : %d et joue avec le pion 'O' \n",joueur2);
	printf("%s\n","______________________________________");
	printf("%s\n","______________________________________");


	grille[10] = 3; //Le père initialise la partie
	grille[11] = 2; // Le joueur 1 commencera
	int tour=0; // Affiche le numéro de tour.

	/* On crée les processus fils */
	int numFils = 0;
	for(numFils=1; numFils<=2; numFils++){
		pid = fork();
		if(pid==0) break;
	}


	if(pid<0){
		printf("Erreur\n");
		exit(-1);
	}


	if(pid>0){
		while(grille[12]==0){
			/* Processus père */
			/* On fait une attente active */
			while(grille[10]!=3){
				sleep(1);
			}

			verifGagnant(grille);
			tour++;

			if(grille[12]==0){
			printf("\n");
			printf("•\n");
			printf("%s\n","______________________________________");

			printf("                 COUP  %d\n",tour);
			printf("%s\n","______________________________________");
			}


			switch(grille[12]){
				case 0 :
					break;
				case 1 :
					printf("Le joueur 1 à gagné ! Félicitations ! X\n");
					break;
				case 2 :
					printf("Le joueur 2 a gagné ! Bien joué ! O\n");
					break;
				default :
					printf("Ex-aequo, on veut une revanche ! XO\n");
					break;
			}		

			/* C'est au tour du fils */
			if(grille[11]==2){
				grille[10]=1;
			}
			else{
				grille[10]=2;
			}


		}
		wait(NULL);
		shmctl(shmid, IPC_RMID, NULL);
	}
	else{
		while(grille[12]==0){
			/* Processus fils */
			/* Attente active */
			while(grille[10]!=numFils){
				sleep(1);
			}

			//verifGagnant(grille);
			if(grille[12]!=0){
				exit(0);
			}

			/* JOUEUR 1 */
			if(numFils==1){
				switch(joueur1){
					case 1 :
						joueurDebutant(grille,pion1);
						break;
					case 2 :
						joueurMalin(grille,pion1);
						break;
					case 3 :
						joueurTricheur(grille,pion1);
						break;
					case 4 :
						joueurPolytech(grille,pion1);
						break;
					default :
						printf("ERROR");
						break;
				}
			}

			if(numFils==2){
				/* JOUEUR 2 */
				switch(joueur2){
					case 1 :
						joueurDebutant(grille,pion2);
						break;
					case 2 :
						joueurMalin(grille,pion2);
						break;
					case 3 :
						joueurTricheur(grille,pion2);
						break;
					case 4 :
						joueurPolytech(grille,pion2);
						break;
					default :
						printf("ERROR");
						break;
				}
			}


			grille[11]=numFils;
			grille[10]=3;
					
					
		}
	}

	return EXIT_SUCCESS;
}

