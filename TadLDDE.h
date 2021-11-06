#include<stdio.h>
#include "TadLDSE.h"

struct PontPais{
	PontPais *ant, *prox;
	char nome[50];
	PontMed *med;
};
struct Descritor{
	PontPais *inicio, *fim;
};
void inicializaDescritor(Descritor &Desc){
	Desc.inicio = Desc.fim = NULL;
}
char vazia(PontPais *inicio){
	return inicio == NULL;
}
PontPais *criaPais(char nomePais[], PontMed *med){
	PontPais *pPais = new PontPais;
	strcpy(pPais->nome,nomePais);
	pPais->ant = pPais->prox = NULL;
	pPais->med = med;
	return pPais;
}
//inserir o primeiro pais
void inserePrimeiro(Descritor &Desc, PontPais*pPais){
	Desc.inicio = Desc.fim = pPais;
}
//ordenado
void inserePais(Descritor &Desc, PontPais*pPais){
	
	//no inicio
	if(stricmp(pPais->nome,Desc.inicio->nome)<0){
		Desc.inicio->ant = pPais;
		pPais->prox = Desc.inicio;
		Desc.inicio = pPais;
	}
	//no fim
	else if(stricmp(pPais->nome,Desc.fim->nome)>0){
		Desc.fim->prox = pPais;
		pPais->ant = Desc.fim;
		Desc.fim = pPais;
	}
	//no meio
	else{
		PontPais *p = Desc.inicio->prox;
		while(stricmp(pPais->nome,p->nome)>0)
			p = p->prox;
		pPais->prox = p;
		pPais->ant = p->ant;
		p->ant = pPais;
		pPais->ant->prox = pPais;
	}
}
void exibir(Descritor Desc){

	PontMed *pMed;
	PontUsuario *pUs;
	
	while(Desc.inicio!=NULL){
		printf("\n|%s|",Desc.inicio->nome);
		pMed = Desc.inicio->med;
		while(pMed!=NULL){
			printf("\n\t|%s|\n\t\t",pMed->nome);
			pUs = pMed->usuario;
			while(pUs!=NULL){
				printf("|%s|",pUs->cod);
				pUs = pUs->prox;
			}
			pMed = pMed->prox;
		}
		Desc.inicio = Desc.inicio->prox;
	}
}

