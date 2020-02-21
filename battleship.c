#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>


//matr este matricea jucatorului
//tab este matricea calculatorului
// variabila oridne va fi cea care va lasa jucatorii sa joace in ordinea descrisa de regulile jocului
void menu(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie); // INITIALIZARE FUNCTIE MENIU CA SA MEARGA APELAREA FUNCTIEI DIN NEW GAME
void quit();


void castigator(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) // functie de stabilire a castigatorului
{   
   int x,y,i,j; 
   
   for(x=0;x<10;x++)
   {
       for(y=0;y<10;y++)
       {
           if( matr[x][y]=='L')    
            {
                scorc=scorc+1; // daca gaseste o barca atacata va adauga la scor
            }
       }
    }

    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            if(tab[i][j]=='L')
            {
                scorj=scorj+1;// daca gaseste o barca atacata va adauga la scor
            }
        }
    }

    if(scorc==20)
    {
            erase();
            mvprintw(1,1,"Ai pierdut :( Catigatorul este calculatorul.");
            getch();
            quit();
    }

    if(scorj==20)
    {
            erase();
            mvprintw(1,1,"Felicitari! Ai castigat! :)");
            getch();
            quit();
    }
    mvprintw(15,1, "Scorul jucatorului este %d , scorul calculatorului este %d dintr-un total de 20 ", scorj, scorc);
    refresh();     
}

