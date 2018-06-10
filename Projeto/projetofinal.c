#include "projeto.h"

int menu = 0; //variavel de estado
void inicializarGlobais(){
   int i;
   for(i = -1; i<=1; i++){
      G_Ruas[i+1].angle = 0.0;
      G_Ruas[i+1].pos.x = 66.0*i; //Cada rua e inicializada 66.0 unidades em x atras da proxima {-66, 0, +66}
      G_Ruas[i+1].pos.y = 0.0;
      G_Ruas[i+1].pos.z = 0.0;
   }
   G_carros[0] = (carro*) malloc(sizeof(carro)); //carros sao declarados como ponteiros de struct para poder checar se um carro existe ou nao
   G_carros[0]->pos.x = 0.0;                    //simplesmente checando se seu ponteiro e nulo ou nao
   G_carros[0]->pos.y = 4.0;
   G_carros[0]->pos.z = 7.0;
   G_carros[0]->direc.x = 1.0; //vetor unitario com direcao x positivo
   G_carros[0]->direc.y = 0.0; //sentido de movimento do carro principal
   G_carros[0]->direc.z = 0.0;
   G_carros[0]->veloc = 1.0;
}

// Initialization routine.
void setup(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   inicializarGlobais();

   angle = 90;
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   //vazio por agora, eventualmente contera as instrucoes de uso no programa
}

// Main routine.
int main(int argc, char **argv) {
   srand(time(NULL));
   printInteraction();

   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Projeto final de CG.c");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   setup();

   glutMainLoop(); 
}
