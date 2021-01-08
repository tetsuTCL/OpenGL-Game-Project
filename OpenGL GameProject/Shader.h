#pragma once
#include<string>
#include<glew.h>
#include"Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//Load the vertex/fragment shaders with the given names
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//Set this as the active shader program
	void SetActive();
	//Set Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	// Sets a Vector3 uniform
	void SetVectorUniform(const char* name, const Vector3& vector);
	// Sets a float uniform
	void SetFloatUniform(const char* name, float value);

private:
	//Compile specificed shader
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	//Check if shader compiled successfully
	bool IsCompiled(GLuint shader);
	//Check if vertex/fragment programs linked
	bool isValidProgram();

	// Store the shader object IDs
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;

};
