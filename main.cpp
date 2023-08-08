//Tema: Manejo de una curva del tipo B-Splin(CUBICA)

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include<conio.h>
#include <math.h>
#include <iostream>

using namespace std;

//Prototipo de funciones----------
void init(void);
void display(void);
void reshape(int,int);

void SplineB(int);
void dibujarEjes(void);
void puntosControl(void);
void polilinea(void);
void graficaCurva(void);

void asignaPuntos(float, float);
int seleccion(int,int);
bool encon(int,int,int);

void mouse(int,int,int,int);
void motion(int,int);

//----------------------------------
float Puntos[100][3];//matriz de puntos de control
int numPuntos=100;    //numero maximo de puntos de control
//----------------------------------

//Variables globales
float dim=500; //Dimension de la pantalla
int PUNTO;     //numero del punto de control
float tam_Punto=0.5f; //tamaño del punto de control
bool click = false;//booleano que sirve para determinar si se esta haciendo click o no
int N; //numero actual de puntos de control
int numCurvas;//numero de curvas a dibujar
//----------------------------------
//Banderas para los menu
/*
    0: Desactivado
    1 :Activado
*/

GLint ingresa = 0;
GLint linea = 0;
GLint curva = 0;
GLint mover = 0;
GLint eje = 0;
//----------------------------------
//FUNCIONES
//Submenus
void menu_ingresa_puntos(int opcion){
 switch(opcion){
 case 1 : ingresa = 1; break;
 case 2 : ingresa = 0; break;
 }
 glutPostRedisplay();
}

void menu_polilinea(int opcion){
 switch(opcion){
 case 1 : linea = 1; break;
 case 2 : linea = 0; break;
 }
 glutPostRedisplay();
}

void menu_curva(int opcion){
 switch(opcion){
 case 1 : curva = 1; break;
 case 2 : curva = 0; break;
 }
 glutPostRedisplay();
}

void menu_mover_puntos(int opcion){
 switch(opcion){
 case 1 : mover = 1; break;
 case 2 : mover = 0; break;
 }
 glutPostRedisplay();
}

void menu_eje(int opcion){
 switch(opcion){
 case 1 : eje = 1; break;
 case 2 : eje = 0; break;
 }
 glutPostRedisplay();
}

//Menu principal
void menu_principal(int opcion)
{
switch(opcion){
case 1: menu_ingresa_puntos(1);break;
case 2: menu_polilinea(1); break;
case 3: menu_curva(1);break;
case 4: menu_mover_puntos(1);break;
case 5: menu_eje(1);break;
case 6: exit(-1);
}
glutPostRedisplay();
}

//Funcion main
int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowSize(dim,dim);
glutInitWindowPosition(100,100);
glutCreateWindow("Moore Salazar_PC3: Curva B-Spline Cubica");
//Menu Jerarquico
 int submenu_ingresar_puntos = glutCreateMenu(menu_ingresa_puntos);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_polilinea = glutCreateMenu(menu_polilinea);
  glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_curva = glutCreateMenu(menu_curva);
  glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_mover_puntos = glutCreateMenu(menu_mover_puntos);
  glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_ejes = glutCreateMenu(menu_eje);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int menu=glutCreateMenu(menu_principal);
 glutAddSubMenu("Ingresar puntos de control",submenu_ingresar_puntos);
 glutAddSubMenu("Mostrar polilínea",submenu_polilinea);
 glutAddSubMenu("Mostrar Curva B-Spline",submenu_curva);
 glutAddSubMenu("Mover Puntos de control ",submenu_mover_puntos);
 glutAddSubMenu("Mostrar Ejes coordenados",submenu_ejes);
 glutAddMenuEntry("Salir",6);
 glutAttachMenu(GLUT_RIGHT_BUTTON);
init();
glutDisplayFunc(display);
glutReshapeFunc(reshape);
//----------
glutMouseFunc( mouse );
glutMotionFunc( motion );

glutMainLoop();
return 0;
}

//Dibuja los ejes de coordenadas(cada cuadrante 20x20)
void dibujarEjes(void)
{
    if(eje==1){
        glBegin(GL_LINES);
        glVertex2f(-20,0);
        glVertex2f(20,0);
        glVertex2f(0,-20);
        glVertex2f(0,20);
        glEnd();

    }
}

//Funcion init
void init(void)
{
    glClearColor(0.0,0.5,0.5,.0);
    glEnable(GL_MAP1_VERTEX_3);
    glShadeModel(GL_FLAT);
}

//Funcion display(Escena)
void display(void)
{
glClear(GL_COLOR_BUFFER_BIT);

glPushMatrix();
glColor3f(1.0,0.0,0.0);

dibujarEjes();//dibuja el eje de coordenadas
glPointSize(5);
glColor3f(1.0,0.0,0.0);
graficaCurva();//dibuja la curva
glColor3f(0.0,1.0,0.0);
puntosControl();//dibuja los puntos de control
glColor3f(0.0,1.0,1.0);
polilinea();//dibuja la polilinea

glPopMatrix();
glFlush();

}

