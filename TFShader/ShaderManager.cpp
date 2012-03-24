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



ShaderManager::ShaderManager( const char * vertexShader, 
                              const char * fragmentShader, 
                              const char * geometryShader )
{
	using std::cout;
	using std::string;
	
	m_vertex_shader		= glCreateShader(GL_VERTEX_SHADER);
	m_fragment_shader	= glCreateShader(GL_FRAGMENT_SHADER);
	m_geometry_shader	= glCreateShader(GL_GEOMETRY_SHADER_EXT);
    
	string	vs			= getShader( vertexShader );
	const char*	pvs		= vs.c_str();
	if (TF_USE_GEOMETRY_SHADER) {
        std::cout << "G";
        glShaderSource(m_vertex_shader, 1, &pvs, NULL);
    } else {
        std::cout << ".";
        const char * source[2] = { "#define USE_GEOMETRY_SHADER\n", pvs };
        glShaderSource(m_vertex_shader, 2, source, NULL);
    }
	glCompileShader(m_vertex_shader);
    
	string	fs			= getShader( fragmentShader );
	const char*	pfs		= fs.c_str();
	glShaderSource(m_fragment_shader, 1, &pfs, NULL);
	glCompileShader(m_fragment_shader);

    if (TF_USE_GEOMETRY_SHADER) {
        string	gs			= getShader( geometryShader );
        const char*	pgs		= gs.c_str();
        glShaderSource(m_geometry_shader, 1, &pgs, NULL);
        glCompileShader(m_geometry_shader);
        
    }

	m_program = glCreateProgram();
    
	glAttachShader(m_program, m_fragment_shader);
	glAttachShader(m_program, m_vertex_shader);
    if (TF_USE_GEOMETRY_SHADER) {
        glAttachShader(m_program, m_geometry_shader);
        
        // TODO: the in/out primitives should be arguments!
        glProgramParameteriEXT(m_program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
        glProgramParameteriEXT(m_program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP);
        
        int		max_vertices;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_vertices);
        glProgramParameteriEXT(m_program, GL_GEOMETRY_VERTICES_OUT_EXT, max_vertices);
    }
    
	glLinkProgram(m_program);
    
	glUseProgram(m_program);
    
	retrieveShaderLog(cout, m_vertex_shader, "Vertex");
	retrieveShaderLog(cout, m_fragment_shader, "Fragment");
	if (TF_USE_GEOMETRY_SHADER) retrieveShaderLog(cout, m_geometry_shader, "Geometry");
	retrieveProgramLog(cout, m_program);
    
    glUseProgram(0);    // disable the shader
    
}



ShaderManager::~ShaderManager()
{
    glDeleteProgram(m_program);
}