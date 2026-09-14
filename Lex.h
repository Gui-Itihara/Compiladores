#include <stdio.h>
#include <ctype.h>
#include <string.h>

// código interno atomo NUMERO
typedef enum{
    EOS,
    ERRO,
    ALGORITMO,
    CARACTERE,
    DIV,
    E,
    ENQUANTO,
    ENTAO,
    ESCREVA,
    FACA,
    FALSO,
    FIM,
    FUNCAO,
    IDENTIFICADOR,
    INICIO,
    INTEIRO,
    LEIA,
    LOGICO,
    MOD,
    OU,
    PROCEDIMENTO,
    SE,
    SENAO,
    VAR,
    VERDADEIRO,
    CONSTCHAR,
    CONSTINT,
    SOMA,
    MULT,
    NAO,
    SUBTRACAO,
    PONTO,
    VIRGULA,
    PONTO_VIRGULA,
    ABRE_PAR,
    FECHA_PAR,
    ATRIBUICAO,
    DIFERENCA,
    MENOR_IGUAL,
    MENOR,
    MAIOR_IGUAL,
    MAIOR,
    IGUAL,
    DOIS_PONTOS,
    COMENTARIO,

}TAtomo;

// Estrutura para comunicar com o analisador sintatico
typedef struct{
   TAtomo atomo;
   int linha;
   union{
       int numero;
       char ID[16];
       char ch;
   }atributo;
}TInfoAtomo;

// declaracao de funcao
void reconhece_comentario(TInfoAtomo *info_atomo);
void reconhece_numero(TInfoAtomo *info_atomo);
void reconhece_id(TInfoAtomo *info_atomo);
void reconhece_char(TInfoAtomo *info_atomo);
int calcula_valor(char *lex);
TAtomo verifica_reservada(char *lexema_original);
TInfoAtomo  obter_atomo(void);