void randomize_map(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) // functie pentru optiunea de randomizare a hartii jucatorului
{

    int i, j, x, y, n, m;
    int dir; //1 pentru directie verticala, 2 pentru directie orizontala
    int lung;
    char **copy; 

    srand(time( NULL));
    copy=malloc(10*sizeof(char*));
    for(i=0;i<10;i++)
    {
        copy[i]=malloc(10*sizeof(char));
    }
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            copy[i][j]=' '; // umplem matricea de 10x10 cu spatii goale
        }
    }

    char *nava;
    nava=malloc(4*sizeof(char));
    
    int ok = 1;
    int aux = 16;
    
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            lung = 0;
            if(matr[i][j]=='O')
            {
                matr[i][j]=' '; // daca au fost casute goale incercate, acestea se vor inlocui cu spatii libere
            }
            if(matr[i][j]=='L' || matr[i][j]=='x')
            {
                           
                if(j+1 < 10 && (matr[i][j+1] == 'L' || matr[i][j+1]=='x' ))
                {
                    dir=1; // afam directia barcii (1 este pentru directie orizontala)
                }
                if(i+1 < 10 && (matr[i+1][j] == 'L' || matr[i+1][j]=='x') )
                {
                    dir=2;// aflam directia barcii (2 este pentru directie verticala)
                }
                if(dir==1) // in cazul in care directia este orizontala
                {
                    int col=j;
                    while(col < 10 && (matr[i][col] == 'L' || matr[i][col] == 'x'))
                    {
                        nava[lung]=matr[i][col]; // retinem 'L' sau 'x' in functie de ce se afla in pozitia respectiva in vectorul nava
                        matr[i][col]=' '; // inlocuim in matricea initiala cu camp gol pentru a nu mai reveni la acea pozitie
                        lung=lung+1; // aflu lungimea barcii
                        col++;
                    }
                }
                if(dir == 2) //in cazul in care directia este verticala
                {
                                
                    int linie=i;
                    while(linie < 10 && (matr[linie][j] == 'L' || matr[linie][j] == 'x'))
                    {
                        nava[lung]=matr[linie][j];
                        matr[linie][j]=' ';
                        lung=lung+1;
                        linie++;
                    }
                }
                if(dir==1) // generarea de pozitii aleatorii pentru barcile de directie orizontala 
                {

                    for( ; ; )
                    {                    
                        x=rand()%10; // generarea a doua coordonate de inceput pentru barca
                        y=rand()%(10-lung);

                        int lungnava=0;
                        ok = 1;
                    
                        for(n=y;n<y+lung;n++)
                        {
                            if(copy[x][n] == 'x' || copy[x][n] == 'L') // daca acolo se aflau barci, nu se vor repozitiona in zona respectiva
                            {
                                ok = 0;
                            }    
                            if(x>0 && (copy[x-1][n] == 'x' || copy[x-1][n] == 'L'))// daca acolo se aflau barci, nu se vor repozitiona in zona respectiva
                            {
                                ok = 0;
                            }
                            if(x+1<10 && (copy[x+1][n] == 'x' || copy[x+1][n] == 'L'))// daca acolo se aflau barci, nu se vor repozitiona in zona respectiva
                            {
                                ok = 0;
                            }
                        }
                        lungnava=0;

                        if(ok == 1) // daca este posibila repozitionarea, aceasta se va efectua 
                        {
                            for(n=y;n<y+lung;n++)
                            {
                                copy[x][n]=nava[lungnava]; // se va retine in copy nava respectiva
                                lungnava++;
                            }
                            break;
                        }
                    }
                    aux++;
                }
                if(dir==2) // generarea de pozitii aleatoare a barcilor de directie verticala
                {
                   for(; ; )
                   {            
                        x=rand()%(10-lung); // generarea aleatoare setului de coordonate a barcilor
                        y=rand()%10;

                        ok = 1;
                        int lungnava=0;

                        for(m=x;m<x+lung;m++)
                        {
                            if(copy[m][y] == 'L' || copy[m][y] == 'x') // daca acolo se afla nava, repozitionarea in acele coordonate nu va fi posibila
                            {
                                ok = 0;
                            }
                            if(y>0 && (copy[m][y-1] == 'L' || copy[m][y-1] == 'x'))// daca acolo se aflau barci, nu se vor repozitiona in zona respectiva
                            {
                                ok = 0;
                            }
                            if(y+1<10 && (copy[m][y] == 'L' || copy[m][y] == 'x')) // daca acolo se afla nava, repozitionarea in acele coordonate nu va fi posibila
                            {
                                ok = 0;
                            }
                        }
                        lungnava = 0;

                        if(ok == 1)
                        {
                            for(m=x;m<x+lung;m++)
                            {
                                copy[m][y]=nava[lungnava];// se va retine in copy nava respectiva
                                lungnava++;
                            }
                            break;
                        }
                    }
                aux++;
                }
            }
        }
    }
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            matr[i][j]=copy[i][j]; // dupa ce matr va fi complet golita se vor muta datele retinute in copy in matr 
        }
    }
}
 
 
void  destroy_in_advance(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie)// functie pentru optiunea de distrugere a cate 10 casute in tabelele jucatorilor
{
    int x, y;
    int nr1=0;
    int nr2=0;

    srand(time(NULL));
    // PENTRU MATRICEA JUCATORULUI
    do
    {
        x=rand()%10; //generez aleator de fiecare data un set de numere 
        y=rand()%10;

        if(matr[x][y] != 'L' && matr[x][y] != 'O')  // caz in care coordonatele nu au fost incercate 
        {
            if(matr[x][y] == 'x') // daca e barca marcheaza
            {
                matr[x][y] = 'L';
                nr1 = nr1 + 1; //retin numarul de afisari
                refresh();
            }
            else
            {
                if(matr[x][y] == ' ') // daca e camp gol rateaza
                {
                    matr[x][y] = 'O';
                    nr1 = nr1 + 1;//retin nr de afisari
                    refresh();
                }
            }            
        }
    }while(nr1 < 10); // cat timp au fost incercate 10 casute

    //PENTRU MATRICEA CALCULATORULUI
    do
    {
        x=rand()%10;
        y=rand()%10;

        if(tab[x][y] != 'L' && tab[x][y] != 'O')
        {
            if(tab[x][y] == 'B')
            {
                tab[x][y] = 'L';
                nr2 = nr2 + 1;// retin numarul de afisari
                refresh();
            }
            else
            {
                if(tab[x][y] != 'L' && tab[x][y] != 'B' && tab[x][y] != 'O')
                {
                    tab[x][y] = 'O';
                    nr2 = nr2 + 1;
                    refresh();
                }
            }            
        }
    }while(nr2 < 10);
}


