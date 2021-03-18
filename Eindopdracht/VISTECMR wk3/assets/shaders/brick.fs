#version 330

const float zoom = 100.0;
const vec3 brickColor = vec3(0.45,0.29,0.23);
const vec3 lineColor = vec3(0.845);
const float brickSize = 1.5;

uniform float time;
in vec2 tex;

void main()
{
    vec2 mtex = tex;
    mtex *= zoom;               //Zoom uit op de stenen
	float xOffset = step(0, (cos(mtex.y))); //Pak de xOffset voor het rijen verschil
    bool edgeX = 0.99 < abs(cos(mtex.x / 2 + xOffset * brickSize)); //Is de X coordinaat een brick of cement
    bool edgeY = 0.99 < abs(sin(mtex.y));   //Is de Y coordinaat een brick of cement

    if(edgeX || edgeY)  
	    gl_FragColor = vec4(lineColor,1.0);
    else
        gl_FragColor = vec4(brickColor,1.0);
}