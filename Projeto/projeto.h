#ifndef HEADERPRINCIPAL
#define HEADERPRINCIPAL

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h> //rand
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h> //strlen, strcpy

//declaracoes de structs usadas nesse codigo
typedef struct vector3{
   float x,y,z; //struct adcional para facilitar leitura e operacoes
} vector3;

typedef struct carro{
   vector3 direc;      //vetor de direcao de movimento
   vector3 pos;        //use atan(z/x) para determinar o seu angulo em relacao a x
   float veloc;
   float rot; //rotacao da roda
} carro;

typedef struct arvore{
   float angle;
   vector3 pos;
} arvore;

typedef struct rua{
   float angle;
   vector3 pos;
} rua;

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
/*
typedef struct camera{
	vector3 pos;
	vector3 look; //direcao que esta olhando
} camera;
*/

typedef struct botao{
	float x, y;
	float largura, altura;
	float cor[3];
	int estadoAtivo; //estado em que esse botao fica ativo
	void (*acao)(void);
	char texto[128];
} botao;

#define PI 3.14159265358979324
#define LARGURAFAIXA 35.0 //distancia do centro da rua ate a primeira faixa a ser desenhada
#define ALTURAARVORE 60 //Arvores tem 6m de altura
#define LARGURACOPA 15 //arvores terao 1.5m de largura da copa
#define QUANTIDADEARVORES 50 //numero maximo de arvores existentes


//declaracoes de variaveis globais
int menu; //variavel de estado, para controlar se o desenho atual deve ser o menu ou a simulacao
long int font, fontRoman;
GLfloat angle, fAspect;
luz G_Luzes[2];
carro *G_Carros[10]; //G_Carros[0] e o carro principal de nossa simulacao
rua G_Ruas[3];
arvore G_Arvores[QUANTIDADEARVORES];
velocimetro G_Velocimetro;
// camera G_Camera;


float modulo(vector3);
vector3 escalarVetor(float, vector3);
vector3 somarVetores(vector3,vector3);
float produtoInterno(vector3, vector3);
vector3 normalizar(vector3);
vector3 computarNormal(vector3, vector3);
float anguloEntreVetores(vector3, vector3);
void writeBitmapString(void*, char*);
void writeStrokeString(void*, char*);
void criarCarro(vector3);
void desenharCaixa(float, float, float);
void desenharCarro();
void desenharRoda();
void desenharRua();
void desenharArvore();
void desenharVelocimetro(velocimetro);
float calcularAnguloPonteiro(float,float);
void desenharObjeto(void(*funcPointer)(void), float, float, float, float);
void atualizarPosicoes(int);

void simulacao();
void inicializarSimulacao();
void animacaoLuz();
void defineIluminacao();
// void moveToAndLookAt(vector3, vector3);
void keySimulacao(unsigned char, int, int);
void specialKeySimulacao(int, int, int);
void resizeSimulacao(int, int);

void desenharBotao(botao);
void inicializarMenu();
void drawMenu();
void keyMenu(unsigned char, int, int);
void mouseMenu(int, int, int, int);
void resizeMenu(int, int);


#endif