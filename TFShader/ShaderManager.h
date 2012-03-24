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
#include <map>

using namespace std;

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    GLuint getProgram(std::string name) { return m_programs[name]; };
    
private:
    std::string getShader(const char* pszFilename);
    void retrieveProgramLog(std::ostream& os, GLuint obj);
    void retrieveShaderLog(std::ostream& os, GLuint obj, const char * shader = "");

    GLuint makeGLSLProgram( const char * vertexShader, 
                            const char * fragmentShader,
                            const char * geometryShader = NULL );

private:
    map<string, GLuint> m_programs;

};



#endif
