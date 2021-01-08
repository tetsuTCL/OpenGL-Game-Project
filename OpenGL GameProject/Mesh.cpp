#include<fstream>
#include<sstream>
#include<stdio.h>
#include<document.h>
#include<SDL_log.h>
#include"Mesh.h"
#include"Renderer.h"
#include"Texture.h"
#include"VertexArray.h"
#include"Math.h"

namespace {
	union Vertex
	{
		float f;
		Uint8 u[4];
	};
}

Mesh::Mesh()
	: mVertexArray(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the version
	if (ver != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// Skip the vertex format/shader for now
	// (This is changed in a later chapter's code)
	size_t vertSize = 8;

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// Is this texture already loaded?
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// Try loading the texture
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// If it's still null, just use the default texture
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());

		// Add the floats
		for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	// We were computing length squared earlier
	mRadius = Math::Sqrt(mRadius);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned>(indices.size()));
	vertexArray.emplace_back(mVertexArray);//Put mVertexArray to new Array for loading FBX
	return true;
}

bool Mesh::LoadFBX(const char* fileName, Renderer* renderer)
{
	//Initialize manager
	fbxsdk::FbxManager* fbxManager = fbxsdk::FbxManager::Create();
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbxManager, IOSROOT);

	//Initialize importer
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (!fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		fbxManager->Destroy();
		return false;
	}

	//Initialize scene
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxGeometryConverter geometryConverter(fbxManager);
	geometryConverter.Triangulate(fbxScene, true);

	//Root Node
	FbxNode* root = fbxScene->GetRootNode();

	//Show
	if (root != 0) PrintName(root, 0);
	if (root)
	{
		int childCount = root->GetChildCount();
		for (int i = 0; i < childCount; i++)
		{
			GetMesh(root->GetChild(i), renderer);
		}
	}
	fbxManager->Destroy();

	return true;
}

void Mesh::GetMesh(FbxNode* node, Renderer* renderer)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute)
	{
		FbxNodeAttribute::EType type = attribute->GetAttributeType();
		switch (type)
		{
		case FbxNodeAttribute::eMesh:

			FbxMesh* mesh = node->GetMesh();
			GetPosition(mesh);
			GetNormal(mesh);
			GetUV(mesh);
			GetTexture(mesh, renderer);
			break;
		}
	}

	for (int i = 0; i < indices.size(); i++)
	{
		//Position
		Vertex vertex;
		vertices.emplace_back(positions[indices[i]].x);
		vertices.emplace_back(positions[indices[i]].y);
		vertices.emplace_back(positions[indices[i]].z);

		//Normal
		vertices.emplace_back(normals[indices[i]].x);
		vertices.emplace_back(normals[indices[i]].y);
		vertices.emplace_back(normals[indices[i]].z);

		//UV
		if (uvs.size() == indices.size())
		{
			vertices.emplace_back(uvs[i].x);
			vertices.emplace_back(uvs[i].y);
		}
		else
		{
			vertices.emplace_back(0.0f);
			vertices.emplace_back(0.0f);
		}

		indices[i] = i;
	}

	if (vertices.size() == 0)
	{
		return;
	}

	if (mTextures.size() == 0)
	{
		mTextures.emplace_back(renderer->GetTexture("Texture/none.png"));
	}

	mVertexArray = new VertexArray(vertices.data(),
		static_cast<unsigned>(vertices.size()) / 8.0f,
		indices.data(), static_cast<unsigned>(indices.size()));
	vertexArray.emplace_back(mVertexArray);

	vertices.clear();
	indices.clear();
	positions.clear();
	normals.clear();
	uvs.clear();

	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		GetMesh(node->GetChild(i), renderer);
	}
}

void Mesh::GetPosition(FbxMesh* mesh)
{
	for (int i = 0; i < mesh->GetPolygonCount() * 3; i++)
	{
		indices.emplace_back(mesh->GetPolygonVertices()[i]);
	}

	FbxVector4* vertex = mesh->GetControlPoints();
	for (int i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		//position
		Vector3 p;
		p.x = (float)vertex[i][0];//X
		p.y = (float)vertex[i][1];//Y
		p.z = (float)vertex[i][2];//Z
		positions.emplace_back(p);

		mRadius = Math::Max(mRadius, p.LengthSq());
	}
}

