// fragment shader uses interpolated 3D tex coords to sample cube map
#version 140
 
uniform samplerCube skyboxSampler;
in vec3 texCoord_v;
out vec4 color_f;
uniform bool fogActive;
  
void main() {
	color_f = texture(skyboxSampler, texCoord_v);
	if (fogActive) {
		color_f = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	}
}