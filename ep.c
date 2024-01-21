//--Caique Alves de Souza - NºUSP 14563126--

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TAMANHO 1000


int indice_arvore(char* nome_arquivo){
	FILE * in;
	FILE * copia_arquivo;
	char * linha;
	char * copia_ponteiro_linha;
	char * quebra_de_linha;
	char * palavra;	
	char input_usuario[255];
	char input_primeira_palavra[255];
	char input_segunda_palavra[255];
	int contador_linhas;
	int numero_linhas;
	int caractere;
	
	clock_t start = clock();

	in = fopen(nome_arquivo, "r");
	copia_arquivo = fopen(nome_arquivo, "r");
	if (in == NULL){
		printf("O arquivo %s não existe.",nome_arquivo);
		exit(0);
	} 

	//Calcula o número de linhas no arquivo
	numero_linhas = 0;
	while(!feof(copia_arquivo))
	{
	caractere = fgetc(copia_arquivo);
	if(caractere== '\n')
	{
		numero_linhas++;
	}
	}
	numero_linhas++;

	//A estrutura a seguir sera usada para guardar as linhas do arquivo para posterior leitura
	char linhas[numero_linhas][255];

	//Declara estrutura de nó e cria no cabeça
	typedef struct No {
    char* palavra;
    int repeticoes;
	int linhas[numero_linhas];
    struct No* proximo_direita;
    struct No* proximo_esquerda;
	} No;

	No* cabeca = (No*)malloc(sizeof(No));
	cabeca ->palavra = " ";
	
	contador_linhas = 0;
	linha = (char *) malloc((TAMANHO + 1) * sizeof(char));

	while(in && fgets(linha, TAMANHO, in)){
		
		if( (quebra_de_linha = strrchr(linha, '\n')) ) *quebra_de_linha = 0;

		
		// fazemos uma copia do endereço que corresponde ao array de chars 
		// usado para armazenar cada linha lida do arquivo pois a função 'strsep' 
		// modifica o endereço do ponteiro a cada chamada feita a esta função (e 
		// não queremos que 'linha' deixe de apontar para o inicio do array).

		copia_ponteiro_linha = linha;
		
		strcpy(linhas[contador_linhas],linha);
		//Tratamento da linha:
			//1.Substitui todos os caracteres maiúsculos por minúsculos
			//2.Substitui "/" e "-" por espaço, para que palavras compostas sejam dividas em palavras distintas
			//3.Remove pontos e vírgulas do texto, para que não sejam considerados como parte das palavras

		for(int i = 0; copia_ponteiro_linha[i]; i++){
			
			copia_ponteiro_linha[i] = tolower(copia_ponteiro_linha[i]);
			if(copia_ponteiro_linha[i]=='/' || copia_ponteiro_linha[i]=='-'){
				copia_ponteiro_linha[i] = ' ';
			}
			if(copia_ponteiro_linha[i]=='.' || copia_ponteiro_linha[i]==','){
				copia_ponteiro_linha[i] = ' ';
			}
			
		}
	
		while( (palavra = strsep(&copia_ponteiro_linha, " ")) ){

			// antes de guardar a palavra em algum tipo de estrutura usada
			// para implementar o índice, será necessário fazer uma copia
			// da mesma, uma vez que o ponteiro 'palavra' aponta para uma 
			// substring dentro da string 'linha', e a cada nova linha lida
			// o conteúdo da linha anterior é sobreescrito.
			
			char * copia_ponteiro_palavra = (char *) malloc(100);
			strcpy(copia_ponteiro_palavra,palavra);
			
			//O algoritmo a seguir insere a palavra contida em "copia_ponteiro_palavra" à lista ligada
	
			No* atual = cabeca;
			int posicao_encontrada = 0;
			int linha_registrada = 0;
		
			while (posicao_encontrada == 0){
				if(strlen(copia_ponteiro_palavra)==0){
					//Evita que palavras vazias(ou seja, espacos) sejam consideradas como palavras e adicionadas a lista ligada
					posicao_encontrada = 1;
				}
				else if(strcmp(copia_ponteiro_palavra,atual->palavra)==0){
				
					atual->repeticoes = (atual->repeticoes)+1;
					for(int posicao_array = 0; linha_registrada==0 ;posicao_array++){
						if(atual->linhas[posicao_array] == NULL){
							if(atual->linhas[posicao_array-1] != contador_linhas+1){
								atual->linhas[posicao_array] = contador_linhas + 1;
							}
							
							linha_registrada = 1 ;
						}
					}
					posicao_encontrada = 1;
				}
				else if(strcmp(copia_ponteiro_palavra,atual->palavra)<0){
					if(atual->proximo_esquerda==NULL){
						No* novo_no = (No*)malloc(sizeof(No));
						if (novo_no == NULL) {
							fprintf(stderr, "Erro ao alocar memória para o nó\n");
							exit(EXIT_FAILURE);
						}
						novo_no ->palavra = copia_ponteiro_palavra;
						novo_no ->repeticoes = 1;
						novo_no->linhas[0] = contador_linhas+1;
						atual->proximo_esquerda = novo_no;
						posicao_encontrada = 1;
					}else{
						atual = atual->proximo_esquerda;
					}
				}
				else if(strcmp(copia_ponteiro_palavra,atual->palavra)>0){
					if(atual->proximo_direita==NULL){
						No* novo_no = (No*)malloc(sizeof(No));
						if (novo_no == NULL) {
							fprintf(stderr, "Erro ao alocar memória para o nó\n");
							exit(EXIT_FAILURE);
						}
						novo_no ->palavra = copia_ponteiro_palavra;
						novo_no ->repeticoes = 1;
						novo_no->linhas[0] = contador_linhas+1;
						atual->proximo_direita = novo_no;
						posicao_encontrada = 1;
					}else{
						atual = atual ->proximo_direita;
					}

				}
			}
			
		}
	contador_linhas++;
	}
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;

	

	

	printf("Tempo para carregar o arquivo e construir o indice: %lf ms\n",seconds*1000);
	printf("Numero de linhas no arquivo: %d\n",contador_linhas);
	


	while(strcmp(input_primeira_palavra,"fim")!=0){
		printf(">");
		fgets(input_usuario, sizeof(input_usuario), stdin);
		clock_t inicio_busca = clock();
		int num_palavras = sscanf(input_usuario, "%s %s", input_primeira_palavra, input_segunda_palavra);
		//Transforma em minúsculas todos os caracteres da palavra a ser buscada
		for(int i = 0; input_segunda_palavra[i]; i++){
			input_segunda_palavra[i] = tolower(input_segunda_palavra[i]);
		}
		if(strcmp(input_primeira_palavra,"busca")==0 && num_palavras==2){
	
			int busca_finalizada =0;
			No*atual = cabeca->proximo_direita;
			while(busca_finalizada==0){
		
				if(strcmp(atual->palavra,input_segunda_palavra)==0){
					printf("Existem %d ocorrências da palavra %s na(s) seguinte(s) linha(s):\n",atual->repeticoes,input_segunda_palavra);
					int indice = 0;
					while(atual->linhas[indice]!=NULL){
						printf("%05d: %s\n",atual->linhas[indice],linhas[(atual->linhas[indice])-1]);
						indice++;
		
						
					}
					
					busca_finalizada = 1;
					clock_t fim_busca = clock();
					float seconds = (float)(fim_busca - inicio_busca) / CLOCKS_PER_SEC;
					printf("Tempo de busca: %lf ms\n",seconds*1000);
					
				}else if(strcmp(input_segunda_palavra,atual->palavra)<0){
					if(atual->proximo_esquerda==NULL){
						printf("Palavra '%s' nao encontrada.\n",input_segunda_palavra);
						busca_finalizada = 1;
						clock_t fim_busca = clock();
						float seconds = (float)(fim_busca - inicio_busca) / CLOCKS_PER_SEC;
						printf("Tempo de busca: %lf ms\n",seconds*1000);
					}else{
						atual = atual->proximo_esquerda;
					}
				}else if(strcmp(input_segunda_palavra,atual->palavra)>0){
					if(atual->proximo_direita==NULL){
						printf("Palavra '%s' nao encontrada.\n",input_segunda_palavra);
						busca_finalizada = 1;
						clock_t fim_busca = clock();
						float seconds = (float)(fim_busca - inicio_busca) / CLOCKS_PER_SEC;
						printf("Tempo de busca: %lf ms\n",seconds*1000);
					}else{
						atual = atual->proximo_direita;
					}
				}
			}
			
			
		}else if(strcmp(input_primeira_palavra,"fim")!=0){
			printf("Opção inválida!\n");
		}
	}


}


