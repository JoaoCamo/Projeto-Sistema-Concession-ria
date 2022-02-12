// João Camillo de Moura
//joaocamo@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct endereco { 
 char logradouro[80]; 
 char bairro[15]; 
 char CEP[10]; 
 char cidade[15]; 
 char estado[3]; 
 char fone[12]; 
 char email[40]; 
}; 
  
struct infocarro { 
 char sigla; 
 int regcarro; 
}; 
 
union dadoscarro { 
 char sigla; 
 struct infocarro reservado; 
}; 
 
typedef struct loja { 
 int regloja;
 char nome[30]; 
 char CNPJ[19]; 
 struct endereco end; 
 int sold; 
 int reserved; 
 union  dadoscarro  tabela[3];
}loja;

struct infoloja { 
 char sigla; 
 char CNPJ[19]; 
}; 
 
union dadosloja { 
 char sigla; 
 struct infoloja reserva; 
};
 
typedef struct montadora { 
 int regcarro;
 char modelo[20]; 
 char cor[10]; 
 float  valor; 
 union dadosloja status; 
}montadora;

struct data { 
 int dia;
 int mes;
 int ano;
}; 
 
typedef struct historicoVendas { 
 int reghist; 
 int regcarro; 
 char modelo[20]; 
 char cor[10]; 
 float valor;  
 int regloja; 
 char nome[30]; 
 char cnpj[19]; 
 struct data dataVenda; 
}historico;

void aloca(loja **p, int tam); //Alocar espaço para struct loja
void aloca_carro(montadora **p, int tam); //Alocar espaço para struct montadora
void aloca_historico(historico **p, int tam); //Alocar espaço para struct historicoVendas
int verifica(); //Verificar o tamanho do arquivo concessionaria.bin
int verifica_carro(); //Verificar o tamanho do arquivo carro.bin
int verifica_historico(); //Verificar o tamanho do arquivo historico.bin
void cadastro(loja *p); //Cadastrar concessionarias
void cadastro_carro(montadora *p); //Cadastrar carros
void reserva_carro(montadora *p, loja *pl); //Reservar carros
int busca(loja *p, char cnpj[19]); //Encontrar o CNPJ digitado
int busca_carro_registro(montadora *p, int num_reg); //Encontrar o registro para carro digitado
void busca_carro(montadora *p); //Consultar os carros por consulta total, status, cor e modelo
void busca_carro_status(montadora *p); //Consultar os carros por status
void busca_historico(historico *p); //Consultar os historicos por CNPJ e modelo de carro
void grava(loja *p,char *str, int pos); //Gravar as concessionarias no arquivo
void grava_carro(montadora *p, char *str, int pos); //Gravar os carros no arquivo
void grava_historico(historico *p, char *str, int pos); //Gravar os historicos no arquivo
void consulta(loja *p); //Consultar a concessionaria por CNPJ
void mostra_carro(montadora *p); //Mostrar todos os carros no arquivo
void termina_reserva(loja *p, montadora *pm, historico *ph); //Libera ou vende o carro selecionado
int busca_vazio(montadora *p, int qtde); //Busca se a registros com carros vendidos
void altera(loja *p, montadora *pm); //Altera informações de uma concessionaria ou de um carro



main(){	
loja *pl=NULL;
montadora *pm=NULL;
historico *ph=NULL;
int op;
aloca(&pl,1);
aloca_carro(&pm,1);
aloca_historico(&ph,1);
do{
	system("cls");
	printf("\n\tMontadora\n\n\n[1]Cadastro para concessionarias\n[2]Cadastro para carros\n[3]Alterar cadastro\n[4]Consulta de concessionaria por CNPJ\n[5]Consulta de carros\n[6]Consultar historico de vendas\n[7]Reservar carros\n[8]Terminar reserva\n[9]Fim\n\nOpcao: ");
	scanf("%i",&op);
	fflush(stdin);
	switch(op){
   		case 1:
			cadastro(pl);
   			break;
		case 2:
			cadastro_carro(pm);
   			break;
   		case 3:
   			altera(pl,pm);
   			break;
   		case 4:
			consulta(pl);
        	break;
        case 5:
        	busca_carro(pm);
        	break;
        case 6:
        	busca_historico(ph);
        	break;
        case 7:
        	reserva_carro(pm,pl);
        	break;
        case 8:
			termina_reserva(pl,pm,ph);
			break;
		}
	}while(op!=9);
}



