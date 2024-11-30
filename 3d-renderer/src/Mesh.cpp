#include "Mesh.h"

// constructor
Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures)

  : textures(textures), VBO(vertices), EBO(indices)
{
  setupMesh();
}

// render the mesh
void Mesh::Draw(Shader &shader) 
{
  // bind appropriate textures
  unsigned int diffuseNr  = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr   = 1;
  unsigned int heightNr   = 1;
  for(unsigned int i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures[i].name;
    if(name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if(name == "texture_specular")
      number = std::to_string(specularNr++);
    else if(name == "texture_normal")
      number = std::to_string(normalNr++); 
    else if(name == "texture_height")
      number = std::to_string(heightNr++); 

    shader.setInt(("material." + name + number).c_str(), i);
    textures[i].bind();
  }

  // draw mesh
  VAO.bind();
  glDrawElements(GL_TRIANGLES, EBO.indices_size, GL_UNSIGNED_INT, 0);
  VAO.unbind();

  glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
  VAO.bind();

  EBO.bind();
  // vertex positions
  VAO.linkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
  // vertex normals
  VAO.linkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  // vertex texture coordinates
  VAO.linkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  // vertex tangent
  VAO.linkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
  // vertex bitangent
  VAO.linkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
  // vertex ids
  VAO.linkAttrib(VBO, 5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
  // vertex weights
  VAO.linkAttrib(VBO, 6, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

  VAO.unbind();
  VBO.unbind();
  EBO.unbind();
}