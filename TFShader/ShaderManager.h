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
    ShaderManager( const char * vertexShader, 
                   const char * fragmentShader,
                   const char * geometryShader );
    ~ShaderManager();
    
    GLuint getProgram() { return m_program; };
    void enableGeometricShader( bool use ) { m_use_geom = use; };
    
private:
    std::string getShader(const char* pszFilename);
    void retrieveProgramLog(std::ostream& os, GLuint obj);
    void retrieveShaderLog(std::ostream& os, GLuint obj, const char * shader = "");
    
private:
    bool m_use_geom;

    GLuint m_vertex_shader;
	GLuint m_fragment_shader;
    GLuint m_geometry_shader;
    GLuint m_program;

};



#endif
