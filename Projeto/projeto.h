#ifndef HEADERPRINCIPAL
#define HEADERPRINCIPAL

#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>



#define PI 3.14159265358979324
//declaracoes de structs usadas nesse codigo
typedef struct vector3{
   float x,y,z; //struct adcional para facilitar leitura
} vector3;

typedef struct carro{
   vector3 direc;      //vetor de direcao de movimento
   vector3 pos;        //use atan(z/x) para determinar o seu angulo em relacao a x
   float veloc;
} carro;

typedef struct arvore{
   float angle;
   vector3 pos;
} arvore;

typedef struct rua{
   float angle;
   vector3 pos;
}  rua;

//declaracoes de variaveis globais
GLfloat angle, fAspect;
rua G_Ruas[3];
carro *G_carros[10]; //G_carros[0] e o carro principal de nossa simulacao


void criarCarro(vector3);
void desenharCarro();
void desenharRua();
void desenharObjeto(void(*funcPointer)(void), float, float, float, float);
void atualizarPosicoes(int);
//        WIP
//desenharArvore();

void simulacao();
void keySimulacao(unsigned char, int, int);
void resizeSimulacao(int, int);

#endif