#include<stdio.h>
#include<string.h>
struct PontUsuario{
	PontUsuario *prox;
	char cod[15], ultCompra[11];
	char sexo;
};
struct PontMed{
	PontMed *prox;
	char nome[100];
	PontUsuario *usuario;
};
PontMed *criaMed(char nomeMed[], PontUsuario *pUsuario){
	PontMed *pMed = new PontMed;
	pMed->prox = NULL;
	strcpy(pMed->nome, nomeMed);
	pMed->usuario = pUsuario;
	return pMed;
}
//medicamentos são inseridos no início
PontMed *insereMed(PontMed *M, PontMed *pMed){
	pMed->prox = M;
	M = pMed;
	return M;
}
//exclui todos os usuários
void excluiUsuarios(PontUsuario *U){
	if(U!=NULL){
		excluiUsuarios(U->prox);
		delete(U);
	}
}
void excluiMedicamentos(PontMed *M){
	if(M!=NULL){
		excluiMedicamentos(M->prox);
		excluiUsuarios(M->usuario);
		delete(M);
	}
}
