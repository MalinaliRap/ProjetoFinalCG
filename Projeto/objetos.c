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

float produtoInterno(vector3 a, vector3 b){
   return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

vector3 normalizar(vector3 vetor){ //recebe um vetor qualquer e retorna sua versao normalizada
   float aux = modulo(vetor);

   vetor.x = vetor.x/aux; vetor.y = vetor.y/aux; vetor.z = vetor.z/aux;
   return vetor;
}

vector3 computarNormal(vector3 a, vector3 b){ //retorna o vetor unitario normal aos vetores dados
   vector3 aux;
   aux.x = (a.y*b.z) - (a.z*b.y);                            //{{i,j,k}
   aux.y = (a.x*b.z) - (a.z*b.x); //forma pseudo determinante {ax,ay,az}
   aux.z = (a.x*b.y) - (a.y*b.x);                           //{bx,by,bz}}
   return aux;
}

float anguloEntreVetores(vector3 a, vector3 b){
   float aux = acos(produtoInterno(a,b) / (modulo(a)*modulo(b))); //pi/2 -> -pi/2
   return  (aux/PI) * 180;
}


void writeBitmapString(void *font, char *string){
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter((void*)font, *c);
} 

void writeStrokeString(void *font, char *string){
   char *c;
   for (c = string; *c != '\0'; c++) glutStrokeCharacter((void*)font, *c);
}


void criarCarro(vector3 direc){ //funcao para criar carros que sempre vem ou da esquerda ou da direita
   int i;
   float aux = G_Ruas[0].pos.z; //pega o valor de z das ruas, para poder criar carros relativo ao centro das ruas
   for(i = 1; i<10; i++){
      if(G_Carros[i] != NULL) continue; //primeiro verifica se ha um espaco vazio no vetor
      G_Carros[i] = (carro*) malloc(sizeof(carro)); //se encontrou um espaco vazio, alocar memoria pro carro novo
      G_Carros[i]->pos.x = direc.x>0? -180.0: 180.0; 
      //se a velocidade x for positiva, o carro vem da esquerda, senao, da direita
      G_Carros[i]->pos.y = 5.0; //sua posicao inicial
      float posNaRua =  (LARGURAFAIXA/2) + ((rand()%2) * LARGURAFAIXA);
      G_Carros[i]->pos.z = (direc.x>0? aux + posNaRua: aux + -posNaRua); //aleatoriza qual faixa ele esta, esquerda ou direita, para seu sentido
      G_Carros[i]->veloc = modulo(direc); //pega o modulo desse vetor, sua velocidade pura
      direc = normalizar(direc); //normaliza o vetor, para garantir que os valores abaixo estejam corretos
      G_Carros[i]->direc.x = direc.x; //vetor unitario com direcao dada pela funcao
      G_Carros[i]->direc.y = direc.y; //sentido de movimento do carro
      G_Carros[i]->direc.z = direc.z;
      G_Carros[i]->rot = 0;
      break; //novo carro criado, abandonar laco e sair da funcao
   }
}
void desenharRoda(){
   glColor3f(0.0, 0.0, 0.0);
   float r;
   glBegin(GL_TRIANGLE_STRIP);
      for(r=0; r<2*PI+PI/50; r+=PI/50){
         glNormal3f(cos(r), sin(r), 0);
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), -1);
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), 1);
      }
   glEnd();

   glColor3f(0.3, 0.3, 0.3);
   glBegin(GL_LINES);
      for(r=0; r<2*PI+PI/4; r+=PI/4){
         glNormal3f(cos(r), sin(r), 1);
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), 1);
         glVertex3f(0, 0, 0);
      }
   glEnd();

   glBegin(GL_LINES);
      for(r=0; r<2*PI+PI/4; r+=PI/4){
         glNormal3f(cos(r), sin(r), -1);
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), -1);
         glVertex3f(0, 0, 0);
      }
   glEnd();

   glColor3f(0.7, 0.7, 0.7);
   glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0, 0, 1);
      glVertex3f(0.0, 0.0, 0.0);
      for(r=0; r<2*PI+PI/50; r+=PI/50){
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), 1);
      }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0, 0, -1);
      glVertex3f(0.0, 0.0, 0.0);
      for(r=0; r<2*PI+PI/50; r+=PI/50){
         glVertex3f((cos(r)*3.5), (sin(r)*3.5), -1);
      }
   glEnd();
}
void desenharCarro(){ //funcao temporariamente desenha apenas um circulo, usada apenas para testar as funcionalidades do carro
   glColor3f(1.0, 0.3, 0.0); //cor do objeto

   // GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
   // GLint especMaterial = 100;
   // glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
   // glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    // 
    // Define a refletância do material
    // Define a concentração do brilho
    

   // glPushMatrix();
   // glScalef(45, 16, 26);
   // glutSolidCube(1);
   // glPopMatrix();
   float r;
   glPushMatrix();
   glScalef(0.8, 0.8, 0.8);
   glBegin(GL_QUAD_STRIP);
      glNormal3f(-1, 0, 0);
      glVertex3f(-22.5   , 0.0 , -13.0);
      glVertex3f(-22.5   , 0.0 , 13.0);
      glVertex3f(-22.5   , 7.5 , -13.0);
      glVertex3f(-22.5   , 7.5 , 13.0);
      glNormal3f(0, 1, 0);
      glVertex3f(-11.25 , 7.5 , -13.0);
      glVertex3f(-11.25 , 7.5 , 13.0);
      glNormal3f(-7.5, 5.625, 0);
      glVertex3f(-5.625, 15.0, -13.0);
      glVertex3f(-5.625, 15.0, 13.0);
      glNormal3f(0, 1, 0);
      glVertex3f(5.625, 15.0, -13.0);
      glVertex3f(5.625, 15.0, 13.0);
      glNormal3f(7.5, 5.625, 0);
      glVertex3f(11.25 , 7.5 , -13.0); //desenha a lataria
      glVertex3f(11.25 , 7.5 , 13.0);
      glNormal3f(0, 1, 0);
      glVertex3f(22.5  , 7.5 , -13.0);
      glVertex3f(22.5  , 7.5 , 13.0);
      glNormal3f(1, 0, 0);
      glVertex3f(22.5  , 0.0 , -13.0);
      glVertex3f(22.5  , 0.0 , 13.0);
      // for(r=0; r<PI+PI/50; r+=PI/50){
         // glVertex3f(13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), -13.0);
         // glVertex3f(13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), 13.0);          contorno das rodas
      // }
      // for(r=0; r<PI+PI/50; r+=PI/50){
         // glVertex3f(-13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), -13.0);
         // glVertex3f(-13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), 13.0);
      // }
      glNormal3f(-1, 0, 0);
      glVertex3f(-22.5, 0.0, -13.0);
      glVertex3f(-22.5, 0.0, 13.0);
   glEnd();

   glBegin(GL_POLYGON);
      glNormal3f(0, 0, 1);
      glVertex3f(-22.5   , 0.0 , 13.0);
      glVertex3f(-22.5   , 7.5 , 13.0);
      glVertex3f(-11.25 , 7.5 , 13.0); //lado esquerdo da lataria
      glVertex3f(-5.625, 15.0, 13.0);
      glVertex3f(5.625, 15.0, 13.0);
      glVertex3f(11.25 , 7.5 , 13.0);
      glVertex3f(22.5  , 7.5 , 13.0);
      glVertex3f(22.5  , 0.0 , 13.0);
      for(r=0; r<PI+PI/50; r+=PI/50){
         glVertex3f(13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), 13.0);
      }
      for(r=0; r<PI+PI/50; r+=PI/50){
         glVertex3f(-13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), 13.0);
      }
      glVertex3f(-22.5, 0.0, 13.0);
   glEnd();

   glBegin(GL_POLYGON);
      glNormal3f(0, 0, -1);
      glVertex3f(-22.5   , 0.0 , -13.0);
      glVertex3f(-22.5   , 7.5 , -13.0);
      glVertex3f(-11.25 , 7.5 , -13.0); //ladi direito da lataria
      glVertex3f(-5.625, 15.0, -13.0);
      glVertex3f(5.625, 15.0, -13.0);
      glVertex3f(11.25 , 7.5 , -13.0);
      glVertex3f(22.5  , 7.5 , -13.0);
      glVertex3f(22.5  , 0.0 , -13.0);
      for(r=0; r<PI+PI/50; r+=PI/50){
         glVertex3f(13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), -13.0);
      }
      for(r=0; r<PI+PI/50; r+=PI/50){
         glVertex3f(-13.5+(cos(r)*3.5), 0.0+(sin(r)*3.5), -13.0);
      }
      glVertex3f(-22.5, 0.0, -13.0);
   glEnd();
   //vidro frente
   glEnable (GL_BLEND); 
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //ATIVA TRANSPAR�NCIA
   
   	glColor4f(1.0,1.0,1.0,0.4);
   		glNormal3f(0, -1, 0);
	   		glBegin(GL_QUADS);
	   			glVertex3f(11.30, 7.5, -10);
	   			glVertex3f(5.7, 15.0, -10);
	   			glVertex3f(5.7, 15.0, 10);
	   			glVertex3f(11.30, 7.5, 10);
	   		glEnd();
	   		glBegin(GL_QUADS);
	   			glVertex3f(-11.30, 7.5, -10);
	   			glVertex3f(-5.7, 15.0, -10);
	   			glVertex3f(-5.7, 15.0, 10);
	   			glVertex3f(-11.30, 7.5, 10);
	   		glEnd();
	   		glNormal3f(0, 0, -1);
	   		glColor4f(1.0,1.0,1.0,0.5);
	   		glBegin(GL_TRIANGLE_STRIP);
	   			glVertex3f(-11.0, 7.5, 13.5);
	   			glVertex3f(-0.5, 7.5, 13.5);
	   			glVertex3f(-5.5, 14.0, 13.5);
	   			glVertex3f(-0.5, 14.0, 13.5);
	   		glEnd();
	   			glBegin(GL_TRIANGLE_STRIP);
	   			glVertex3f(10.0, 7.5, 13.5);
	   			glVertex3f(0.5, 7.5, 13.5);
	   			glVertex3f(5.5, 14.0, 13.5);
	   			glVertex3f(0.5, 14.0, 13.5);
	   		glEnd();
	   		glBegin(GL_TRIANGLE_STRIP);
	   			glVertex3f(-11.0, 7.5, -13.5);
	   			glVertex3f(-0.5, 7.5, -13.5);
	   			glVertex3f(-5.5, 14.0, -13.5);
	   			glVertex3f(-0.5, 14.0, -13.5);
	   		glEnd();
	   		glBegin(GL_TRIANGLE_STRIP);
	   			glVertex3f(10.0, 7.5, -13.5);
	   			glVertex3f(0.5, 7.5, -13.5);
	   			glVertex3f(5.5, 14.0, -13.5);
	   			glVertex3f(0.5, 14.0, -13.5);
	   		glEnd();
	   		
	   		//farois
	   		glNormal3f(1, 1, 0);
	   		glColor3f(1.0,1.0,0.0);
	   		glBegin(GL_POLYGON);
		   		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(22.55, 4.5+(cos(r)*1.5), 10.0+(sin(r)*1.5));
	      		}
	      		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(22.55, 4.5+(cos(r)*1.5), 10.0+(-sin(r)*1.5));
	      		}
	   		glEnd();
	   		glBegin(GL_POLYGON);
		   		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(22.55, 4.5+(cos(r)*1.5), -10.0+(sin(r)*1.5));
	      		}
	      		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(22.55, 4.5+(cos(r)*1.5), -10.0+(-sin(r)*1.5));
	      		}
	   		glEnd();
	   		
			glNormal3f(1, 1, 0);
	   		glColor3f(1.0,0.0,0.0);
	   		glBegin(GL_POLYGON);
		   		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(-22.55, 4.5+(cos(r)*1.5), 10.0+(sin(r)*1.5));
	      		}
	      		glColor3f(1.0,1.0,1.0);
	      		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(-22.55, 4.5+(cos(r)*1.5), 10.0+(-sin(r)*1.5));
	      		}
	   		glEnd();
	   		
	   		glBegin(GL_POLYGON);
		   		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(-22.55, 4.5+(cos(r)*1.5), -10.0+(sin(r)*1.5));
	      		}
	      		glColor3f(1.0,0.0,0.0);
	      		for(r=0; r<PI+PI/50; r+=PI/50){
	         		glVertex3f(-22.55, 4.5+(cos(r)*1.5), -10.0+(-sin(r)*1.5));
	      		}
	   		glEnd();
	   		
	   		
	   		
   glPopMatrix();
   // desenharObjeto(&desenharRoda, -13.5, 0.0, +13.5, 0);
   // desenharObjeto(&desenharRoda, -13.5, 0.0, -13.5, 180);
   // desenharObjeto(&desenharRoda, +13.5, 0.0, -13.5, 180);
   // desenharObjeto(&desenharRoda, +13.5, 0.0, +13.5, 0);

}

