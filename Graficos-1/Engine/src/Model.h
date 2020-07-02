#ifndef MODEL_H
#define MODEL_H

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif // !DLLEXPORT

#include "Mesh.h"

#include <assimp/scene.h>

#include "Material.h"

#include <vector>

#include "Entity.h"

#include <iostream>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class DLLEXPORT Model
{
public:
	// model data 
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection;

	// constructor, expects a filepath to a 3D model.
	Model(string const &path, bool flipUv, bool gamma = false);

	// draws the model, and thus all its meshes
	virtual void Draw(Shader shader);

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path, bool flipUv);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};


#endif