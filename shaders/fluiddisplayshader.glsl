# version 330 core

out vec4 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_field; // v_x, v_y, p, ??
uniform float di;
uniform float dl;

void main()
{
    vec4 field = texture(x_field, texcoord);
    vec4 field_out = field;
    vec4 fieldsc_out = field+vec4(0.5);
    vec4 a_out = vec4(field.aaa, 1.0);

    // -10 to 10
    float p_norm = field.b/100.f;

    float r = 2-abs(8*p_norm-0.4);
    float g = 2-abs(8*p_norm);
    float b = 2-abs(8*p_norm+0.4);

    vec4 pres_mapped_out = vec4(r,g,b, 1.0);

    vec4 field_N = texture(x_field, texcoord + vec2(0.0, di)); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_field, texcoord + vec2(0.0, -di));
    vec4 field_E = texture(x_field, texcoord + vec2(di, 0.0));
    vec4 field_W = texture(x_field, texcoord + vec2(-di, 0.0));

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    float dvx_dx = dfield_dx.r;
    float dvy_dy = dfield_dy.g;

    float div = dvx_dx + dvy_dy;
    vec4 div_out = vec4(div, div, div, 1.0);


    vec4 combi_out = vec4(field.a, (div<0) ? -div*10 : 0, (div>0) ? div*10 : 0, 1.0);


    x_out = field_out; // vx, vy, p
    //x_out = fieldsc_out; // vx, vy, p
    //x_out = div_out; // divergence
    //x_out = a_out; // t
    //x_out = combi_out;
}
