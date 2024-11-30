#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model 
{
public:
  // model data 
  std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  std::vector<Texture> textures; //stores all the textures not loaded but stored for later use
  std::vector<Mesh> meshes;
  std::string directory;
  std::string fullpath;
  bool gammaCorrection;

  // constructor, expects a filepath to a 3D model.
  Model(std::string const& path, bool gamma = false);

  // draws the model, and thus all its meshes
  void Draw(Shader& shader);

  void UnLoadTexture(Texture texture);

  void LoadTexture(Texture texture);

private:
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes std::vector.
  void loadModel(std::string const& path);

  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
  void processNode(aiNode* node, const aiScene* scene);


  Mesh processMesh(aiMesh* mesh, const aiScene* scene);

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  
};

