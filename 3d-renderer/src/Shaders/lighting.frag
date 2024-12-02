#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
uniform Light pointLight2;
uniform vec3 objColor; // Object color passed from the main program
uniform bool monochrome;
uniform bool auxLightOn;

uniform bool blinnPhong;

void main()
{
    vec3 normalVec = normalize(Normal);

    // Ambient shading
    vec3 ambient = pointLight.ambient * pointLight.aIntensity * vec3(texture(material.diffuse, TexCoords));

    // Diffuse shading
    vec3 lightVec = normalize(pointLight.position - FragPos);
    
    float diff = max(dot(normalVec, lightVec), 0.0);
    
    vec3 diffuse = pointLight.diffuse * pointLight.dIntensity * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular shading
    vec3 viewVec = normalize(viewPos - FragPos);

    float spec = 0.0;
    if (blinnPhong) 
    {
      vec3 halfwayVec = normalize(lightVec + viewVec);
      spec = pow(max(dot(normalVec, halfwayVec), 0.0), material.shininess * 3.5);
    } 
    else
    {
      vec3 reflectVec = reflect(-lightVec, normalVec);
      spec = pow(max(dot(viewVec, reflectVec), 0.0), material.shininess);
    }

    vec3 specular =  pointLight.specular * pointLight.sIntensity * spec * vec3(texture(material.specular, TexCoords));

    if (auxLightOn) 
    {
        vec3 lightVec2 = normalize(pointLight2.position - FragPos);
        float diff2 = max(dot(normalVec, lightVec2), 0.0);

        float spec2 = 0.0;

        if (blinnPhong) 
        {
          vec3 halfwayVec2 = normalize(lightVec2 + viewVec);
          spec2 = pow(max(dot(normalVec, halfwayVec2), 0.0), material.shininess);
        } 
        else
        {
          vec3 reflectVec2 = reflect(-lightVec2, normalVec);
          spec2 = pow(max(dot(viewVec, reflectVec2), 0.0), material.shininess);
        }

        ambient  += pointLight2.ambient * pointLight2.aIntensity * vec3(texture(material.diffuse, TexCoords));
        diffuse  += pointLight2.diffuse * pointLight2.dIntensity * diff2 * vec3(texture(material.diffuse, TexCoords));
        specular += pointLight2.specular * pointLight2.sIntensity * spec2 * vec3(texture(material.specular, TexCoords));
    }
    // Combine results and apply object color
    vec3 result = (ambient + diffuse + specular) * objColor;

     // Apply monochrome effect if enabled
    if (monochrome) {
        float gray = dot(result, vec3(0.299, 0.587, 0.114)); // Grayscale conversion
        result = vec3(gray);
    }

    FragColor = vec4(result, 1.0);
}