void desenharCaixa(float w, float h, float l){ //funcao adcional para desenhar faixas, uma vez que foi necessario adcionar uma profundidade
   glBegin(GL_QUADS);               //virtual, para que a faixa nao funcionasse de forma imprevisivel quando vista de angulos laterais
      glNormal3f(0, 1, 0);
      glVertex3f(-w, +h, -l);
      glVertex3f(+w, +h, -l); 
      glVertex3f(+w, +h, +l); //desenha o topo da faixa, a parte horizontal visivel
      glVertex3f(-w, +h, +l);
   glEnd();

   glBegin(GL_QUADS);
      glNormal3f(0, -1, 0);
      glVertex3f(-w, -h, -l);
      glVertex3f(+w, -h, -l); 
      glVertex3f(+w, -h, +l); //desenha a base
      glVertex3f(-w, -h, +l);
   glEnd();

   glBegin(GL_QUADS);
      glNormal3f(1,0,0);
      glVertex3f(+w, -h, -l);
      glVertex3f(+w, +h, -l); 
      glVertex3f(+w, +h, +l); //desenha a lateral esquerda
      glVertex3f(+w, -h, +l);
   glEnd();

   glBegin(GL_QUADS);
      glNormal3f(-1, 0, 0);
      glVertex3f(-w, -h, -l);
      glVertex3f(-w, +h, -l); 
      glVertex3f(-w, +h, +l); //desenha a lateral direita
      glVertex3f(-w, -h, +l);
   glEnd();

   glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);
      glVertex3f(-w, -h, +l);
      glVertex3f(+w, -h, +l); //desenha a frente da faixa 
      glVertex3f(+w, +h, +l); 
      glVertex3f(-w, +h, +l);
   glEnd();

   glBegin(GL_QUADS);
      glNormal3f(0, 0, -1);
      glVertex3f(-w, -h, -l);
      glVertex3f(+w, -h, -l); //desenha o fundo da faixa 
      glVertex3f(+w, +h, -l); 
      glVertex3f(-w, +h, -l);
   glEnd();
}

