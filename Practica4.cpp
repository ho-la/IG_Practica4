
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

void practica4();
void drawPieza(int size);
// Variables globales
int w = 900;
int h = 500;

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
GLfloat KaS[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdS[] = { 0.9, 0.9, 0.2, 1.0 };
GLfloat KsS[] = { 0.9, 0.9, 0.1, 1.0 };//Brillo amarillo
GLfloat KaL[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdL[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat KsL[] = { 0.9, 0.9, 0.9, 1.0 }; //Brillo Blanco
GLfloat KaT[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat KdT[] = { 0.2, 0.2, 0.7, 1.0 };
GLfloat KsT[] = { 0.9, 0.9, 0.0, 1.0 }; //Brillo Azul

GLfloat zoom = 5.0f;
GLboolean dibujar[]={false,false,false,false};
GLboolean puesto[]={false,false,false,false};
GLfloat x1=0.0f,x2=0.0f,x3=0.0f,x4=0.0f;
GLfloat ty1=0.0f,ty2=0.0f,ty3=0.0f,ty4=0.0f,z1=0.0f,z2=0.0f,z3=0.0f,z4=0.0f;
GLfloat pos1[]={x1,ty1,z1};
GLfloat pos2[]={x2,ty2,z2};
GLfloat pos3[]={x3,ty3,z3};
GLfloat pos4[]={x4,ty4,z4};
GLfloat rx1=0.0f,rx2=0.0f,rx3=0.0f,rx4=0.0f,ry1=0.0f,ry2=0.0f,ry3=0.0f,ry4=0.0f,rz1=0.0f,rz2=0.0f,rz3=0.0f,rz4=0.0f;
GLfloat rot1[]={rx1,ry1,rz1};
GLfloat rot2[]={rx2,ry2,rz2};
GLfloat rot3[]={rx3,ry3,rz3};
GLfloat rot4[]={rx4,ry4,rz4};
GLboolean anima=true;
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
    glLightfv(GL_LIGHT1, GL_DIFFUSE , IC);
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
   
    filename[0] = "common/img/imgChess.bmp";
    filename[1] = "common/img/imgLadrillo.bmp";
    filename[2] = "common/img/imgMarmol.bmp";

    /* 
    else{
        filename[0] = "common/img/imgSun.bmp";
        filename[1] = "common/img/imgEarth.bmp";
        filename[2] = "common/img/imgMoon.bmp";
    }
    */
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
        GLfloat eye[3]    = {0.0f,  3.0f,  5.0f};
        GLfloat center[3] = {0.0f,  2.0f, -5.0f};
        GLfloat up[3]     = {0.0f,  1.0f,  0.0f};
        gluLookAt(    eye[0],    eye[1],    eye[2],
                   center[0], center[1], center[2],
                       up[0],     up[1],     up[2]);
    //glTranslatef(0.0f, 0.0f, zoom);
 // Dibujamos la escena(M)
    practica4();
    
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

void keyboard(unsigned char key,int x,int y){
    switch( key){
        case '1':
            if(!puesto[0]){
                puesto[0]=true;
                dibujar[0]=true;
                dibujar[1]=false;
                dibujar[2]=false;
                dibujar[3]=false;
            }
            break;
        case '2':
            if(!puesto[1]){
                puesto[1]=true;
                dibujar[0]=false;
                dibujar[1]=true;
                dibujar[2]=false;
                dibujar[3]=false;
            }
            break;
        case '3':
            if(!puesto[2]){
                puesto[2]=true;
                dibujar[0]=false;
                dibujar[1]=false;
                dibujar[2]=true;
                dibujar[3]=false;
            }
            break;
        case '4':
            if(!puesto[3]){
                puesto[3]=true;
                dibujar[0]=false;
                dibujar[1]=false;
                dibujar[2]=false;
                dibujar[3]=true;
            }
            break; 
        case 'f':
            dibujar[0]=false;
            dibujar[1]=false;
            dibujar[2]=false;
            dibujar[3]=false;
            break;    
    }
                
    glutPostRedisplay();
}

void funKeyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            if(dibujar[0]){
                z1--;
            }
            if(dibujar[1]){
                z2--;
            }
            if(dibujar[2]){
                z3--;
            }
            if(dibujar[3]){
                z4--;
            }
            if(PL1[2]>-13)
                 PL1[2] -= 0.5f;
            break;
        case GLUT_KEY_DOWN:
            if(dibujar[0]){
                z1++;
            }
            if(dibujar[1]){
                z2++;
            }
            if(dibujar[2]){
                z3++;
            }
            if(dibujar[3]){
                z4++;
            }
            if(PL1[2]<-0.5)
                 PL1[2] += 0.5f;
            break;
        case GLUT_KEY_LEFT:
            if(dibujar[0]){
                x1--;
            }
            if(dibujar[1]){
                x2--;
            }
            if(dibujar[2]){
                x3--;
            }
            if(dibujar[3]){
                x4--;
            }
            if(PL1[0]>-4)
                 PL1[0] -= 0.5f;
            break;
        case GLUT_KEY_RIGHT:
            if(dibujar[0]){
                x1++;
            }if(dibujar[1]){
                x2++;
            }
            if(dibujar[2]){
                x3++;
            }
            if(dibujar[3]){
                x4++;
            }
            if(PL1[0]<4)
                 PL1[0] += 0.5f;
            break;
    } 
    glutPostRedisplay();    
}

void funIdle() { 
    glutPostRedisplay();  
}

void practica4(){
    //drawRoom();
    glPushMatrix();
    if(puesto[3]){
        glTranslatef(x4,ty4,z4);
        glRotatef(rz4, 0.0, 0.0, 1.0);
        glRotatef(ry4, 0.0, 1.0, 0.0);
        glRotatef(rx4, 1.0, 0.0, 0.0);
        drawPieza(4);
    }
    glPopMatrix();
    glPushMatrix();
    if(puesto[2]){
        glTranslatef(x3,ty3,z3);
        glRotatef(rz3, 0.0, 0.0, 1.0);
        glRotatef(ry3, 0.0, 1.0, 0.0);
        glRotatef(rx3, 1.0, 0.0, 0.0);
        drawPieza(3);
    }    
    glPopMatrix();
    glPushMatrix();
    if(puesto[1]){
        glTranslatef(x2,ty2,z2);
        glRotatef(rz2, 0.0, 0.0, 1.0);
        glRotatef(ry2, 0.0, 1.0, 0.0);
        glRotatef(rx2, 1.0, 0.0, 0.0);
        drawPieza(2);
    }
    glPopMatrix();
    glPushMatrix();
    if(puesto[0]){
        glTranslatef(x1,ty1,z1);
        glRotatef(rz1, 0.0, 0.0, 1.0);
        glRotatef(ry1, 0.0, 1.0, 0.0);
        glRotatef(rx1, 1.0, 0.0, 0.0);
        drawPieza(1);
    }
    glPopMatrix();
}
void drawPieza(int size){
    // LADO FRONTAL: lado multicolor
    glBegin(GL_POLYGON);
        glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 es rojo
        glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 es verde
        glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 es azul
        glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 es morado
    glEnd();
    //LADO TRASERO: lado blanco
    glBegin(GL_POLYGON);
        glColor3f(   1.0,  1.0, 1.0 );
        glVertex3f(  0.5, -0.5, 0.5 );
        glVertex3f(  0.5,  0.5, 0.5 );
        glVertex3f( -0.5,  0.5, 0.5 );
        glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();
    // LADO DERECHO: lado morado
    glBegin(GL_POLYGON);
        glColor3f(  1.0,  0.0,  1.0 );
        glVertex3f( 0.5, -0.5, -0.5 );
        glVertex3f( 0.5,  0.5, -0.5 );
        glVertex3f( 0.5,  0.5,  0.5 );
        glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();
    // LADO IZQUIERDO: lado verde
    glBegin(GL_POLYGON);
        glColor3f(   0.0,  1.0,  0.0 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
    // LADO SUPERIOR: lado azul
    glBegin(GL_POLYGON);
        glColor3f(   0.0,  0.0,  1.0 );
        glVertex3f(  0.5,  0.5,  0.5 );
        glVertex3f(  0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();
    // LADO INFERIOR: lado rojo
    glBegin(GL_POLYGON);
        glColor3f(   1.0,  0.0,  0.0 );
        glVertex3f(  0.5, -0.5, -0.5 );
        glVertex3f(  0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
}