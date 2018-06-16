#include "projeto.h"

float modulo(vector3 vetor){ //retorna o modulo do vetor (normalize vector)
   float aux = (vetor.x * vetor.x) + (vetor.y * vetor.y) + (vetor.z * vetor.z);
   return sqrt(aux);
}

vector3 escalarVetor(float escalar, vector3 vetor){
   vetor.x *= escalar; vetor.y *= escalar; vetor.z *= escalar;
   return vetor;
}

vector3 somarVetores(vector3 a, vector3 b){
   a.x += b.x; a.y += b.y; a.z += b.z;
   return a;
}

vector3 normalizar(vector3 vetor){ //recebe um vetor qualquer e retorna sua versao normalizada
   float aux = modulo(vetor);

   vetor.x = vetor.x/aux; vetor.y = vetor.y/aux; vetor.z = vetor.z/aux;
   return vetor;
}


void criarCarro(vector3 direc){ //funcao para criar carros que sempre vem ou da esquerda ou da direita
   int i;
   for(i = 1; i<10; i++){
      if(G_Carros[i] != NULL) continue; //primeiro verifica se ha um espaco vazio no vetor
      G_Carros[i] = (carro*) malloc(sizeof(carro)); //se encontrou um espaco vazio, alocar memoria pro carro novo
      G_Carros[i]->pos.x = direc.x>0? -180.0: 180.0; //se a velocidade x for positiva, o carro vem da esquerda, senao, da direita
      G_Carros[i]->pos.y = 4.0; //sua posicao inicial
      G_Carros[i]->pos.z = (direc.x>0? 52.5: -17.5) +((rand()%2 * 2) * -17.5); //aleatoriza qual faixa ele esta, esquerda ou direita, para seu sentido
      G_Carros[i]->veloc = modulo(direc);
      direc = normalizar(direc); //normaliza o vetor, para garantir que os valores abaixo estejam corretos
      G_Carros[i]->direc.x = direc.x; //vetor unitario com direcao dada pela funcao
      G_Carros[i]->direc.y = direc.y; //sentido de movimento do carro
      G_Carros[i]->direc.z = direc.z;
      break; //novo carro criado, abandonar laco e sair da funcao
   }
}

void desenharCarro(){ //funcao temporariamente desenha apenas um circulo, usada apenas para testar as funcionalidades do carro
   glColor3f(0.0, 1.0, 1.0); //cor do objeto
   // Capacidade de brilho do material
   GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
   GLint especMaterial = 100;
   // Define a refletância do material
   glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
   // Define a concentração do brilho
   glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

   glutSolidCube(10.0);
   glPushMatrix();
      glTranslatef(11.0, 0.0, 0.0); //desenha um circulo vermelho menor na frente da forma inicial ,para ser possivel discernir direcao
      glColor3f(1.0, 0.0, 0.0);
      glutSolidSphere(1.0, 100, 100); //raio, fatias em z, pilhas em z. 100 * 100 partes
   glPopMatrix();

}

void desenharFaixa(float w, float h, float l){ //funcao adcional para desenhar faixas, uma vez que foi necessario adcionar uma profundidade
   glBegin(GL_QUADS);               //virtual, para que a faixa nao funcionasse de forma imprevisivel
      glVertex3f(-w, 0, -l);
      glVertex3f(+w, 0, -l); 
      glVertex3f(+w, 0, +l); //desenha a faixa horizontal normalmente
      glVertex3f(-w, 0, +l);
   glEnd();

   glBegin(GL_QUADS);
      glVertex3f(-w, -h, +l);
      glVertex3f(+w, -h, +l); //desenha uma face lateral diretamente abaixo 
      glVertex3f(+w, +h, +l); //para que caso a faixa seja vista de lado, ela nao seja invisivel
      glVertex3f(-w, +h, +l);
   glEnd();
}

void desenharRua(){
   glColor3f(0.3, 0.3, 0.3); //cinza
   GLfloat especularidade[4]={0.1,0.1,0.1,1.0};
   GLint especMaterial = 10;
   glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
   glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
   
   glBegin(GL_QUADS);
      glVertex3f(-120.0, 0.0, -70.5);
      glVertex3f(+120.0, 0.0, -70.5); //desenha a base da rua
      glVertex3f(+120.0, 0.0, +70.5); //retangulo 24m:14.1m com centro em (0,0,0)
      glVertex3f(-120.0, 0.0, +70.5);
   glEnd();

   glColor3f(1.0, 1.0, 0.0); //amarelo
   desenharFaixa(120, 0.5, 0.5);
   
   int i;
   glColor3f(1.0, 1.0, 1.0);

   glPushMatrix(); //empilha a matriz atual para alterar apenas ela
   for (i = -2; i < 2; ++i){
      glPushMatrix();
      glTranslatef(i*60, 0.0, 35.5); //move para um dos lados da rua e desenha a faixa la
      desenharFaixa(10, 0.5, 0.5);
      glPopMatrix(); //remove a matriz do topo


      glPushMatrix(); //adciona outra matriz no topo
      glTranslatef(i*-60, 0.0, -35.5); //move para o lado oposto
      desenharFaixa(10, 0.5, 0.5);
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

//todo Implementar Arvores / Predios / Objeto de fundo
//todo Implementar desenho da grama (Com texturas!!)