void desenharRua(){
   glColor3f(0.3, 0.3, 0.3); //cinza
   desenharCaixa(120, 0.2, 70.5); //desenha a base da rua, retangulo 24m:14.1m com centro em 0,0,0
   glColor3f(1.0, 1.0, 0.0); //amarelo
   desenharCaixa(120, 0.5, 0.5); //faixa amarela central
   int i;
   glColor3f(1.0, 1.0, 1.0);
   
   glPushMatrix(); //empilha a matriz atual para alterar apenas ela
   for (i = -2; i < 2; ++i){
      glPushMatrix();
      glTranslatef(i*60, 0.0, 35.5); //move para um dos lados da rua e desenha a faixa la
      desenharCaixa(10, 0.5, 0.5);
      glPopMatrix(); //remove a matriz do topo


      glPushMatrix(); //adciona outra matriz no topo
      glTranslatef(i*-60, 0.0, -35.5); //move para o lado oposto
      desenharCaixa(10, 0.5, 0.5);
      glPopMatrix();
   }
   glPopMatrix();
}

void desenharArvore(){ 
   float rotation; //rotacao 1
   vector3 aux = {0,0,0}; //vetor auxiliar para calcular as normais
   vector3 auxb = {0,0,0};
   glColor3f(0.31, 0.22, 0.19); //marrom
   glBegin(GL_TRIANGLE_STRIP);
      for(rotation = 0.0; rotation<2*PI+PI/30; rotation+= PI/30){
         aux.x = (cos(rotation+(PI/30)))-(cos(rotation)); //vetor AB = B-A
         aux.y = 0.0;
         aux.z = (sin(rotation+(PI/30)))-(sin(rotation)); //dois vetores para descrever o plano

         auxb.x = 0.0;
         auxb.y = ALTURAARVORE/1.5 - (0.0);
         auxb.z = 0.0; 
         aux = computarNormal(auxb, aux); // calcula a normal a esses dois vetores, que sera equivalente ao plano

         glNormal3f(aux.x, aux.y, aux.z);
         glVertex3f(cos(rotation)*LARGURACOPA/3, 0.0, sin(rotation)*LARGURACOPA/3); //desenha tronco
         glVertex3f(cos(rotation)*LARGURACOPA/5, ALTURAARVORE/1.5, sin(rotation)*LARGURACOPA/5);
      }
   glEnd();

   glColor3f(0.41, 0.52, 0.2); //verde bonito
   glBegin(GL_TRIANGLE_STRIP);
      for(rotation = 0.0; rotation<2*PI+PI/30; rotation+= PI/30){
         aux.x = (cos(rotation+(PI/30)))-(cos(rotation)); //vetor AB = B-A
         aux.y = 0.0;
         aux.z = (sin(rotation+(PI/30)))-(sin(rotation));

         auxb.x = -(cos(rotation)*LARGURACOPA);
         auxb.y = ALTURAARVORE - (ALTURAARVORE/3);
         auxb.z = -(sin(rotation)*LARGURACOPA); 
         aux = computarNormal(auxb, aux);

         glNormal3f(aux.x, aux.y, aux.z);
         glVertex3f(cos(rotation)*LARGURACOPA, ALTURAARVORE/3, sin(rotation)*LARGURACOPA);
         glVertex3f(0, ALTURAARVORE, 0);
      }
   glEnd();


}

