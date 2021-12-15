// inspired by https://learnopengl.com/Advanced-Lighting/Bloom
#version 330

//layout (location = 0) out vec4 FragmentColor;
//layout (location = 1) out vec4 BrightColor;

// input color
in vec3 vertexColor;

// The final color
out vec4 FragmentColor;

// Additional overall color when not using per-vertex Color input
uniform vec3 OverallColor;

void main() {
  // Just pass the color to the output
  FragmentColor = vec4(vertexColor + OverallColor, 1.0f);
  
  // bloom filter
  /*float exposure = 1.0f;
  // check whether fragment output is higher than threshold, if so output as brightness 
  //if(brightness > 0.4)
  //      FragmentColor = vec4(FragmentColor.rgb, 1.0);
  //else
  //      FragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
        
  const float gamma = 2.2;
  vec3 hdrColor = FragmentColor.rgb;      
  vec3 bloomColor = FragmentColor.rgb;
  hdrColor += bloomColor; // additive blending
  // tone mapping
  vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  // also gamma correct while we're at it       
  result = pow(result, vec3(1.0 / gamma));
  FragmentColor = vec4(result, 1.0);*/
}
