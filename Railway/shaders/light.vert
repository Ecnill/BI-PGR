#version 140

struct Material {     
  vec3  ambient;       
  vec3  diffuse;      
  vec3  specular;      
  float shininess;    
  bool  useTexture;    
};

struct Light {         
  vec3  ambient;       
  vec3  diffuse;       
  vec3  specular;     
  vec3  position;      
  vec3  spotDirection; 
  float spotCosCutOff;
  float spotExponent;  
};
//-------------------------------------------------------
uniform sampler2D texSampler; 

in vec3 position;           
in vec3 normal;             
in vec2 texCoord;          

uniform float time;  
uniform Material material; 

uniform mat4 PVMmatrix;  
uniform mat4 Pmatrix;
uniform mat4 Vmatrix;       
uniform mat4 Mmatrix;       
uniform mat4 normalMatrix;  

uniform vec3 reflectorPosition;   
uniform vec3 reflectorDirection; 

smooth out vec2 texCoord_v;  
smooth out vec4 color_v;     
//-------------------------------------------------------
float dotNormalLight(vec3 vertexNormal, vec3 lightDirection) {
	return max(0.0f, dot(vertexNormal, lightDirection));
}
//-------------------------------------------------------
float dotReflectVertex(vec3 R, vec3 V) {
	return max(0.0f, dot(R, V));
}
//-------------------------------------------------------
vec4 directionalLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {
  vec3 ret = vec3(0.0);
  vec3 L = normalize(light.position);					
  vec3 R = reflect(-L, vertexNormal);						
  vec3 V = normalize(-vertexPosition);
  ret += material.diffuse * light.diffuse *  dotNormalLight(vertexNormal, L);
  ret += material.specular * light.specular * pow(dotReflectVertex(R, V), material.shininess);
  ret += material.ambient * light.ambient;
  return vec4(ret, 1.0);
}
//-------------------------------------------------------
vec4 spotLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {
  vec3 ret = vec3(0.0);
  vec3 L = normalize(light.position - vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  ret += material.diffuse * light.diffuse * dotNormalLight(vertexNormal, L);
  ret += material.specular * light.specular * pow(dotReflectVertex(R, V), material.shininess);
  ret += material.ambient * light.ambient;
  return vec4(ret, 1.0);
}
//-------------------------------------------------------
void main() {

	Light sun;
	float sunSpeed = 0.5f;
	sun.ambient  = vec3(0.0);
	sun.diffuse  = vec3(1.0, 1.0, 0.5f);
	sun.specular = vec3(1.0);
	sun.position = (Vmatrix * vec4(cos(time * sunSpeed), 0.0f, sin(time * sunSpeed), 0.0f)).xyz;

	Light trainReflector;
	trainReflector.ambient       = vec3(0.2f);
	trainReflector.diffuse       = vec3(1.0);
	trainReflector.specular      = vec3(1.0);
	trainReflector.spotCosCutOff = 0.95f;
	trainReflector.spotExponent  = 0.0;

	trainReflector.position = (Vmatrix * vec4(reflectorPosition, 1.0f)).xyz;
	trainReflector.spotDirection = normalize((Vmatrix * vec4(reflectorDirection, 0.0f)).xyz);

	vec3 vertexPosition = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;       
	vec3 vertexNormal   = normalize( (Vmatrix * normalMatrix * vec4(normal, 0.0) ).xyz);   

	vec3 globalAmbientLight = vec3(0.4f);
	vec4 outputColor = vec4(material.ambient * globalAmbientLight, 0.0);

	outputColor += directionalLight(sun, material, vertexPosition, vertexNormal);
	outputColor += spotLight(trainReflector, material, vertexPosition, vertexNormal);
	gl_Position = PVMmatrix * vec4(position, 1);   

	color_v = outputColor;
	texCoord_v = texCoord;
}
