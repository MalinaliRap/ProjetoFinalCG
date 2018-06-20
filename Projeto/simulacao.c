#include "projeto.h"

int G_estadoDia; //variavel para controlar a maquina de estado da animacao do dia
float G_rotacaoLuzes; //variavel para controlar a rotacao das luzes em torno da cena
float G_distanciaLuzes; //variavel para definir o raio de rotacao das luzes
float G_timeScale; //variavel para controlar a quantidade de quadros por segundo
float G_corDoCeu[4] = {0.36, 0.79, 0.96, 0.0}; //cor usada pra alterar o clearColor e simular o ceu de fundo

void inicializarGlobais(){
   int i;
   for(i = -1; i<=1; i++){
      G_Ruas[i+1].angle = 0.0;
      G_Ruas[i+1].pos.x = 240*i; //Cada rua e inicializada 240 unidades em x atras da proxima {-240, 0, +240}
      G_Ruas[i+1].pos.y = 0.0; //a razão disso e que cada rua expande 120 unidades em comprimento para cada direcao, assim, duas ruas
      G_Ruas[i+1].pos.z = 0.0;//adjacentes devem ter seu centro a 240 unidades de distancia
   }
   G_Carros[0] = (carro*) malloc(sizeof(carro)); //carros sao declarados como ponteiros de struct para poder checar se um carro existe ou nao
   G_Carros[0]->pos.x = 0.0;                    //simplesmente checando se seu ponteiro e nulo ou nao
   G_Carros[0]->pos.y = 5.0;
   G_Carros[0]->pos.z = 18.25;
   G_Carros[0]->direc.x = 1.0; //vetor unitario com direcao x positivo
   G_Carros[0]->direc.y = 0.0; //sentido de movimento do carro principal
   G_Carros[0]->direc.z = 0.0;
   G_Carros[0]->veloc = 100.0;

   for(i=0; i<QUANTIDADEARVORES; i++){
   	G_Arvores[i].angle = 0.0;
   	G_Arvores[i].pos.x = (rand()%360) - 180; //arvores espalhadas aleatoriamente em x
   	G_Arvores[i].pos.y = 0;
   	G_Arvores[i].pos.z = (rand()%10)==9? (G_Ruas[0].pos.z + (LARGURAFAIXA*2)+10) + rand()%50 : (G_Ruas[0].pos.z - (LARGURAFAIXA*2)-10) - rand()%150;
   }

   G_Luzes[0].pos.x = 0.0;
   G_Luzes[0].pos.y = 250.0; //declaracoes de iluminacao
   G_Luzes[0].pos.z = 0.0; //luz 0
	G_Luzes[0].ambiente[0] = 0.2;  G_Luzes[0].ambiente[1] = 0.2;  G_Luzes[0].ambiente[2] = 0.2;  G_Luzes[0].ambiente[3] = 1.0;
   G_Luzes[0].difusa[0] = 1.0;    G_Luzes[0].difusa[1] = 0.75;   G_Luzes[0].difusa[2] = 0.21;   G_Luzes[0].ambiente[3] = 1.0;
   G_Luzes[0].especular[0] = 0.6; G_Luzes[0].especular[1] = 0.6; G_Luzes[0].especular[2] = 0.6; G_Luzes[0].ambiente[3] = 0.6;
   G_Luzes[0].enabled = 1;
   //luz 1
   G_Luzes[1].pos = escalarVetor(-1, G_Luzes[0].pos); //posicao invertida em relacao a luz 0
   G_Luzes[1].ambiente[0] = 0.1;  G_Luzes[1].ambiente[1] = 0.1;  G_Luzes[1].ambiente[2] = 0.1;  G_Luzes[1].ambiente[3] = 1.0;
   G_Luzes[1].difusa[0] = 0.1;    G_Luzes[1].difusa[1] = 0.2;    G_Luzes[1].difusa[2] = 0.4;    G_Luzes[1].ambiente[3] = 1.0;
   G_Luzes[1].especular[0] = 0.5; G_Luzes[1].especular[1] = 0.5; G_Luzes[1].especular[2] = 0.5; G_Luzes[1].ambiente[3] = 1.0;
   G_Luzes[1].enabled = 0;

   G_estadoDia = 1;
   G_rotacaoLuzes = PI/2;
   G_distanciaLuzes = 250;
   G_timeScale = 30;

   G_Velocimetro.tamanho = 2;
   G_Velocimetro.max = 120;
   G_Velocimetro.ponteiro = calcularAnguloPonteiro(G_Velocimetro.max, G_Carros[0]->veloc);

   // G_Camera.pos.x = 0.0;
   // G_Camera.pos.y = 100.0; //posicao da camera
   // G_Camera.pos.z = 180.0;

   // G_Camera.look.x = 0.0;
   // G_Camera.look.y = -100; //posicao para aonde a camera olha
   // G_Camera.look.z = -180;
   // G_Camera.look = normalizar(G_Camera.look); //transforma em modulo para facilitar calculos
}