void logo() //functie de intampinare a utilizatorului
{
	init_pair( 1, COLOR_YELLOW, COLOR_BLACK );
	attron( A_BOLD | COLOR_PAIR(1));
		mvprintw(0, 6, "Pentru navigare in menu folositi tastele directionale sus/jos!");
	mvprintw(2, 0,"XX       XXXX     XX      XX    XX      XXXXXX    XXXXX   XX       XX   XXXX");
	mvprintw(3, 0,"XX      XX  XX   xXXx    xXXx   XX      XX       XX       XX       XX   XX  XX");
	mvprintw(4, 0,"XXXXX   XX  XX    XX      XX    XX      XXXX      XXXX    XXXXXX   XX   XXXX\n");
	mvprintw(5, 0,"XX  XX  XXXXXX    XX      XX    XX      XX           XX   XX  XX   XX   XX\n");
	mvprintw(6, 0,"XXXXX   XX  XX    XXx     XXx   XXX     XXXXXX   XXXXX    XX  XX   XX   XX\n");
        attroff( A_BOLD | COLOR_PAIR(1));
    getch();
    endwin();
}

void atacare_nave(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) // functie de atacare a calculatorului
{
	int x, y;

	srand(time(NULL));
	do
	{
		x=rand()%10; //generez doua coordonate pentru a ataca
		y=rand()%10;
		if(matr[x][y]=='x') 
		{
			matr[x][y] = 'L'; 
			*ordine = 1; // daca calculatorul a lovit, 'ordine' nu se va schimba si calculatorul va mai avea o a doua incercare  
			mvprintw(1,70,"%d", scorc);
            refresh();   
          //  sleep(3);  // asteapta 3 secunde de la atacarea jucatorului
		}
		else
        if(matr[x][y]==' ')
		{            
			matr[x][y] = 'O';
			*ordine = 0; // daca  calc a ratat, variabila se va schimba. Randul va fi al jucatorului
            refresh();   
           // sleep(3);  // asteapta 3 secunde de la atacarea jucatorului
		}
	} while ( *ordine == 1); 
}

