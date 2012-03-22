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



void ShaderManager::retrieveShaderLog(std::ostream& os, GLuint obj)
{
	int cbInfoLog = 0;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &cbInfoLog);
	
	if ( cbInfoLog > 0 )
	{
		char*	pbuffInfoLog = new char[cbInfoLog];
		int		cbInfoRead  = 0;
		glGetShaderInfoLog(obj, cbInfoLog, &cbInfoRead, pbuffInfoLog);
        
		os << "Shader Info Log: " << pbuffInfoLog << "\n";
        
		delete [] pbuffInfoLog;
	}
	else
	{
		printf("Shader Info Log: OK\n");
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
		}
	}
	
	return strFile;
}



ShaderManager::ShaderManager( const char * vertexShader, const char * fragmentShader )
{
	using std::cout;
	using std::string;
	
	m_vertex_shader		= glCreateShader(GL_VERTEX_SHADER);
	m_fragment_shader	= glCreateShader(GL_FRAGMENT_SHADER);
	m_geometry_shader	= glCreateShader(GL_GEOMETRY_SHADER_EXT); // FIXME: not sure if this is needed
    
	string	vs			= getShader( vertexShader );
	const char*	pvs		= vs.c_str();
	glShaderSource(m_vertex_shader, 1, &pvs, NULL);
	glCompileShader(m_vertex_shader);
    
	string	fs			= getShader( fragmentShader );
	const char*	pfs		= fs.c_str();
	glShaderSource(m_fragment_shader, 1, &pfs, NULL);
	glCompileShader(m_fragment_shader);
    
	m_program = glCreateProgram();
    
	glAttachShader(m_program, m_fragment_shader);
	glAttachShader(m_program, m_vertex_shader);
    
	glProgramParameteriEXT(m_program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_LINES);
	glProgramParameteriEXT(m_program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP);
    
	int		max_vertices;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_vertices);
	glProgramParameteriEXT(m_program, GL_GEOMETRY_VERTICES_OUT_EXT, max_vertices);
    
	glLinkProgram(m_program);
    
	glUseProgram(m_program);
    
	retrieveShaderLog(cout, m_vertex_shader);
	retrieveShaderLog(cout, m_fragment_shader);
	retrieveProgramLog(cout, m_program);
    
    glUseProgram(0);    // disable the shader
}



ShaderManager::~ShaderManager()
{
    glDeleteProgram(m_program);
}