void inicializarSimulacao(void){
   angle = 45;
	resizeSimulacao(500, 500); //para redefinir a matrix de projecao inicial ao iniciar a simulacao pela primeira vez
	inicializarGlobais();
	glClearColor(G_corDoCeu[0], G_corDoCeu[1], G_corDoCeu[2],G_corDoCeu[3]);
   glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
}
void animacaoLuz(){
	int i;
	float porDoSol[3] = {0.79, 0.21, 0.018}; //cor para o por do sol
	float amanhecer[3] = {1.0, 0.75, 0.21};
	float ceuDia[3] = {0.39, 0.70, 0.96};		
	// float luzDaNoite[3] = {0.13, 0.25, 0.38};//cor durante a noite
	if(G_estadoDia == 2){ //anoitecer
		if(G_Luzes[0].difusa[0] >= porDoSol[0]){ //se a cor ja tiver chegado no por do sol
			for(i=0; i<3; i++){ //desejado - inicial, formula de incremento continuo para ir de um numero para outro de forma constante
				G_Luzes[0].difusa[i] += (porDoSol[i] - amanhecer[i]) / (G_timeScale*5.0); //ao longo de 5 segundos, a cor se aproximara da desejada
				G_corDoCeu[i] += (porDoSol[i] - ceuDia[i]) / (G_timeScale*5.0);
			}
			glClearColor(G_corDoCeu[0], G_corDoCeu[1], G_corDoCeu[2],G_corDoCeu[3]);
		}

		G_Luzes[0].pos.x = cos(G_rotacaoLuzes) * G_distanciaLuzes; //gira as luzes em torno do centro
		G_Luzes[0].pos.y = sin(G_rotacaoLuzes) * G_distanciaLuzes;

		G_Luzes[1].pos.x = cos(G_rotacaoLuzes - PI) * G_distanciaLuzes;
		G_Luzes[1].pos.y = sin(G_rotacaoLuzes - PI) * G_distanciaLuzes;

		G_rotacaoLuzes+= PI/(10*G_timeScale); //a luz leva 10 segundos para girar ate o lado oposto 
		if(G_Luzes[0].pos.y < 0 && G_Luzes[0].enabled){
			G_Luzes[0].enabled = 0;
			for(i=0; i<3; i++){
				G_Luzes[0].difusa[i] = 0.0;
				G_corDoCeu[i] = 0.0;
			}
			glDisable(GL_LIGHT0);
		}
		if(G_Luzes[1].pos.y > 0 && !G_Luzes[1].enabled){
			glEnable(GL_LIGHT1);
			G_Luzes[1].enabled = 1;
			for(i=0; i<3; i++){
				G_corDoCeu[i] = 0;
			}
			glClearColor(0.0, 0.0, 0.0, 0.0);
		}
		if(G_rotacaoLuzes >= 3*PI/2){ //se o sol completamente se pos (pi/2 -> 3pi/2), entao o anoiteceu aconteceu
			G_rotacaoLuzes = -PI/2; //inverte para -pi/2, para facilitar os calculos
			G_estadoDia = -1;
			printf("Anoitecimento completo\n");
		}
	}

	if(G_estadoDia == -2){ //amanhecer
		if(G_Luzes[0].enabled){
			if(G_Luzes[0].difusa[0] <= amanhecer[0]){ //comecar a trocar sua cor
				for(i=0; i<3; i++){
					G_Luzes[0].difusa[i] += (amanhecer[i] - 0.0) / (G_timeScale*5.0);
					G_corDoCeu[i] += (ceuDia[i] - 0.0) / (G_timeScale*5.0);
				}
				glClearColor(G_corDoCeu[0], G_corDoCeu[1], G_corDoCeu[2],G_corDoCeu[3]);
			}
		}
		G_Luzes[0].pos.x = cos(G_rotacaoLuzes) * G_distanciaLuzes;
		G_Luzes[0].pos.y = sin(G_rotacaoLuzes) * G_distanciaLuzes;

		G_Luzes[1].pos.x = cos(G_rotacaoLuzes - PI) * G_distanciaLuzes;
		G_Luzes[1].pos.y = sin(G_rotacaoLuzes - PI) * G_distanciaLuzes;

		G_rotacaoLuzes+= PI/(10*G_timeScale); //a luz leva 10 segundos para descer ate o lado oposto 
		if(G_Luzes[1].pos.y < 0 && G_Luzes[1].enabled){
			G_Luzes[1].enabled = 0;
			glDisable(GL_LIGHT1);
		}
		if(G_Luzes[0].pos.y > 0 && !G_Luzes[0].enabled){
			glEnable(GL_LIGHT0);
			G_Luzes[0].enabled = 1;
		}
		if(G_rotacaoLuzes >= PI/2){ //se o sol chegou ao pico, o dia completamente nasceu
			G_estadoDia = 1;
			printf("Amanhecer completo\n");
		}
	}
}