// Função para criar um Novo nó

int indice_lista(char* nome_arquivo){
	FILE * in;
	FILE * copia_arquivo;
	char * linha;
	char * copia_ponteiro_linha;
	char * quebra_de_linha;
	char * palavra;	
	char input_usuario[255];
	char input_primeira_palavra[255];
	char input_segunda_palavra[255];
	int contador_linhas;
	int numero_linhas;
	int caractere;
	
	clock_t start = clock();

	in = fopen(nome_arquivo, "r");
	copia_arquivo = fopen(nome_arquivo, "r");
	if (in == NULL){
		printf("O arquivo %s não existe.",nome_arquivo);
		exit(0);
	} 

	//Calcula o número de linhas no arquivo
	numero_linhas = 0;
	while(!feof(copia_arquivo))
	{
	caractere = fgetc(copia_arquivo);
	if(caractere== '\n')
	{
		numero_linhas++;
	}
	}
	numero_linhas++;

	//A estrutura a seguir sera usada para guardar as linhas do arquivo para posterior leitura
	char linhas[numero_linhas][255];

	//Declara estrutura de nó e cria no cabeça
	typedef struct No {
    char* palavra;
    int repeticoes;
	int linhas[numero_linhas];
    struct No* proximo;
	} No;

	No* cabeca = (No*)malloc(sizeof(No));
	
	contador_linhas = 0;
	linha = (char *) malloc((TAMANHO + 1) * sizeof(char));

	while(in && fgets(linha, TAMANHO, in)){
		
		if( (quebra_de_linha = strrchr(linha, '\n')) ) *quebra_de_linha = 0;

		
		// fazemos uma copia do endereço que corresponde ao array de chars 
		// usado para armazenar cada linha lida do arquivo pois a função 'strsep' 
		// modifica o endereço do ponteiro a cada chamada feita a esta função (e 
		// não queremos que 'linha' deixe de apontar para o inicio do array).

		copia_ponteiro_linha = linha;
		strcpy(linhas[contador_linhas],linha);
		//Tratamento da linha:
			//1.Substitui todos os caracteres maiúsculos por minúsculos
			//2.Substitui "/" e "-" por espaço, para que palavras compostas sejam dividas em palavras distintas
			//3.Remove pontos e vírgulas do texto, para que não sejam considerados como parte das palavras

		for(int i = 0; copia_ponteiro_linha[i]; i++){
			
			copia_ponteiro_linha[i] = tolower(copia_ponteiro_linha[i]);
			if(copia_ponteiro_linha[i]=='/' || copia_ponteiro_linha[i]=='-'){
				copia_ponteiro_linha[i] = ' ';
			}
			if(copia_ponteiro_linha[i]=='.' || copia_ponteiro_linha[i]==','){
				copia_ponteiro_linha[i] = ' ';
			}
			
		}

		while( (palavra = strsep(&copia_ponteiro_linha, " ")) ){

			// antes de guardar a palavra em algum tipo de estrutura usada
			// para implementar o índice, será necessário fazer uma copia
			// da mesma, uma vez que o ponteiro 'palavra' aponta para uma 
			// substring dentro da string 'linha', e a cada nova linha lida
			// o conteúdo da linha anterior é sobreescrito.

			
	
			char * copia_ponteiro_palavra = (char *) malloc(100);
			strcpy(copia_ponteiro_palavra,palavra);

			//O algoritmo a seguir insere a palavra contida em "copia_ponteiro_palavra" à lista ligada
	
			No* atual = cabeca;
			No* proximo = cabeca->proximo;
			int posicao_encontrada = 0;
			int linha_registrada = 0;
			while (posicao_encontrada == 0){
				if(strlen(copia_ponteiro_palavra)==0){
					//Evita que palavras vazias(ou seja, espacos) sejam consideradas como palavras e adicionadas a lista ligada
					posicao_encontrada = 1;
				}
				else if(proximo==NULL){
					No* novo_no = (No*)malloc(sizeof(No));
					if (novo_no == NULL) {
						fprintf(stderr, "Erro ao alocar memória para o nó\n");
						exit(EXIT_FAILURE);
					}
					novo_no ->palavra = copia_ponteiro_palavra;
					novo_no ->repeticoes = 1;
					novo_no->linhas[0] = contador_linhas+1;
					
					novo_no ->proximo = NULL;
					atual->proximo = novo_no;
					posicao_encontrada = 1;
				}
				else if(strcmp(copia_ponteiro_palavra,proximo->palavra)<0){
					No* novo_no = (No*)malloc(sizeof(No));
					if (novo_no == NULL) {
						fprintf(stderr, "Erro ao alocar memória para o nó\n");
						exit(EXIT_FAILURE);
					}
					novo_no ->palavra = copia_ponteiro_palavra;
					novo_no ->repeticoes = 1;
					novo_no->linhas[0] = contador_linhas+1;
					novo_no ->proximo = proximo;
					atual->proximo = novo_no;
					posicao_encontrada = 1;
				}
				else if(strcmp(copia_ponteiro_palavra,proximo->palavra)==0){
					proximo->repeticoes = (proximo->repeticoes)+1;
					for(int posicao_array = 0; linha_registrada==0 ;posicao_array++){
						if(proximo->linhas[posicao_array] == NULL){
							if(proximo->linhas[posicao_array-1] != contador_linhas+1){
								proximo->linhas[posicao_array] = contador_linhas + 1;
							}
							
							linha_registrada = 1 ;
						}
					}
					posicao_encontrada = 1;
				}else{
					atual = proximo;
					proximo = atual->proximo;
				}       
				}  
		}

		contador_linhas++;
	}

	No* atual = cabeca->proximo;  // IgNora a cabeça e vai para o primeiro nó real


	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;

	

	

	printf("Tempo para carregar o arquivo e construir o indice: %lf ms\n",seconds*1000);
	printf("Numero de linhas no arquivo: %d\n",contador_linhas);
	
	

	

	while(strcmp(input_primeira_palavra,"fim")!=0){
		printf(">");
		fgets(input_usuario, sizeof(input_usuario), stdin);
		clock_t inicio_busca = clock();
		int num_palavras = sscanf(input_usuario, "%s %s", input_primeira_palavra, input_segunda_palavra);
		//Transforma em minúsculas todos os caracteres da palavra a ser buscada
		for(int i = 0; input_segunda_palavra[i]; i++){
			input_segunda_palavra[i] = tolower(input_segunda_palavra[i]);
		}
		if(strcmp(input_primeira_palavra,"busca")==0 && num_palavras==2){
			
			//Este trecho de código realiza a busca da palavra
			No* atual = cabeca->proximo;
			int busca_finalizada = 0;
			while (busca_finalizada == 0)
			{
				if(strcmp(input_segunda_palavra,atual->palavra)==0){
					printf("Existem %d ocorrências da palavra %s na(s) seguinte(s) linha(s):\n",atual->repeticoes,input_segunda_palavra);
					int indice = 0;
					while(atual->linhas[indice]!=NULL){
						printf("%05d: %s\n",atual->linhas[indice],linhas[(atual->linhas[indice])-1]);
						indice++;
						
					}
					busca_finalizada = 1;
					clock_t fim_busca = clock();
					float seconds = (float)(fim_busca - inicio_busca) / CLOCKS_PER_SEC;
					printf("Tempo de busca: %lf ms\n",seconds*1000);
				}else{
					if(atual->proximo==NULL){
						printf("Palavra '%s' nao encontrada.\n",input_segunda_palavra);
						busca_finalizada = 1;
						clock_t fim_busca = clock();
						float seconds = (float)(fim_busca - inicio_busca) / CLOCKS_PER_SEC;
						printf("Tempo de busca: %lf ms\n",seconds*1000);
					}else{
						atual = atual->proximo;
					}
				}
			}
			

		}else if(strcmp(input_primeira_palavra,"fim")!=0){
			printf("Opção inválida!\n");
		}
	}

}

int main(int argc, char ** argv){
        
	if(argc == 3) {
		if(strcmp(argv[2],"lista")==0){
			indice_lista(argv[1]);
		
		}
		else if(strcmp(argv[2],"arvore")==0){
			indice_arvore(argv[1]);
		}
		else{
			puts("A estrutura de busca deve ser 'lista' ou 'arvore'");
		}
	}else{
		printf("O comando de execução do programa deve conter o arquivo a ser analisado e a estrutura a ser usada!");
	}

	return 0;
}