void aloca(loja **p,int tam){
	if((*p=(loja*)realloc(*p,tam*sizeof(loja)))==NULL)
    	exit(1);
}

void aloca_carro(montadora **p,int tam){
	if((*p=(montadora*)realloc(*p,tam*sizeof(montadora)))==NULL)
		exit(1);
}

void aloca_historico(historico **p,int tam){
	if((*p=(historico*)realloc(*p,tam*sizeof(historico)))==NULL)
		exit(1);
}

int verifica(){
	FILE *fptr=NULL;
	long int cont=0;
	if((fptr=fopen("concessionaria.bin","rb"))==NULL)
    	return cont;
	else{
    	fseek(fptr,0,2);
    	cont=ftell(fptr)/sizeof(loja);
    	fclose(fptr);
    	return cont;
    }
}

int verifica_carro(){
	FILE *fptr=NULL;
	long int cont=0;
	if((fptr=fopen("carro.bin","rb"))==NULL)
    	return cont;
	else{
    	fseek(fptr,0,2);
    	cont=ftell(fptr)/sizeof(montadora);
    	fclose(fptr);
    	return cont;
	}
}

int verifica_historico(){
	FILE *fptr=NULL;
	long int cont=0;
	if((fptr=fopen("historico.bin","rb"))==NULL)
		return cont;
	else{
		fseek(fptr,0,2);
		cont=ftell(fptr)/sizeof(historico);
		fclose(fptr);
		return cont;
	}
}

void cadastro(loja *p){
	int i;
	int qtde;
	qtde=verifica();
	system("cls");
	if(qtde<5){
		p->regloja=qtde+1;
		printf("\nRegistro: %i\n",p->regloja);
		printf("\nNome: ");
		gets(p->nome);
		fflush(stdin);
		printf("\nCNPJ: ");
		gets(p->CNPJ);
		fflush(stdin);
		printf("\nLogradouro: ");
		gets(p->end.logradouro);
		fflush(stdin);
		printf("\nBairro: ");
		gets(p->end.bairro);
		fflush(stdin);
		printf("\nCEP: ");
		gets(p->end.CEP);
		fflush(stdin);
		printf("\nCidade: ");
		gets(p->end.cidade);
		fflush(stdin);
		printf("\nEstado: ");
		gets(p->end.estado);
		fflush(stdin);
		printf("\nTelefone: ");
		gets(p->end.fone);
		fflush(stdin);
		printf("\nEmail: ");
		gets(p->end.email);
		fflush(stdin);
		p->sold = 0;
		p->reserved = 0;
		for(i=0;i<3;i++){
			p->tabela[i].sigla = 'L';
		}
		if(p->nome[0] != '\0' && p->CNPJ[0] != '\0' && p->end.logradouro[0] != '\0' && p->end.bairro[0] != '\0' && p->end.CEP[0] != '\0' && p->end.cidade[0] != '\0' && p->end.estado[0] != '\0' && p->end.fone[0] != '\0' && p->end.email[0] != '\0'){
			grava(p,"ab",1);
			printf("\nCadastrado com sucesso\n\n");
		}
		else
			printf("\nInformacoes invalidas\n");
	}
	else
		printf("\nSistema cheio\n\n");
	system("pause");
}