void desenharVelocimetro(velocimetro vel){ //funcao especial para desenhar o velocimetro, unico caso onde nao usamos desenharObjeto
   glDisable(GL_LIGHTING); //desabilitamos iluminacao temporariamente, para que ela nao afete o velocimetro
   glMatrixMode(GL_PROJECTION); //mudamos a matriz que desejamos alterar para projecao
   glPushMatrix(); //salvamos a matriz de projecao tridimensional em perspectiva anterior
   glLoadIdentity();
   glOrtho(0.0, 10.0, 0.0, 10.0, -1, 1); //criamos uma projecao ortogonal
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix(); //salvamos a matriz anterior
   glLoadIdentity(); //carregamos a identidade na projecao de modelo

   float r;
   glTranslatef(vel.tamanho, 10-vel.tamanho, 0.0); //o desenho sera feito no topo superior esquerdo da simulacao

   glColor3f(0.8, 0.0, 0.0);
   glBegin(GL_TRIANGLE_FAN); //ele e composto de varios arcos, 1 externo para borda, um interno para leitura, e um para o ponteiro
      glVertex2f(0.0, 0.0);
      for(r=0; r<=PI+PI/100; r+=PI/100){ //raio externo do velocimetro
         glVertex3f(cos(r)*vel.tamanho, sin(r)*vel.tamanho, 0.0);
      }
   glEnd();


   glColor3f(1.0,1.0,1.0);
   glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0.0, 0.0);
      for(r=0; r<=PI+PI/100; r+=PI/100){ //parte branca, para escrita ser legivel
         glVertex3f(cos(r)*(vel.tamanho*9/10), sin(r)*(vel.tamanho*9/10), 0.1);
      }
   glEnd();

   glColor3f(0.6, 0.0, 0.0);
   glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0.0, 0.0);
      for(r=0; r<=PI+PI/100; r+=PI/100){ //centro vermelho, para servir como "base" do ponteiro
         glVertex3f(cos(r)*vel.tamanho/4, sin(r)*vel.tamanho/4, 0.2);
      }
   glEnd();

   int i;
   r = 0;
   float offset; //dependendo do comprimento da string, ela deve ser jogada mais a esquerda para centralizar corretamente
   char aux[4]; //string auxiliar para converter inteiros que deverao ser corretamente desenhados na tela
   glColor3f(0.0, 0.0, 0.0);
   for(i=0; i<=vel.max; i+=10){
      sprintf(aux, "%d", i); //converte i para string e salva em aux
      offset = strlen(aux) * 0.1; //cos negativo para que x seja invertido e o circulo venha da esquerda pra direita e nao vice-versa
      glRasterPos3f(-(cos(r) * (vel.tamanho*8/10)) - offset, sin(r) * (vel.tamanho*8/10), 0.3); //define a posicao que o texto sera impresso
      writeBitmapString((void*)font, aux); //imprime o texto
      r+= PI/(vel.max/10);
   }
   
   i = (vel.ponteiro/PI) * vel.max; //pega a velocidade atual com base no angulo do ponteiro 
   sprintf(aux, "%d", i);
   offset = strlen(aux) * 0.1;
   glRasterPos3f(-offset, (vel.tamanho)/2, 0.4);
   writeBitmapString((void*)font, aux);

   glLineWidth(2);
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0); //cos invertido para o ponteiro acompanhar a velocidade real
      glVertex3f(-cos(vel.ponteiro) * (vel.tamanho*9/10), sin(vel.ponteiro) * (vel.tamanho*9/10), 0.3);
   glEnd();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix(); //popamos as matrizes novas, para retornamos as funcoes de projecao anteriores
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
   glEnable(GL_LIGHTING); //e por fim, re habilitamos a iluminacao
}

float calcularAnguloPonteiro(float max, float velocidade){ //recebe a velocidade maxima do ponteiro e a velocidade atual
   //a velocidade e recebida em decimetros por segundo, entao basta converter a velocidade maxima em decimetros tambem
   return (velocidade/(max*100/36)) * PI;
}

void desenharBotao(botao b){
   glPushMatrix();
      glTranslatef(b.x, b.y, 0.0);
      glColor3f(b.cor[0],b.cor[1],b.cor[2]);
      glBegin(GL_QUADS);
         glVertex3f(0.0,       0.0,      0.0);
         glVertex3f(b.largura, 0.0,      0.0); 
         glVertex3f(b.largura, b.altura, 0.0); 
         glVertex3f(0.0,       b.altura , 0.0);
      glEnd();
      float offset = strlen(b.texto) * 1;
      glColor3f(1-b.cor[0], 1-b.cor[1], 1-b.cor[2]);
      glRasterPos3f(b.largura/2 - offset, b.altura/2, 0.4);
      writeBitmapString((void*)font, b.texto);
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
