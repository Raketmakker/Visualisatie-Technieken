#version 330

uniform float time;

in vec3 normal;

void main()
{
	
	vec3 lightDirection = normalize(vec3(cos(time * 3), 1, 1));
	vec3 viewDirection = vec3(0, 0, 1);
	
	float layers = 5.0;
	float shininess = 100.0;
	float ambient = 0.2;
	float deffuse = 0.8 * dot(normalize(normal), lightDirection);

	vec3 reflection = reflect(-lightDirection, normalize(normal));

	float specular = pow(max(0.0, dot(reflection, viewDirection)), shininess); 
	float factor = ambient + deffuse + specular;
	float color = roundEven(factor * layers) / layers;

	gl_FragColor = vec4(color, color, color, 1.0);
}