void cadastro_carro(montadora *p){
	int qtde;
	int vend;
	qtde = verifica_carro();
	vend = busca_vazio(p, qtde);
	if(vend != -1)
		qtde = vend;
	system("cls");
	if(qtde<50){
		p->regcarro=qtde+1;
		printf("\nRegistro: %i\n",p->regcarro);
		printf("\nModelo: ");
		gets(p->modelo);
		fflush(stdin);
		printf("\nCor: ");
		gets(p->cor);
		fflush(stdin);
		printf("\nValor: ");
		scanf("%f", &(p->valor));
		fflush(stdin);
		p->status.sigla = 'L';
		if(p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0){
			if(vend == -1){
				grava_carro(p,"ab",1);
				printf("\nCadastrado com sucesso\n\n");
			}
			else{
				grava_carro(p,"rb+",vend);
				printf("\nO registro %i foi utilizado porque o carro que estava nesse registro foi vendido\n\n", p->regcarro);
			}
		}
		else
			printf("\nInformacoes invalidas\n");
		
	}
	else
		printf("\nSistema cheio\n\n");
	system("pause");
}

void reserva_carro(montadora *p, loja *pl){
	int carro_reg;
	int pos;
	int pos2;
	int i;
	char cnpj[19];
	busca_carro_status(p);
	printf("\nDigite o numero do registro a ser reservado: ");
	scanf("%i", &carro_reg);
	fflush(stdin);
	printf("\nDigite o CNPJ de sua loja: ");
	gets(cnpj);
	fflush(stdin);
	pos=busca_carro_registro(p, carro_reg);
	pos2=busca(pl, cnpj);
	if(pos == -1 || pos2 == -1 || p->status.sigla != 'L')
		printf("\nRegistro inexistente/CNPJ inexistente/Carro ja reservado");
	else{
		if(pl->reserved < 3){
			p->status.reserva.sigla = 'R';
			strcpy(p->status.reserva.CNPJ, cnpj);
			grava_carro(p,"rb+",pos);
			pl->reserved+=1;
			for(i=0;i<3;i++){
				if(pl->tabela[i].sigla == 'L'){
					pl->tabela[i].reservado.sigla = 'R';
					pl->tabela[i].reservado.regcarro = carro_reg;
					grava(pl,"rb+",pos2);
					break;
 					}
				}
			printf("\nCarro reservado com sucesso");
		}
		else
			printf("\nTabelas sem espaco para uma reserva adicional");
	}
	printf("\n\n");
	system("pause");
}

int busca(loja *p, char cnpj[19]){
	FILE *fptr;
	int i;
	int achou = -1;
	int qtde;
	qtde=verifica();
	system("cls");
	if((fptr=fopen("concessionaria.bin","rb"))==NULL)
		printf("\nErro");
	else{
		for(i=0;i<qtde;i++){
			fseek(fptr,i*sizeof(loja),0);
      		fread(p,sizeof(loja),1,fptr);
	      	int compara = strcmp(p->CNPJ,cnpj);
    	  	if(compara==0){
        		achou=i;
        		i=qtde;
 				}
  			}
  		fclose(fptr);
		}
	return achou;
}

int busca_carro_registro(montadora *p, int num_reg){
	FILE *fptr=NULL;
	int i;
	int achou=-1;
	int qtde;
	qtde=verifica_carro();
	system("cls");
	if((fptr=fopen("carro.bin","rb"))==NULL)
		printf("\nErro");
	else{
		for(i=0;i<qtde;i++){
			fseek(fptr,i*sizeof(montadora),0);
			fread(p,sizeof(montadora),1,fptr);
			if(p->regcarro==num_reg){
        		achou=i;
        		i=qtde;
 			}
  		}
		fclose(fptr);
	}
	return achou;
}

