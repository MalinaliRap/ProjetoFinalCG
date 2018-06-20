#include "projeto.h"
botao botoes[5];
int estadoMenu;

void iniciarSimulacao(){
	menu = 0; // sai do menu
	inicializarSimulacao();
	glutPostRedisplay();
}

void instrucoes(){ //funcoes auxiliares que serao usadas em conjunto com os botoes para definirem suas acoes
	estadoMenu = 1;
}

void voltar(){
	estadoMenu = 0;
}

void sair(){
	exit(0);
}

void inicializarMenu(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	estadoMenu = 0;

	botoes[0].x = 10; botoes[0].y = 10; botoes[0].estadoAtivo = 0;
 	strcpy(botoes[0].texto, "Iniciar"); botoes[0].acao = &iniciarSimulacao;
	botoes[0].largura = strlen(botoes[0].texto) * 2; botoes[0].altura = 5;
	botoes[0].cor[0] = 0.0; botoes[0].cor[1] = 1.0; botoes[0].cor[2] = 0.0;

	botoes[1].x = 45; botoes[1].y = 10; botoes[1].estadoAtivo = 0;
	strcpy(botoes[1].texto, "Instrucoes"); botoes[1].acao = &instrucoes;
	botoes[1].largura = strlen(botoes[1].texto) * 2; botoes[1].altura = 5;
	botoes[1].cor[0] = 1.0; botoes[1].cor[1] = 1.0; botoes[1].cor[2] = 0.0;

	botoes[2].x = 80; botoes[2].y = 10; botoes[2].estadoAtivo = 0;
	strcpy(botoes[2].texto, "Sair"); botoes[2].acao = &sair;
	botoes[2].largura = strlen(botoes[2].texto) * 2; botoes[2].altura = 5;
	botoes[2].cor[0] = 1.0; botoes[2].cor[1] = 0.0; botoes[2].cor[2] = 0.0;

	botoes[3].x = 80; botoes[3].y = 10; botoes[3].estadoAtivo = 1;
	strcpy(botoes[3].texto, "Voltar"); botoes[3].acao = &voltar;
	botoes[3].largura = strlen(botoes[3].texto) * 2; botoes[3].altura = 5;
	botoes[3].cor[0] = 1.0; botoes[3].cor[1] = 1.0; botoes[3].cor[2] = 0.0;

	botoes[4].x = 10; botoes[4].y = 10; botoes[4].estadoAtivo = 1;
	strcpy(botoes[4].texto, "Iniciar"); botoes[4].acao = &iniciarSimulacao;
	botoes[4].largura = (strlen(botoes[4].texto) * 2) + 30; botoes[4].altura = 10; //botao intencionalmente mais largo que o restante
	botoes[4].cor[0] = 0.0; botoes[4].cor[1] = 1.0; botoes[4].cor[2] = 0.0;
}

void desenharQuadrado(float w, float h){ //desenha um quadrado de tamanho 2w e 2h
	glBegin(GL_QUADS);
		glVertex2f(-w, -h);
		glVertex2f(w, -h);
		glVertex2f(w, h);
		glVertex2f(-w, h);
	glEnd();
}

void escreverStroke(float x, float y, float sx, float sy, char *texto){
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(x, y, 0);
	glScalef(sx, sy, 0);
	writeStrokeString((void*)fontRoman, texto); //escreve usando o metodo de triangulacao (stroke)
	glPopMatrix(); //permite que o caracter seja escalonado livremente
}

void escreverBitmap(float x, float y, char *texto){
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(x, y, 0);
		float offset = strlen(texto) * 0.1; //calcula o quanto o texto se movera para a direita ao ser escrito
		glRasterPos3f(-offset, 5, 0.4); //move o raster em contrapartida para centralizar a escrita
		writeBitmapString((void*)font, texto);
	glPopMatrix();
}

void desenharSeta(float x, float y, float angl, float scale){
	glPushMatrix();
		glTranslatef(x,y, 0.0);
		glScalef(scale, scale, 0.0);
		glRotatef(angl, 0.0, 0.0, 1.0);

		glBegin(GL_TRIANGLES);
			glVertex2f(0.0, 1.0);
			glVertex2f(0.5, 0.0);
			glVertex2f(-0.5,0.0);
		glEnd();

		glBegin(GL_QUADS);
			glVertex2f(0.3, 0.0);
			glVertex2f(0.3, -0.6);
			glVertex2f(-0.3, -0.6);
			glVertex2f(-0.3, 0.0);
		glEnd();
	glPopMatrix();
}

void drawMenu(void){
	glClear(GL_COLOR_BUFFER_BIT);
	int i;
	for(i=0; i<5; i++){
		if(botoes[i].estadoAtivo == estadoMenu){
			desenharBotao(botoes[i]); //se o botao deve aparecer nesse estado do menu, ele e desenhado
		}
	}
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(50,60,0); //move para 50,60
		desenharQuadrado(40,30); //desenha um quadrado 80:60
	glPopMatrix();

	switch(estadoMenu){
		case 0:
			escreverStroke(10,80, 0.05, 0.05, "Projeto final de OpenGL");
			escreverStroke(20, 70, 0.05, 0.05, "Movimento Estrada");
			escreverStroke(27, 45, 0.03, 0.03, "Hamilton Francisco Culik");
			escreverStroke(28, 40, 0.03, 0.03, "Gabriel Becher Malinali");
			escreverStroke(33, 35, 0.03, 0.03, "Alberto Emanuel");
			break;
		case 1:
			escreverBitmap(35.5, 80, "Para criar veiculos");
			escreverBitmap(35.5, 75, "A - No sentido oposto");
			escreverBitmap(35.5, 70, "Z - No seu sentido");
			escreverBitmap(36, 65, "Para trocar entre dia/noite");
			escreverBitmap(36, 60, "D - Dia->Noite|Noite->Dia");

			desenharSeta(50, 50, 0, 4);
			desenharSeta(45, 45, 90, 4);
			desenharSeta(55, 45, -90, 4);
			desenharSeta(50, 40, 180, 4);

			escreverBitmap(42, 50, "Para acelerar");
			escreverBitmap(40, 27, "Para desacelerar");
			escreverBitmap(25, 40, "Para virar");
			escreverBitmap(61, 40, "Para virar");
			break;
	}
   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resizeMenu(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
    											
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyMenu(unsigned char key, int x, int y){
	// 
}

void mouseMenu(int button, int state, int x, int y){ //cria-se uma funcao de controle para entrada de mouse
	int i;
	x = x*100 / 500; //valores variam de acordo com o tamanho da janela e nao as proporcoes de ortho, necessario converter
	y = 100-(y*100 / 500); //y invertido, precisa converter certo
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state== GLUT_DOWN){ //detecta se o botao esquerdo foi clicado, botao PRESSIONADO, nao soltado
				for(i=0; i<5; i++){
					if(x>botoes[i].x && x<botoes[i].x + botoes[i].largura && 
						y>botoes[i].y && y<botoes[i].y + botoes[i].altura){
						if(botoes[i].estadoAtivo == estadoMenu){//se o botao fica ativo nesse estado
							botoes[i].acao(); //ativa a acao atrelada a esse botao no vetor de acoes
							break; //break necessario para evitar o caso de um botao ser duplamente ativado
							//devido a alguma mudanca de estado ativo
						}
					}
				}
				
			}
		default:
			break;
	}
	glutPostRedisplay();
}

