/*
Integracao entre Lexico e Sintatico

Para compilar no vscode use:

gcc miniLex.c miniSintatico.c -Wall -Og -g -o compilador

-Wall: habilita todos avisos do compilador. Este modo indica possíveis erros cometidos no programa.
-Og: somente aplicar otimizações que não atrapalham debuging.
-g: informações para debug usando gdb
-o: output -- nome do executável gerado 

Para criar um arquivo de Makefile acesse:
https://diveintosystems.org/book/Appendix2/makefiles.html#_make_and_makefiles
*/

#include <stdio.h>
#include <stdlib.h>
#include "miniLex.h"

// varivel global declarada no miniLex.c
extern char *strAtomo[];

// variavel global do analisador sintatico
TInfoAtomo lookahead;
TInfoAtomo info_atomo;

void consome( TAtomo atomo );

int main(){
    lookahead = obter_atomo();
    programa();
    consome(EOS);
    printf("Compilador\nAnalise sintatica concluida sem erros\n");
    return 0;
}
void consome( TAtomo atomo ){
    if( lookahead.atomo == atomo ){
        lookahead = obter_atomo();
    }
    else{
        printf("# %d:erro sintatico, esperado [%s] encontrado [%s]\n",
               lookahead.linha, nome_atomo[atomo], nome_atomo[lookahead.atomo]);
        exit(1);
    }
}