void busca_carro(montadora *p){
	int i;
	int qtde;
	int cont = 0;
	int opcao;
	char cor_carro[10];
	char modelo_carro[20];
	FILE *fptr = NULL;
	qtde = verifica_carro();
	system("cls");
	if((fptr=fopen("carro.bin","rb"))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		do{
		printf("\n[1]Consulta total\n[2]Consulta por status\n[3]Consulta por cor\n[4]Consulta por modelo\nOpcao: ");
		scanf("%i", &opcao);
		fflush(stdin);
		system("cls");
		switch(opcao){
			case 1:
				mostra_carro(p);
				break;
			case 2:
				busca_carro_status(p);
      			break;
      		case 3:
      			printf("\nDigite a cor que deseja: ");
      			gets(cor_carro);
      			fflush(stdin);
      			for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(montadora),0);
   					fread(p,sizeof(montadora),1,fptr);
   					int compara = strcmp(p->cor, cor_carro);
   					if(compara == 0 && p->status.sigla == 'L' && p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0)
   						printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f\nStatus: %c\n",p->regcarro,p->modelo,p->cor,p->valor,p->status.sigla);
   					else
   						cont++;
  					}
  				if(cont == i)
  					printf("\nNao foram encontrados carros com essa cor/Carros com essa nao estao mais disponiveis");
  				break;
  			case 4:
  				printf("\nDigite o modelo que deseja: ");
      			gets(modelo_carro);
      			fflush(stdin);
      			for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(montadora),0);
   					fread(p,sizeof(montadora),1,fptr);
   					int compara = strcmp(p->modelo, modelo_carro);
   					if(compara == 0 && p->status.sigla == 'L' && p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0)
   						printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f\nStatus: %c\n",p->regcarro,p->modelo,p->cor,p->valor,p->status.sigla);
   					else
   						cont++;
  					}
  				if(cont == i)
  					printf("\nNao foram encontrados carros com esse modelo/Carros desse modelo nao estao mais disponiveis");
  				break;
  			}
  		}while(opcao <= 0 || opcao > 4);
  		fclose(fptr);
		}
	printf("\n\n");
    system("pause");
}

void busca_carro_status(montadora *p){
	int i;
	int qtde;
	int cont = 0;
	int opcao;
	FILE *fptr = NULL;
	qtde = verifica_carro();
	system("cls");
	if((fptr=fopen("carro.bin","rb"))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		do{
		printf("\n[1]Carros livres\n[2]Carros reservados\n\nOpcao: ");
		scanf("%i", &opcao);
		fflush(stdin);
		system("cls");
		switch(opcao){
			case 1:
				for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(montadora),0);
   					fread(p,sizeof(montadora),1,fptr);
   					if(p->status.sigla == 'L' && p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0)
   						printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f\nStatus: %c\n",p->regcarro,p->modelo,p->cor,p->valor,p->status.sigla);
   				 	else
   						cont++;
      				}
      			if(cont == i)
  					printf("\nNao foram encontrados carros livres");
      			break;
			case 2:
				for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(montadora),0);
   					fread(p,sizeof(montadora),1,fptr);
   					if(p->status.sigla != 'L' && p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0)
   						printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f\nStatus: %c\n",p->regcarro,p->modelo,p->cor,p->valor,p->status.sigla);
   				 	else
   						cont++;
      				}
      			if(cont == i)
  					printf("\nNao foram encontrados carros reservados");
      			break;
      		}
    	}while(opcao <= 0 || opcao > 2);
    	fclose(fptr);
    }
}

