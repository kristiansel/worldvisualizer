# version 330 core
layout(location = 0) in vec2 position;
out vec2 texcoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = 1*(vec2(0.0, 1.0)+vec2(1.0, -1.0)*(position * vec2(0.5) + vec2(0.5)));
    texcoord.t = 1.0-texcoord.t;
}