void defineIluminacao (void){
   // Define os parametros da luz de numero 0, se estiver ativada
   if(G_Luzes[0].enabled){
	   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, G_Luzes[0].ambiente);
	   GLfloat posicaoLuz[4]={G_Luzes[0].pos.x, G_Luzes[0].pos.y, G_Luzes[0].pos.z, 1.0};
	   glLightfv(GL_LIGHT0, GL_AMBIENT, G_Luzes[0].ambiente);
	   glLightfv(GL_LIGHT0, GL_DIFFUSE, G_Luzes[0].difusa);
	   glLightfv(GL_LIGHT0, GL_SPECULAR, G_Luzes[0].especular);
	   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
   }
   // Define os parametros da luz de numero 1, se estiver ativada
	if(G_Luzes[1].enabled){
	   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, G_Luzes[1].ambiente);
	   GLfloat posicaoLuz[4]={G_Luzes[1].pos.x, G_Luzes[1].pos.y, G_Luzes[1].pos.z, 1.0};
	   glLightfv(GL_LIGHT1, GL_AMBIENT, G_Luzes[1].ambiente);
	   glLightfv(GL_LIGHT1, GL_DIFFUSE, G_Luzes[1].difusa);
	   glLightfv(GL_LIGHT1, GL_SPECULAR, G_Luzes[1].especular);
	   glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);
   }
}

//funcao de animacao, onde todas as alteracoes de posicoes sao feitas antes de redesenhar na tela
void atualizarPosicoes(int a){
   int i;
   vector3 aux;
   aux = escalarVetor(-G_Carros[0]->veloc/G_timeScale, G_Carros[0]->direc); //calcula movimento relativo ao carro principal
   for(i=0; i<3; i++){
      G_Ruas[i].pos = somarVetores(G_Ruas[i].pos, aux); //aplica movimento
      if(G_Ruas[i].pos.x < -300.0){ //caso o centro da rua esteja pra sair da tela (180(lateral) + 120(comprimento))
         G_Ruas[i].pos.x = 420 + (G_Ruas[i].pos.x + 300.0); //mova a para tras da rua mais distante
      }													//o centro da rua mais distante estaria a 180. +240 = 420.
      else if(G_Ruas[i].pos.x > 300){ //caso esteja pra sair pelo lado direito, faca o processo oposto
      	G_Ruas[i].pos.x = -420 + (G_Ruas[i].pos.x - 300);
      }
   }

   for(i=0; i<QUANTIDADEARVORES; i++){
      G_Arvores[i].pos = somarVetores(G_Arvores[i].pos, aux); //aplica movimento
      if(G_Arvores[i].pos.x < -180){ //caso o centro da arvore esteja pra sair da tela (180(lateral))
         G_Arvores[i].pos.x = 180; //mova a para o lado direito da tela
         G_Arvores[i].pos.z = (rand()%10)==9? (G_Ruas[0].pos.z + (LARGURAFAIXA*2)+10) + rand()%50 : (G_Ruas[0].pos.z - (LARGURAFAIXA*2)-10) - rand()%150;
         // e aleatorize novamente sua posicao em z
      }													//o centro da rua mais distante estaria a 180. +240 = 420.
      else if(G_Arvores[i].pos.x > 180){ //caso esteja pra sair pelo lado direito, faca o processo oposto
      	G_Arvores[i].pos.x = -180; //mova a para o lado esquerdo da tela
         G_Arvores[i].pos.z = (rand()%10)==9? (G_Ruas[0].pos.z + (LARGURAFAIXA*2)+10) + rand()%50 : (G_Ruas[0].pos.z - (LARGURAFAIXA*2)-10) - rand()%150;
         // e aleatorize novamente sua posicao em z
      }
   }

   for(i=1; i<10; i++){
      if(G_Carros[i] == NULL) continue; //caso o carro nao exista, nao realizar movimentacao para ele
      //mover o carro com base na velocidade relativa ao carro principal [0]
      aux = escalarVetor(-G_Carros[0]->veloc/G_timeScale, G_Carros[0]->direc); //calcula o movimento relativo do carro principal
      G_Carros[i]->pos = somarVetores(G_Carros[i]->pos, aux);  //movimenta com base nisso
      aux = escalarVetor(G_Carros[i]->veloc/G_timeScale, G_Carros[i]->direc); //calcula o movimento com base na propria velocidade
      G_Carros[i]->rot += modulo(aux); //gira suas rodas com base na sua velocidade
      G_Carros[i]->pos = somarVetores(G_Carros[i]->pos, aux); //movimenta nisso
      if(G_Carros[i]->pos.x < -180 || G_Carros[i]->pos.x > 180){ //caso ele saia da tela
         free(G_Carros[i]); //desalocar sua memoria e por fim
         G_Carros[i] = NULL;//abandonar o ponteiro anterior
      }
   }

   animacaoLuz();

   glutPostRedisplay();
}


