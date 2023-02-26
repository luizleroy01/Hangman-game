#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int tentativas=0, erros=0;
int tamanho_palavra,possibilidades=0;
char *palavrasecreta;
char **basededados;
char alfabeto[]="abcdefghijklmnopqrstuvwxyz";
char vogais[]={'a','e','i','o','u'};


void esconde_palavra(){

    int i;
    for(i=0;i<tamanho_palavra;i++){
        palavrasecreta[i]='-';
    }
    palavrasecreta[tamanho_palavra]='\0';
}


char comeco_forca(){

    char resp;
    printf("Escolha uma palavra da base de dados e digite(s) quando terminar: ");
    system("Call br-sem-acentos.txt");
    scanf("%c",&resp);
        if(resp=='s' || resp=='S'){
            printf("Quantas letras a palavra possui? ");
            scanf("%d",&tamanho_palavra);
            palavrasecreta=(char*)malloc(tamanho_palavra*sizeof(char));
            esconde_palavra();
        }


    return resp;

}


char** carrega_base_dados(){

    FILE *arquivo;
    char palavraauxiliar[100];
    arquivo=fopen("br-sem-acentos.txt","r");
    if(arquivo==NULL){
        printf("Arquivo nao encontrado.\n");
        return NULL;
    }
    char **matriz;


    while(!feof(arquivo)){
        fscanf(arquivo,"%s",&palavraauxiliar);
        if(strlen(palavraauxiliar)==tamanho_palavra){
            if(possibilidades==0){
                matriz=(char**)malloc(sizeof(char*));
                matriz[possibilidades]=(char*)malloc(tamanho_palavra*sizeof(char));
                strcpy(matriz[possibilidades],palavraauxiliar);
            }
            else{
                matriz=(char**)realloc(matriz,(possibilidades+1)*sizeof(char*));
                matriz[possibilidades]=(char*)malloc(strlen(palavraauxiliar)*sizeof(char));
                strcpy(matriz[possibilidades],palavraauxiliar);
            }
            possibilidades++;
        }
    }


    fclose(arquivo);
    return matriz;
}


void placar(){


    printf("---------------------------------------\n");
    printf("%d      -> Tentativas\n",tentativas);
    printf("%d      -> Erros\n",erros);
    printf("%d      -> Possibilidades de palavras\n",possibilidades);
    printf("%s      -> Palavra\n",palavrasecreta);
    printf("---------------------------------------\n");

}


bool verifica_alfabeto(char letra){
 int i;
 for(i=0;i<26;i++){
  if(alfabeto[i]==tolower(letra) && alfabeto[i]!=' '){
   return true;
  }
 }
 return false;
}


void retiraLetrasRepetidas(char palavraverificada[]){
 int i,j;
 for(i=0;i<tamanho_palavra;i++){
  for(j=i+1;j<tamanho_palavra;j++){
   if(tolower(palavraverificada[i])==tolower(palavraverificada[j])){
    palavraverificada[j]='-';
   }
  }
 }
}


char letraFrequente(){

 int tamanhostringona=(tamanho_palavra*possibilidades);
 char *auxC=(char*)malloc(tamanhostringona*(sizeof(char)));
 int i=0,auxQ=0,auxQuant=0,j;
 char *stringAux=(char*)malloc(tamanho_palavra*(sizeof(char)));
 char letra;
 do{

  strcpy(stringAux,basededados[i]);
  retiraLetrasRepetidas(stringAux);
  strcat(auxC,stringAux);
  i++;
 }while(i<possibilidades);
 auxC[i]='\0';

 for(i=0;i<tamanhostringona;i++){
  for(j=0;j<tamanhostringona;j++){
   if(auxC[i]!='-' && auxC[i]!='\0'){
    if(tolower(auxC[i])==tolower(auxC[j])){
     auxQ++;
    }
   }

  }

  if(auxQ>auxQuant && verifica_alfabeto(auxC[i])){
   auxQuant=auxQ;
   letra=tolower(auxC[i]);
  }
  auxQ=0;
 }

 free(auxC);
 free(stringAux);
 return letra;
}


