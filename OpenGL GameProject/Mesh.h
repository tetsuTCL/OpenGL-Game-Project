#pragma once
#include<string>
#include<vector>
#include<fbxsdk.h>
#include"Math.h"
#include"VertexArray.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	//Load/Unload  Mesh
	bool Load(const std::string& fileName, class Renderer* renderer);
	bool LoadFBX(const char* fileName, class Renderer* renderer);
	void Unload();
	//
	std::vector<VertexArray*> GetVertexArray() { return vertexArray; }
	//Get texture from index
	class Texture* GetTexture(size_t index);
	//Get shader name
	const std::string& GetShaderName() const { return mShaderName; }
	//// Get object space bounding sphere radius
	float GetRadius()const { return mRadius; }
	// Get specular power of mesh
	float GetSpecPower() const { return mSpecPower; }
private:
	void GetMesh(FbxNode* node, Renderer* renderer);
	void GetPosition(FbxMesh* mesh);
	void GetNormal(FbxMesh* mesh);
	void GetUV(FbxMesh* mesh);
	void GetTexture(FbxMesh* mesh, Renderer* renderer);

	void PrintName(FbxNode* node, int indent);

	std::vector<VertexArray*> vertexArray;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::string uvName;

private:
	//Group of Mesh Textures
	std::vector<class Texture*> mTextures;
	//Vertex Array of Mesh
	VertexArray* mVertexArray;
	//Shader name
	std::string mShaderName;
	// Stores object space bounding sphere radius
	float mRadius;
	// Specular power of surface
	float mSpecPower;

};