void nave_calculator(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) //functie pentru generare aleatoare de nave a calculatorului
{
    int i,j, n,m;
    
    int err=0;
    int lungime;
	
	int x,y,k;  // k variabila pentru barca pozitionata spre dreapta sau in jos
    
    srand(time(NULL));
    for(i=0;i<4;i++)
    {
        for(j=0; j< 4 - i; j++)
        {
            do
	            {
                    int greseala=0;     
                    
                    x=rand()%10;
		            y=rand()%10; // ca sa am coodonatele intre 0 si 9
		            k=rand()%2;  // k ul va fi ori 0 sau 1, dupa care se vor pune conditii pentru pozitionarea barcii
                    
                    lungime=b[i][j];
                    
                	if((k==0  && x<11-lungime )||(k==1 &&  y<11-lungime ))    
                    {
                        if (k==0)
                        {
                            if(x>0 && x < 10 - lungime)  //casuta 9 va fi goala, posibil sa fie ocupata dinainte
                            {
                                for(m=x-1;m<x+lungime+1;m++) // verific daca e ocupat cu o pozitie inainte si dupa lung totala a barcii
					            {
                                    if(tab[m][y]=='B' ) // verificarea pentru coloana cu barca
                                    {
                                        greseala = 1;                                        
                                    }
                                    if(y>0 && tab[m][y-1] == 'B') // verificare pentru coloana din stanga barcii
                                    {
                                        greseala = 1;                                        
                                    }
                                    if(y<9 && tab[m][y+1] == 'B') //verificare pentru coloana din dreapta barcii
                                    {
                                        greseala = 1;                                       
                                    }
                                }
				                for(m=x;m<x+lungime;m++)
			            	    {
				            	    if(greseala==0)
				            	        tab[m][y]='B'; //completarea cu barci
				            	    else err=1;
			                    }
			            	    if(greseala==0) err=0;  
                            }
                            else 
                            {
                                if(x == 0 )
                                {
                                    for(m=x;m<x+lungime+1;m++)
                                    {
                                        if(tab[m][y]=='B' )  // verificarea pe coloana cu barca
                                        {
                                            greseala = 1;                                          
                                        }
                                        if(y<9 && tab[m][y+1] =='B') // verificare in dreapta barcii
                                        {
                                            greseala = 1;
                                        }
                                        if(y>0 && tab[m][y-1]=='B') // verificare in stanga barcii
                                        {
                                            greseala = 1;
                                        }
                                    }    
				                    for(m=x;m<x+lungime;m++)
			            	        {
				            	        if(greseala==0)
				            	            tab[m][y]='B'; //completarea cu barci
				            	        else err=1;
			                        }
			            	        if(greseala==0) err=0;
                                }
                                else
                                {
                                    if( x == 10 - lungime )
                                    {
                                        for(m=x-1;m<x+lungime;m++)
					                    {
                                            if(tab[m][y]=='B' )// verificarea pe coloana cu barca 
                                            {
                                                greseala = 1;                                               
                                            }
                                            if(y>0 &&  tab[m][y-1] == 'B' ) // verificarea in stanga barcii
                                            {
                                                greseala = 1; 
                                            }
                                            if(y<9 && tab[m][y+1] == 'B')
                                            {
                                                greseala = 1; // verificare in  dreapta barcii
                                            }
                                        }
				                        for(m=x;m<x+lungime;m++)
			            	            {
				            	            if(greseala==0)
				            	                tab[m][y]='B'; // completarea cu barci
				            	            else err=1;
			                            }
			            	            if(greseala==0) err=0;
                                    }
                                }
                            }
                        }
                        else 
                        {
                            if(y>0 && y<10-lungime)
                            {
                                for(n=y-1;n<y+lungime+1;n++)
					            {
					        	    if(tab[x][n]=='B' )// verificare pe linia cu barca 
                                    {
                                        greseala=1;                                       
                                    }
                                    if(x>0 &&  tab[x-1][n] == 'B') // verificare deasupra barca
                                    {
                                        greseala=1;                                       
                                    }
                                    if(x<9 && tab[x+1][n] == 'B') // verificare sub barcii
                                    {
                                        greseala = 1;                                       
                                    }
				                }
				        	    for(n=y;n<y+lungime;n++)
				        	    {
				        		    if(greseala==0)
					        	        tab[x][n]='B'; // completarea cu barci
					        	    else err=1;
					            }
					            if(greseala==0)err=0;
                            }
                            else
                            {
                                if(y==0)
                                {
                                    for(n=y;n<y+lungime+1;n++)
					                {
					        	        if(tab[x][n]=='B') // verificare pe linia cu barca 
                                        {
                                            greseala=1;
                                        }
                                        if(x<9 && tab[x+1][n] == 'B') // verificare sub barca
                                        {
                                            greseala = 1;
                                        }
                                        if(x>0 && tab[x-1][n] == 'B')  // verificare deasupra barcii
                                        {
                                            greseala = 1; 
                                        }
				                    }
				        	        for(n=y;n<y+lungime;n++)
				        	        {
				        		        if(greseala==0)
					        	            tab[x][n]='B'; // completarea cu barci
					        	        else err=1;
					                }
					                if(greseala==0)err=0;
                                }
                                else
                                {
                                    if(y == 10 - lungime)
                                    {
                                         for(n=y-1;n<y+lungime;n++)
					                    {
					        	            if(tab[x][n]=='B' )// verificare pe linia cu barca
                                            {
                                                greseala=1;
                                            }
                                            if(x>0 && tab[x-1][n] == 'B') // verificare deasupra barcii
                                            {
                                                greseala = 1;
                                            }
                                            if(x<9 && tab[x+1][n]=='B')
                                            {
                                                greseala = 1; // verificare sub barca
                                            }
				                        }
				        	            for(n=y;n<y+lungime;n++)
				        	            {
				        		            if(greseala==0)
					        	                tab[x][n]='B'; // completarea cu barci
					        	            else err=1;
					                    }
					                    if(greseala==0)err=0;
                                    }
                                }
                                
                            } 
                        }
                    }
                    else err=1;
            }while(err); 
        }
    }
}

