// inspired by https://learnopengl.com/Lighting/Multiple-lights
#version 330
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};   
uniform Material material;

#define NO_LIGHTS 10  
struct Light {
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform Light lights[NO_LIGHTS];

// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of a diffuse light, position
uniform vec3 LightDirection;

// Color of the light, ambient 
uniform vec3 lightColor;

// Position ov camera/viewer, spectacular
uniform vec3 viewPos;

// 
uniform float ambientStrength;

// 
uniform float specularStrength;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
//in vec4 normal;
in vec3 normal;
in vec3 FragPos;
//in vec3 normal_specular;

// The final color
out vec4 FragmentColor;

vec3 CalculateLight(Light light, vec3 normal_, vec3 viewDir, vec3 FragPos)
{
    // light direction
    vec3 lightDir = normalize(light.direction - FragPos);
    
    // diffuse shading
    float diff = max(dot(normal_, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal_);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // combine results
    vec3 ambient  = light.color * light.ambient * material.ambient;
    vec3 diffuse  = light.color * light.diffuse  * material.diffuse * diff;
    vec3 specular = light.color * light.specular * material.specular * spec;
    return (ambient + diffuse + specular);
}

void main() {  
  // properties
  vec3 norm = normalize(normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  
  // define an output color value
  vec3 result = vec3(0.0);

  // add the directional light's contribution to the output for all point lights
  for(int i = 0; i < NO_LIGHTS; i++)
    result += CalculateLight(lights[i], norm, viewDir, FragPos);
  
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y)+ TextureOffset) * vec4(result, 1.0);
  FragmentColor.a = Transparency;
}
