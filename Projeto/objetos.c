#include "projeto.h"

float modulo(vector3 vetor){ //retorna o modulo do vetor (normalize vector)
   float aux = (vetor.x * vetor.x) + (vetor.y * vetor.y) + (vetor.z * vetor.z);
   return sqrt(aux);
}

vector3 normalizar(vector3 vetor){ //recebe um vetor qualquer e retorna sua versao normalizada
   float aux = modulo(vetor);

   vetor.x = vetor.x/aux; vetor.y = vetor.y/aux; vetor.z = vetor.z/aux;
   return vetor;
}


void criarCarro(vector3 direc){
   int i;
   for(i = 1; i<10; i++){
      if(G_carros[i] != NULL) continue; //primeiro verifica se ha um espaco vazio no vetor
      G_carros[i] = (carro*) malloc(sizeof(carro)); //se encontrou um espaco vazio, alocar memoria pro carro novo
      G_carros[i]->pos.x = direc.x>0? -100.0: 100.0; //se a velocidade x for positiva, o carro vem da esquerda, senao, da direita
      G_carros[i]->pos.y = 4.0; //sua posicao inicial
      G_carros[i]->pos.z = (direc.x>0? 21.0: -7.0) +((rand()%2 * 2) * -7); //aleatoriza qual faixa ele esta, esquerda ou direita, para seu sentido
      G_carros[i]->veloc = modulo(direc);
      direc = normalizar(direc); //normaliza o vetor, para garantir que os valores abaixo estejam corretos
      G_carros[i]->direc.x = direc.x; //vetor unitario com direcao dada pela funcao
      G_carros[i]->direc.y = direc.y; //sentido de movimento do carro
      G_carros[i]->direc.z = direc.z;
      break; //novo carro criado, abandonar laco e sair da funcao
   }
}

void desenharCarro(){ //funcao temporariamente desenha apenas um circulo, usada apenas para testar as funcionalidades do carro
   glColor3f(0.0, 1.0, 1.0);
   glutSolidSphere(4.0, 100, 100); //raio, fatias em z, pilhas em z. 100 * 100 partes
   glPushMatrix();
      glTranslatef(5.0, 0.0, 0.0); //desenha um circulo vermelho menor na frente do circulo inicial ,para ser possivel discernir direcao
      glColor3f(1.0, 0.0, 0.0);
      glutSolidSphere(1.0, 100, 100); //raio, fatias em z, pilhas em z. 100 * 100 partes
   glPopMatrix();

}

void desenharRua(){
   glColor3f(0.3, 0.3, 0.3); //cinza
   glBegin(GL_QUADS);
      glVertex3f(-33.0, 0.0, -30);
      glVertex3f(+33.0, 0.0, -30); //desenha a base da rua
      glVertex3f(+33.0, 0.0, +30); //retangulo 66:60 com centro em (0,0,0)
      glVertex3f(-33.0, 0.0, +30);
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
   for(x = -1; x<=1; x++){ //faz 3 retangulos, um em cada ponta da rua e um no meio
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix(); //empilha a matriz atual para alterar apenas ela
      glTranslatef(x*33.0, 0.0, 15.0); //progressivamente move ao longo da rua para desenhar as faixas, fazendo 3 retangulos brancos
      glBegin(GL_QUADS);
         glVertex3f(-2.0, 0.0, -0.5);
         glVertex3f(+2.0, 0.0, -0.5); //retangulo 4:1 com centro em (0.0.0)
         glVertex3f(+2.0, 0.0, +0.5);
         glVertex3f(-2.0, 0.0, +0.5);
      glEnd();
      glPopMatrix(); //remove a matriz do topo

      glPushMatrix(); //adciona outra matriz no topo
      glTranslatef(x*33.0, 0.0, -15.0); //progressivamente move ao longo do lado oposto (z=-10) para desenhar os retangulos da outra faixa
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
void atualizarPosicoes(int a){
   int i;
   for(i=0; i<3; i++){
      G_Ruas[i].pos.x -= G_carros[0]->veloc;
      if(G_Ruas[i].pos.x < -99.0){ //caso a rua esteja pra sair da tela
         G_Ruas[i].pos.x = 99.0 + (G_Ruas[i].pos.x + 99.0); //mova a para tras da rua mais distante
      }
   }

   for(i=1; i<10; i++){
      if(G_carros[i] == NULL) continue; //caso o carro nao exista, nao realizar movimentacao para ele
      //mover o carro com base na velocidade relativa ao carro principal [0]
      G_carros[i]->pos.x += (G_carros[i]->veloc * G_carros[i]->direc.x) - (G_carros[0]->veloc * G_carros[0]->direc.x);
      if(G_carros[i]->pos.x < -100 || G_carros[i]->pos.x > 100){ //caso ele saia da tela
         free(G_carros[i]); //desalocar sua memoria e por fim
         G_carros[i] = NULL;//abandonar o ponteiro anterior
      }
   }

   glutPostRedisplay();
}