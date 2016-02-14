#include "stdafx.h"
#include "AnimatedMesh.h"


AnimatedMesh::AnimatedMesh(TextureLoader *textureLoader)
	: mTextureLoader(textureLoader)
{
}


AnimatedMesh::~AnimatedMesh(void)
{
}

bool AnimatedMesh::LoadModel(const std::string& filename) {
	//clear any existing data
	mBones.clear();
	mBoneCount = 0;
	mMeshes.clear();
	mMeshCount = 0;

	//attempt to open the file
	boost::filesystem3::ifstream file(filename);
	if(file.fail()) {
		return false;
	}

	//get file length
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int fileLength = file.tellg();
	file.seekg(pos);

	std::string token;
	std::string ignore;
	
	while(file >> token, !file.eof()) {
		if(token == "numJoints") {
			file >> mBoneCount;
		} else if (token == "numMeshes") {
			file >> mMeshCount;
		} else if (token == "joints") {
			file >> ignore; //opening brace
			for(int i = 0; i < mBoneCount; i++) {
				readBone(file, fileLength);
			}
			file >> ignore; // closing brace
		} else if (token == "mesh") {
			readMesh(file, fileLength);
		}
	}
	assert(mBoneCount == mBones.size());
	assert(mMeshCount == mMeshes.size());
	return true;
}

void AnimatedMesh::SaveModel(const std::string& filename) {
	std::cout << "MD5Version 10" << std::endl;
	std::cout << "commandline \"\"" << std::endl << std::endl;
	
	std::cout << "numJoints " << mBones.size() << std::endl;
	std::cout << "numMeshes " << mMeshes.size() << std::endl << std::endl;
	
	std::cout << "joints {" << std::endl;
	for(int i = 0; i < mBones.size(); i++) {
		Bone& bone = mBones[i];
		std::cout << "\t\"" << bone.name << "\"\t" << bone.parentId << " "
			<< "( " << bone.position.x << " " << bone.position.y << " " << bone.position.z << " ) "
			<< "( "	<< bone.orientation.x << " " << bone.orientation.y << " " << bone.orientation.z << " )"
			<< std::endl;
	}
	std::cout << "}" << std::endl;
}

bool AnimatedMesh::LoadAnim(const std::string& filename) {
	return true;
}

void AnimatedMesh::Update(unsigned int elapsedTimeMillis) {
}

