#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>

//DEFINES--------------------------------------------

#define TAM_BLOCO 100
#define PASSO	10

#define NORTE  0
#define LESTE  1
#define SUL    2
#define OESTE  3


/* ESTRUTURAS------------ */

typedef struct _esf 
{
	GLint  x_pos, z_pos;
	GLint dir;

} ESFERA;

//GLOBALS--------------------------------------------

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat mat_amarelo[] = {0.8, 0.8, 0.1, 1.0};
GLfloat mat_verde[] = { 0.1, 0.6, 0.1, 1.0 };
GLfloat mat_vermelho[] = { 0.7, 0.1, 0.1, 1.0 };
GLfloat light_position[] = { 0.0, 500.0, 0.0, 1.0 };
GLfloat luz_branca[] = {1.0,1.0,1.0,1.0};
GLfloat lmodel_ambient[] = {0.6,0.6,0.6,1.0};


GLfloat jog_x= TAM_BLOCO , jog_z=TAM_BLOCO ;
GLfloat mov_x=PASSO, mov_z=0;
GLint angulo=0;
GLint wire = 0;


ESFERA g_esfera;

/*labirinto */
GLint mapa[25][25] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                       1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
		       1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,
		       1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,
		       0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,
		       0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,
		       0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,
		       1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,
		       1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,
		       1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,
		       1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,
		       1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,
		       1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,
		       1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,
		       1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,
		       1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,
		       1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,
		       1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,
		       1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,
		       1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,
		       1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,
		       1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,
		       1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,
		       1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
		       1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0};

//FUNCOES--------------------------------------------
int pode_mover(float pos_x, float pos_z, float vet_x, float vet_z)
{
	float mundo_x = pos_x + vet_x ;
	float mundo_z = pos_z + vet_z ;

	int ind_x = (int) ((mundo_x + TAM_BLOCO/2) / TAM_BLOCO);
	int ind_z = (int) ((mundo_z + TAM_BLOCO/2) / TAM_BLOCO);

	if(mapa[ind_x][ind_z]) return 0;
	else return 1;
}


//---------------------------------------------------------------
unsigned GetTickCount()
{
        struct timeval tv;
        if(gettimeofday(&tv, NULL) != 0)
                return 0;

        return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
//---------------------------------------------------------------
void display(void)
{
	int x, z;
  	int x_mun, z_mun;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// limpa os pixels da tela

	glLoadIdentity();// Carrega a identidade

	gluLookAt(jog_x,25,jog_z, jog_x+mov_x,25,jog_z+mov_z, 0,1,0); // (visão do personagem)

	glPushMatrix(); //

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_verde);

	glBegin ( GL_QUADS);

	 glVertex3f(-10000, -TAM_BLOCO/2, -10000);
 	 glVertex3f(-10000, -TAM_BLOCO/2, 10000);
	 glVertex3f(10000, -TAM_BLOCO/2, 10000);
	 glVertex3f(10000, -TAM_BLOCO/2, -10000);

  	glEnd();

 	glPopMatrix();

 //desenha esfera
 /*glPushMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_vermelho);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glTranslatef(g_esfera.x_pos, 5 ,g_esfera.z_pos);

  glutSolidSphere(20,20,16);
*/
 //glPopMatrix();


  for(x=0; x < 25; x++)
  {
	for(z=0; z < 25; z++)
	{
	  if(mapa[x][z]) //tem um bloco
	  {
  	     x_mun = x * TAM_BLOCO;
		 z_mun = z * TAM_BLOCO;

		 //cubo grande
		glPushMatrix();

		 glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amarelo);
		 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		
		 glTranslatef(x_mun, 5 ,z_mun);

		 if(wire) glutWireCube(TAM_BLOCO);
		 else glutSolidCube(TAM_BLOCO);
			
		glPopMatrix();

	  }//if(mapa[x][z]) 
	}//for
  }//for

 glutSwapBuffers();            
}

//-----------------------------------------------------------------

void Keyboard_Function(unsigned char key, int x, int y)
{
 switch (key) 
 {
  
  case  27: exit(0);break; //ESC -> encerra aplicativo... 
  case 'w':
  case 'W': wire =!wire;
			glutPostRedisplay();
			break;
 }

}

//-----------------------------------------------------------------

void Special_Function(int key, int x, int y)
{
 float rad;
 
 switch (key) 
 {
  case GLUT_KEY_DOWN: 

	if(pode_mover(jog_x, jog_z, - mov_x, -mov_z))
	{
	  jog_x -= mov_x;
	  jog_z -= mov_z;
	}
	break;
	  
	
  case GLUT_KEY_UP: 

	  if(pode_mover(jog_x, jog_z,  mov_x, mov_z))
	  {
	   jog_x += mov_x;
	   jog_z += mov_z;
	  }
	  break;
  
  case GLUT_KEY_LEFT: 
	
	angulo -= 10;

	 if(angulo < 0) angulo +=360;


	rad =  (float) (3.14159 * angulo / 180.0f);

	mov_x =  cos(rad) * PASSO;
	mov_z =  sin(rad) * PASSO;
	break;

  
  case GLUT_KEY_RIGHT: 

	  angulo += 10;

	if(angulo >= 360) angulo -=360;
	 
	 rad =  (float) (3.14159 * angulo / 180.0f);

	mov_x = cos(rad) * PASSO;
	mov_z = sin(rad) * PASSO;

	break;
	 
 }//fim do switch

}