void exclui_palavras(char letra){
 int i,j,z=0;
 bool encontrou_palavra=false;
 int indicePalavra,contadorExcluidas=0;
 i=0;
 do{
  if(letra==0){
   for(j=0;j<tamanho_palavra;j++){
    if(palavrasecreta[j]!='-'){
     if(tolower(palavrasecreta[j])!=tolower(basededados[i][j])){
      encontrou_palavra=true;
      indicePalavra=i;
     }
    }
   }
  }else{

   for(j=0;j<tamanho_palavra;j++){
if(tolower(basededados[i][j])==tolower(letra)){
     encontrou_palavra=true;
     indicePalavra=i;
    }
   }
  }

   if(encontrou_palavra==true){
    contadorExcluidas++;
     strcpy(basededados[i]," ");
   }
  i++;
  encontrou_palavra=false;
 }while(i!=possibilidades);
 for(j=0;j<possibilidades;j++){
  if(strcmp(basededados[j]," ")==0){
   //free(basededados[j]);
   //printf("para aqui\n");
  }else{
   strcpy(basededados[z],basededados[j]);
   z++;
  }
 }

    possibilidades=possibilidades-contadorExcluidas;
    //basededados=(char**)realloc(basededados,possibilidades*(sizeof(char*)));

}
bool verificaFimForca(){
	int i;
	for(i=0;i<possibilidades;i++){
		if(strcmp(palavrasecreta,basededados[i])==0){
			return false;
		}
	}
	return true;
}

void removeletra_alfabeto(char letra){

 int i;
 for(i=0;i<26;i++){
  if(alfabeto[i]==tolower(letra)){
   alfabeto[i]=' ';
  }
 }
}


void chute_letras(){

    int qtletrarep=0,posicao=0,contauxletrarep=0;
    char letra = letraFrequente();
    removeletra_alfabeto(letra);

        printf("Quantas vezes a letra '%c' aparece na palavra? \n",letra);
        scanf("%d",&qtletrarep);
            if(qtletrarep>0){
            if(qtletrarep>1){
   do{
    printf("Digite a posicao da letra '%c' quando aparece pela %d vez: \n", letra,(contauxletrarep+1));
    scanf("%d",&posicao);
    posicao=posicao-1;
    palavrasecreta[posicao]=letra;
    contauxletrarep++;
   }while(contauxletrarep<qtletrarep);
  }else{
   printf("Essa letra aparece em qual posicao? \n");
   scanf("%d",&posicao);
   posicao=posicao-1;
   palavrasecreta[posicao]=letra;
  }
  exclui_palavras(0);
  tentativas++;
  return;

 }
 exclui_palavras(letra);
 tentativas++;
 erros++;
}



bool chute_vogal(){

    if(tentativas==5){
        return true;
    }
 removeletra_alfabeto(vogais[tentativas]);

 int qtletrasrep=0,posicao=0,contauxletrasrep=0;
 printf("Quantas vezes a letra '%c' aparece na palavra?\n",vogais[tentativas]);
 scanf("%d",&qtletrasrep);
 if(qtletrasrep>0){
  if(qtletrasrep>1){
   do{
    printf("Digite a posicao da letra '%c' quando aparece pela %d vez: \n",(vogais[tentativas]),(contauxletrasrep+1));
    scanf("%d",&posicao);
    posicao=posicao-1;
    palavrasecreta[posicao]=vogais[tentativas];
    contauxletrasrep++;
   }while(contauxletrasrep<qtletrasrep);
  }else{
   printf("Essa letra aparece em qual posicao? \n");
   scanf("%d",&posicao);
   posicao=posicao-1;
   palavrasecreta[posicao]=vogais[tentativas];
  }
  exclui_palavras(0);
  tentativas++;
  return true;
 }
 exclui_palavras(vogais[tentativas]);
 tentativas++;
 erros++;
 return false;
}


int main(){

    char resp;
    int i;
    bool encontra_vogal=false;


    resp=comeco_forca();

    basededados=carrega_base_dados();
    system("cls");



    while(verificaFimForca()){


		placar();
        if(encontra_vogal==false){
            
            encontra_vogal=chute_vogal();
            
        }
        if(encontra_vogal==true){
           
            chute_letras();
            
        }
		system("pause");
        system("cls");

    }

    placar();
    printf("Ahh, ja sei!! A palavra escolhida eh %s\n",palavrasecreta);
    printf("FIM!");
    free(palavrasecreta);
    free(basededados);
    return 0;



} 
