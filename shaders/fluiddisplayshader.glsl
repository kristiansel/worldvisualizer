# version 330 core

out vec3 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_field; // v_x, v_y, p, ??

void main()
{
    vec2 tex_coy_rev = vec2(texcoord.s, 1.0-texcoord.t);

    vec4 field = texture(x_field, tex_coy_rev);

    // -10 to 10
    float p_norm = field.b/100.f;

    //float r = (p_norm>0) ? p_norm : 0;
    //float g = (p_norm<0) ? 1.f+p_norm : 1.f-p_norm;
    //float b = (p_norm<0) ? -p_norm : 0;

    float r = 2-abs(8*p_norm-0.4);
    float g = 2-abs(8*p_norm);
    float b = 2-abs(8*p_norm+0.4);


    x_out = vec3(r,g,b);

    //x_out = vec3(1.0, 0.5, 0.1);
}
