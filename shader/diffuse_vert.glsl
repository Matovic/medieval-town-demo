#version 330
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// This will be passed to the fragment shader
out vec2 texCoord;

// Normal to pass to the fragment shader
//out vec4 normal;
out vec3 normal;

// Fragmet position will be passed to the fragment shader
out vec3 FragPos;  

void main() {
  // Copy the input to the fragment shader
  texCoord = TexCoord;

  // Normal in world coordinates
  //normal = normalize(ModelMatrix * vec4(Normal, 0.0f));
  //Normal_ = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * Normal;
  // Fragment position in world coordinates
  FragPos = vec3(ModelMatrix * vec4(Position, 1.0));
  //normal_specular = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * Normal;
  normal = mat3(transpose(inverse(ModelMatrix))) * Normal;

  // Calculate the final position on screen
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
