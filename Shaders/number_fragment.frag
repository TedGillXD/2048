#version 330 core
out vec4 color;

in vec2 texCoord;

uniform sampler2D number;

void main(){
	//color = vec4(1, 1, 1, 1);
	color = texture(number, texCoord);
}