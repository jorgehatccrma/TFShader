//
//  TFVisualizer.cpp
//  TFShader
//
//  Created by Jorge Herrera on 3/20/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#include <iostream>
#include "TFVisualizer.h"


// A helper macro to get a position
#define BUFFER_OFFSET(i) ((char *)NULL + (i))



TFVisualizer::TFVisualizer( GLfloat ** mat, 
                           unsigned int nRows, 
                           unsigned int nCols ) : 
                        m_nRows(nRows), 
                        m_nCols(nCols),
                        m_t0Col(0)
{
    // create the VBOs
    m_vertices = new FullVertex[m_nRows * m_nCols];
    // TODO: for now I'll hardcode FellVertex, but I could use C++ unions to 
    // handle different vertex representations gracefully (Basic1DVertex, 
    // Basic3DVertex, ColoredVertex, FullVertex or others
    GLuint * indices = new GLuint[m_nRows * m_nCols];
        
    // TODO: handle row and column major? For now I'll use column major (based 
    // on a classic spectrogram with Frequency as rows and Time as columns)
    for (unsigned int c=0; c<m_nCols; c++) {
        for (unsigned int r=0; r<m_nRows; r++) {

            // vertex array
            m_vertices[c*m_nRows+r].location[X_POS] = 0;
            m_vertices[c*m_nRows+r].location[Y_POS] = mat[r][c];
            m_vertices[c*m_nRows+r].location[Z_POS] = -(GLfloat)r/(m_nRows-1);
            m_vertices[c*m_nRows+r].color[R_POS] = 1.0;
            m_vertices[c*m_nRows+r].color[G_POS] = 1.0;
            m_vertices[c*m_nRows+r].color[B_POS] = 0.0;
            m_vertices[c*m_nRows+r].color[A_POS] = 1.0;
            m_vertices[c*m_nRows+r].tex[U_POS] = 0.0;
            m_vertices[c*m_nRows+r].tex[V_POS] = 0.0;
            m_vertices[c*m_nRows+r].normal[X_POS] = 0.0;
            m_vertices[c*m_nRows+r].normal[Y_POS] = 0.0;
            m_vertices[c*m_nRows+r].normal[Z_POS] = 1.0;

            // index array
            indices[c*m_nRows+r] = c*m_nRows+r;
        }
    }
    
    // initialize to unit scales
    m_displayScales[X_POS] = 1.0;
    m_displayScales[Y_POS] = 1.0;
    m_displayScales[Z_POS] = 1.0;
    
    
    
    // setup the VBOs
    glGenBuffers(1, &m_vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(FullVertex) * (m_nRows*m_nCols), 
                 NULL, 
                 GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 
                    0, 
                    sizeof(FullVertex) * (m_nRows*m_nCols), 
                    m_vertices); // Actually upload the data
    
    // Set the pointers to our data.  Except for the normal value (which always 
    // has a size of 3), we must pass the size of the individual component. ie. 
    // A vertex has 3 points (x, y, z), texture coordinates have 2 (u, v) etc.
    // Basically the arguments are (ignore the first one for the normal 
    // pointer), Size (many components to read), Type (what data type is it), 
    // Stride (how far to move forward - in bytes - per vertex) and Offset 
    // (where in the buffer to start reading the data - in bytes)
    
    // Make sure you put glVertexPointer at the end as there is a lot of work
    // that goes on behind the scenes with it, and if it's set at the start, it 
    // has to do all that work for each gl*Pointer call, rather than once at
    // the end.
    glTexCoordPointer(2, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(28));
    glNormalPointer(GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(36));
    glColorPointer(4, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(12));
    glVertexPointer(3, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(0));
    
    // When we get here, all the vertex data is effectively on the card
    
    // Our Index Buffer, same as above, the variable needs to be accessible 
    // wherever we draw
    glGenBuffers(1, &m_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    // Upload the index array, this can be done the same way as above (with 
    // NULL as the data, then a glBufferSubData call, but doing it all at once 
    // for convenience)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 (m_nRows*m_nCols) * sizeof(GLuint), 
                 indices, 
                 GL_STATIC_DRAW);
    
    // clean up
    delete indices;

}

TFVisualizer::~TFVisualizer()
{
    delete[] m_vertices;
}



void TFVisualizer::render( float time )
{
    GLuint program = m_programs["lines"];
    glUseProgram(program);
    
    GLint vertexTime = glGetUniformLocation(program, "time_s");
    glUniform1f(vertexTime,time);

    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // Resetup our pointers.  This doesn't reinitialise any data, only how we 
    // walk through it
    glTexCoordPointer(2, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(28));
    glNormalPointer(GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(36));
    glColorPointer(4, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(12));
    glVertexPointer(3, GL_FLOAT, sizeof(FullVertex), BUFFER_OFFSET(0));
    
    // Actually do our drawing!
    glPushMatrix();
    GLfloat displayScale = glGetUniformLocation(program, "displayScale");
    glUniform3f(displayScale, 
                m_displayScales[X_POS], 
                m_displayScales[Y_POS], 
                m_displayScales[Z_POS]);
    GLint x_val = glGetUniformLocation(program, "x_val");
    for (unsigned int c = 0; c < m_nCols; ++c)
    {
        unsigned int t0 = (c+m_t0Col) % m_nCols;
        
//        glUniform1f(x_val, (GLfloat)c/(m_nCols-1));
        glUniform1f(x_val, 2.0*c/(m_nCols-1) - 1.0);
        glDrawElements(GL_POINTS, 
                       m_nRows, 
                       GL_UNSIGNED_INT, 
                       BUFFER_OFFSET(t0 * m_nRows * sizeof(GLuint)));
    }
    glPopMatrix();
    
    
    // Disable our client state back to normal drawing
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    glUseProgram(0); // go back to the fixed pipeline
}



void TFVisualizer::setDisplayScales(GLfloat xs, GLfloat ys, GLfloat zs)
{
    m_displayScales[X_POS] = xs;
    m_displayScales[Y_POS] = ys;
    m_displayScales[Z_POS] = zs;
}



void TFVisualizer::updateTFmatrix( GLfloat * column, unsigned int length )
{
    if ( length != m_nRows ) {
        using namespace std;
        cerr << "Rows missmatch: " << m_nRows << " != " << length << endl;
        return;
    }
    
    // TODO: handle row and column major? For now I'll use column major (based 
    // on a classic spectrogram with Frequency as rows and Time as columns)
    for (unsigned int r=0; r<m_nRows; r++) {
        
        // vertex array
        m_vertices[m_t0Col*m_nRows+r].location[Y_POS] = column[r];
    }

    // update the VBO in the graphics card
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 
                    m_t0Col*sizeof(FullVertex) * m_nRows, 
                    sizeof(FullVertex) * m_nRows, 
                    m_vertices+m_t0Col*m_nRows); // Actually upload the data

    // update the "read pointer"
    m_t0Col = ++m_t0Col % m_nCols;    
}




