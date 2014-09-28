# version 330 core

out vec4 x_out; // (v_x, v_y, pressure, unused)
in vec2 texcoord;

uniform sampler2D x_pcorr; // v_x, v_y, p, ??
uniform sampler2D h_map;

uniform float rho;
uniform float mu;
uniform float dl;
uniform float di;
uniform float dt;

void main()
{
    vec4 field = texture(x_pcorr, texcoord);
    vec4 field_N = texture(x_pcorr, texcoord + vec2(0.0, di)); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_pcorr, texcoord + vec2(0.0, -di));
    vec4 field_E = texture(x_pcorr, texcoord + vec2(di, 0.0));
    vec4 field_W = texture(x_pcorr, texcoord + vec2(-di, 0.0));

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    float dp_dx = dfield_dx.b;
    float dp_dy = dfield_dy.b;

    float inv_dens = 1.f/rho;

    float dvx_dt_press = -inv_dens * dp_dx;
    float dvy_dt_press = -inv_dens * dp_dy;

    float vx = field.r + dt * dvx_dt_press;
    float vy = field.g + dt * dvy_dt_press;

    // advection of transported quantity (alpha channel)
    //upwinding?
    float vxda_dx = (vx > 0) ? vx*(field.a - field_W.a)/dl : vx*(field_E.a - field.a)/dl;
    float vyda_dy = (vy > 0) ? vy*(field.a - field_S.a)/dl : vy*(field_N.a - field.a)/dl;

    float a_out = field.a - dt * (vxda_dx + vyda_dy);

    // generate/consume some a...
    float h_map_sample = texture(h_map, texcoord).r;

    float da_dt_source = (h_map_sample < 0) ? sin(2*0.006283185*h_map_sample)/10.f : 0; // 2*pi*x/1000 == 0.006283185

    a_out = a_out + dt*da_dt_source;



    x_out = vec4(vx, vy, field.b, a_out);

    //x_out = field;

// serial working implementation
//            //pressure grad
//            float dvx_dt_press = -1.f/m_params.density * d_dx(m_next->p, i, j);
//
//            m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_press;
//
//            float dvy_dt_press = -1.f/m_params.density * d_dy(m_next->p, i, j);
//
//            m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_press;
//
//            //non-coupled transport (central difference)
//            float dconc_dt_adv = -m_next->v_x(i,j) * d_dx(m_transp.conc, i, j)
//                                 -m_next->v_y(i,j) * d_dy(m_transp.conc, i, j);
//
//            m_transp.conc(i,j) = std::max(m_transp.conc(i,j) + m_simParams.dt * dconc_dt_adv, 0.f);
}
