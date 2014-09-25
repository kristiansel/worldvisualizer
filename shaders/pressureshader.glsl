# version 330 core

out vec3 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_dash; // intermediate solution (only advection/diffusion/source applied)

uniform float rho;
uniform float mu;
uniform float dl;
uniform float di;
uniform float dt;

void main()
{
    vec2 tex_coy_rev = vec2(texcoord.s, 1.0-texcoord.t);

    vec4 field = texture(x_dash, tex_coy_rev);
    vec4 field_N = texture(x_dash, tex_coy_rev + vec2(0.0, di)); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_dash, tex_coy_rev + vec2(0.0, -di));
    vec4 field_E = texture(x_dash, tex_coy_rev + vec2(di, 0.0));
    vec4 field_W = texture(x_dash, tex_coy_rev + vec2(-di, 0.0));

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    float dvx_dx = dfield_dx.r;
    float dvy_dy = dfield_dy.g;

    float div = dvx_dx + dvy_dy;

    float next_p = (field_W.b + field_E.b + field_N.b + field_S.b - dl*dl*div*rho/dl) / 4.f;

    x_out = vec3(field.rg, next_p);

//    m_next->p(i,j) = ( m_next->p(i-1,j) + m_next->p(i+1,j)
//                                 + m_next->p(i,j-1) + m_next->p(i,j+1)
//                                 - m_simParams.dl*m_simParams.dl*m_calc.div(i,j)*m_params.density/m_simParams.dl ) / 4.f;

//    float r = my_texel.r;
//    float g = my_texel.g;
//    float b = my_texel.b;
//
//    float intermed_max = max(r, g);
//    float total_max = max(intermed_max, b);
//
//    float intermed_min = min(r, g);
//    float total_min = min(intermed_min, b);
//
//    x_out = vec3(total_max>1.0, total_min<0.0, 0.0);
    //x_out = vec3(1.0, 0.2, 0.4);
}