void busca_historico(historico *p){
	int i;
	int qtde;
	int cont = 0;
	int opcao;
	char cnpj_loja[19];
	char modelo_carro[20];
	FILE *fptr = NULL;
	qtde = verifica_historico();
	system("cls");
	if((fptr=fopen("historico.bin","rb"))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		do{
		printf("\n[1]Consulta por CNPJ\n[2]Consulta por modelo\n\nOpcao: ");
		scanf("%i", &opcao);
		fflush(stdin);
		system("cls");
		switch(opcao){
			case 1:
				printf("\nDigite o CNPJ da loja: ");
      			gets(cnpj_loja);
      			fflush(stdin);
				for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(historico),0);
   					fread(p,sizeof(historico),1,fptr);
   					int compara = strcmp(p->cnpj, cnpj_loja);
   					if(compara == 0){
   						printf("\nRegistro: %i\nRegistro do carro: %i\nModelo: %s\nCor: %s\nData da venda: %i/%i/%i\n", p->reghist, p->regcarro, p->modelo, p->cor, p->dataVenda.dia, p->dataVenda.mes, p->dataVenda.ano);
					   }
					else
						cont++;
				}
				if(cont == i)
					printf("\nNao foram encontrados carros comprados por esse CNPJ");
				else
					printf("\nEsses foram os carros comprados pelo CNPJ: %s", cnpj_loja);
				break;
			case 2:
				printf("\nDigite o modelo do carro: ");
      			gets(modelo_carro);
      			fflush(stdin);
				for(i=0;i<qtde;i++){
					fseek(fptr,i*sizeof(historico),0);
   					fread(p,sizeof(historico),1,fptr);
   					int compara = strcmp(p->modelo, modelo_carro);
   					if(compara == 0){
   						printf("\nRegistro da concessionaria: %i\nNome: %s\nCNPJ: %s\nData da venda: %i/%i/%i\n", p->regloja, p->nome, p->cnpj, p->dataVenda.dia, p->dataVenda.mes, p->dataVenda.ano);
					   }
					else
						cont++;
				}
				if(cont == i)
					printf("\nNao foram encontradas concessionarias que compraram carros desse modelo");
				else
					printf("\nEssas foram as concessionarias que compraram carros do modelo: %s", modelo_carro);
				break;
			}
		}while(opcao <= 0 || opcao > 2);
		fclose(fptr);
	}
	printf("\n\n");
	system("pause");
}

void grava(loja *p, char *str, int pos){
	FILE *fptr=NULL;
	if((fptr=fopen("concessionaria.bin",str))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		if(strcmp(str,"rb+")==0)
			fseek(fptr,pos*sizeof(loja),0);
		fwrite(p,sizeof(loja),1,fptr);
		}
	fclose(fptr);
}

void grava_carro(montadora *p, char *str, int pos){
	FILE *fptr=NULL;
	if((fptr=fopen("carro.bin",str))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		if(strcmp(str,"rb+")==0);
			fseek(fptr,pos*sizeof(montadora),0);
		fwrite(p,sizeof(montadora),1,fptr);
		}
	fclose(fptr);
}

void grava_historico(historico *p, char *str, int pos){
	FILE *fptr=NULL;
	if((fptr=fopen("historico.bin",str))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		if(strcmp(str,"rb+")==0);
			fseek(fptr,pos*sizeof(historico),0);
		fwrite(p,sizeof(historico),1,fptr);
		}
	fclose(fptr);
}

void consulta(loja *p){
	int i;
	int pos;
	char cnpj[19];
	FILE *fptr=NULL;
	system("cls");
	printf("\nDigite o CNPJ da sua concessionaria: ");
	gets(cnpj);
	fflush(stdin);
	pos=busca(p, cnpj);
	system("cls");
	if(pos==-1)
		printf("\nCNPJ inexistente");
	else{
		printf("\nNome: %s\nVendidos: %i\nReservados: %i",p->nome,p->sold,p->reserved);
		for(i=0;i<3;i++){
		if(p->tabela[i].sigla != 'L')
			printf("\nTabela %i: %c - %i", i+1, p->tabela[i].sigla, p->tabela[i].reservado.regcarro);
		}
	}
printf("\n\n");
system("pause");
}

void mostra_carro(montadora *p){
	int i;
	int qtde;
	FILE *fptr = NULL;
	qtde = verifica_carro();
	system("cls");
	if((fptr=fopen("carro.bin","rb"))==NULL)
		printf("\nErro ao abrir o arquivo");
	else{
		for(i=0;i<qtde;i++){
			fseek(fptr,i*sizeof(montadora),0);
   			fread(p,sizeof(montadora),1,fptr);
   			if(p->modelo[0] != '\0' && p->cor[0] != '\0' && p->valor != 0){
   				printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f",p->regcarro,p->modelo,p->cor,p->valor);
   				if(p->status.sigla == 'L')
					printf("\nStatus: %c\n\n", p->status.sigla);
				else
					printf("\nStatus: %c - %s\n\n", p->status.reserva.sigla, p->status.reserva.CNPJ);
      			}
      		}
    	fclose(fptr);
  		}
}

