#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of a diffuse light, position
uniform vec3 LightDirection;

// Color of the light, ambient 
uniform vec3 lightColor;

// Position ov camera/viewer, spectacular
uniform vec3 viewPos;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;
in vec3 FragPos;
in vec3 normal_specular;

// The final color
out vec4 FragmentColor;

void main() {
  // Compute diffuse lighting
  vec3 norm = normalize(normal_specular);
  vec3 lightDir = normalize(LightDirection - FragPos);
  float diff = max(dot(normal, vec4(lightDir, 1.0f)), 0.0f);
  vec4 diffuse = vec4(diff * lightColor, 1.0f);
  
  // Compute ambient lighting 
  float ambientStrength = 0.1;
  vec4 ambient = vec4(ambientStrength * lightColor, 1.0f);
  
  // Compute spectacular lighting
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm); 
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec4 specular = vec4(specularStrength * spec * lightColor, 1.0f); 

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * (diffuse + ambient + specular);
  FragmentColor.a = Transparency;
}