void simulacao(void){
	GLfloat especularidade[4]={0.4,0.4,0.4,1.0};
   GLint especMaterial = 20;
   glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
   glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   int i;
   float anglAux;

   desenharVelocimetro(G_Velocimetro);
   defineIluminacao();
   // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
   glColor3f(0.0, 0.7, 0.0);
   // GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
   // GLint especMaterial = 60;
    
    // glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    
   // glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
   // glColor3f(0.2, 0.2, 0.2);

   glBegin(GL_QUADS);
   	glVertex3f(-250, -1, 600);
   	glVertex3f(-250, -1, -600); //desenha a grama ao redor
   	glVertex3f(250, -1, -600);
   	glVertex3f(250, -1, 600);
   glEnd();
   //rotina para desenhar as ruas
   for(i=0; i<3; i++){
      desenharObjeto(&desenharRua, G_Ruas[i].pos.x, G_Ruas[i].pos.y, G_Ruas[i].pos.z, G_Ruas[i].angle);
   }

   //calculamos o angulo do vetor de direcao do carro para definirmos a rotacao de seu desenho usando a funcao
   //atan para z/x. Como atan so retorna valores entre -pi/2 e pi/2, e necessario observar o caso onde o vetor aponta na
   //direcao contraria, ou seja, seu x seja menor que 0, para assim pegarmos o complemento do angulo gerado.
   for(i=0; i<10; i++){
      if(G_Carros[i] == NULL) continue;
      if(G_Carros[i]->direc.x >= 0) anglAux = atan(G_Carros[i]->direc.z / G_Carros[i]->direc.x) * 180 / PI;
      else anglAux = (atan(G_Carros[i]->direc.z / G_Carros[i]->direc.x)*180/PI)-180;
      desenharObjeto(&desenharCarro, G_Carros[i]->pos.x, G_Carros[i]->pos.y, G_Carros[i]->pos.z, -anglAux); //-angl porque opengl gira antihorario
   }

   for(i=0; i<QUANTIDADEARVORES; i++){
   	desenharObjeto(&desenharArvore, G_Arvores[i].pos.x, G_Arvores[i].pos.y, G_Arvores[i].pos.z, G_Arvores[i].angle);
   }
   // glLoadIdentity(); Chamar so se constantemente dar glulookat, ja que glulookat deixa a matriz de modelo alterada para ele
   glutSwapBuffers();
   glutTimerFunc(1000/G_timeScale, atualizarPosicoes, 1); //chama a funcao de animacao a cada 33milisegundos, equivalendo a aproximadamente 30 quadros por segundo
}

void resizeSimulacao(int w, int h){
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fAspect = (GLfloat)w/(GLfloat)h;
   gluPerspective(angle,fAspect,0.4,500); //cria uma perspectiva de visao com angulo angle, fAspect = retangulo da base, znear = 0.4 e zfar = 500
   glRotatef(30, 1, 0, 0);
   glTranslatef(0, -100, -180); //quase equivalente ao lookat abaixo
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   // gluLookAt(0, 100, 180.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //de (x,x,x), olhe para (y,y,y) com vetor cima (z,z,z)
}