//-----------------------------------------------------------------
void Move_Esfera(void)
{
	GLint mapa_x, mapa_z;
	
	GLint frente, esquerda, direita;
	GLint num_rnd, dir_tmp;

	unsigned int inicio_clock ;
	
	inicio_clock = GetTickCount() ;


	switch(g_esfera.dir)
	{
		case NORTE : g_esfera.z_pos -= PASSO; break;
		case LESTE : g_esfera.x_pos += PASSO; break;
		case SUL   : g_esfera.z_pos += PASSO; break;
		case OESTE : g_esfera.x_pos -= PASSO; break;
	}

	
	if( (g_esfera.x_pos % TAM_BLOCO == 0) &&
		(g_esfera.z_pos % TAM_BLOCO == 0) )
	{
		frente = 0;
		esquerda = 0;
		direita = 0;

		mapa_x = (int) ((g_esfera.x_pos + TAM_BLOCO/2) / TAM_BLOCO);
		mapa_z = (int) ((g_esfera.z_pos + TAM_BLOCO/2) / TAM_BLOCO);
		
		switch(g_esfera.dir )
			{
				case NORTE :  if(mapa[mapa_x][mapa_z - 1] == 0)
							  {
								  frente = 1;
							  }
							  if(mapa[mapa_x + 1][mapa_z] == 0)
							  {
								  direita = 1;
							  }
							  if(mapa[mapa_x - 1][mapa_z] == 0)
							  {
								  esquerda = 1;
							  }

							  break;

				case LESTE :  if(mapa[mapa_x + 1][mapa_z] == 0)
							  {
								  frente = 1;
							  }
							  if(mapa[mapa_x][mapa_z + 1] == 0)
							  {
								  direita = 1;
							  }
							  if(mapa[mapa_x][mapa_z - 1] == 0)
							  {
								  esquerda = 1;
							  }
							  break;

				case SUL   :  if(mapa[mapa_x][mapa_z + 1] == 0)
							  {
								  frente = 1;
							  }
							  if(mapa[mapa_x - 1][mapa_z] == 0)
							  {
								  direita = 1;
							  }
							  if(mapa[mapa_x + 1][mapa_z] == 0)
							  {
								  esquerda = 1;
							  }

							  break;

				case OESTE :  if(mapa[mapa_x - 1][mapa_z] == 0)
							  {
								  frente = 1;
							  }
							  if(mapa[mapa_x][mapa_z - 1] == 0)
							  {
								  direita = 1;
							  }
							  if(mapa[mapa_x][mapa_z + 1] == 0)
							  {
								  esquerda = 1;
							  }
							  break;
			}


		dir_tmp = g_esfera.dir;


		if(frente)
		{
			if(esquerda && direita)
			{
				num_rnd = rand() % 10;
				
				if(num_rnd < 4) g_esfera.dir = dir_tmp;
				else if(num_rnd > 6) g_esfera.dir = (dir_tmp + 1) & 3;
				else g_esfera.dir = (dir_tmp - 1) & 3;
			}
			else 
			if(esquerda) 
			{
				if(!(rand()%2)) g_esfera.dir = dir_tmp ;
				else g_esfera.dir = (dir_tmp - 1) & 3;
			}
			else
			if(direita)
			{
				if(!(rand()%2)) g_esfera.dir = dir_tmp ;
				else g_esfera.dir = (dir_tmp + 1) & 3;
			}
			else  g_esfera.dir = dir_tmp ;
		}
		else
		{
			if(esquerda && direita)
			{
				if(!(rand()%2)) g_esfera.dir = (dir_tmp + 1) & 3;
				else g_esfera.dir = (dir_tmp - 1) & 3;
			}
			else 
			if(esquerda) 
			{
				g_esfera.dir = (dir_tmp - 1) & 3;
			}
			else
			if(direita)
			{
				g_esfera.dir = (dir_tmp + 1) & 3;
			}
			else  g_esfera.dir = (dir_tmp + 2) & 3 ;
		}

	}


	//esperar um pouco nos computadores mais rapidos
	while( (GetTickCount() - inicio_clock) < 25) ; 

 glutPostRedisplay();
}

//-----------------------------------------------------------------

void Inicializa(void)
{

 glShadeModel(GL_SMOOTH);

 glClearColor(0.3, 0.6, 0.8, 0.0);
 
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_branca);
 glLightfv(GL_LIGHT0, GL_SPECULAR, luz_branca);
 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);

 //Especifica sistema de coordenadas de projecao
 glMatrixMode(GL_PROJECTION);
 // Inicializa sistema de coordenadas de projecao
 glLoadIdentity();

 //Especifica a projecao perspectiva
 gluPerspective(90,1,0.1,3000);

 //Especifica sistema de coordenadas do modelo
 glMatrixMode(GL_MODELVIEW);

 // Inicializa sistema de coordenadas de projecao
 glLoadIdentity();

 glEnable(GL_DEPTH_TEST);			 

 // inicializa numeros aleatorios
 srand(GetTickCount());
/*
 //posicao inicial da esfera
g_esfera.x_pos = 3 * TAM_BLOCO;
g_esfera.z_pos = TAM_BLOCO;
g_esfera.dir = LESTE;
*/ 
}

//---------------------------------------------------------------
void free_mem(void)
{
	std::clog << "Exiting...\n";
}

//---------------------------------------------------------------
int main(int argc, char **argv)
{
	std::clog << "Begin...\n";

	glutInit(&argc, argv);
 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 	glutCreateWindow("Labirinto 3D");
 	glutFullScreen();  
 
 	Inicializa();

 	glutDisplayFunc(display);
 	glutKeyboardFunc(Keyboard_Function);    
 	glutSpecialFunc(Special_Function);      
 	glutIdleFunc(Move_Esfera);

 	glutMainLoop();

	return 0;
}

