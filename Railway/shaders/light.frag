#version 140

struct Material {         
  vec3  ambient;            
  vec3  diffuse;            
  vec3  specular;          
  float shininess;          
  bool  useTexture;        
};

uniform sampler2D texSampler;  // sampler for the texture access
uniform Material material;     // current material

smooth in vec4 color_v;        // incoming fragment color (includes lighting)
smooth in vec2 texCoord_v;     // fragment texture coordinates
out vec4       color_f;        // outgoing fragment color

void main() {
	color_f = color_v;

  // if material has a texture -> apply it
	if (material.useTexture) {
		color_f =  color_v * texture(texSampler, texCoord_v);
	}
}