//control de la ventana(cada cuadrante del eje de coordenadas es de 20 x 20)
void reshape(int w, int h)
{
glViewport(0,0,(GLsizei)w, (GLsizei)h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-20.0,20.0,-20.0,20,-1.0,1.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}

//dibuja los cuadrilateros que funcionan como puntos de control
void dibujaPunto( float x, float y, float size )
{

        glPushMatrix();
        glTranslatef( x, y, 0.0f );
        glScalef( size, size, 1.0f );
        glBegin( GL_QUADS );
        glColor3f(1,0,1);
        glVertex2f( -1, -1 );
        glVertex2f(  1, -1 );
        glVertex2f(  1,  1 );
        glVertex2f( -1,  1 );
        glEnd();
        glPopMatrix();

}

//dibuja una curva B-Spline cubica a partir de 4 puntos de control
void SplineB(int n){
    glColor3f(1,1,1);
    glBegin(GL_LINE_STRIP);

    //u varia de 0 a 1
    for(float u=0.0;u<=1;u+=0.01) {

		float un = 1.0f-u;

		// calculamos las funciones
		float f0 = un*un*un/6.0f;
		float f1 = (3*u*u*u - 6*u*u +4)/6.0f;
		float f2 = (-3*u*u*u +3*u*u + 3*u + 1)/6.0f;
		float f3 =  u*u*u/6.0f;

		//Asignamos a cada coordenada su valor correspondiente de Qi
		float x = f0*Puntos[n][0] +
				  f1*Puntos[n+1][0] +
				  f2*Puntos[n+2][0] +
				  f3*Puntos[n+3][0] ;

		float y = f0*Puntos[n][1] +
				  f1*Puntos[n+1][1] +
				  f2*Puntos[n+2][1] +
				  f3*Puntos[n+3][1] ;

		float z = f0*Puntos[n][2] +
				  f1*Puntos[n+1][2] +
				  f2*Puntos[n+2][2] +
				  f3*Puntos[n+3][2] ;

		glVertex3f( x,y,z );
	}

	glEnd();

}

//grafica N-3 curvas(cada una a partir de 4 puntos de control)
void graficaCurva(){
    if(curva==1){
        int c=0;
        while(c<numCurvas){
        SplineB(c);
        c++;
        };
    }
}

//dibuja todos los puntos de control de la matriz de puntos
void puntosControl(){
    glColor3f(1,1,0);
	for(int i=0;i<N;++i) {
		dibujaPunto(Puntos[i][0],Puntos[i][1], tam_Punto);
	}
}

//Manejo del mouse
void mouse( int button, int state, int x, int y )
{
    float nuevoX=(x-250)/12.5,nuevoY=(-(y-250))/12.5;//corrige la coordenada obtenida
    PUNTO=seleccion(nuevoX,nuevoY);

    //Logica para mover puntos de control(adaptado de genpfault_stackOverflow: https://stackoverflow.com/questions/26612428/moving-shape-via-mouse-in-opengl)
        if( (button == GLUT_LEFT_BUTTON)&&(GLUT_DOWN == state)&&(mover==1)&&(PUNTO!=-1) )
    {
            click = true;
            Puntos[seleccion(nuevoX,nuevoY)][0] = nuevoX;
            Puntos[seleccion(nuevoX,nuevoY)][1] = nuevoY;
            glutPostRedisplay();

    }
    else
    {
        click = false;
    };

    //Logica para dibujar puntos de control
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)&& (ingresa==1)&&(PUNTO==-1)){

        if(N<numPuntos){
            dibujaPunto(nuevoX,nuevoY,tam_Punto);
            asignaPuntos(nuevoX,nuevoY);
            polilinea();
            graficaCurva();
        }

    }
    glEnd();
    glFlush();

/*
    //mostrar matriz
    int filas = (sizeof(Points)/sizeof(Points[0]));
    int columnas = (sizeof(Points[0])/sizeof(Points[0][0]));
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cout<<Points[i][j]<<endl;
        }
    }
    cout<<"----------------"<<endl;
*/
}

//Funcion que devuelve el indice del punto de control
int seleccion(int x, int y){
    int p=0;
    int numeroP=-1; //numero del punto del punto de control

    for(p=0;p<N;p++){
        if(encon(p,x,y)){
            numeroP=p;
        }
    }
    return numeroP;
}

//booleano que determina si en dichas coordenadas hay un punto de control
bool encon(int p,int x,int y){

    return Puntos[p][0] - 2*tam_Punto <= x && x <= Puntos[p][0] + 2*tam_Punto
            &&
            Puntos[p][1] - 2*tam_Punto <= y && y <= Puntos[p][1] + 2*tam_Punto
            ;
}

//funcion que reasigna las coordenadas de un punto de control(funcion que se activa al dar click con el mouse)
void motion( int x, int y )
{
    float nuevoX=(x-250)/12.5,nuevoY=(-(y-250))/12.5;
        if( click )
    {
        Puntos[PUNTO][0]= nuevoX;
        Puntos[PUNTO][1] = nuevoY;

        glutPostRedisplay();
    }
}

//Agregar un nuevo punto de control a la matriz(max. 50)
void asignaPuntos(float x, float y){

        Puntos[N][0]=x;
        Puntos[N][1]=y;
        N++;
        numCurvas=N-3;//asigna las curvas a dibujar(N-3)
        //puntosControl();

}

//Dibuja la polilinea a partir de los puntos de control almacenados en la matriz de puntos
void polilinea(){
    if(linea==1){
        glColor3f(0,0,1);
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<N;++i) {
		glVertex3fv( Puntos[i] );
	}
	glEnd();
    }
}
