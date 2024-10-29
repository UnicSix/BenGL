// Fragment shader
#version 330

in vec4 vCol;
in vec2 texCoord;

out vec4 color;

struct DirectionalLight{
  vec3 color;
  float ambientIntensity;
};

uniform sampler2D theTex;
uniform DirectionalLight dirLight;

void main()
{
  vec4 ambientColor = vec4(dirLight.color, 1.0f)*dirLight.ambientIntensity;
  color = texture(theTex, texCoord)*ambientColor;
}
