#pragma once

#include "TextureLoader.h"

class AnimatedMesh
{
public:
	AnimatedMesh(TextureLoader *textureLoader);
	~AnimatedMesh(void);

	bool LoadModel(const std::string& filename);
	void SaveModel(const std::string& filename);
	bool LoadAnim(const std::string& filename);
	void Update(unsigned int elapsedTimeMillis);
	void Render();

protected:
	typedef std::vector<glm::vec3> PositionBuffer;
	typedef std::vector<glm::vec3> NormalBuffer;
	typedef std::vector<glm::vec2> Tex2DBuffer;
	typedef std::vector<GLuint> IndexBuffer;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
		int startWeight;
		int weightCount;
	};
	typedef std::vector<Vertex> VertexList;

	struct Triangle
	{
		int indices[3];
	};
	typedef std::vector<Triangle> TriangleList;

	struct Bone
	{
		std::string name;
		int parentId;
		glm::vec3 position;
		glm::quat orientation;
	};
	typedef std::vector<Bone> BoneList;

	struct BoneWeight
	{
		int boneId;
		float bias;
		glm::vec3 position;
	};
	typedef std::vector<BoneWeight> WeightList;

	struct Mesh
	{
		std::string shader;
		//This vertex list stores the vertices in the bind pose
		VertexList verts;
		TriangleList tris;
		WeightList weights;

		GLuint textureId;

		PositionBuffer positionBuffer;
		NormalBuffer normalBuffer;
		Tex2DBuffer tex2DBuffer;
		IndexBuffer indexBuffer;
	};
	typedef std::vector<Mesh> MeshList;

private:
	TextureLoader *mTextureLoader;
	unsigned int mBoneCount;
	unsigned int mMeshCount;

	BoneList mBones;
	MeshList mMeshes;

	glm::mat4x4 mLocalToWorldMatrix;

	void readBone(boost::filesystem3::ifstream& file, int length);
	void readMesh(boost::filesystem3::ifstream& file, int length);
};

