//
//  main.cpp
//  TFShader
//
//  Created by Jorge Herrera on 3/19/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include <time.h>
#include <math.h>

#include "utils.h"
#include "TFVisualizer.h"


#define NUM_ROWS 512
#define NUM_COLS 100


// Forward Declaraions
void idleFunc();
void displayFunc();
void reshapeFunc( int width, int height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );
void initialize();
void changeLookAt();



// globals (don't do this at home, kids!)
GLsizei g_width  = 600;
GLsizei g_height = 480;

TFVisualizer * g_TFvis;

bool g_showTriangle = false;

void initialize()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glFrontFace( GL_CCW );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    // transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*
    // lighting
    glEnable( GL_LIGHTING );
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );
     */
    
    // seed random number generator (just in case)
    srand( (unsigned int)time(NULL) );
    
    
    
    // TF specific stuff
    
    float ** mat = createTFmat( NUM_ROWS, NUM_COLS );
    
    g_TFvis = new TFVisualizer(mat, NUM_ROWS, NUM_COLS);
    
    for (int r=0; r<NUM_ROWS; ++r) delete mat[r];
    delete mat;

    
}


void reshapeFunc( int w, int h )
{
    // save the new window size
    g_width = (GLsizei)w; 
    g_height = (GLsizei)h;
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h ); // map the view port to the client area

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    

    gluPerspective( 60.0, (GLfloat) w / (GLfloat) h, 0.1, 300.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
    // place the "camera"
    changeLookAt();

}


void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'Q':
        case 'q':
            exit(1);
            break;
        case 'T':
        case 't':
            g_showTriangle = !g_showTriangle;
            break;
        case 'X':
        case 'x':
        case 'Y':
        case 'y':
        case 'Z':
        case 'z':
            GLfloat * ds = g_TFvis->getDisplayScales();
            GLfloat factor = 1.2;
            if ( key == 'X' ) {
                g_TFvis->setDisplayScales(ds[0]*factor, ds[1], ds[2]);
            } else if ( key == 'x' ) {
                g_TFvis->setDisplayScales(ds[0]/factor, ds[1], ds[2]);
                
            } else if ( key == 'Y' ) {
                g_TFvis->setDisplayScales(ds[0], ds[1]*factor, ds[2]);
            } else if ( key == 'y' ) {
                g_TFvis->setDisplayScales(ds[0], ds[1]/factor, ds[2]);
                
            } else if ( key == 'Z' ) {
                g_TFvis->setDisplayScales(ds[0], ds[1], ds[2]*factor);
            } else if ( key == 'z' ) {
                g_TFvis->setDisplayScales(ds[0], ds[1], ds[2]/factor);
                
            }
            break;
    }
    
    glutPostRedisplay( );
}



void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // when left mouse button is down, move left
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        // when right mouse button down, move right
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else
    {
    }
    
    glutPostRedisplay( );
}


void specialFunc( int key, int x, int y )
{
    if( key == GLUT_KEY_LEFT)
    {
        std::cout << "Left arrow";
    }
    if( key == GLUT_KEY_RIGHT)
    {
        std::cout << "Right arrow";
    }
    if( key == GLUT_KEY_DOWN)
    {
        std::cout << "Down arrow";
    }
    if( key == GLUT_KEY_UP)
    {
        std::cout << "Up arrow";
    }
    if( key == GLUT_KEY_PAGE_UP)
    {
        std::cout << "PageUp arrow";
    }
    if( key == GLUT_KEY_PAGE_DOWN)
    {
        std::cout << "PageDown arrow";
    }
    
    glutPostRedisplay( );
}


void idleFunc( )
{
    glutPostRedisplay( );    // render the scene
}


void displayFunc( )
{
    // time tracker
    static double initialTime = current_time_in_ms();
    double currentTime = current_time_in_ms();
    float accumTime = (float)(currentTime - initialTime)/1000.0;
    
    static long numFrames = 0;
    numFrames++;
    
    if (numFrames % 10 == 0) {
        GLfloat * newColumn =  new GLfloat[NUM_ROWS];
        for (int r=0; r < NUM_ROWS; r++) {
            newColumn[r] = (float)rand()/RAND_MAX - 0.5;
        }
        g_TFvis->updateTFmatrix(newColumn, NUM_ROWS);
        delete[] newColumn;
    }
    
    
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // set line width
    glLineWidth( 1.0f );
    
    // save the current transformation
    glPushMatrix();
        
    // set the view point
    changeLookAt();

    // Draw something

//    static float angle = 0;
//    glRotatef(angle, 0, 1, 0);
//    angle += 0.01;
    if (g_showTriangle) {
        glPushMatrix();
        glBegin( GL_TRIANGLES );
        glColor4f( 1., 0., 0., 1. );
        glVertex3f( -1, -1, 0 );
        glColor4f( 0., 1., 0., 1. );
        glVertex3f( 0, 1, 0 );
        glColor4f( 0., 0., 1., 1. );
        glVertex3f( 1, -1, 0 );
        glEnd();
        glPopMatrix();
    }

    glRotatef(30, 1, 0, 0);
    g_TFvis->render( accumTime );    // render the TF matrix
    
    // restore state
    glPopMatrix();
    
    
    // flush!
    glFlush( );
    // swap the double buffer
    glutSwapBuffers( );
}


void changeLookAt()
{
    gluLookAt(0, 0, 1,  // eye position
              0, 0, 0,  // look at this point
              0, 1, 0); // "up" vector
}


int main (int argc, char * argv[])
{

    // GLUT stuff    
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( g_width, g_height );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Hello GL" );    
    glutIdleFunc( idleFunc );
    glutDisplayFunc( displayFunc );
    glutReshapeFunc( reshapeFunc );
    glutKeyboardFunc( keyboardFunc );
    glutMouseFunc( mouseFunc );
    glutSpecialFunc( specialFunc );
    
    // our own initialization
    initialize();
    
    
    // let GLUT handle the current thread from here
    glutMainLoop();
    
    return 0;
}

