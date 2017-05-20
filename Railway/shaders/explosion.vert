#version 140

uniform mat4 PVMmatrix;   
uniform mat4 Vmatrix;     
uniform mat4 Mmatrix;     

in vec3 position;      
in vec2 texCoord;          

smooth out vec2 texCoord_v; 
smooth out  vec4  posFog;	

void main() {
  gl_Position = PVMmatrix * vec4(position, 1);   
   posFog = (Vmatrix * Mmatrix * vec4(position, 1.0));
  texCoord_v = texCoord;

}
