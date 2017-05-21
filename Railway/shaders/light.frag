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

uniform bool fogActive;

smooth in vec4 color_v;        // incoming fragment color (includes lighting)
smooth in vec2 texCoord_v;     // fragment texture coordinates
out vec4       color_f;        // outgoing fragment color

//-------------------------------------------------------
void main() {
	color_f = color_v;
	if (material.useTexture) {
		color_f =  color_v * texture(texSampler, texCoord_v);
	}

	if (fogActive) {
		float z = gl_FragCoord.z;
		float density = 2.5f;
		float e = 2.718f;
		float f = pow(e, -(density * z));
		vec4 fog_color = vec4(0.9f, 0.9f, 0.9f, 0.9f);
		color_f += f * color_f + (1.0f - f) * fog_color;
	}
}