void AnimatedMesh::Render() {
	GLuint vbo[4];
	glGenBuffers(4, vbo);
	GLuint sampler;
	glGenSamplers(1, &sampler);
	for(unsigned int i = 0; i < mMeshes.size(); i++) {
		Mesh& mesh = mMeshes[i];
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.verts.size(), &mesh.positionBuffer[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.verts.size(), &mesh.normalBuffer[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.verts.size(), &mesh.tex2DBuffer[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glActiveTexture(GL_TEXTURE0);
		GLuint texture = mTextureLoader->GetTexture(mesh.shader);
		glBindTexture(GL_TEXTURE_2D, mTextureLoader->GetTexture(mesh.shader));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.tris.size() * 3, &mesh.indexBuffer[0], GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, mesh.tris.size() * 3, GL_UNSIGNED_INT, NULL);
	}
	glDeleteBuffers(3, vbo);
	glDeleteSamplers(1, &sampler);
}

void AnimatedMesh::readBone(boost::filesystem3::ifstream& file, int length) {
	Bone bone;
	std::string ignore;
	file >> bone.name >> bone.parentId >> ignore;
	file >> bone.position.x >> bone.position.y >> bone.position.z >> ignore >> ignore;
	file >> bone.orientation.x >> bone.orientation.y >> bone.orientation.z >> ignore;
	file.ignore(length, '\n');

	size_t n;
    while ((n = bone.name.find('\"') ) != std::string::npos ) bone.name.erase(n,1);
	float t = 1.0f - (bone.orientation.x * bone.orientation.x) - 
					 (bone.orientation.y * bone.orientation.y) -
					 (bone.orientation.z * bone.orientation.z);
    if (t < 0.0f) {
        bone.orientation.w = 0.0f;
    } else {
        bone.orientation.w = -sqrtf(t);
    }
	mBones.push_back(bone);
}

void AnimatedMesh::readMesh(boost::filesystem3::ifstream& file, int length) {
	Mesh mesh;
	std::string ignore;
	std::string token;
    int numVerts, numTris, numWeights;

    file >> ignore; // opening brace
    while (file >> token, token != "}") { // read until the closing brace
		if (token == "shader") {
			file >> mesh.shader;
			size_t n;
			while ((n = mesh.shader.find('\"') ) != std::string::npos ) mesh.shader.erase(n,1);
        } else if (token == "numverts") {
            file >> numVerts;               // Read in the vertices
			file.ignore(length, '\n');
            for (int i = 0; i < numVerts; i++) {
				Vertex vert;
				std::string ignore;
				file >> ignore >> ignore >> ignore;                    // vert vertIndex (
				file >> vert.textureCoord.x >> vert.textureCoord.y >> ignore;  //  s t )
				file >> vert.startWeight >> vert.weightCount;
				file.ignore(length, '\n');
				mesh.verts.push_back(vert);
				mesh.tex2DBuffer.push_back(vert.textureCoord);
            }  
        } else if (token == "numtris" ) {
            file >> numTris;
            file.ignore(length, '\n');
            for (int i = 0; i < numTris; i++) {
                Triangle tri;
                file >> ignore >> ignore;
				file >> tri.indices[0] >> tri.indices[1] >> tri.indices[2];
				file.ignore(length, '\n');
				mesh.tris.push_back(tri);
                mesh.indexBuffer.push_back((GLuint)tri.indices[0]);
                mesh.indexBuffer.push_back((GLuint)tri.indices[1]);
                mesh.indexBuffer.push_back((GLuint)tri.indices[2]);
            }              
        } else if (token == "numweights") {
            file >> numWeights;
            file.ignore(length, '\n');
            for (int i = 0; i < numWeights; i++) {
                BoneWeight weight;
                file >> ignore >> ignore;
				file >> weight.boneId >> weight.bias >> ignore;
				file >> weight.position.x >> weight.position.y >> weight.position.z >> ignore;
                file.ignore(length, '\n');
				mesh.weights.push_back(weight);
            }
        } else {
			file.ignore(length, '\n');
        }
    }

	//make sure the file wasn't lying to us (...or we misread something...)
	assert(numVerts == mesh.verts.size());
	assert(numTris == mesh.tris.size());
	assert(numWeights == mesh.weights.size());

	//compute the vertices in the bind pose
	for(unsigned int i = 0;  i < mesh.verts.size(); i++) {
		Vertex& vert = mesh.verts[i];
		vert.position = glm::vec3(0);
		for(int j = vert.startWeight; j < vert.startWeight + vert.weightCount; j++) {
			BoneWeight& weight = mesh.weights[j];
			Bone& bone = mBones[weight.boneId];
			//convert the weight position from bone local to object local
			glm::vec3 rotatedPos = bone.orientation * weight.position;
			vert.position += (bone.position + rotatedPos) * weight.bias;
		}
		mesh.positionBuffer.push_back(vert.position);
		//clear the normal for later
		vert.normal = glm::vec3(0);
	}
	//compute the normals in the bind pose
	for(unsigned int i = 0; i < mesh.tris.size(); i++) {
		Triangle& tri = mesh.tris[i];
		glm::vec3 v0 = mesh.verts[tri.indices[0]].position;
		glm::vec3 v1 = mesh.verts[tri.indices[1]].position;
		glm::vec3 v2 = mesh.verts[tri.indices[2]].position;
		glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);
		mesh.verts[tri.indices[0]].normal += normal;
		mesh.verts[tri.indices[1]].normal += normal;
		mesh.verts[tri.indices[2]].normal += normal;
	}
	//normalize the normals and convert to joint-local space
	for(unsigned int i = 0; i < mesh.verts.size(); i++)
	{
		Vertex& vert = mesh.verts[i];
		glm::vec3 normal = glm::normalize(vert.normal);
		mesh.normalBuffer.push_back(normal);
		
		vert.normal = glm::vec3(0);
		for(int j = vert.startWeight; j < vert.startWeight + vert.weightCount; j++) {
			const BoneWeight& weight = mesh.weights[j];
			const Bone& bone = mBones[weight.boneId];
			vert.normal += (normal * bone.orientation) * weight.bias;
		}
	}
    mMeshes.push_back(mesh);
}