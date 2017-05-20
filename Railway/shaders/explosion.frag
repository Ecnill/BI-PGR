#version 140

uniform float time;				
uniform mat4 Vmatrix;			
uniform sampler2D texSampler;	

smooth in vec3 position_v;		
smooth in vec2 texCoord_v;		

out vec4 color_f;				

uniform	vec4  fColor;				
uniform	float fStart;				
uniform	float fEnd;			
uniform bool  fog;	
smooth in vec4 posFog;			

float calcFog(vec4  fColor, float fStart, float fEnd, float d ){
	float f = (fEnd - d) / (fEnd - fStart);	
	f = 1.0 - clamp(f, 0.0, 1.0);
	return f;
}		

void main() {

  color_f =  color_f = texture(texSampler, texCoord_v);

   if (fog) {
    float fogCoord = abs(posFog.z / posFog.w);
	color_f = mix(color_f, fColor, calcFog(fColor, fStart, fEnd, fogCoord));
  }

}
