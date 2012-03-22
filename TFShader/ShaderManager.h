//
//  ShaderManager.h
//  TFShader
//
//  Created by Jorge Herrera on 3/20/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#ifndef TFShader_ShaderManager_h
#define TFShader_ShaderManager_h

#include <GLUT/GLUT.h>

class ShaderManager
{
public:
    ShaderManager( const char * vertexShader, const char * fragmentShader );
    ~ShaderManager();
    
    GLuint getProgram() { return m_program; };
    
    
private:
    std::string getShader(const char* pszFilename);
    void retrieveProgramLog(std::ostream& os, GLuint obj);
    void retrieveShaderLog(std::ostream& os, GLuint obj);
    
private:
    GLuint m_vertex_shader;
	GLuint m_fragment_shader;
    GLuint m_geometry_shader;
    GLuint m_program;

};



#endif