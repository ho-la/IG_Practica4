#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include "RgbImage.h"

#define PI 3.141592654
#define NT 3

void initFunc();
void funReshape(int w, int h);
void funDisplay();
void funKeyboard(int key, int x, int y);
void destroyFunc();
void funIdle();
void keyboard(unsigned char key,int x,int y);

void glDrawSphere(char color,float radio,bool luz);
void drawCone();
void P3Tarea1();
void P3Tarea2();
void P3Tarea3();

// Variables globales
int w = 900;
int h = 500;
GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat lunax = 5.5, lunay=0,lunaz = 0.0;
//GLfloat PL0[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat PL0[] = { 0.0f, 0.0f, -10.0f, 1.0f };//ultimo parametro 0=direccionar,1=posicional
GLfloat PL1[] = {3.0f, 2.0f, -6.0f, 1.0f };//Posicion de la luz de la vela
GLfloat IC[]  = { 1.9f, 1.5f, 1.5f, 1.0f };
GLfloat IA[]  = {0.2f, 0.2f, 0.2f, 1.0f };
GLfloat ISol[]  = { 5.0f, 5.0f, 5.0f, 1.0f };
GLfloat PLS[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat PLT[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat PLL[] = { 0.0, 0.0, 0.0, 1.0 };

GLuint textureName[NT];
//Mis variables globales
//Si gira de 6h en 6h
GLfloat anio = (360.0/365.0)/4; //365*24 horas
GLfloat dia = (360.0/4.0); //24 horas
GLfloat mes= anio*12;  // Luna gira 12 veces sobre a Tierra en un año

int Anio = 0; 
int Dia = 0; 
int Mes= 0;  

GLfloat RAnio = 0.0f;
GLfloat RDia = 0.0f;
GLfloat RMes=0.0f;

GLfloat KaS[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdS[] = { 0.9, 0.9, 0.2, 1.0 };
GLfloat KsS[] = { 0.9, 0.9, 0.1, 1.0 };//Brillo amarillo
GLfloat KaL[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdL[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat KsL[] = { 0.9, 0.9, 0.9, 1.0 }; //Brillo Blanco
GLfloat KaT[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdT[] = { 0.2, 0.2, 0.7, 1.0 };
GLfloat KsT[] = { 0.9, 0.9, 0.0, 1.0 }; //Brillo Azul

GLfloat zoom = -10.0f;
GLfloat giroVertical=0.0f;
int iniX;
bool luna_on = false;

GLboolean anima=true;
GLboolean esTarea1;
int tarea=1;
int main(int argc, char** argv) {
    
 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("QiaoLang Chen y Alexey Sorokin");
    
    
 // Inicializamos GLEW
    GLenum err = glewInit();
    if(GLEW_OK != err) printf("Error: %s\n", glewGetErrorString(err));
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    printf("This system supports OpenGL Version: %s\n", oglVersion);
    
 // Inicializaciones específicas
    initFunc();
    
 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(funKeyboard);
    glutIdleFunc(funIdle); 
      
 // Bucle principal
    glutMainLoop();
    
 // Destrucción de objetos
    destroyFunc();
    
    return(0);
}

void initFunc() {
    
 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Modelo de Iluminación
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
        
 // Parámetros de la Luz ambiental global
    //GLfloat IA[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat IA[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    //GLfloat IA[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, IA);
    // Cálculo de la iluminación an ambas caras
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
 // Parámetros de la Luz 0 (direccional=sol)
    GLfloat Ia0[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat Id0[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat Is0[] = { 0.3, 0.3, 0.3, 1.0 }; // Brillo en blanco para identificarla mejor
    glLightfv(GL_LIGHT0, GL_AMBIENT , Ia0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE , Id0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Is0);
    glEnable(GL_LIGHT0);
    
 // Parámetros de la Luz 1 (posicional=bombilla)
    GLfloat Ia1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    //GLfloat Id1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat Is1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT , Ia1);
    if(esTarea1){
        glLightfv(GL_LIGHT1, GL_DIFFUSE , IC);
    }    
    else{
        glLightfv(GL_LIGHT1, GL_DIFFUSE , ISol);
        Is1[1] = 1.0f; // Brillo en amarillo para identificarla mejor
    }    
    glLightfv(GL_LIGHT1, GL_SPECULAR, Is1);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION , 0.90f);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION   , 0.05f);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
    glEnable(GL_LIGHT1);
    
     // Parámetros de la Luz Luna (posicional=bombilla)
    GLfloat Ial[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat Idl[] = { 1.9, 1.9, 1.9, 1.0 };
    GLfloat Isl[] = { 1.0, 1.0, 1.0, 1.0 }; // Brillo en blanco para identificarla mejor
    glLightfv(GL_LIGHT3, GL_AMBIENT , Ial);
    glLightfv(GL_LIGHT3, GL_DIFFUSE , Idl);
    glLightfv(GL_LIGHT3, GL_SPECULAR, Isl);
    glLightf (GL_LIGHT3, GL_CONSTANT_ATTENUATION , 0.90);
    glLightf (GL_LIGHT3, GL_LINEAR_ATTENUATION   , 0.05);
    glLightf (GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.01);
    
 // Modelo de Sombreado
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
 // Texturas
    glEnable(GL_TEXTURE_2D);
    glGenTextures(NT,textureName);
    const char *filename[NT];
    if(tarea==1){
        filename[0] = "common/img/imgChess.bmp";
        filename[1] = "common/img/imgLadrillo.bmp";
        filename[2] = "common/img/imgMarmol.bmp";
    } 
    else{
        filename[0] = "common/img/imgSun.bmp";
        filename[1] = "common/img/imgEarth.bmp";
        filename[2] = "common/img/imgMoon.bmp";
    }
    for(unsigned i=0; i<NT; i++) {
        
     // Seleccionamos la textura a configurar
	glBindTexture(GL_TEXTURE_2D, textureName[i]);
		
     // Cargamos la textura (RgbImage)
        RgbImage texture(filename[i]);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.GetNumCols(), texture.GetNumRows(), GL_RGB, GL_UNSIGNED_BYTE, texture.ImageData());
    
     // Configuramos la textura
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    }
    
}

void destroyFunc() {
    
    glDeleteTextures(NT,textureName);
    
}

void funReshape(int wnew, int hnew) {
    
 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Captura de w y h
    w = wnew;
    h = hnew;
    
}

void funDisplay() {
    
 // Borramos el buffer de color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 // Para configurar la matriz matriz P
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
 // Matriz de Proyección P (Cámara): Perspectiva (gluPerspective)
    GLfloat fovy = 50.0f, aspectRatio = (GLfloat)w/(GLfloat)h, nplane = 0.1f, fplane = 30.0f;
    //GLfloat fovy = 75.0f, aspectRatio = (GLfloat)w/(GLfloat)h, nplane = 0.1f, fplane = 25.0f;
    gluPerspective(fovy,aspectRatio,nplane,fplane);
      
 // Para configurar las matrices M y V
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();
    
 // Matriz de Vista V (Cámara)
    
    if(esTarea1){
        GLfloat eye[3]    = {0.0f,  3.0f,  5.0f};
        GLfloat center[3] = {0.0f,  2.0f, -5.0f};
        GLfloat up[3]     = {0.0f,  1.0f,  0.0f};
        gluLookAt(    eye[0],    eye[1],    eye[2],
                   center[0], center[1], center[2],
                       up[0],     up[1],     up[2]);
    } 
    else{
        glTranslatef(0.0f, 0.0f, -10.0f);
    }
 // Dibujamos la escena(M)
    switch (tarea){
        case 1: P3Tarea1();
            break;
        case 2: P3Tarea2();
            break;
        default: P3Tarea3();
    }
    
 // Intercambiamos los buffers
    glutSwapBuffers();
    
}

void drawLights1() {
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE , IC);
    glEnable(GL_LIGHT1);
 // Definimos el material de las esferas que representan las luces mediante glColor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
 
    glPushMatrix();
        glTranslatef(PL1[0],PL1[1],PL1[2]);
        glutSolidSphere(0.02,10,10);
    glPopMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, PL1);

    glDisable(GL_COLOR_MATERIAL);
}

void drawLights2(){
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE , ISol);
    glEnable(GL_LIGHT1);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glLightfv(GL_LIGHT1, GL_POSITION, PLS);
    
    glEnable(GL_LIGHT2);
    if (luna_on)
        glEnable(GL_LIGHT3);
    else
        glDisable(GL_LIGHT3);
    glEnable(GL_COLOR_MATERIAL);
    
    glLightfv(GL_LIGHT2, GL_POSITION, PLT);
    PLL[0]=lunax;
    PLL[1]=lunay;
    PLL[2]=lunaz;
    glLightfv(GL_LIGHT3, GL_POSITION, PLL);

    glDisable(GL_COLOR_MATERIAL);
    
}
void drawRoom() {
    
 // Definimos el material de la habitación
    
    GLfloat Ka[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat Kd[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat Ks[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT  , Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE  , Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR , Ks);
    glMaterialf (GL_FRONT, GL_SHININESS, 50.0f);
    
// Definimos  la habitación
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(0.0f,2.5f,-10.f);
        glScalef(5.0f,2.5f,10.0f);
        
     // SUELO (y = -1)
        glBindTexture(GL_TEXTURE_2D, textureName[0]);
        glBegin(GL_QUADS);
            glNormal3f( 0.0f,  1.0f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
            glTexCoord2f(2.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
            glTexCoord2f(2.0f, 4.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
            glTexCoord2f(0.0f, 4.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glEnd();

     // PARED IZQUIERDA (x = -1)
        glBindTexture(GL_TEXTURE_2D, textureName[1]);
        glBegin(GL_QUADS);
            glNormal3f( 1.0f,  0.0f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
            glTexCoord2f(8.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);   
            glTexCoord2f(8.0f, 2.0f); glVertex3f(-1.0f,  1.0f, -1.0f);    
            glTexCoord2f(0.0f, 2.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
        glEnd();

     // PARED DEL FONDO (z = -1)
        glBindTexture(GL_TEXTURE_2D, textureName[1]);
        glBegin(GL_QUADS);
            glNormal3f( 0.0f,  0.0f,  1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -0.5f);
            glTexCoord2f(4.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -0.5f);
            glTexCoord2f(4.0f, 2.0f); glVertex3f( 1.0f,  1.0f, -0.5f);
            glTexCoord2f(0.0f, 2.0f); glVertex3f(-1.0f,  1.0f, -0.5f);
        glEnd();

     // PARED DERECHA (x = 1)
        glBindTexture(GL_TEXTURE_2D, textureName[1]);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f,  0.0f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
            glTexCoord2f(8.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
            glTexCoord2f(8.0f, 2.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
            glTexCoord2f(0.0f, 2.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glEnd();

     // TECHO (y = 1)
        glBindTexture(GL_TEXTURE_2D, textureName[2]);
        glBegin(GL_QUADS);
            glNormal3f( 0.0f, -1.0f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  
            glTexCoord2f(4.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
            glTexCoord2f(4.0f, 8.0f); glVertex3f( 1.0f,  1.0f,  1.0f);    
            glTexCoord2f(0.0f, 8.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glEnd();
    
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
}

void drawObject(GLfloat s, GLint c) {
    
 // Definimos el material del objeto
    GLfloat Ka[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat Kd[] = { 0.7f, 0.7f, 0.3f, 1.0f };
    GLfloat Ks[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT  , Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE  , Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR , Ks);
    glMaterialf (GL_FRONT, GL_SHININESS, 50.0f);
    
 // Definimos el objeto
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, textureName[2]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, -5.0f);
        glRotatef(rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        //glutSolidTeapot(s);
        glutSolidSphere(1.0,200,200);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_2D);
    
}

void keyboard(unsigned char key,int x,int y){
    switch(key){
        case '+':
            if(esTarea1){
               IC[0]+=0.2;
               IC[1]+=0.2;
               IC[2]+=0.2;
            }
            else{
               IA[0]+=0.2;
               IA[1]+=0.2;
               IA[2]+=0.2;
               glLightModelfv(GL_LIGHT_MODEL_AMBIENT, IA);
            }   
            break;
               
        case '-' :
            if(esTarea1){
                IC[0]-=0.2;
                IC[1]-=0.2;
                IC[2]-=0.2;
            }
            else{
                IA[0]-=0.2;
                IA[1]-=0.2;
                IA[2]-=0.2;
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, IA);
            } 
            break;
        case 'e' :
           luna_on= !luna_on;
           break;
    }
    glutPostRedisplay();
}

void funKeyboard(int key, int x, int y) {
    if(esTarea1){
        switch(key) {
           case GLUT_KEY_UP:
               if(PL1[2]>-13)
                    PL1[2] -= 0.5f;
               break;
           case GLUT_KEY_DOWN:
               if(PL1[2]<-0.5)
                    PL1[2] += 0.5f;
               break;
           case GLUT_KEY_LEFT:
               if(PL1[0]>-4)
                    PL1[0] -= 0.5f;
               break;
           case GLUT_KEY_RIGHT:
               if(PL1[0]<4)
                    PL1[0] += 0.5f;
               break;
              
        } 
    }
    else{
        if(anima==true)
            switch(key) {
                case GLUT_KEY_F1:
                    anima = false;
                    break;
        }    
        else{
            switch(key){
                case GLUT_KEY_F1:
                    anima = true;
                    break;
                case GLUT_KEY_RIGHT:
                    //rotY -= 5.0f;
                    Anio++;
                    Dia+=365;
                    Mes+=12;
                    break;
                case GLUT_KEY_LEFT:
                    //rotY += 5.0f;
                    Anio--;
                    Dia-=365;
                    Mes-=12;
                    break;
            }
        }    
    }
    glutPostRedisplay();    
}

void funIdle() {
    /*if(anima){
        RAnio += 0;//anio;
        RDia += dia;
        RMes += mes;
    }
     * */ 
    if (anima) {
        Anio++;
        Dia+=365;
        Mes+=12;
    }
    Sleep(50);   
    glutPostRedisplay();  
}

void glDrawSphere(char color,float radio, bool luz){
    switch(color){
        case 'y':   
                    glMaterialfv(GL_FRONT, GL_AMBIENT  , KaS);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE  , KdS);
                    glMaterialfv(GL_FRONT, GL_SPECULAR , KsS);
                    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
                    if(tarea==3){
                        glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
                        glBindTexture(GL_TEXTURE_2D, textureName[0]);
                        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glEnable(GL_TEXTURE_GEN_S);
                        glEnable(GL_TEXTURE_GEN_T);
                    }
                        glutSolidSphere(2.0,50,50);
                    if(tarea==3){    
                        glDisable(GL_TEXTURE_GEN_T);
                        glDisable(GL_TEXTURE_GEN_S);
                    }
                    break;
        case 'w':  
                    glMaterialfv(GL_FRONT, GL_AMBIENT  , KaL);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE  , KdL);
                    glMaterialfv(GL_FRONT, GL_SPECULAR , KsL);
                    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
                    if(tarea==3){
                        glBindTexture(GL_TEXTURE_2D, textureName[2]);
                        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glEnable(GL_TEXTURE_GEN_S);
                        glEnable(GL_TEXTURE_GEN_T);
                    }    
                        glutSolidSphere(0.15,15,15);
                    if(tarea==3){    
                        glDisable(GL_TEXTURE_GEN_T);
                        glDisable(GL_TEXTURE_GEN_S);
                    }
                    break;
        case 'b':   
                    glMaterialfv(GL_FRONT, GL_AMBIENT  , KaT);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE  , KdT);
                    glMaterialfv(GL_FRONT, GL_SPECULAR , KsT);
                    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
                    if(tarea==3){
                        glBindTexture(GL_TEXTURE_2D, textureName[1]);
                        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                        glEnable(GL_TEXTURE_GEN_S);
                        glEnable(GL_TEXTURE_GEN_T);
                    } 
                    glutSolidSphere(0.75,30,30);
                    if(tarea==3){ 
                        glDisable(GL_TEXTURE_GEN_T);
                        glDisable(GL_TEXTURE_GEN_S);
                    }
                    break;
    }
}

void drawCono(){
     // Definimos el material del Objeto
    GLfloat Ka[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat Kd[] = { 0.5, 0.5, 0.2, 1.0 };
    GLfloat Ks[] = { 0.9, 0.9, 0.9, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT  , Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE  , Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR , Ks);
    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
    
 // Definimos el Objeto
    glPushMatrix();
        glTranslatef(PL1[0], PL1[1]-2,PL1[2]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2,2.0,10,10);
    glPopMatrix();
}

void P3Tarea1() {
    esTarea1=true;
    drawLights1();
    drawRoom();
    drawCono();
    drawObject(1.0f,100);
}
void P3Tarea2() {
        /*
        1. Colocar una tenue luz ambiental para iluminar la imagen globalmente (esta luz ambiental se puede
            aumentar con la tecla + y disminuir con la tecla -).
        2. Colocar una fuente de luz intensa en el sol.
        3. Añadir un tono amarillo al Sol, azul a la Tierra y blanco a la Luna.
        4. Transformar la luna en una nueva estrella haciendo que se encienda y apague con la tecla E.
        */
    drawLights2();
    glPushMatrix();
        //Dibujar sol
        glPushMatrix();
            //glColor3f(1.0, 1.0, 0.0);
            glRotatef(90,1.0f,0.0f,0.0f);
            glDrawSphere('y',2.0f,true);
        glPopMatrix();
        //Dibujar tierra
        //glRotatef(RAnio,0.0f,1.0f,0.0f);
        glRotatef(Anio%360,0.0,1.0,0.0);
        glTranslatef(4.0f,0.0f,0.0f);
        //glRotatef(RDia,0.0f,1.0f,0.0f);
        glRotatef(Dia%360,0.0,1.0,0.0);
        glPushMatrix();
            glColor3f(0.0, 0.0, 1.0);
            glRotatef(90,1.0f,0.0f,0.0f);
            glDrawSphere('b',0.5f,false);
        glPopMatrix();
        //glRotatef(-RDia,0.0f,1.0f,0.0f);
        glRotatef(-Dia%360,0.0,1.0,0.0);
        //Dibujar luna
        //glRotatef(RMes,0.0f,1.0f,0.0f);
        glRotatef(Mes%360,0.0f,1.0f,0.0f);
        glTranslatef(1.5,0.0,0.0);
        glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glRotatef(90,1.0f,0.0f,0.0f);
            glDrawSphere('w',0.1f,true);
        glPopMatrix();
        //cos el x, sin el z
        //lunax=1.5*cos(RMes);
        //lunaz=1.5*sin(RMes);
        lunax = 4.0*cos((Anio%360)*PI/180)+1.5*cos((Anio%360+Mes%360)*PI/180);
        lunaz = -(4.0*sin((Anio%360)*PI/180)+1.5*sin((Anio%360+Mes%360)*PI/180));
        //incrementar variables
    glPopMatrix();
    esTarea1=false;
}
void P3Tarea3() {
    /*añadir las texturas del Sol, Tierra y Luna*/
    P3Tarea2();
}