// Fragment shader
#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

struct DirectionalLight{
  vec3 color;
  float ambientIntensity;
  vec3 direction;
  float diffuseIntensity;
};

struct Material{
  float specularIntensity;
  float shininess;
};

uniform sampler2D theTex;
uniform DirectionalLight dirLight;
uniform Material material;

uniform vec3 eyePos;

void main()
{
  vec4 ambientColor = vec4(dirLight.color, 1.0f)*dirLight.ambientIntensity;

  float diffuseFactor = max(dot(normalize(normal), normalize(dirLight.direction)), 0.0f);
  vec4 diffuseColor = vec4(dirLight.color, 1.0f)* dirLight.diffuseIntensity* diffuseFactor;

  vec4 specularColor = vec4(0,0,0,0);

  if(diffuseFactor>0.0f){
    vec3 fragToEye = normalize(eyePos - fragPos);
    vec3 reflectedVertex = normalize(reflect(dirLight.direction, normalize(normal)));

    float specularFactor = dot(fragToEye, reflectedVertex);
    if(specularFactor>0.0f){
      specularFactor = pow(specularFactor, material.shininess);
      specularColor = vec4(dirLight.color* material.specularIntensity* specularFactor, 1.0f);
    }
  }

  color = texture(theTex, texCoord)* (ambientColor+diffuseColor+specularColor);
}
