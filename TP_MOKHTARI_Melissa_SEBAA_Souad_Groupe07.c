#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<ctype.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include <stdbool.h>

int comparer(char nomRech[256],char nom[256])
{
    bool trouve=false;
    if(strlen(nomRech)==0){
        printf("ERREUR; Requete vide");
    }
    else{
        switch (nomRech[0]){//on coence eon a première lettre
            case '?':fonction_Question(0,0,nomRech,nom,&trouve) ;break;
            case '*':fonction_Etoile(0,0,nomRech,nom,&trouve)   ;break;
            default :fonction_lettre(0,0,nomRech,nom,&trouve)   ;break;
        }
    }
    if(trouve){ //printf("found\n",&trouve);
    return 1;}
    else{//printf("not found");
    return 0;}

}
//stop ie le nom ne corresppent pas
// trouve ie on l'a trouv�
//autre ie on est en face de ? ou *
////////////////////////////////////////////////////////////////////////////////////////////
void fonction_Etoile(int indiceRech,int indiceNom,char nomRech[256],char nom[256],bool*trouve)
{
   bool autre=false;bool stop=false;
    while(nomRech[(indiceRech)]=='*')
    {
        (indiceRech)++;
    }
    while(!(stop) && !(*trouve) && !(autre))
    {
        if((indiceRech)==(strlen(nomRech))){

            (*trouve)=true;
        }
        else{
            if((indiceRech)>=strlen(nomRech)){(stop)=true;}
            else{
                if(nomRech[(indiceRech)]=='?'){
                    while((indiceNom)<strlen(nom)){
                        fonction_Question(indiceRech,indiceNom,nomRech,nom,trouve);
                        (indiceNom)++;
                    }
                    (stop)=true;
                }
                else{//si le prochain caractere est une lettre

                    while((indiceNom)<strlen(nom))
                    {
                        if(nomRech[(indiceRech)]==nom[(indiceNom)]){
                            fonction_lettre(indiceRech,indiceNom,nomRech,nom,trouve);
                        }
                        (indiceNom)++;
                    }
                    stop=true;
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
void fonction_Question(int indiceRech,int indiceNom,char nomRech[256],char nom[256],bool* trouve)
{
    bool autre=false;bool stop=false;

    while(!(stop) && !(*trouve) && !(autre))
    {
        if(nomRech[(indiceRech)]!='?'){
            if(nomRech[(indiceRech)]=='*'){ fonction_Etoile(indiceRech,indiceNom,nomRech,nom,trouve); }//le prochain caractere est une etoile
            else{fonction_lettre(indiceRech,indiceNom,nomRech,nom,trouve);}//le prochain caractere est une lettre
            (autre)=true;
        }
        else{
            if((indiceRech)<strlen(nomRech)&&(indiceNom)>=strlen(nom)){//ie fin nom mais nom rech a '?' sup
                (stop)=true;
            }
            else{
                (indiceNom)++;
                (indiceRech)++;
                if((indiceRech)>=strlen(nomRech)&&(indiceNom)<strlen(nom)){//mot  fini ie pas le meme recherche
                    (stop)=true;
                }
                else{
                   if((indiceNom)>=strlen(nom)&&(indiceRech)>=strlen(nomRech)){//fin des deux mots
                    (*trouve)=true;
                    }
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////

void fonction_lettre(int indiceRech,int indiceNom,char nomRech[256],char nom[256],bool * trouve)
{
    bool autre=false;bool stop=false;


    while(!(stop) && !(*trouve) && !(autre))
    {
        if(nomRech[(indiceRech)]=='?' || nomRech[(indiceRech)]=='*'){
             if(nomRech[(indiceRech)]=='?'){fonction_Question(indiceRech,indiceNom,nomRech,nom,trouve);}//le prochain caractere est une ?
             if(nomRech[(indiceRech)]=='*'){fonction_Etoile(indiceRech,indiceNom,nomRech,nom,trouve);}//le prochain caractere est une lettre
            (autre)=true;

        }
        else{
            if(nomRech[(indiceRech)]!=nom[(indiceNom)]){//lettres pas compatibles
                (stop)=true;
            }
            else{
                if((indiceRech)>=strlen(nomRech)&&(indiceNom)>=strlen(nom)){//fin des deux mots
                    (*trouve)=true;
                }
                else{//on incr�mente
                        (indiceNom)++;
                        (indiceRech)++;
                        if((indiceRech)>=strlen(nomRech)&&(indiceNom)<strlen(nom) &&nomRech[(indiceRech)]!='?' && nomRech[(indiceRech)]!='*')
                         {//fin du mot rech et pas du nom (pas ? ou * � la suite)
                             (stop)=true;
                         }
                }

            }
        }
    }
}


void afficher_info(char *path, int tab[])
{
    printf("-----------------------------------\nPath : %s\n",path);
    struct stat descr;
    char date[255];
    stat(path, &descr);
    if(tab[1]) //c'est l'option -a donc afficher les informations des options: d,s,t,p
    {
        strftime(date, 255, "%D %T", gmtime( &(descr.st_atim.tv_sec)) );
        printf("\tDernier accès le....... %s\n", date);
        printf("\tProtection............. ");
            printf( (descr.st_mode & S_IRUSR) ? "r" : "-");
            printf( (descr.st_mode & S_IWUSR) ? "w" : "-");
            printf( (descr.st_mode & S_IXUSR) ? "x" : "-");
            printf( (descr.st_mode & S_IRGRP) ? "r" : "-");
            printf( (descr.st_mode & S_IWGRP) ? "w" : "-");
            printf( (descr.st_mode & S_IXGRP) ? "x" : "-");
            printf( (descr.st_mode & S_IROTH) ? "r" : "-");
            printf( (descr.st_mode & S_IWOTH) ? "w" : "-");
            printf( (descr.st_mode & S_IXOTH) ? "x\n" : "-\n");
        printf("\tTaille du fichier...... %ld octets\n", descr.st_size);
        printf("\tType du fichier........ ");
        printf( (S_ISDIR(descr.st_mode)) ? "d\n" : "-\n");
    }
    else
    {
        if(tab[2]) //c'est l'option -d
        {
            strcpy(date, "");
            strftime(date, 255, "%D %T", gmtime( &(descr.st_atim.tv_sec)) );
            printf("\tDernier accès le....... %s\n", date);
        }
        if(tab[3]) //c'est l'option -p
        {
            printf("\tProtection............. ");
            printf( (descr.st_mode & S_IRUSR) ? "r" : "-");
            printf( (descr.st_mode & S_IWUSR) ? "w" : "-");
            printf( (descr.st_mode & S_IXUSR) ? "x" : "-");
            printf( (descr.st_mode & S_IRGRP) ? "r" : "-");
            printf( (descr.st_mode & S_IWGRP) ? "w" : "-");
            printf( (descr.st_mode & S_IXGRP) ? "x" : "-");
            printf( (descr.st_mode & S_IROTH) ? "r" : "-");
            printf( (descr.st_mode & S_IWOTH) ? "w" : "-");
            printf( (descr.st_mode & S_IXOTH) ? "x\n" : "-\n");
        }
        if(tab[4]) printf("\tTaille du fichier...... %ld octets\n", descr.st_size); //c'est l'option -s
        if(tab[5]) //c'est l'option -t
        {
            printf("\tType du fichier........ ");
            printf( (S_ISDIR(descr.st_mode)) ? "d\n" : "-\n");
        }
    }
    if(tab[6]) //c'est l'option -m
    {
        strcpy(date, "");
        strftime(date, 255, "%D %T", gmtime( &(descr.st_mtim.tv_sec)) );
        printf("\tDernière modification du fichier le..... %s\n", date);
    }
}

void Chercher(char path[], char file_name[], int tab[], int nb_niveaux_parcourus, int *cpt)
{
    DIR *directory = opendir(path);
    if(directory == NULL) { printf("cannot open : %s\n", path); return;}

    struct dirent *entry = readdir(directory);
    while(entry != NULL) //parcourir les entrées du repertoire directory
    {
        if(comparer(file_name, entry->d_name))//si le nom du fichier de l'entrée correspond à celui donné par l'utilisateur
            {
                char string[255];
                strcpy(string, path);
                afficher_info(strcat(strcat(string, "/"), entry->d_name), tab);
                (*cpt) ++;
            }
        if(entry->d_type == 4 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && (tab[0]==-1 || nb_niveaux_parcourus<tab[0]))//si cette entrée est un repertoire (différent de . et ..) et que le nombre de niveaux parcourus < de celui entrée par l'utilisateur
            {
                char string[255] =""; strcat(string, path);
                if (strcmp("/", string) != 0) strcat(string, "/");
                strcat(string, entry->d_name);//mettre à jour le path en ajoutant à ce chemin le repertoire qu'on veut parcourir
                //effectuer la recherche dans ce repertoire
                Chercher(string, file_name, tab, nb_niveaux_parcourus+1, cpt);
            }
        entry = readdir(directory);//passer à la prochaine entrée
    }
    closedir(directory);
}

//Cette fonction récupère les arguments de la commande entrés par l'utilisateur dans les variables correspondantes et retourne 0 si il n'y a aucune erreur sinon elle retourne un entier > 0
int RecupererEntrees(int argv, char *argc[], int tab[], char path[], char file_name[]){
    int err = 0; tab[0] = -1;
    for(int i = 1; i<7; i++) tab[i] = 0; //initialisation
    if (argv == 2) { //dans ce cas l'utilisateur aurait entré une commande de cette forme : Search nom_fichier donc
        strcpy(path, "."); //le repertoire dans lequel on va chercher c'est le repertoire courant
        strcpy(file_name, argc[1]); //Pour récupérer le nom du fichier
    }else {// dans ce cas l'utilisateur aurait entré au moins une option ou bien un nom repertoire
        int i;
        //Déterminer si le premier argument donné par l'utilisateur est une option ou bien un nom épertoire
        if (strncmp("-", argc[1], 1) != 0) //si argc[1] argument ne commence pas par un - donc c'est un nom repertoire
        {
            strcpy(path, argc[1]);
            i = 2; //pour continuer l'évaluation des arguments entrés par l'utilisateur depuis argc[2]
        }
        else  //sinon argc[1] est une option et on considère le repertoire courant le repertoire dans lequel on va chercher
        {
            strcpy(path, ".");
            i = 1;
        }
        //Déterminer les autres arguments
        while(i < argv){
            if(strcmp("-a", argc[i]) == 0) tab[1] = 1;
            else if(strcmp("-d", argc[i]) == 0) tab[2] = 1;
            else if(strcmp("-p", argc[i]) == 0) tab[3] = 1;
            else if(strcmp("-s", argc[i]) == 0) tab[4] = 1;
            else if(strcmp("-t", argc[i]) == 0) tab[5] = 1;
            else if(strcmp("-m", argc[i]) == 0) tab[6] = 1;
            else if(strcmp("-0", argc[i]) == 0) tab[0] = 0;
            else if(strncmp("-", argc[i], 1) == 0) {
                int nb = atoi(argc[i]); //conversion string à un nombre, afin d'avoir le nombre de niveaux à parcourir en int
                if (nb == 0)  return(2); //si il y a une erreur dans la saisie
                tab[0] = -nb;
            }
            else{
                if((i+1) != argv) //si format de la commande est non respecté
			        return(3);
                strcpy(file_name, argc[i]);// sinon récupérer le nom du fichier
            }
            i ++;
        }
    }
    return err;
}

int main(int argv, char *argc[]){
    char path[255] = ""; //on récupère le chemin dans lequel on va chercher dans cette variable
    char file_name[255] = ""; //on récupére le nom du fichier à chercher dans cette variable
    int tab[7]; // Ce tableau permet de récupérer les options entrées par l'utilisateur ou chaque option a une entrée dans ce tableau
    //tab[0] est positionné à 1 si l'utilisateur entre le nombre de niveau, -1 sinon (dans ce cas on va parcourir toute l'arborescence)
    //tab[1] est à 1 si l'utilisateur entre l'option -a, 0 sinon
    //tab[2] est à 1 si l'utilisateur entre l'option -d, 0 sinon
    //tab[3] est à 1 si l'utilisateur entre l'option -p, 0 sinon
    //tab[4] est à 1 si l'utilisateur entre l'option -s, 0 sinon
    //tab[5] est à 1 si l'utilisateur entre l'option -t, 0 sinon
    //tab[6] est à 1 si l'utilisateur entre l'option -m, 0 sinon
    int err = RecupererEntrees(argv, argc, tab, path, file_name); //procéder à une vérification d'erreur et récupération des arguments entrés par l'utilisateur
    if(err) {
        printf("\nCommande erronnée\n");
        if(err == 2) printf("Les options entrées non reconnues !\n");
        else if(err == 3)
        {
            printf("Format de l'instruction non respecté !\n");
            printf("Si vous avez entré un nom de fichier contenant * ou ?, veuillez utiliser des quotes '*' '?'\n");
        }
        return 1;
    }

    int cpt = 0;// nombre de résultats trouvées
    int nb_niveaux_parcourus = 0;// le nombre de niveau parcourus
    Chercher(path, file_name, tab, nb_niveaux_parcourus, &cpt);//faire la recherche

    printf("\nNombre de résultats trouvés : %d\n", cpt);


    printf("Remarque : si vous avez entré un nom de fichier contenant * ou ?, veuillez utiliser des quotes '*' '?', sinon les résultats risquent d'etre erronnés\n");
    return 0;
}
