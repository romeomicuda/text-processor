#define MAX_INPUT 81

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // praca so znakmi
#include <string.h> // praca s retazcami
#include <unistd.h> // funkcia getopt()

void lSwitch(char line[], char outLine[])       // funkcia pre prepinac -l , zmeni na male pismena
{
    char c;
    while(*line != '\0')
    {
        c = *line;
        *outLine = tolower(c);
        line++;
        outLine++;
    }
    *outLine = '\0';
}

void uSwitch(char line[], char outLine[]){          // funkcia pre prepinac -u, zmeni na velke pismena
    char c;
    while(*line != '\0'){
        c = *line;
        *outLine = toupper(c);
        line++;
        outLine++;
    }
    *outLine = '\0';
}

void aSwitch(char line[],char outLine[]){       // prepinac -a, cize zoskupuje medzery a nechava iba slova
    while(*line != '\0'){
        if (isalpha(*line)) {                   // ci je pismeno
            *outLine = *line;
            outLine++;
        }

        else
            if ((isalpha(*(line + 1))) || ((*(line + 1)) == '\0')) {        // ak je buduce pismeno alebo ak je na konci
                *outLine = ' ';
                outLine++;
            }
        line++;
    }
    *outLine = '\0';
}

void cSwitch(char line[],char outLine[]){           // prepinac -c ,zoskupuje pismena

    while(*line != '\0'){
        if (isalpha(*line)) {
            *outLine = *line;
            outLine++;
        }
        line++;
    }
    *outLine = '\0';
}

void rSwitch(char line[], char outLine[], char* replace) {      // prepinac -r
    char* p;
    while (*line != '\0') {
        while(1) {
            p = strstr(line, replace);          // hladanie retazcov replace
            if (p != NULL) {
                if ((strcmp(line, p) == 0)) {       // ak je pointer p na hladanom retazci
                    for (int i = 0; i < strlen(replace); i++) {     // nahradenie danym poctom hviezdiciek
                        *outLine = '*';
                        outLine++;
                        line++;
                    }
                }
                else{               // ak su dva retazce za sebou
                    break;
                }
            }
            if (p  == NULL)         //  ukoncenie while cyklu
                break;
        }
        if (*line == '\0')          // koniec retazca
            break;
        *outLine = *line;
        outLine++;
        line++;
    }
    *outLine = '\0';
}

char cezShift(char c, int shift) {      // funkcia na posun cez Cezarovu sifru
    char let;
    int num;
    if (isupper(c))
        num = c - 65;
    else
        num = c - 97;
    num = (num + shift) % 26;
    if (num<0)                          // ak je posun zaporne cislo
        num = 26 + num;                 // posun do predu v abecede
    if (isupper(c))
        let = num + 'A';
    else
        let = num + 'a';
    return let;
}

void eSwitch(char line[], char outLine[], int shift)        // prepinac -e a shift je cislo, o ktore sa ma posunut
{
    while(*line != '\0'){
        if(isalpha(*line))                              // pismeno sa transofrmuje pomocu cezarovej sifry
            *outLine = cezShift(*line, shift);
        else
            *outLine = *line;
        line++;
        outLine++;
    }
    *outLine = '\0';
}

void inSwitch(char line[], char KeyShift, char outLine[],char *param) {     // spracovanie vstupnych prepinacov
    char *replace= NULL;                // parameter pre prepinac -r
    int shift;                          // parameter pre pripinac -e
   switch(KeyShift){
        case 'l':
            lSwitch(line, outLine);
            break;
        case 'u':
            uSwitch(line, outLine);
            break;
        case 'a':
            aSwitch(line, outLine);
            break;
        case 'c':
            cSwitch(line, outLine);
            break;
       case 'r':
           replace = param;
           rSwitch(line, outLine, replace);
           break;
       case 'e':
           shift = atoi(param);                     // premane na integer
           eSwitch(line, outLine, shift);
    }
}

void wSwitch(char line[], char lineOut[]) {
    int flag = 0;

    while (*line != '\0') {
        if (isalnum(*line)) {           // ak je pismeno alfanumericky znak
            *lineOut = *line;
            lineOut++;
            flag = 1;                       // pre spravne odsadzovanie
        }
        else if (isalnum(*(line +1 )) && ((*(line +1)) != '\0') && flag) { // ak je buduce pismeno alfanumericky znak alebo je na konci
            *lineOut = '\n';
            lineOut++;
        }
        line++;
    }
    *lineOut = '\0';
}

void gSwitch(char line[], char lineOut[], int group)
{
    while(*line != '\0'){
        for(int i=0;i<group;i++){
            if (*line == '\0')
                break;
            *lineOut = *line;
            line++;
            lineOut++;
        }
        if (*line == '\0')
            break;
        *lineOut = ' ';
        lineOut++;
    }
    *lineOut='\0';
}


void outSwitch(char line[],char keyShift, char lineOut[],char *param){      // funkcia pre spracovanie vystupnych prepinacov
    int group;                                  // pocet zoskupenych pismen
    switch(keyShift) {                          // keyShift je prepinac
        case 'w':
            wSwitch(line, lineOut);
            break;
        case 'g':
            group = atoi(param);
            gSwitch(line, lineOut, group);
            break;
    }
}

void printEditor(char lineOut[])        //funkcia pre vypis
{
    printf("%s\n",lineOut);
}

