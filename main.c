#include <stdio.h>
#include <stdlib.h>
#include "miniLex.h"

// nomes dos atomos, na MESMA ORDEM do enum TAtomo no miniLex.h
// (se voce adicionar/remover algo do enum, atualize aqui tambem)
char *nome_atomo[] = {
    "fim_de_arquivo",       // EOS
    "erro_lexico",          // ERRO
    "algoritmo",            // ALGORITMO
    "caractere",            // CARACTERE
    "div",                  // DIV
    "e",                    // E
    "enquanto",             // ENQUANTO
    "entao",                // ENTAO
    "escreva",              // ESCREVA
    "faca",                 // FACA
    "falso",                // FALSO
    "fim",                  // FIM
    "funcao",               // FUNCAO
    "identificador",        // IDENTIFICADOR
    "inicio",               // INICIO
    "inteiro",              // INTEIRO
    "leia",                 // LEIA
    "logico",               // LOGICO
    "mod",                  // MOD
    "ou",                   // OU
    "procedimento",         // PROCEDIMENTO
    "se",                   // SE
    "senao",                // SENAO
    "var",                  // VAR
    "verdadeiro",           // VERDADEIRO
    "constchar",            // CONSTCHAR
    "constint",             // CONSTINT
    "soma",                 // SOMA
    "mult",                 // MULT
    "subtracao",            // SUBTRACAO
    "ponto",                // PONTO
    "virgula",              // VIRGULA
    "ponto_virgula",        // PONTO_VIRGULA
    "abre_par",             // ABRE_PAR
    "fecha_par",            // FECHA_PAR
    "atribuicao",           // ATRIBUICAO
    "diferenca",            // DIFERENCA
    "menor_igual",          // MENOR_IGUAL
    "menor",                // MENOR
    "maior_igual",          // MAIOR_IGUAL
    "maior",                // MAIOR
    "igual",                // IGUAL
    "dois_pontos",          // DOIS_PONTOS
    "comentario",           // COMENTARIO
};

// le o arquivo inteiro para uma string alocada dinamicamente
char *le_arquivo(char *nome_arquivo){
    FILE *f = fopen(nome_arquivo, "r");
    if(f == NULL){
        printf("Erro: nao foi possivel abrir o arquivo '%s'\n", nome_arquivo);
        exit(1);
    }

    // descobre o tamanho do arquivo
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    // aloca memoria (+1 para o '\0' final)
    char *conteudo = malloc(tamanho + 1);
    if(conteudo == NULL){
        printf("Erro: memoria insuficiente\n");
        fclose(f);
        exit(1);
    }

    // le o conteudo todo de uma vez
    size_t lidos = fread(conteudo, 1, tamanho, f);
    conteudo[lidos] = '\0';

    fclose(f);
    return conteudo;
}

// variavel externa do lexico - precisamos apontar ela pro conteudo do arquivo
extern char *buffer;

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    buffer = le_arquivo(argv[1]);

    TInfoAtomo info;
    do{
        info = obter_atomo();

        if(info.atomo == ERRO){
            printf("# %d: erro lexico\n", info.linha);
            break;
        }
        else if(info.atomo == EOS){
            printf("# %d: fim de arquivo\n", info.linha);
        }
        else if(info.atomo == IDENTIFICADOR){
            printf("# %d: %s: %s\n", info.linha, nome_atomo[info.atomo], info.atributo.ID);
        }
        else if(info.atomo == CONSTINT){
            printf("# %d: %s: %d\n", info.linha, nome_atomo[info.atomo], info.atributo.numero);
        }
        else if(info.atomo == CONSTCHAR){
            printf("# %d: %s: %c\n", info.linha, nome_atomo[info.atomo], info.atributo.ch);
        }
        else{
            printf("# %d: %s\n", info.linha, nome_atomo[info.atomo]);
        }

    }while(info.atomo != EOS);

    return 0;
}