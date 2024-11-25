#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float aIntensity;
  float dIntensity;
  float sIntensity;
  
  float constant;
  float linear;
  float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light pointLight;

void main() 
{
  vec3 normalVec = normalize(Normal);

  // ambient shading
  vec3 ambient = pointLight.ambient * pointLight.aIntensity * vec3(texture(material.texture_diffuse1, TexCoords)); 
  
  // diffuse shading
  vec3 lightVec = normalize(pointLight.position - FragPos);
  float diff = max(dot(normalVec, lightVec), 0.0);
  vec3 diffuse = pointLight.diffuse * pointLight.dIntensity * diff * vec3(texture(material.texture_diffuse1, TexCoords)); 

  // specular shading
  vec3 viewVec = normalize(viewPos - FragPos);
  vec3 reflectVec = reflect(-lightVec, normalVec);
  float spec = pow(max(dot(viewVec, reflectVec), 0.0), material.shininess);
  vec3 specular = pointLight.specular * pointLight.sIntensity * spec * vec3(texture(material.texture_specular1, TexCoords));

  vec3 result = (ambient + diffuse + specular);
  FragColor = vec4(result, 1.0);
}