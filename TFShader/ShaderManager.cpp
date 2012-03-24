//
//  ShaderManager.cpp
//  TFShader
//
//  Created by Jorge Herrera on 3/20/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "ShaderManager.h"


#define TF_USE_GEOMETRY_SHADER true


void ShaderManager::retrieveShaderLog(std::ostream& os, GLuint obj, const char * shader)
{
	int cbInfoLog = 0;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &cbInfoLog);
	
	if ( cbInfoLog > 0 )
	{
		char*	pbuffInfoLog = new char[cbInfoLog];
		int		cbInfoRead  = 0;
		glGetShaderInfoLog(obj, cbInfoLog, &cbInfoRead, pbuffInfoLog);
        
		os << shader << " Shader Info Log: " << pbuffInfoLog << "\n";
        
		delete [] pbuffInfoLog;
	}
	else
	{
		printf("%s Shader Info Log: OK\n", shader);
	}
}



void ShaderManager::retrieveProgramLog(std::ostream& os, GLuint obj)
{
	int		cbInfoLog = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &cbInfoLog);
	
	if ( cbInfoLog > 0 )
	{
		char*	pbuffInfoLog = new char[cbInfoLog];
		int		cbInfoRead  = 0;
		glGetProgramInfoLog(obj, cbInfoLog, &cbInfoRead, pbuffInfoLog);
        
		os << "Program Info Log: " << pbuffInfoLog << "\n";
        
		delete [] pbuffInfoLog;
	}
	else
	{
		os << "Program Info Log: OK\n";
	}
}



std::string ShaderManager::getShader(const char* pszFilename)
{
	using std::getline;
	using std::ifstream;
	using std::string;
	
	string		str, strFile;
	ifstream	ifs(pszFilename);
	if ( ifs.is_open() )
	{
		while ( getline(ifs, str) )
		{
			strFile += str;
            strFile += "\n";
		}
	}
	
	return strFile;
}



GLuint ShaderManager::makeGLSLProgram( const char * vertexShader, 
                                       const char * fragmentShader, 
                                       const char * geometryShader )
{
	using std::cout;
	using std::string;
	
	GLuint vertex_shader	= glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader	= glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometry_shader	= glCreateShader(GL_GEOMETRY_SHADER_EXT);
    
	string	vs			= getShader( vertexShader );
	const char*	pvs		= vs.c_str();
	if (geometryShader) {
        std::cout << "G";
        glShaderSource(vertex_shader, 1, &pvs, NULL);
    } else {
        std::cout << ".";
        const char * source[2] = { "#define USE_GEOMETRY_SHADER\n", pvs };
        glShaderSource(vertex_shader, 2, source, NULL);
    }
	glCompileShader(vertex_shader);
    
	string	fs			= getShader( fragmentShader );
	const char*	pfs		= fs.c_str();
	glShaderSource(fragment_shader, 1, &pfs, NULL);
	glCompileShader(fragment_shader);

    if (geometryShader) {
        string	gs			= getShader( geometryShader );
        const char*	pgs		= gs.c_str();
        glShaderSource(geometry_shader, 1, &pgs, NULL);
        glCompileShader(geometry_shader);
        
    }

	GLuint program = glCreateProgram();
    
	glAttachShader(program, fragment_shader);
	glAttachShader(program, vertex_shader);
    if (geometryShader) {
        glAttachShader(program, geometry_shader);
        
        // TODO: the in/out primitives should be arguments!
        glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
        glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP);
        
        int		max_vertices;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_vertices);
        glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, max_vertices);
    }
    
	glLinkProgram(program);
    
//	glUseProgram(program);
    
	retrieveShaderLog(cout, vertex_shader, "Vertex");
	retrieveShaderLog(cout, fragment_shader, "Fragment");
	if (geometryShader) retrieveShaderLog(cout, geometry_shader, "Geometry");
	retrieveProgramLog(cout, program);
    
//    glUseProgram(0);    // disable the shader

    return program;
    
}


ShaderManager::ShaderManager()
{
    m_programs["points"] = makeGLSLProgram( "tf.vert", "tf.frag" );
    m_programs["lines"] = makeGLSLProgram( "tf.vert", "tf.frag", "tf.geom" );
}

ShaderManager::~ShaderManager()
{
    for (map<string, GLuint>::iterator it = m_programs.begin(); 
         it != m_programs.end(); 
         it++) {
        glDeleteProgram((*it).second);
    }
}