void programa(){
    consome(ALGORITMO);
    consome(IDENTIFICADOR);
    consome(PONTO_VIRGULA);
    bloco();
    consome(PONTO);
}
void bloco(){
    declaracao_variaveis();
    declaracao_de_rotinas();
    comando_composto();
}
void declaracao_variaveis(){
    if(lookahead.atomo == VAR){
        consome(VAR);
        lista_variaveis();
        consome(PONTO_VIRGULA);
        while(lookahead.atomo == IDENTIFICADOR){
            lista_variaveis();
            consome(PONTO_VIRGULA);
        }

    }
}
void lista_variaveis(){
    consome (IDENTIFICADOR);
    while(lookahead.atomo == VIRGULA){
        consome(VIRGULA);
        consome(IDENTIFICADOR);

    }
    consome(DOIS_PONTOS);
    tipo();
}
void declaracao_de_rotinas(){
    while(lookahead.atomo == FUNCAO || lookahead.atomo == PROCEDIMENTO){
        if(lookahead.atomo == FUNCAO){
            declaracao_de_funcao();
        }
        else{
            declaracao_de_procedimento();
        }
    }
}
void declaracao_de_funcao(){
    consome(FUNCAO);
    tipo();
    consome(IDENTIFICADOR);
    parametros_formais();
    declaracao_variaveis();
    comando_composto();
}
void declaracao_de_procedimento(){
    consome(PROCEDIMENTO);
    consome(IDENTIFICADOR);
    parametros_formais();
    declaracao_variaveis();
    comando_composto();
}
void tipo(void){
    switch(lookahead.atomo){
        case CARACTERE:
            consome(CARACTERE);
            break;
        case INTEIRO:
            consome(INTEIRO);
            break;
        case LOGICO:
            consome(LOGICO);
            break;
        default:
            printf("# %d:erro sintatico, esperado tipo, encontrado [%s]\n",
                   lookahead.linha, nome_atomo[lookahead.atomo]);
            exit(1);
    }
}
void parametros_formais(){
    consome(ABRE_PAR);
    if(lookahead.atomo == FECHA_PAR){
        consome(FECHA_PAR);
        return;
    }
    parametro_formal();
    while(lookahead.atomo == PONTO_VIRGULA){
        consome(PONTO_VIRGULA);
        parametro_formal();
    }
    consome(FECHA_PAR);
}
void parametro_formal(){
    if(lookahead.atomo == VAR){
        consome(VAR);
    }
    lista_variaveis();
}
void comando_composto(){
    consome(INICIO);
    comando();
    while(lookahead.atomo == PONTO_VIRGULA){
        consome(PONTO_VIRGULA);
        comando();
    }
    consome(FIM);   
}
void comando(){
    switch(lookahead.atomo){
        case IDENTIFICADOR:
            comando_atribuicao_ou_chamada_procedimento();
            break;
        case LEIA:
            comando_entrada();
            break;
        case ESCREVA:
            comando_saida();
            break;
        case SE:
            comando_condicional();
            break;
        case ENQUANTO:
            comando_repeticao();
            break;
        case INICIO:
            comando_composto();
            break;
        default:
            printf("# %d:erro sintatico, esperado inicio de comando, encontrado [%s]\n",
                   lookahead.linha, nome_atomo[lookahead.atomo]);
            exit(1);
    }
}
void comando_atribuicao_ou_chamada_procedimento(void){
    consome(IDENTIFICADOR);
    if(lookahead.atomo == ATRIBUICAO){
        consome(ATRIBUICAO);
        expressao();
    }
    else{
        if(lookahead.atomo == ABRE_PAR){
            consome(ABRE_PAR);
            lista_expressao();
            consome(FECHA_PAR);
        }
    }
}
void comando_entrada(){
    consome(LEIA);
    consome(ABRE_PAR);
    consome(IDENTIFICADOR);
    while(lookahead.atomo == VIRGULA){
        consome(VIRGULA);
        consome(IDENTIFICADOR);
    }
    consome(FECHA_PAR);
}
void comando_saida(){
    consome(ESCREVA);
    consome(ABRE_PAR);
    lista_expressao();
    consome(FECHA_PAR);
}
void comando_condicional(){
    consome(SE);
    expressao();
    consome(ENTAO);
    comando();
    if(lookahead.atomo == SENAO){
        consome(SENAO);
        comando();
    }
}
void comando_repeticao(){
    consome(ENQUANTO);
    expressao();
    consome(FACA);
    comando();   
}
void lista_expressao(){
    expressao();
    while(lookahead.atomo == PONTO_VIRGULA){
        consome(PONTO_VIRGULA);
        expressao();
    }
}
void expressao(void){
    expressao_simples();
    if(lookahead.atomo == DIFERENCA || lookahead.atomo == MENOR ||
       lookahead.atomo == MENOR_IGUAL || lookahead.atomo == MAIOR_IGUAL ||
       lookahead.atomo == MAIOR || lookahead.atomo == IGUAL){
        operador_relacional();
        expressao_simples();
    }
}
void operador_relacional(){
    switch(lookahead.atomo){
        case DIFERENCA:
            consome(DIFERENCA);
            break;
        case MENOR:
            consome(MENOR);
            break;
        case MENOR_IGUAL:
            consome(MENOR_IGUAL);
            break;
        case MAIOR_IGUAL:
            consome(MAIOR_IGUAL);
            break;
        case MAIOR:
            consome(MAIOR);
            break;
        case IGUAL:
            consome(IGUAL);
            break;
    }
}
void expressao_simples(){
    termo();
    while(lookahead.atomo == SOMA)
        operador_adicao();
        termo();
}
void operador_adicao(){
    switch(lookahead.atomo){
        case SOMA:
            consome(SOMA);
            break;
        case SUBTRACAO:
            consome(SUBTRACAO);
            break;
        case MOD:
            consome(MOD);
            break;
        case OU:
            consome(OU);
            break;
    }
}
void termo(void){
    fator();
    while(lookahead.atomo == MULT || lookahead.atomo == DIV || lookahead.atomo == E){
        operador_multiplicacao();
        fator();
    }
}
void operador_multiplicacao(){
    switch(lookahead.atomo){
        case MULT:
             consome(MULT);
             break;
        case DIV:
            consome(DIV);
            break;
        case E:
            consome(E);
            break;
    }
}
void fator(){
    switch(lookahead.atomo){
        case IDENTIFICADOR:
            consome(IDENTIFICADOR);
            if(lookahead.atomo == ABRE_PAR){
                consome(ABRE_PAR);
                lista_expressao();
                consome(FECHA_PAR);
            }
            break;
        case CONSTINT:
            consome(CONSTINT);
            break;
        case CONSTCHAR:
            consome(CONSTCHAR);
            break;
        case ABRE_PAR:
            consome(ABRE_PAR);
            expressao();
            consome(FECHA_PAR);
            break;
        case SOMA:
            consome(SOMA);
            fator();
            break;
        case SUBTRACAO:
            consome(SUBTRACAO);
            fator();
            break;
        case NAO:
            consome(NAO);
            fator();
            break;
        case VERDADEIRO:
            consome(VERDADEIRO);
            break;
        case FALSO:
            consome(FALSO);
            break; 
        default:
            printf("# %d:erro sintatico, esperado inicio de fator, encontrado [%s]\n",
                   lookahead.linha, nome_atomo[lookahead.atomo]);
            exit(1);           
    }   

}