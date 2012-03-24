//
//  TFVisualizer.h
//  TFShader
//
//  Created by Jorge Herrera on 3/20/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#ifndef TFShader_TFVisualizer_h
#define TFShader_TFVisualizer_h

#include "ShaderManager.h"

#define TF_VERTEX_SHADER    "tf.vert"
#define TF_FRAGMENT_SHADER  "tf.frag"
#define TF_GEOMETRY_SHADER  "tf.geom"



// Location/Normals
#define X_POS 0
#define Y_POS 1
#define Z_POS 2
// Texture Coordinates
#define U_POS 0
#define V_POS 1
// Colours
#define R_POS 0
#define G_POS 1
#define B_POS 2
#define A_POS 3


typedef struct
{
    GLfloat location;
} Basic1DVertex;

typedef struct
{
    GLfloat location[3];
    GLubyte padding[20]; // Pads the struct out to 32 bytes for performance increase
} Basic3DVertex;

typedef struct
{
    GLfloat location[3];
    GLfloat color[4];
    GLubyte padding[4]; // Pads the struct out to 32 bytes for performance increase
} ColoredVertex;

typedef struct
{
    GLfloat location[3];
    GLfloat color[4];
    GLfloat tex[2];
    GLfloat normal[3];
    GLubyte padding[16]; // Pads the struct out to 64 bytes for performance increase
} FullVertex;




class TFVisualizer {

public:
    TFVisualizer( GLfloat ** mat, 
                 unsigned int nRows, 
                 unsigned int nCols );
    ~TFVisualizer();
    
    void render( float time );
    void enableGeometricShader( bool use );    
    
public:
    GLfloat * getDisplayScales() {return m_displayScales; };
    void setDisplayScales(GLfloat xs, GLfloat ys, GLfloat zs);

    void updateTFmatrix( GLfloat * column, unsigned int length );
    
    
private:
    unsigned int m_nRows, m_nCols;
    unsigned int m_t0Col;
    
    GLuint m_vertexVBO;
    GLuint m_indexVBO;
    
    GLuint m_program;
    
    ShaderManager * m_shaderManager;
    FullVertex * m_vertices;
    
    GLfloat m_displayScales[3];
    
    
    
};

#endif




