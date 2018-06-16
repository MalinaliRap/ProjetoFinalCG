#include "projeto.h"

// Initialization routine.
void setup(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   inicializarSimulacao();
   angle = 45;
}

// OpenGL window reshape routine.
void resize(int w, int h){
   if(menu){} //resizeMenu(w,h);
   else resizeSimulacao(w,h); //resize para a simulacao, que usa projecao em perspectiva
}

void drawScene(void){
   if(menu){} //menu();           pre declaracao do formato da funcao de desenho
   else simulacao();       //se estiver no menu, desenha o menu, se nao, desenha a simulacao
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   if(menu){} //keyMenu(key, x, y);
   else keySimulacao(key, x, y);
}

void specialInput(int key, int x, int y){
   if(menu){} //keyMenu(key, x, y);
   else specialKeySimulacao(key, x, y);
}
// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   //vazio por agora, eventualmente contera as instrucoes de uso no programa
}

// Main routine.
int main(int argc, char **argv) {
   srand(time(NULL));
   printInteraction();
   menu = 0; //inicializando variavel de estado

   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Projeto final de CG.c");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialInput);

   setup();

   glutMainLoop(); 
}