void keySimulacao(unsigned char key, int x, int y){
   vector3 aux = {0.0, 0.0, 0.0}; //vetor auxiliar feito para dar uma direcao para os carros criados
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'z': //cria um carro vindo da esquerda
      	aux.x = 150.0;
         criarCarro(aux);
         break;
      case 'a': //cria um carro vindo da direita
         aux.x = -100.0;
         criarCarro(aux);
         // glMatrixMode(GL_PROJECTION); //processo feito pra evitar que a camera gira em volta do centro da simulacao
         // glTranslatef(0, 100, 180); //move pra origem
         // glRotatef(10, 0, 1, 0); //gira a camera
         // glTranslatef(0, -100, -180); //volta pro original         
         // glMatrixMode(GL_MODELVIEW);// isso faz com ela gire em torno do proprio eixo
         break;
      case 'd': //troca entre dia e noite
      	if(G_estadoDia == 1 || G_estadoDia == -1){
      		G_estadoDia *= 2;
      	}
      	break;
   }
}

void specialKeySimulacao(int key, int x, int y){
	vector3 aux = {0,0,0}; //inicializado como 0,0,0 para evitar complicacoes com lixo de memoria
	// vector3 aux2 = {0,0,0};
	switch(key){
		case GLUT_KEY_UP:
			if(G_Carros[0]->veloc < ((G_Velocimetro.max-10)*100)/36){
				G_Carros[0]->veloc += 1;
				G_Velocimetro.ponteiro = calcularAnguloPonteiro(G_Velocimetro.max, G_Carros[0]->veloc);
			}
			break;
		case GLUT_KEY_DOWN:
			if(G_Carros[0]->veloc > (10*100)/36){
				G_Carros[0]->veloc -= 1;
				G_Velocimetro.ponteiro = calcularAnguloPonteiro(G_Velocimetro.max, G_Carros[0]->veloc);
			}
			break;

		case GLUT_KEY_LEFT:
			aux.x = G_Carros[0]->direc.z; //ortogonal para esquerda
			aux.z = -G_Carros[0]->direc.x;
			aux = normalizar(aux);
			aux = escalarVetor(0.1, aux);
			G_Carros[0]->direc = somarVetores(G_Carros[0]->direc, aux);
			G_Carros[0]->direc = normalizar(G_Carros[0]->direc);
			break;
		case GLUT_KEY_RIGHT:
			aux.x = -G_Carros[0]->direc.z; //ortogonal para esquerda
			aux.z = G_Carros[0]->direc.x;
			aux = normalizar(aux);
			aux = escalarVetor(0.1, aux);
			G_Carros[0]->direc = somarVetores(G_Carros[0]->direc, aux);
			G_Carros[0]->direc = normalizar(G_Carros[0]->direc);
			break;
/*
		case GLUT_KEY_F1:
			aux2.x = G_Carros[0]->pos.x;
			aux2.y = G_Carros[0]->pos.y + 5;
			aux2.z = G_Carros[0]->pos.z;
			aux.x = 1.0;
			aux.y = 0.0;
			aux.z = 0.0; 
			moveToAndLookAt(aux, aux2);
			break;
		case GLUT_KEY_F2:
			aux2.x = 0;
			aux2.y = 100;
			aux2.z = 180;
			aux.x = 0.0;
			aux.y = 0.0;
			aux.z = 0.0; 
			moveToAndLookAt(aux, aux2);
			break;
*/
	}
}

/*
void moveToAndLookAt(vector3 a, vector3 b){
	// vector3 aux = computarNormal(G_Camera.look,b);           funcao experimental de transformacao da camera
	vector3 aux = {0,0,-1}; //angulo padrao
	vector3 normal = computarNormal(aux, b);

	normal = normalizar(aux);
	// float anguloAux = anguloEntreVetores(G_Camera.look, b);
	float anguloAux = anguloEntreVetores(aux, b);


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	// glTranslatef(-G_Camera.pos.x, -G_Camera.pos.y, -G_Camera.pos.z);
	glTranslatef(a.x, a.y, a.z);
	glRotatef(anguloAux, normal.x, normal.y, normal.z);
	glMatrixMode(GL_MODELVIEW);
	// G_Camera.pos = a;
	// G_Camera.look = b;
}
*/