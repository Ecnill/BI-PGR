// fragment shader uses interpolated 3D tex coords to sample cube map
#version 140
 
uniform samplerCube skyboxSampler;
in vec3 texCoord_v;
out vec4 color_f;
uniform bool fogActive;
  
void main() {
	color_f = texture(skyboxSampler, texCoord_v);

	if (fogActive) {
		float z = gl_FragCoord.z / gl_FragCoord.w;
		float density = 0.5f;
		float e = 2.718f;
		float f = pow(e, -(density * z));
		vec4 fog_color = vec4(0.9f, 0.9f, 0.9f, 0.9f);
		color_f = f * color_f + (1.0f - f) * fog_color;
	}
}