#pragma once
#include <string>
#include <glew.h>

namespace test_Engine
{
	extern void fatalError(std::string errorString);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 
	extern GLenum glCheckError_(const char* file, int line);
}