void Mesh::GetNormal(FbxMesh* _mesh)
{
	//Normals
	for (int i = 0; i < _mesh->GetElementNormalCount(); i++)
	{
		FbxGeometryElementNormal* normal = _mesh->GetElementNormal(i);

		FbxGeometryElement::EMappingMode mapping_mode = normal->GetMappingMode();		//Mapping Mode
		FbxGeometryElement::EReferenceMode reference_mode = normal->GetReferenceMode();	//References Mode

		if (mapping_mode == FbxGeometryElement::eByControlPoint ||
			mapping_mode == FbxGeometryElement::eByPolygonVertex)
		{
			if (reference_mode == FbxGeometryElement::eDirect)
			{
				for (int j = 0; j < normal->GetDirectArray().GetCount(); j++)
				{
					Vector3 n;
					n.x = (float)normal->GetDirectArray().GetAt(j)[0];
					n.y = (float)normal->GetDirectArray().GetAt(j)[1];
					n.z = (float)normal->GetDirectArray().GetAt(j)[2];
					normals.emplace_back(n);
				}
			}
		}
		/*else if(mapping_mode == FbxGeometryElement::eByControlPoint)
		{
			if (reference_mode == FbxGeometryElement::eDirect)
			{
				for (int j = 0; j < normal->GetDirectArray().GetCount(); j++)
				{
					Vertex v;
					v.normal.x = (float)normal->GetDirectArray().GetAt(i)[0];
					v.normal.y = (float)normal->GetDirectArray().GetAt(i)[1];
					v.normal.z = (float)normal->GetDirectArray().GetAt(i)[2];
					vertices.emplace_back(v);
				}
			}
		}*/

	}
}

void Mesh::GetUV(FbxMesh* mesh)
{
	for (int i = 0; i < mesh->GetElementUVCount(); i++)
	{
		FbxGeometryElementUV* uv = mesh->GetElementUV(i);

		FbxGeometryElement::EMappingMode mappingMode = uv->GetMappingMode(); //Mapping Mode
		FbxGeometryElement::EReferenceMode referenceMode = uv->GetReferenceMode();	//References Mode

		uv->GetName();

		if (mappingMode == FbxGeometryElement::eByPolygonVertex)
		{
			if (referenceMode == FbxGeometryElement::eDirect)
			{
				for (int j = 0; j < uv->GetDirectArray().GetCount(); j++)
				{
					Vector2 vec2UV;
					vec2UV.x = (float)uv->GetDirectArray().GetAt(i)[0];
					vec2UV.y = (float)uv->GetDirectArray().GetAt(i)[1];
					uvs.emplace_back(vec2UV);
				}
			}
			else if (referenceMode == FbxGeometryElement::eIndexToDirect)
			{
				for (int j = 0; j < uv->GetIndexArray().GetCount(); j++)
				{
					int index = uv->GetIndexArray().GetAt(i);

					Vector2 vec2UV;
					vec2UV.x = (float)uv->GetDirectArray().GetAt(index)[0];
					vec2UV.y = (float)uv->GetDirectArray().GetAt(index)[1];
					uvs.emplace_back(vec2UV);
				}
			}
		}
	}
}

void Mesh::GetTexture(FbxMesh* _mesh, Renderer* _renderer)
{
	FbxNode* node = _mesh->GetNode();

	for (int i = 0; i < node->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* fbxMaterial = node->GetMaterial(i);//Material
		FbxProperty fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		for (int j = 0; j < fbxProperty.GetSrcObjectCount<FbxFileTexture>(); j++)
		{
			FbxFileTexture* texture = fbxProperty.GetSrcObject<FbxFileTexture>(j);
			if (texture)
			{
				std::string textureName = texture->GetRelativeFileName();

				std::string uvSetName = texture->UVSet.Get().Buffer();
				if (uvName == uvSetName)
				{
					Texture* texture = _renderer->GetTexture(textureName);
					if (texture == nullptr)
					{
						texture = _renderer->GetTexture("Texture/none.png");//Default
					}
					mTextures.emplace_back(texture);
				}
			}
		}
	}
}

void Mesh::PrintName(FbxNode* node, int indent)
{
	for (int i = 0; i < indent; i++)
	{
		printf("");
	}

	const char* eTypeNames[] =
	{
		"eUnknown",
		"eNull",
		"eMarker",
		"eSkeleton",
		"eMesh",
		"eNurbs",
		"ePatch",
		"eCamera",
		"eCameraStereo",
		"eCameraSwitcher",
		"eLight",
		"eOpticalReference",
		"eOpticalMarker",
		"eNurbsCurve",
		"eTrimNurbsSurface",
		"eBoundary",
		"eNurbsSurface",
		"eShape",
		"eLODGroup",
		"eSubDiv",
		"eCachedEffect",
		"eLine"
	};

	const char* name = node->GetName();

	int attributeCount = node->GetNodeAttributeCount();
	if (attributeCount == 0)
	{
		printf("%s\n", name);
	}
	else
	{
		printf("%s (", name);
	}
	for (int i = 0; i < attributeCount; ++i)
	{
		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
		FbxNodeAttribute::EType type = attribute->GetAttributeType();

		printf("%s", eTypeNames[type]);
		if (i + 1 == attributeCount)
		{
			printf(")\n");
		}
		else
		{
			printf(", ");
		}
	}

	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		PrintName(node->GetChild(i), indent + 1);
	}
}

void Mesh::Unload()
{
	//delete mVertexArray;
	//mVertexArray = nullptr;
	while (!vertices.empty())
	{
		delete vertexArray.back();
	}
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}
