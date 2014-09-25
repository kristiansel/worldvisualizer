# version 330 core

out vec3 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_prev; // v_x, v_y, p, divergence
uniform sampler2D source; // Sv_x, Sv_y, ?, ?

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

    vec4 field = texture(x_prev, tex_coy_rev);
    vec4 field_N = texture(x_prev, (tex_coy_rev.xy + vec2(0.0, dif))); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_prev, tex_coy_rev + vec2(0.0, -dif));
    vec4 field_E = texture(x_prev, tex_coy_rev + vec2(dif, 0.0));
    vec4 field_W = texture(x_prev, tex_coy_rev + vec2(-dif, 0.0));

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    float vx = field.r;
    float dvx_dx = dfield_dx.r;
    float dvx_dy = dfield_dy.r;

    float vy = field.g;
    float dvy_dx = dfield_dx.g;
    float dvy_dy = dfield_dy.g;

    float p = field.b;

    // Apply advection
    float dvx_dt_adv = -vx * dvx_dx - vy * dvx_dy ;
    float dvy_dt_adv = -vx * dvy_dx - vy * dvy_dy ;

    float out_vx = vx + dt * dvx_dt_adv;
    float out_vy = vy + dt * dvy_dt_adv;
    float out_p = p;

    // Calculate second derivatives
    float inv_dlsq = 1.f/(dl*dl);
    vec4 d2field_dx2 = (field_E - 2*field + field_W) * inv_dlsq;
    vec4 d2field_dy2 = (field_N - 2*field + field_S) * inv_dlsq;

    float d2vx_dx2 = d2field_dx2.r;
    float d2vx_dy2 = d2field_dy2.r;

    float d2vy_dx2 = d2field_dx2.g;
    float d2vy_dy2 = d2field_dy2.g;

    // Apply diffusion
    float nu = mu/rho;
    float dvx_dt_diff = nu * (d2vx_dx2 + d2vx_dy2);
    float dvy_dt_diff = nu * (d2vy_dx2 + d2vy_dy2);

    out_vx = out_vx + dt * dvx_dt_diff;
    out_vy = out_vy + dt * dvy_dt_diff;

    // Apply source
    //float dvx_dt_src = 0.5*(-m_previous->v_x(i,j)+m_params.S_vx(i,j));
    //float dvx_dt_src = (m_time<3.f) ? m_params.S_vx(i,j) : 0.f;
//
//                //Velocity forcing term
//            float dvx_dt_src = 0.5*(-m_previous->v_x(i,j)+m_params.S_vx(i,j));
////            float dvx_dt_src = (m_time<3.f) ? m_params.S_vx(i,j) : 0.f;
//
//            m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_src;
////            m_next->v_x(i,j) = m_params.S_vx(i,j);
//
//            float dvy_dt_src = 0.5*(-m_previous->v_y(i,j)+m_params.S_vy(i,j));
////            float dvy_dt_src = (m_time<3.f) ? m_params.S_vy(i,j) : 0.f;
//
//            m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_src;
////            m_next->v_y(i,j) = m_params.S_vy(i,j);
//
//            m_calc.div(i,j) = d_dx(m_next->v_x, i, j) + d_dy(m_next->v_y, i, j);




    vec3 val = vec3(out_vx, out_vy, out_p);

    // Try to output value outside 0-1 range;
    //val = vec3(-1, 2, 100);

    x_out = val;
    //x_out = vec4(0.5, 0.5, 0.8, 1.0);
}
