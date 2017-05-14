// fragment shader uses interpolated 3D tex coords to sample cube map
#version 140
 
uniform samplerCube skyboxSampler;
in vec3 texCoord_v;
out vec4 color_f;
  
void main() {
	color_f = texture(skyboxSampler, texCoord_v);
		
}