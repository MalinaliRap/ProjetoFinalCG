
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>


#define PI 3.14159265358979324


typedef struct rua{
   float angle;
   float pos[3];
}  rua;

GLfloat angle, fAspect;
rua G_Ruas[3];


void inicializarGlobais(){
   int i;
   for(i = -1; i<=1; i++){
      G_Ruas[i+1].angle = 0.0;
      G_Ruas[i+1].pos[0] = 66.0*i; //Cada rua e inicializada 66.0 unidades em x atras da proxima {-66, 0, +66}
      G_Ruas[i+1].pos[1] = 0.0;
      G_Ruas[i+1].pos[2] = 0.0;
   }
}

void desenharRua(){
   glColor3f(0.3, 0.3, 0.3); //cinza
   glBegin(GL_QUADS);
      glVertex3f(-33.0, 0.0, -20);
      glVertex3f(+33.0, 0.0, -20); //desenha a base da rua
      glVertex3f(+33.0, 0.0, +20); //retangulo 66:40 com centro em (0,0,0)
      glVertex3f(-33.0, 0.0, +20);
   glEnd();

   glColor3f(1.0, 1.0, 0.0); //amarelo
   glBegin(GL_QUADS);
      glVertex3f(-33.0, 0.0, -0.5);
      glVertex3f(+33.0, 0.0, -0.5); //desenha uma unica faixa central amarela sobre a rua
      glVertex3f(+33.0, 0.0, +0.5);
      glVertex3f(-33.0, 0.0, +0.5);
   glEnd();

   glPushMatrix(); //empilha a matriz atual para salvar-la
   int x;
   for(x = -1; x<=1; x++){
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix(); //empilha a matriz atual para alterar apenas ela
      glTranslatef(x*33.0, 0.0, 10.0); //progressivamente move ao longo da rua para desenhar as faixas, fazendo 10 retangulos brancos
      glBegin(GL_QUADS);
         glVertex3f(-2.0, 0.0, -0.5);
         glVertex3f(+2.0, 0.0, -0.5); //retangulo 4:1 com centro em (0.0.0)
         glVertex3f(+2.0, 0.0, +0.5);
         glVertex3f(-2.0, 0.0, +0.5);
      glEnd();
      glPopMatrix(); //remove a matriz do topo

      glPushMatrix(); //adciona outra matriz no topo
      glTranslatef(x*33.0, 0.0, -10.0); //progressivamente move ao longo do lado oposto (z=-10) para desenhar os retangulos da outra faixa
      glBegin(GL_QUADS);
         glVertex3f(-2.0, 0.0, -0.5);
         glVertex3f(+2.0, 0.0, -0.5);
         glVertex3f(+2.0, 0.0, +0.5);
         glVertex3f(-2.0, 0.0, +0.5);
      glEnd();
      glPopMatrix();
   }
   glPopMatrix();
}

//funcao especial que recebe a funcao de um objeto a ser desenhado, move a origem para (x,y,z) e rotaciona angl normal a y para desenhar
void desenharObjeto(void (*funcPointer)(void), float x, float y, float z, float angl){
   glPushMatrix();
      glTranslatef(x, y, z); //move a parte para x,y,z
      glRotatef(angl, 0.0, 1.0, 0.0); //rotaciona a angl graus com normal y
      (*funcPointer)(); //finalmente, desenha com base na funcao passada
   glPopMatrix();
}

//funcao de animacao, onde todas as alteracoes de posicoes sao feitas antes de redesenhar na tela
void anima(int a){
   int i;
   for(i=0; i<3; i++){
      G_Ruas[i].pos[0] -= 2.0;
      printf("%d %f\n",i,G_Ruas[i].pos[0]);
      if(G_Ruas[i].pos[0] < -99.0){
         G_Ruas[i].pos[0] = 99.0 + (G_Ruas[i].pos[0] + 99.0);
      }
   }


   glutPostRedisplay();
   // glutTimerFunc(330, anima, 1);
}

void drawScene(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   int i;
   for(i=0; i<3; i++){
      desenharObjeto(&desenharRua, G_Ruas[i].pos[0], G_Ruas[i].pos[1], G_Ruas[i].pos[2], G_Ruas[i].angle);
   }
   // glLoadIdentity(); Chamar so se constantemente dar glulookat, ja que glulookat deixa a matriz primaria alterada para ele
   glutSwapBuffers();
   glutTimerFunc(33, anima, 1); //chama a funcao de animacao a cada 33milisegundos, equivalendo a aproximadamente 30 quadros por segundo

}

// Initialization routine.
void setup(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   inicializarGlobais();

   angle = 45;
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fAspect = (GLfloat)w/(GLfloat)h;
   gluPerspective(angle,fAspect,0.4,500); //cria uma perspectiva de visao com angulo angle, fAspect = retangulo da base, znear = 0.4 e zfar = 500
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 50.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //de (x,x,x), olhe para (y,y,y) com vetor cima (z,z,z)
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{

}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();

   glutInit(&argc, argv);

   // glutInitContextVersion(4, 3);
   // glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("cameraAnimada.c");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   // glewExperimental = GL_TRUE;
   // glewInit();
   setup();

   glutMainLoop(); 
}