void termina_reserva(loja *p, montadora *pm, historico *ph){
	int carro_reg;
	int n_tabela;
	int pos;
	int pos2;
	int hist_reg;
	int opcao;
	int i;
	char cnpj[19];
	system("cls");
	printf("\nDigite o CNPJ da sua concessionaria: ");
	gets(cnpj);
	fflush(stdin);
	pos=busca(p, cnpj);
	system("cls");
	if(pos==-1)
		printf("\nCNPJ inexistente");
	else{
		printf("\nNome: %s\nVendidos: %i\nReservados: %i",p->nome,p->sold,p->reserved);
		for(i=0;i<3;i++){
		if(p->tabela[i].sigla != 'L')
			printf("\nTabela %i: %c - %i", i+1, p->tabela[i].sigla, p->tabela[i].reservado.regcarro);
		}
		printf("\n\nQual a tabela que deseja usar: ");
		scanf("%i", &n_tabela);
		fflush(stdin);
		if(p->tabela[n_tabela-1].sigla == 'L' || n_tabela > 3){
			printf("\n\nValor invalido");
		}
		else{
			carro_reg = p->tabela[n_tabela-1].reservado.regcarro;
			pos2 = busca_carro_registro(pm, carro_reg);
			printf("\nVoce deseja\n\n[1]Vender\n[2]Liberar\n\nOpcao: ");
			scanf("%i", &opcao);
			fflush(stdin);
			switch(opcao){
				case 1:
					printf("\nDigite a data da venda (** ** ****)\n");
					scanf("%i %i %i", &(ph->dataVenda.dia), &(ph->dataVenda.mes), &(ph->dataVenda.ano));
					fflush(stdin);
					hist_reg = verifica_historico();
					ph->reghist = hist_reg+1;
					ph->regcarro = pm->regcarro;
					strcpy(ph->modelo, pm->modelo);
					strcpy(ph->cor, pm->cor);
					ph->valor = pm->valor;
					ph->regloja = p->regloja;
					strcpy(ph->nome, p->nome);
					strcpy(ph->cnpj, p->CNPJ);
					grava_historico(ph,"ab",1);
					p->tabela[n_tabela-1].sigla = 'L';
					p->reserved-=1;
					p->sold+=1;
					grava(p,"rb+",pos);
					pm->modelo[0] = '\0';
					pm->cor[0] = '\0';
					pm->valor = 0;
					pm->status.sigla = 'L';
					grava_carro(pm,"rb+",pos2);
					printf("\n\nCarro vendido com sucesso");
					break;
				case 2:
					p->tabela[n_tabela-1].sigla = 'L';
					p->reserved-=1;
					grava(p,"rb+",pos);
					pm->status.sigla = 'L';
					grava_carro(pm,"rb+",pos2);
					printf("\n\nCarro liberado com sucesso");
					break;
			}
		}
	}
	printf("\n\n");
	system("pause");
}

int busca_vazio(montadora *p, int qtde){
	FILE *fptr = NULL;
	int i;
	int achou = -1;
	system("cls");
	if((fptr=fopen("carro.bin","rb"))==NULL)
		printf("\nErro");
	else{
	for(i=0;i<qtde;i++){
    	fseek(fptr,i*sizeof(montadora),0);
    	fread(p,sizeof(montadora),1,fptr);
    	if(p->modelo[0] == '\0' || p->cor[0] == '\0' || p->valor == 0){
    		achou=i;
    		i=qtde;
			}
		}
	fclose(fptr); 
	}
	return achou;
}