int isnumber(char str[]){       // funkcia vracia 1 ak je to cislo inak 0.
    if (*str == '\0')
        return 0;
    while(*str!='\0' ){
        if (!((*str >= 48 && *str <= 57)|| *str == 45 ))
            return 0;
        str++;
    }
    return 1;
}

int isposnumber(char str[]){        // funkia vracia 1 ak je to kladne cislo inak 0.
    if (*str == '\0')
        return 0;
    while(*str!='\0' ){
        if (!((*str >= 48 && *str <= 57)))
            return 0;
        str++;
    }
    return 1;
}
void repeatInput(char str[]){
    str[strlen(str)]='\n';              // pridanie odsadenia
    while((strcmp(str,"\n"))){          // ukoncenie ak uzivatel zadal ENTER
        printf("%s",str);
        fgets(str,MAX_INPUT,stdin);
    }
    exit(0);
}

void spracovanie(int argc, char *argv[], int arr[], char *paramg, char *paramr, char *parame,char str[]){
    strcpy(str,"ulacwe:r:g:");              // prepinace
    int c;                                          // integer pre dany prepinca
    int o = 6;                                      // pocitadlo pre vystupne prepinace
    int i = 0;                                      // pocitadlo pre vstupne prepinace
    while ((c = getopt (argc, argv, str)) != -1){           // spracovanie CMD argumentov do pola arr
        if( c == '?')
            exit(0);
        if (c == 'g' || c == 'w'){                          // vystupne argumenty idu na koniec od 6 indexu
            arr[o]=c;
            o++;
            if (c == 'g') {
                if (!(isposnumber(optarg)))                 // ak nie je parameter kladne cislo
                    exit(0);
                strcpy(paramg, optarg);                     // kopirovanie parametru prepinaca -g
            }
        }
        else if (c == 'u' || c == 'l' || c == 'a' || c == 'c' || c == 'r' || c == 'e') {       // vstupne indexi idu na zaciatok od 0. indexu
            arr[i] = c;
            i++;
            if (c == 'r')
                strcpy(paramr, optarg);                 // kopirovanie parametru prepinaca -r
            else if(c == 'e') {
                if (!(isnumber(optarg)))                // ak nie je parameter cislo
                    exit(0);
                strcpy(parame, optarg);                 // kopirovanie parametru prepinaca -e
            }
        }
    }
}

void transformacia(int arr[], int flag, char *param, char *paramr, char *parame, char *paramg, char line[], char lineOut[]){
    for (int j=0; j<6; j++){                // vykonanie vstupnych prepinacov od indexu 0, lebo tam su vstupne prepinace
        if (!(arr[j]>=97 && arr[j]<=122))       // ak prepincac nie je z abecedy tak sa preskoci
            continue;
        if (arr[j] == 'c')
            flag = 1;                           // vlajka pri pouziti prepinaca c
        if (arr[j] == 'r')
            strcpy(param, paramr);              // kopirovanie parametru prepinaca -r
        else if(arr[j] == 'e')
            strcpy(param, parame);              // kopirovanie parametru prepinaca -e
        inSwitch(line,arr[j],lineOut, param);   // spracovanie vstupnych prepinacov
        strcpy(line,lineOut);                   // kopirovanie do stringu line, lebo buduce prepinace pracuju so stringom line (inSwitch)
    }
    for (int j=6; j<8;j++){                         // vykonanie vystupnych prepinacov od indexu 6, lebo tam su vystupne argumenty
        if (arr[j] == 'g') {
            if (flag == 0)                          // ak nebol pouzity prepinac -c
                repeatInput(line);                  // zopakovanie vstupu
            strcpy(param, paramg);                  // kopirovanie parametru prepinaca -g
        }
        outSwitch(line,arr[j],lineOut, param);      // spracovanie vystupnych prepinacov
        strcpy(line,lineOut);                       // potrebne pre funkciu outSwitch
    }
}



int run_text_processor(int argc, char *argv[]) {

    char line[MAX_INPUT];                           // vstupny string ktory spracuje funkcia fgets
    char lineOut[MAX_INPUT];                        // vystupny string, ktory sa vypise do konzoly
    char str[20];                                   // string, ktory obsahuje argumenty CMD
    int arr[8];                                     // pole, ktore sa naplni CMD argumentami ako char
    char parame[30];                                // pamat pre CMD arguemnt -e
    char paramr[30];                                // pamat pre CMD argument -r
    char paramg[30];                                // pamat pre CMD arguemnt -g
    char param[30];                                 // pamat pre optarg
    int flag = 0;                                   // vlajka pre CMD argument -g, nutnost pouzit argument -c
    opterr = 0;                                     // zabranenie chybovemu hlaseniu pre funkciu getopt

    spracovanie(argc, argv, arr, paramg, paramr, parame, str);      // spracovanie CMD argumentov to pola arr[]

    while(1){
        fgets(line,MAX_INPUT,stdin);            // citanie vstupu
        if(strcmp(line,"\n")==0)                // ukoncovacia podmienka pre vstup \n
            break;
        line[strlen(line)-1]='\0';              // orezanie stringu line od "\n"
        if (argc == 1)                          // pri zadani iba prog.exe
            repeatInput(line);                  // zopakovanie vstupu
        transformacia(arr, flag, param, paramr, parame, paramg, line ,lineOut);         // uprava textu podla prepinacov a spracovanie do lineOut
        printEditor(lineOut);                         // vypisovanie stringu lineOut
    }
    return 0;
}

int main(int argc, char *argv[]) {
    run_text_processor(argc, argv);     // funkcia pre spracovanie textu
    return 0;
}
