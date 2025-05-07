#version 400
out vec4 FragColor;
  
//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
//	if (FragColor.rgba == vec4(0,0,0,1))
	//	FragColor = vec4(ourColor,1.0f);
	//else
	//	FragColor = vec4(ourColor, 1.0f);
}
