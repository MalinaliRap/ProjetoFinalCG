#include "projeto.h"


void simulacao(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   int i;
   float anglAux;
   //rotina para desenhar as ruas
   for(i=0; i<3; i++){
      desenharObjeto(&desenharRua, G_Ruas[i].pos.x, G_Ruas[i].pos.y, G_Ruas[i].pos.z, G_Ruas[i].angle);
   }

   //calculamos o angulo do vetor de direcao do carro para definirmos a rotacao de seu desenho usando a funcao
   //atan para z/x. Como atan so retorna valores entre -pi/2 e pi/2, e necessario observar o caso onde o vetor aponta na
   //direcao contraria, ou seja, seu x seja menor que 0, para assim pegarmos o complemento do angulo gerado.
   for(i=0; i<10; i++){
      if(G_carros[i] == NULL) continue;
      if(G_carros[i]->direc.x >= 0) anglAux = atan(G_carros[i]->direc.z / G_carros[i]->direc.x) * 180 / PI;
      else anglAux = (atan(G_carros[i]->direc.z / G_carros[i]->direc.x)*180/PI)-180;
      desenharObjeto(&desenharCarro, G_carros[i]->pos.x, G_carros[i]->pos.y, G_carros[i]->pos.z, anglAux);
   }
   // glLoadIdentity(); Chamar so se constantemente dar glulookat, ja que glulookat deixa a matriz de modelo alterada para ele
   glutSwapBuffers();
   glutTimerFunc(33, atualizarPosicoes, 1); //chama a funcao de animacao a cada 33milisegundos, equivalendo a aproximadamente 30 quadros por segundo
}

void resizeSimulacao(int w, int h){
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fAspect = (GLfloat)w/(GLfloat)h;
   gluPerspective(angle,fAspect,0.4,500); //cria uma perspectiva de visao com angulo angle, fAspect = retangulo da base, znear = 0.4 e zfar = 500
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //de (x,x,x), olhe para (y,y,y) com vetor cima (z,z,z)
}

void keySimulacao(unsigned char key, int x, int y){
   vector3 aux = {0.0, 0.0, 0.0};
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'z':
         aux.x = -1.0;
         criarCarro(aux);
         break;
      case 'a':
         aux.x = 1.5;
         criarCarro(aux);
         break;
   }
}