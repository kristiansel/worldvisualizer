# version 330 core

out vec3 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_pcorr; // v_x, v_y, p, divergence

uniform float rho;
uniform float mu;
uniform float dl;
uniform float di;
uniform float dt;

void main()
{
    // Get field data around point
    float dif = di;

    vec2 tex_coy_rev = vec2(texcoord.s, 1.0-texcoord.t);

    vec4 field = texture(x_pcorr, tex_coy_rev);
    vec4 field_N = texture(x_pcorr, tex_coy_rev + vec2(0.0, dif)); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_pcorr, tex_coy_rev + vec2(0.0, -dif));
    vec4 field_E = texture(x_pcorr, tex_coy_rev + vec2(dif, 0.0));
    vec4 field_W = texture(x_pcorr, tex_coy_rev + vec2(-dif, 0.0));

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    float dp_dx = dfield_dx.b;
    float dp_dy = dfield_dy.b;

    float inv_dens = 1.f/rho;

    float dvx_dt_press = inv_dens * dp_dx;
    float dvy_dt_press = inv_dens * dp_dy;

    float vx = field.r + dt * dvx_dt_press;
    float vy = field.g + dt * dvy_dt_press;

    x_out = vec3(vx, vy, field.b);

//    //pressure grad
//    float dvx_dt_press = -1.f/m_params.density * d_dx(m_next->p, i, j);
//
//    m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_press;
//
//    float dvy_dt_press = -1.f/m_params.density * d_dy(m_next->p, i, j);
//
//    m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_press;


    x_out = field.xyz;
}
