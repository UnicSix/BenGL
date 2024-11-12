// Fragment shader
#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;

out vec4 color;

struct DirectionalLight{
  vec3 color;
  float ambientIntensity;
  vec3 direction;
  float diffuseIntensity;
};

uniform sampler2D theTex;
uniform DirectionalLight dirLight;

void main()
{
  vec4 ambientColor = vec4(dirLight.color, 1.0f)*dirLight.ambientIntensity;

  float diffuseFactor = max(dot(normalize(normal), normalize(dirLight.direction)), 0.0f);
  vec4 diffuseColor = vec4(dirLight.color, 1.0f)* dirLight.diffuseIntensity* diffuseFactor;

  color = texture(theTex, texCoord)* (ambientColor+diffuseColor);
}