void altera(loja *p, montadora *pm){
	int opcao;
	int carro_reg;
	int pos;
	int pos2;
	int i;
	char cnpj[19];
	system("cls");
	printf("\n[1]Alterar Concessionaria\n[2]Alterar Carro\n\nOpcao: ");
	scanf("%i", &opcao);
	fflush(stdin);
	system("cls");
	switch(opcao){
		case 1:
			printf("\nDigite o CNPJ da sua concessionaria: ");
			gets(cnpj);
			fflush(stdin);
			pos=busca(p, cnpj);
			system("cls");
			if(pos==-1)
				printf("\nCNPJ inexistente");
			else{
				printf("\nRegistro: %i\nNome: %s\nEstado: %s\nCidade: %s\nBairro: %s\nCEP: %s\nEmail: %s\nFone: %s\nLogradouro: %s\nVendidos: %i\nReservados: %i",p->regloja, p->nome, p->end.estado, p->end.cidade, p->end.bairro, p->end.CEP, p->end.email, p->end.fone, p->end.logradouro, p->sold, p->reserved);
				for(i=0;i<3;i++){
				if(p->tabela[i].sigla != 'L')
					printf("\nTabela %i: %c - %i", i+1, p->tabela[i].sigla, p->tabela[i].reservado.regcarro);
					}
				printf("\n\nDeseja alterar\n\n[1]Nome\n[2]CNPJ\n[3]Ambos\n\nOpcao: ");
				scanf("%i", &opcao);
				fflush(stdin);
				system("cls");
				switch(opcao){
					case 1:
						printf("\nNovo nome: ");
						gets(p->nome);
						fflush(stdin);
						break;
					case 2:
						printf("\nNovo CNPJ: ");
						gets(p->CNPJ);
						fflush(stdin);
						for(i=0;i<3;i++){
							if(p->tabela[i].sigla != 'L'){
								carro_reg = p->tabela[i].reservado.regcarro;
								pos2=busca_carro_registro(pm, carro_reg);
								strcpy(pm->status.reserva.CNPJ, p->CNPJ);
							}
						}
						break;
					case 3:
						printf("\nNovo nome: ");
						gets(p->nome);
						fflush(stdin);
						printf("\nNovo CNPJ: ");
						gets(p->CNPJ);
						fflush(stdin);
						for(i=0;i<3;i++){
							if(p->tabela[i].sigla != 'L'){
								carro_reg = p->tabela[i].reservado.regcarro;
								pos2=busca_carro_registro(pm, carro_reg);
								strcpy(pm->status.reserva.CNPJ, p->CNPJ);
							}
						}
						break;
				}
				grava(p,"rb+",pos);
				grava_carro(pm,"rb+",pos2);
				printf("\nAlterado com sucesso");
			}
			break;
		case 2:
			busca_carro(pm);
			printf("\nDigite o registro do carro que deseja alterar: ");
			scanf("%i", &carro_reg);
			fflush(stdin);
			pos=busca_carro_registro(pm, carro_reg);
			system("cls");
			if(pos==-1)
				printf("\nRegistro inexistente");
			else{
				if(pm->modelo[0] == '\0' || pm->cor[0] == '\0' || pm->valor == 0)
					printf("Este carro nao esta sujeito a alteracao");
				else{
					printf("\nRegistro: %i\nModelo: %s\nCor: %s\nValor: %.2f",pm->regcarro,pm->modelo,pm->cor,pm->valor);
   					if(pm->status.sigla == 'L')
						printf("\nStatus: %c\n\n", pm->status.sigla);
					else
						printf("\nStatus: %c - %s\n\n", pm->status.reserva.sigla, pm->status.reserva.CNPJ);
					printf("\n\nDeseja alterar\n\n[1]Modelo\n[2]Cor\n[3]Valor\n\nOpcao: ");
					scanf("%i", &opcao);
					fflush(stdin);
					system("cls");
					switch(opcao){
						case 1:
							printf("\nDigite o novo modelo: ");
							gets(pm->modelo);
							fflush(stdin);
							break;
						case 2:
							printf("\nDigite a nova cor: ");
							gets(pm->cor);
							fflush(stdin);
							break;
						case 3:
							printf("\nDigite o novo valor: ");
							scanf("%f", &(pm->valor));
							fflush(stdin);
							break;
					}
				}
				grava_carro(pm,"rb+",pos);
				printf("\nAlterado com sucesso");
			}
			break;
		}
	printf("\n\n");
	system("pause");
}
