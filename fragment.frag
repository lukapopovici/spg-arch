#version 400
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 tex1 = texture(texture1, TexCoord);
    vec4 tex2 = texture(texture2, TexCoord);
    
    // Use TexCoord.x to split the triangle
    // If x < 0.5, use first texture, else use second texture
    if(TexCoord.x < 0.5) {
        FragColor = tex1;  // Left half: first texture
    } else {
        FragColor = tex2;  // Right half: second texture
    }
}