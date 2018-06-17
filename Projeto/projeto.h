#ifndef HEADERPRINCIPAL
#define HEADERPRINCIPAL

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h> //strlen



#define PI 3.14159265358979324
//declaracoes de structs usadas nesse codigo
typedef struct vector3{
   float x,y,z; //struct adcional para facilitar leitura e operacoes
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

typedef struct luz{
	vector3 pos;
	GLfloat ambiente[4];
	GLfloat difusa[4];
	GLfloat especular[4];
	char enabled; //variavel booleana para determinar se essa luz esta ativada ou nao
} luz;

typedef struct velocimetro{
	float tamanho;
	float ponteiro; //guardado em radianos
	float max; //max de velocidade que o ponteiro mostra
} velocimetro;

//declaracoes de variaveis globais
int menu; //variavel de estado, para controlar se o desenho atual deve ser o menu ou a simulacao
GLfloat angle, fAspect;
luz G_Luzes[2];
rua G_Ruas[3];
carro *G_Carros[10]; //G_Carros[0] e o carro principal de nossa simulacao
velocimetro G_velocimetro;


float modulo(vector3);
vector3 escalarVetor(float, vector3);
vector3 somarVetores(vector3,vector3);
vector3 normalizar(vector3);
void writeBitmapString(void*, char*);
void criarCarro(vector3);
void desenharCarro();
void desenharRua();
void desenharVelocimetro(velocimetro);
float calcularAnguloPonteiro(float,float);
void desenharObjeto(void(*funcPointer)(void), float, float, float, float);
void atualizarPosicoes(int);
//        WIP
//desenharArvore();

void simulacao();
void inicializarSimulacao();
void animacaoLuz();
void defineIluminacao();
void keySimulacao(unsigned char, int, int);
void specialKeySimulacao(int, int, int);
void resizeSimulacao(int, int);

#endif