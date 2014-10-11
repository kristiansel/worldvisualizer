# version 330 core

// x_t1 is the field variables in the next time-step (u, v, h)

out vec4 x_t1; // next timestep, next jacobi iteration solution
in vec2 texcoord;

uniform sampler2D x_t0; // previous timestep accepted solution
uniform sampler2D height; // terrain height

uniform float g; // gravity 9.81 m/s2
uniform float dl; // spacial distance between each cell (m)
uniform float di; // distance in texcoord to next pixel (1/resolution)
uniform float dt;

void main()
{
    vec4 field = texture(x_t0, texcoord);
    vec4 field_N = texture(x_t0, texcoord + vec2(0.0, di)); // y goes up the screen inverse texco/pos
    vec4 field_S = texture(x_t0, texcoord + vec2(0.0, -di));
    vec4 field_E = texture(x_t0, texcoord + vec2(di, 0.0));
    vec4 field_W = texture(x_t0, texcoord + vec2(-di, 0.0));

    // extract field variables
    float u = field.r;
    float v = field.g;
    float h = field.b;

    // Calculate upwinding terms
    float udu_dx = (u > 0) ? u*(field.r - field_W.r)/dl : u*(field_E.r - field.r)/dl;
    float vdu_dy = (v > 0) ? v*(field.r - field_S.r)/dl : v*(field_N.r - field.r)/dl;

    float udv_dx = (u > 0) ? u*(field.g - field_W.g)/dl : u*(field_E.g - field.g)/dl;
    float vdv_dy = (v > 0) ? v*(field.g - field_S.g)/dl : v*(field_N.g - field.g)/dl;

    float udh_dx = (u > 0) ? u*(field.b - field_W.b)/dl : u*(field_E.b - field.b)/dl;
    float vdh_dy = (v > 0) ? v*(field.b - field_S.b)/dl : v*(field_N.b - field.b)/dl;

    // Calculate first derivatives
    float inv_2dl = 1.f/(2.0*dl);
    vec4 dfield_dx = (field_E - field_W) * inv_2dl;
    vec4 dfield_dy = (field_N - field_S) * inv_2dl;

    //float H = texture(height, texcoord).r;
    float H_N = texture(height, texcoord + vec2(0.0, di)).r; // y goes up the screen inverse texco/pos
    float H_S = texture(height, texcoord + vec2(0.0, -di)).r;
    float H_E = texture(height, texcoord + vec2(di, 0.0)).r;
    float H_W = texture(height, texcoord + vec2(-di, 0.0)).r;

//    float H_N = (field_N.b>0.0) ? texture(height, texcoord + vec2(0.0, di)).r : 0.f; // y goes up the screen inverse texco/pos
//    float H_S = (field_S.b>0.0) ? texture(height, texcoord + vec2(0.0, -di)).r : 0.f;
//    float H_E = (field_E.b>0.0) ? texture(height, texcoord + vec2(di, 0.0)).r : 0.f;
//    float H_W = (field_W.b>0.0) ? texture(height, texcoord + vec2(-di, 0.0)).r : 0.f;

    float dH_dx = (H_E - H_W) * inv_2dl;
    float dH_dy = (H_N - H_S) * inv_2dl;

    // test with flat under-surface
    //float dH_dx = 0;
    //float dH_dy = 0;
//
//    float du_dt = (h>0) ? g*(dfield_dx.b+dH_dx) -udu_dx - vdu_dy : 0.0;
//    float dv_dt = (h>0) ? g*(dfield_dy.b+dH_dy) -udv_dx - vdv_dy : 0.0;

    bool is_x_bound = (field_E.b <= 0 || field_W.b <= 0 || field.b <= 0);
    bool is_y_bound = (field_N.b <= 0 || field_S.b <= 0 || field.b <= 0);



    float grav_force_u = (!is_x_bound) ? g*(dfield_dx.b+dH_dx) : 0 ;
    float grav_force_v = (!is_y_bound) ? g*(dfield_dy.b+dH_dy) : 0 ;

    float net_rain_evap = (true) ? 0.000001 : 0.0; // m/s

    float b = 0.01; // damping coefficient;

    float du_dt = grav_force_u -udu_dx - vdu_dy - b*u;
    float dv_dt = grav_force_v -udv_dx - vdv_dy - b*v;
    float dh_dt = -h*(dfield_dx.r+dfield_dy.g) -udh_dx - vdh_dy + net_rain_evap;

    float u_out = u + dt * du_dt;
    float v_out = v + dt * dv_dt;
    float h_out = h + dt * dh_dt;

    // cap velocity for stability;
    //float CFL_max = 0.8*dl/dt;
    //u_out = (abs(u_out) < CFL_max) ? u_out : u_out/abs(u_out)*CFL_max;
    //v_out = (abs(v_out) < CFL_max) ? v_out : v_out/abs(v_out)*CFL_max;

//    bool non_fluid_filled = (field_E.b <= 0 && field_W.b <= 0 && field.b <= 0 && field_N.b <= 0 && field_S.b <= 0);
//    if (non_fluid_filled)
//    {
//        v_out = 0;
//        u_out = 0;
//    }

    bool dry_cell = (h < 0.0001);
    if (dry_cell)
    {
        u_out = 0;
        v_out = 0;
    }

    // output
    x_t1 = vec4(u_out, v_out, h_out, 1.0);

    //x_t1 = vec4(u, v, h, 1.0);

    //x_t1 = vec4(u, v, h, 1.0);

    //x_t1 = vec4(dH_dx>1, dH_dy>1, 0.0, 1.0);
}