void newgame(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie)//functia de joc
{
    init_pair( 2, COLOR_RED, COLOR_BLACK);
    init_pair( 3, COLOR_GREEN, COLOR_BLACK);
    init_pair( 4, COLOR_MAGENTA, COLOR_BLACK);
    int cazx = 0;
    int cazy = 0;
    int i,j;
    scorc=0;
    scorj=0;
    clear();
    noecho();

    if(*start == 0) // variabila pt a verifica daca a inceput jocul
    {
        *start = 1;
    }
    for( ; ; )
    {
        int chr;
        chr = getch();
   
        switch(chr)  //parcurgerea meniului cu sageti
        {
            case 65: 
                cazy--;
                if(cazy < 0)
                {
                    cazy = 9;
                }
                break;
            case 66:
                cazy++;
                if(cazy > 9)
                {
                    cazy = 0;
                }        
                break;
            case 68:
                cazx--;
                if(cazx < 0)
                { cazx = 9; }
                break;
            case 67:
                cazx++;
                if(cazx > 9)
                {
                    cazx = 0;
                }        
                break;  
            case 'Q':
                clear();
                menu(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;
            case 'q':
                clear();
                menu(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;
           case 'e':
                destroy_in_advance(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;
            case 'E':
                destroy_in_advance(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;
             case 'R':
                randomize_map(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;
            case 'r':
                randomize_map(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                break;              
            case 10:
                if (*ordine == 0) //randul jucatorului sa atace
                    {
                       if(tab[cazy][cazx]=='B')
                        {
                            tab[cazy][cazx]='L';  // DACA ACOLO SE AFLA O BARCA VA SCHIMBA CU L 
                            *ordine = 0; // daca jucatorul va lovi o barca i se va mai oferi o tura
                            refresh();
                        }
                        else
                        if(tab[cazy][cazx] != 'L' && tab[cazy][cazx] != 'B' && tab[cazy][cazx] != 'O')
                        {
                            tab[cazy][cazx]='O';  // DACA ACOLO NU SE AFLA O BARCA VA SCHIMBA CU 0
                            *ordine = 1; // va urma randul calculatorului
                            atacare_nave(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                            refresh();
                        } 
                    }    
                }

        for(i=0; i<10; i++)
        {
            for(j=0;j<10;j++)
            {
                mvprintw(i ,j , "%c", matr[i][j]); // afisarea matricei jucatorului
                  
            }
    
        }
 
        for(i=0; i<10; i++) //parcurgerea matricei calculatorului cu highlight
        {
            for(j=0;j<10;j++)
            {
                if(i == cazy && j == cazx)
                    attron( A_REVERSE );
                if(tab[i][j]=='L')    
                {
                    attron( A_BOLD | COLOR_PAIR(3));
                    mvprintw(i ,j+20 ,"L"); // AFISEAZA L COLORAT DACA A LOVIT O BARCA
                    attroff( A_BOLD | COLOR_PAIR(3));
                }    
                else
                {
                    if(tab[i][j]=='O')
                    {
                        attron( A_BOLD | COLOR_PAIR(2)); //AFISEAZA O COLORAT DACA NU A LOVIT NICIO BARCA
                        mvprintw(i ,j+20 ,"O");
                        attroff( A_BOLD | COLOR_PAIR(2));
                        refresh();
                    }
                    else
                    {
                        {
                            attron( A_BOLD | COLOR_PAIR(4)); // AFISEAZA X  NORMAL 
                            mvprintw(i ,j+20 ,"X");
                            attroff( A_BOLD | COLOR_PAIR(4));
                            refresh();
                        }
                    }   
                }
                if(i == cazy && j == cazx)
                    attroff( A_REVERSE );
            }
        }
        castigator(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie); // functie de afisare a castigatorului
    }
}

void howtoplay(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie)   //functie de afisare a instructiunilor
{
  erase(); // reinitializam ecranul 
  init_pair(1, COLOR_WHITE, COLOR_BLACK); // initializam culoarea
  bkgd(COLOR_PAIR(1)); // alegem culoarea backgroundului 
  char c; // caracterul citit de la tastatura

    // Instructiuni 
  mvprintw(5,5,"Acest joc foloseste o matrice de dimensiune 10x10 pe care vor fi plasate navele jucatorului.");
  mvprintw(7,5,"Folositi sagetile de pe tastatura pentru a va muta pe tabela si pentru a ataca folositi tasta ENTER");  
  mvprintw(9,5,"Pentru fiecare nava nimerita jucatorul are dreptul la inca o tura, continuand asa pana cand nu va mai nimeri.");
  mvprintw(11,5,"Oponentul utilizatorului este un robot ce are harti generate aleator si alege aleator nave");
  mvprintw(13,5 ,"Scopul jocului este sa tintesti toate navele opoentului.");
  mvprintw(15,5, "Jocul are doua functii care pot fi accesate prin apasarea unor taste.");
  mvprintw(17,5,"Functia de Destroy-in-advance presupune distrugerea a cate 10 casute, atat pentru calculator cat si pentru jucator.");
  mvprintw(19,5, "Aceasta functie poate fi activata prin apasarea tastelor 'e' sau 'E'.");
  mvprintw(21,5,"Functia Ranndomize-map i se aplica jucatorului si va repozitiona in mod aleator navele ramase ale jucatorului");
  mvprintw(23,5,"Aceasta functie poate fi accesata prin apasarea tastelor 'r' sau 'R'");
  attron(A_BOLD); // Se Scrie boldat
  mvprintw(25,10,"Apasa 'Q' sau 'q' pentru a te intoarce la meniul principal.");
  attroff(A_BOLD);
  for( ; ; )  //O bucla infinita din care se poate iesi pentru a reveni la meniu doar prin apasarea tasei Q(81)
  {
    c = getch(); // citim caracterul 
    switch(c)
    {
      case 81: // in care utilizatorul apasa pe Q
      {
        erase(); // reinitializam ecranul
        menu(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie); //ne intoarcem la meniu      
        break;
      }
      case 113:
      {
          erase();
          menu(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
          break;
      }
        default:
        break;
    }
  }
    refresh();
}

void quit()    //FUNCTIE DE IESIRE DIN JOC
{
    endwin();
    exit(0);
}

void resumegame(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) // functie de reluare a jocului
{
    newgame(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
}

void menu(char **matr, char **tabla, int scorj, int scorc,char **tab,int **b, int *ordine, int scormax, int *start, char **copie) //functia de meniu
{
    int linie,coloana;
    const char txt1[] = "NEW GAME";// alegerile posibile
    const char txt2[] = "RESUME GAME";
    const char txt3[] = "HOW TO PLAY";
    const char txt4[] = "QUIT";
    int caz = 1;
    int i, j;
    
    int tasta;

    init_pair( 1, COLOR_WHITE, COLOR_BLACK );
    attron( COLOR_PAIR( 1 ) );

    do
    {
    getmaxyx(stdscr,linie,coloana);
    
    mvprintw(linie/2+2, coloana/2-8, "by ANEMONA MORARU");
    mvprintw(linie-1, 0, "SAGEATA SUS, JOS - PENTRU MISCARE; ENTER - sel");
    
    refresh();

        switch( caz ) // afisare evidentiata a functiei selectate
        {
        case 1:
            mvprintw( linie/2+4, coloana/2-3, txt2 );
            mvprintw( linie/2+5, coloana/2-3, txt3 );
            mvprintw( linie/2+6, coloana/2-3, txt4 );
            attron( A_REVERSE );
            mvprintw( linie/2+3, coloana/2-3, txt1 );
            break;
        case 2:
            mvprintw( linie/2+3, coloana/2-3, txt1 );
            mvprintw( linie/2+5, coloana/2-3, txt3 );
            mvprintw( linie/2+6, coloana/2-3, txt4 );
            attron( A_REVERSE );
            mvprintw( linie/2+4, coloana/2-3, txt2 );
            break;
        case 3:
            mvprintw( linie/2+3, coloana/2-3, txt1 );
            mvprintw( linie/2+4, coloana/2-3, txt2 );
            mvprintw( linie/2+6, coloana/2-3, txt4 );
            attron( A_REVERSE );
            mvprintw( linie/2+5, coloana/2-3, txt3 );
            break;
        case 4:
            mvprintw( linie/2+3, coloana/2-3, txt1 );
            mvprintw( linie/2+4, coloana/2-3, txt2 );
            mvprintw( linie/2+5, coloana/2-3, txt3 );
            attron( A_REVERSE );
            mvprintw( linie/2+6, coloana/2-3, txt4 );
        }
        attroff( A_REVERSE );
        tasta = getch();
        refresh();
        
        switch(tasta)
        {
            case 65:
                caz--;
                if(caz < 1)
                {
                    caz=4;
                }
                break;
            case 66:
                caz++;
                if(caz > 4)
                {
                    caz=1;
                }    
                break;
            case 10:
                if(caz==1 && *start == 0)
                {
                    newgame(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                    break;
                }
                if(caz==1 && *start == 1) // Daca jocul a fost inceput, se vor sterge datele retinute de la ultima incercare
                {
                    for(i=0;i<10;i++)
                    {
                        for(j=0;j<10;j++)
                        {
                            matr[i][j]=' '; // stergerea datelor retinute 
                        }
                    }
                    for(i=0; i<10; i++)
                    {
                       for(j=0;j<10;j++)
                        {
                            matr[i][j]=copie[i][j]; // inserarea datelor initiale
                        } 
                    }
                    for(i=0;i<10;i++)
                    {
                        for(j=0;j<10;j++)
                        {
                            tab[i][j]=' '; // stergere date calculator
                        }
                    }
                    nave_calculator(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie); // inserarea datelor noi ale calculatorului
                    newgame(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie); //inceperea jocului
                    break;
                }
                if(caz==2)
                {
                    resumegame(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                    break;
                }
                if(caz==3)
                {
                    howtoplay(matr, tabla, scorj, scorc, tab, b, ordine, scormax, start, copie);
                    break;
                }
                if(caz==4)
                {
                    quit();
                    break;
                }
        }   
    } while( caz != 3 || tasta != 10 );

    move( 9, 0 );
    printw( "SFARITUL PROGRAMULUI, APASATI ORICE TASTA..." );
    getch();
    endwin();
    exit(0);
}


int main(int argc, char *argv[])
{
    int i, j;
    FILE *pFile=fopen(argv[1],"r");// fac citirea din fisier
    char **matr;
    char c;
    char **tabla;
    int scorc; 
    int scorj;
    int **b;
    char **tab;
    int ordine = 0;// ordine va fi ori zero ori unu, zero va fi variabila in care va fi randul jucatorului, unu cea in care va fi randul calculatorului
    int scormax = 20; //numarul maxim de casute ce pot fi lovite
    char **copie;

    scorc = 0;
    scorj = 0;
    

    copie=malloc(10*sizeof(char*));
    for(i=0;i<10;i++)
    {
        copie[i]=malloc(10*sizeof(char));
    }
    b=malloc(4*sizeof(int*));
    for(i=0;i<4;i++)
    {
        b[i]=malloc((4-i)*sizeof(int));
    }
    for(i=0;i<4;i++)//dimensiunea si nr barcilor
    {
        for(j=0; j< 4-i; j++)
        {
            b[i][j]=i+1; //atribuim fiecarei barci lungimea 
        }
    }
    tab=malloc(10*sizeof(char*));
    for(i=0;i<10;i++)  // matricea inamicului (calculatorului)
    {
        tab[i]=malloc(10*sizeof(char));
    }
    tabla=malloc(10*sizeof(char*));
    for(i=0;i<10;i++)
    {
        tabla[i]=malloc(10*sizeof(char));
    }
    matr=malloc(10*sizeof(char*));
    for(i=0; i<10;i++)
    {
        matr[i]=malloc(10*sizeof(char));
    }
    for(i=0; i<10; i++)
    {
        for(j=0;j<10;j++)
        {
            fscanf(pFile, "%c", &matr[i][j]);
        }
        fscanf(pFile, "%c", &c); // se retine intr-o variabila EOF \n
    }
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            copie[i][j]=matr[i][j];
        }
    }
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++)
        {
            tabla[i][j]=0;
        }
    }
    
    srand(time(NULL));  
    int *start = (int *)malloc(sizeof(int)); // variabila pentru a verifica daca am inceput sau nu jocul
    *start=0;
        
    nave_calculator(matr, tabla, scorj, scorc, tab, b, &ordine, scormax, start, copie);    
    initscr();
    noecho();
    start_color();
    logo();
    clear();
    noecho();
    cbreak(); 
    menu(matr, tabla, scorj, scorc, tab, b, &ordine, scormax, start, copie); //apelareaa funtiei de aplelare a meniului
    
    for(i=0;i<10;i++)//eliberarea de memorie
    {
        free(copie[i]);
    }
    free(copie);
    for(i=0;i<4; i++)
    {
        free(b[i]);
    }
    free(b);
    for(i=0;i<10;i++)
    {
        free(tab[i]);
    }
    free(tab);
    for(i=0;i<10;i++)
    {
        free(matr[i]);
    }
    free(matr);
   
    return 0;
}