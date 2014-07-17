# version 330 core

out vec4 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_prev;
uniform sampler2D source;

uniform float rho;
uniform float mu;
uniform float dl;
uniform float di;
uniform float dt;

void main()
{
    // float val = texture(layerTexture, texcoord).r;

    x_out = vec4(0.5, 0.5, 0.5, 1.0);
}
