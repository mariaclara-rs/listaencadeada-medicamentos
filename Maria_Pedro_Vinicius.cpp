#include "TadLDDE.h"
#include "meuconio.h"
#include<conio.h>
#include<string.h>
#define PRETO 0
#define VERDE 10
#define FUNDO 8

bool excluiPais(Descritor &Desc, char nomePais[]);
void tela(char titulo[]){
	textcolor(VERDE);
    int i,x;
    //informacoes:
    x = 40-(strlen(titulo)/2);
    textbackground(FUNDO);
    clrscr();

    //horizontal
    for(i=2;i<=79;i++){
        gotoxy(i,2);        printf("%c",205);
        gotoxy(i,25);        printf("%c",205);
    }
    //vertical
    for(i=3;i<=24;i++){
        gotoxy(1,i);        printf("%c",186);
        gotoxy(80,i);        printf("%c",186);
    }
    //cantos
    gotoxy(1,2);        printf("%c",201);
    gotoxy(80,2);        printf("%c",187);
    gotoxy(1,25);        printf("%c",200);
    gotoxy(80,25);        printf("%c",188);

    //informacoes
    gotoxy(x,1); printf(titulo);
    textcolor(PRETO);
}
void printC(int colIni, int colFim, int y, char frase[]){ 
     int x = ((colFim+colIni)/2)-(strlen(frase)/2);
     if(x<0) x=0;
     gotoxy(x,y);
     printf("%s",frase);
}
PontPais *buscaPais(Descritor Desc, char pais[]){

	while(stricmp(pais,Desc.inicio->nome)>0 && Desc.inicio->prox!=NULL)
		Desc.inicio = Desc.inicio->prox;
	if(stricmp(Desc.inicio->nome,pais)==0)
		return Desc.inicio;
	else
		return NULL;
}
PontMed *buscaMed(PontMed *pMed, char med[]){
	
	while(stricmp(med,pMed->nome)!=0 && pMed->prox!=NULL)
		pMed = pMed->prox;
	if(stricmp(med,pMed->nome)==0)
		return pMed;
	else
		return NULL;
}
void excluiMed(Descritor &Desc){
	char nomeMed[100];
	bool vazio;
	PontMed *ant, *atual;
	PontPais *pPais, *pAux;

	do{
		vazio = true;
		pPais = Desc.inicio;
		tela("Excluir");
		gotoxy(2,1);
		printf("[ENTER] Voltar");
		printC(0,80,12,"Medicamento: ");
		fflush(stdin);
		gets(nomeMed);
		if(strcmp(nomeMed,"\0")!=0){
			while(pPais!=NULL){
				//inicio
				atual = pPais->med;
				if(stricmp(atual->nome,nomeMed)==0){
					excluiUsuarios(atual->usuario);
					pPais->med = pPais->med->prox;
					delete(atual);
					if(pPais->med==NULL){
						pAux = pPais;
						pPais = pPais->prox;
						excluiPais(Desc,pAux->nome);
					}
					else
						pPais = pPais->prox;
					vazio = false;
				}
				else{
					while(stricmp(atual->nome,nomeMed)!=0 && atual->prox!=NULL){
						ant = atual;
						atual = atual->prox;
					}
					if(stricmp(atual->nome,nomeMed)==0){
						excluiUsuarios(atual->usuario);
						ant->prox = atual->prox;
						delete(atual);
						vazio = false;		
					}
					pPais = pPais->prox;
				}
						
			}
			if(vazio){
				tela("(!)");
				printC(0,80,13,"Medicamento nao encontrado!");
			}
			else{
				printC(0,80,14,"Excluido!");
			}
			printC(0,80,16,"[ENTER] prosseguir");
	        gotoxy(2,1);
			printf("              ");
	        getch();
	    }
	}while(strcmp(nomeMed,"\0")!=0 && !vazia(Desc.inicio));
}
bool excluiPais(Descritor &Desc, char nomePais[]){
	
	PontPais *pPais;
	PontMed *pMed;

	//inicio
	if(stricmp(Desc.inicio->nome,nomePais)==0){
		excluiMedicamentos(Desc.inicio->med);
		Desc.inicio = Desc.inicio->prox;
		if(vazia(Desc.inicio)){
			delete(Desc.fim);
			inicializaDescritor(Desc);
		}
		else{
			delete(Desc.inicio->ant);
			Desc.inicio->ant = NULL;
		}
	}
	//fim
	else if(stricmp(Desc.fim->nome,nomePais)==0){
		excluiMedicamentos(Desc.fim->med);
		Desc.fim = Desc.fim->ant;	
		delete(Desc.fim->prox);
		Desc.fim->prox = NULL;
	}
	else{
		pPais = buscaPais(Desc, nomePais);
		if(pPais!=NULL){
			excluiMedicamentos(pPais->med);
			pPais->ant->prox = pPais->prox;
			pPais->prox->ant = pPais->ant;
			delete(pPais);
		}
		else
			return false;
	}
	return true;

}
void insereUsuario(FILE *arq, Descritor &Desc){
	PontUsuario *usuarioAux = new PontUsuario;
	usuarioAux->prox = NULL;
	PontMed *medAux;
	PontPais *paisAux;
	char nomePais[50], nomeMed[100], tecla=0, aux[50];
	int qtde=0;
	//409-95-6118,F,Amlodipine Besylate,06/01/2018,China
	
	tela("Insercao de usuarios");
	gotoxy(2,1);printf("[ESC] Voltar");
	
	while(!feof(arq) && tecla!=27){
		fscanf(arq,"%[^,],%c,%[^,],%[^,],%[^\n]\n",&usuarioAux->cod,&usuarioAux->sexo,&nomeMed,&usuarioAux->ultCompra,&nomePais);
		if(!vazia(Desc.inicio)){
			paisAux = buscaPais(Desc, nomePais);
			if(paisAux!=NULL){
			//pais existe
				medAux = buscaMed(paisAux->med, nomeMed);
				if(medAux!=NULL){
				//medicamento existe
				//usuario insere no inicio
					usuarioAux->prox = medAux->usuario;
					medAux->usuario = usuarioAux;
				}
				else{
				//medicamento não existe
					//cabeça da lista de medicamentos, ponteiro para o med criado
					paisAux->med = insereMed(paisAux->med, criaMed(nomeMed, usuarioAux));	
				}
			}
			else{
				inserePais(Desc,criaPais(nomePais,criaMed(nomeMed, usuarioAux)));
			}
		}
		else
			inserePrimeiro(Desc,criaPais(nomePais,criaMed(nomeMed, usuarioAux)));
		qtde++;
		sprintf(aux,"Inseridos agora: %03d",qtde);
		printC(0,80,13,aux);
		printC(0,80,15,"[ENTER] Continuar inserindo ");
		tecla = getch();
		if(tecla!=27)
			usuarioAux = new PontUsuario();
	}
}
bool buscaUsuario(PontMed *Cabeca, PontMed *pMed,char cod[]){
	bool encontrou = false;
	PontUsuario *pUsuario;
	while(Cabeca!=pMed && !encontrou){
		pUsuario = Cabeca->usuario;
		while(pUsuario!=NULL && stricmp(cod,pUsuario->cod)!=0)
			pUsuario = pUsuario->prox;
		if(pUsuario!=NULL)
			encontrou = true;
		Cabeca = Cabeca->prox;
	}
	return encontrou;		
}
void exibirUsuarios(FILE *arq, PontMed *pMed, char sexo){
	bool vazio = true;
	PontUsuario *pU;
	if(sexo == 'F')
		fprintf(arq,"\tFeminino\n");
	else
		fprintf(arq,"\tMasculino\n");
	PontMed *L = pMed;
	while(pMed!=NULL){
		pU = pMed->usuario;
		while(pU!=NULL){
			if(pU->sexo == sexo && !buscaUsuario(L,pMed,pU->cod)){
				fprintf(arq,"\t\t%s\n",pU->cod);
				vazio = false;
			}
			pU = pU->prox;
		}
		pMed = pMed->prox;
	}
	if(vazio){
		fprintf(arq,"\t\tNão há usuários\n");
	}
}
void Relatorio(Descritor Desc){
	FILE *arq = fopen("relatorio.txt","w");
	PontMed *pMed;

	while(Desc.inicio!=NULL){
		fprintf(arq,"%s\n",Desc.inicio->nome);
		
		pMed = Desc.inicio->med;
		exibirUsuarios(arq, pMed, 'F');
		exibirUsuarios(arq, pMed, 'M');
		
		Desc.inicio = Desc.inicio->prox;
	}
	fclose(arq);
}
void medicamentoPais(Descritor Desc){
	PontPais *pPais;
	PontMed *pMed;
	char nomePais[50], aux[70];
	gotoxy(2,1);
	printf("[ENTER] Voltar");
	printC(0,80,13,"Pais:");
	fflush(stdin);
	gets(nomePais);	
	system("cls");
	if(strcmp(nomePais,"\0")!=0){
		
		pPais = buscaPais(Desc, nomePais);
		if(pPais!=NULL){
			sprintf(aux,"Medicamentos - %s\n",nomePais);
			printC(0,80,1,aux);
			printf("\n");
			pMed = pPais->med;
			while(pMed!=NULL){
				printf("\t%s\n",pMed->nome);
				pMed = pMed->prox;
			}
		}
		else{
			tela("(!)");
			printC(0,80,13,"Pais nao encontrado!");
		}
		gotoxy(2,1);
		printf("[ENTER] Voltar");
		getch();
	}
}
void usuarioMed(Descritor Desc){
	PontMed *pMed;
	PontUsuario *pU;
	char nomeMed[100], aux[120];
	bool vazia=true;
	gotoxy(2,1);
	printf("[ENTER] Voltar");
	printC(0,80,12,"Medicamento:");
	fflush(stdin);
	gets(nomeMed);
	if(strcmp(nomeMed,"\0")!=0){
		system("cls");
		gotoxy(2,1);
		printf("[ENTER] Voltar");
		gotoxy(1,3);
		while(Desc.inicio!=NULL){
			pMed = buscaMed(Desc.inicio->med,nomeMed);
			if(pMed!=NULL){
				pU = pMed->usuario;
				while(pU!=NULL){
					printf("\n\tcod: %s sexo: %c ultima compra: %s pais: %s",pU->cod,pU->sexo,pU->ultCompra,Desc.inicio->nome);
					pU = pU->prox;
				}
				vazia=false;
			}
			Desc.inicio = Desc.inicio->prox;
		}
		if(vazia){
			tela("(!)");
			gotoxy(2,1);
			printf("[ENTER] Voltar");
			printC(0,80,12,"Medicamento nao encontrado!");
		}
		else{
			sprintf(aux,"Usuarios - %s\n",nomeMed);
			printC(0,80,1,aux);
		}
		getch();
	}
}
void printLinha(char letra,char cor){
	textcolor(cor);
	switch(letra){
		case 'A':	gotoxy(29,9);
					printf("Inserir novos usuarios");
				    //printC(0,80,9,"Inserir novos usuarios");
				    break;
		case 'B':
					gotoxy(29,10);
					printf("Visao geral");
					//printC(0,80,10,"Exibir");
					break;
		case 'C':	
				    gotoxy(29,11);
					printf("Excluir pais");
					//printC(0,80,11,"Excluir pais");			
					break;
		case 'D':	
					gotoxy(29,12);
					printf("Excluir medicamento");		
				//	printC(0,80,12,"Excluir medicamento");
					break;
		case 'E':
					gotoxy(29,13);
					printf("Gerar relatorio");
					//printC(0,80,13,"Gerar relatorio");
					break;
		case 'F':
					gotoxy(29,14);
					printf("Exibir medicamentos de um pais");
				    //printC(0,80,14,"Exibir medicamentos de um pais");
					break;
		case 'G':
					gotoxy(29,15);
					printf("Exibir usuarios de um medicamento");
				    //printC(0,80,15,"Exibir usuarios de um medicamento");
					break;		
	}
}
void alerta(void){
	tela("(!)");
	printC(0,80,13,"Lista Vazia!");
	gotoxy(2,1); printf("[ENTER] Voltar");
}
void menu(void){

	Descritor Desc;
	inicializaDescritor(Desc);
	FILE *arq = fopen("DadosMedicamentos.csv","r");
	char opc,p;
	char letra='A', cor=PRETO;
	do{
		tela("MENU");
		gotoxy(2,1);printf("[Esc]: fechar");
	    printC(0,80,8,"Opcoes:");
	    printLinha('A',PRETO); 
	    printLinha('B',PRETO);
	    printLinha('C',PRETO);
	    printLinha('D',PRETO);
	    printLinha('E',PRETO);
	    printLinha('F',PRETO);
	    printLinha('G',PRETO);
		do{
			if(kbhit()){
				p = getch();
				printLinha(letra,PRETO);
				if(p==-32)
			    {
			    	p=getch();
			    	switch(p){
			    		case 72:	if(letra=='A'){
										letra='G';
									}else{
										letra--;
									}
										
										
			    				break;
			    		case 80:	if(letra=='G')
										letra='A';
									else
										letra++;
			    				
								break;
			    	}
			    }
			}
			cor==VERDE? cor = PRETO : cor = VERDE;
			printLinha(letra,cor);
			Sleep(300);
				
		}while(p!=13 && p!=27);
		if(p==13){
			p=0;
			switch(letra){
	        case 'A':
	        	if(!feof(arq))
	        		insereUsuario(arq, Desc);
	        	else{
	        		tela("(!)");
	        		gotoxy(2,1);
	        		printf("[ENTER] Voltar");
	        		printC(0,80,13,"Fim de arquivo!");
	        		getch();
	        	}
	            break;
	        case 'B':            	
	        	if(!vazia(Desc.inicio)){
	            	//tela("Exibir");
	            	system("cls");
	            	gotoxy(1,1);
	            	textcolor(PRETO );
					printf("[ENTER] Voltar");
	 				exibir(Desc);
				}
				else
					alerta();
	        	getch();
	        	break;
	        case 'C':
	        	if(!vazia(Desc.inicio)){
                    char nomePais[50];
                    do{
                        tela("Excluir Pais");
                        gotoxy(2,1);
						printf("[ENTER] Voltar");
                        printC(0,74,12,"Pais: ");
                        fflush(stdin);
                        gets(nomePais);
						if(strcmp(nomePais,"\0")!=0){
	                        if(excluiPais(Desc,nomePais)){
	                            printC(0,80,14,"Excluido");
	                        }
	                        else{
	                        	tela("(!)");
	                            printC(0,80,13,"Pais nao encontrado!");
	                        }
	                        printC(0,80,16,"[ENTER] prosseguir");
	                        gotoxy(2,1);
							printf("              ");
	                        getch();
	                    }

                    }while(strcmp(nomePais,"\0")!=0 && !vazia(Desc.inicio));
                    if(vazia(Desc.inicio)){
                    	alerta();
                    	getch();
                    }
                }
                else{
                    alerta();
                    getch();
                }
				break;
	        case 'D':
	        	if(!vazia(Desc.inicio)){
	        		excluiMed(Desc);
	        	}
	        	else{
	        		alerta();
	        		getch();
	        	}
	        	break;
	        case 'E':
	        	if(!vazia(Desc.inicio)){
	        		tela("Relatorio");
	        		Relatorio(Desc);
	        		printC(0,80,13,"Relatorio criado!");
	        	}
	        	else
	        		alerta();
	        	getch();
	        	break; 	
	        case 'F':
	        	if(!vazia(Desc.inicio)){
	        		tela("Exibir medicamentos de um pais");
	        		medicamentoPais(Desc);
	        	}
	        	else{
	        		alerta();
	        		getch();
	        	}
	        	break; 
	        case 'G':
	        	if(!vazia(Desc.inicio)){
	        		tela("Exibir usuarios de um medicamento");
	        		usuarioMed(Desc);
	        	}
	        	else{
	        		alerta();
	        		getch();
	        	}
	        	break;
	    	}	
		} 
		
    }while(p!=27);
    fclose(arq);
}
int main(void){
	menu();
	system("cls");
	return 1;
}
