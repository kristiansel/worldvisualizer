#include "weather.h"

Weather::Weather(ClosedMap<float>& height_map, unsigned int dim) :
//    m_size(height_map.getSizeExp()),
//    m_field( {ClosedMap<float>(height_map.getSizeExp()),
//              ClosedMap<float>(height_map.getSizeExp()),
//              ClosedMap<float>(height_map.getSizeExp())} ),
//    m_temp( {ClosedMap<float>(height_map.getSizeExp()),
//              ClosedMap<float>(height_map.getSizeExp()),
//              ClosedMap<float>(height_map.getSizeExp())} ),
//    m_previous(&m_field),
//    m_next(&m_temp),
//    m_timeStep(0),
//    m_time(0.f),
//    m_params( {nullptr,
//             ClosedMap<float>(height_map.getSizeExp()),
//             ClosedMap<float>(height_map.getSizeExp()),
//             1.f,   // Density
//             0.1f} ), // Viscosity
//    m_simParams( {1.f, 0.1f} ), // dl, dt
//    m_calc( {ClosedMap<float>(height_map.getSizeExp())} ),
//    m_transp( {ClosedMap<float>(height_map.getSizeExp())} )

    m_size(7),
    m_field( {ClosedMap<float>(log2(dim)),
              ClosedMap<float>(log2(dim)),
              ClosedMap<float>(log2(dim))} ),
    m_temp( {ClosedMap<float>(log2(dim)),
              ClosedMap<float>(log2(dim)),
              ClosedMap<float>(log2(dim))} ),
    m_previous(&m_field),
    m_next(&m_temp),
    m_timeStep(0),
    m_time(0.f),
    m_params( {nullptr,
             ClosedMap<float>(log2(dim)),
             ClosedMap<float>(log2(dim)),
             1.f,   // Density
             0.5f} ), // Viscosity
    m_simParams( {1.f, 0.1f} ), // dl, dt
    m_calc( {ClosedMap<float>(log2(dim))} ),
    m_transp( {ClosedMap<float>(log2(dim))} )
{
    //ctor

    std::cout << "initializing weather field variables... ";
    // initialize field variables
    for (int i = 0; i<m_field.v_x.getDimension(); i++)
    {
        for (int j = 0; j<m_field.v_x.getDimension(); j++)
        {

            m_previous->v_x(i,j) = 0.f;
            m_previous->v_y(i,j) = 0.f;
            m_previous->p(i,j) = 1.f;

            m_next->v_x(i,j) = 0.f;
            m_next->v_y(i,j) = 0.f;
            m_next->p(i,j) = 1.f;



            m_params.S_vx(i,j) = 0.f ;
            m_params.S_vy(i,j) = 0.f ;

//            m_params.S_vx(i,j) = std::min(std::abs(height_map(i, j)/4000.f), 1.f);
//            m_params.S_vy(i,j) = std::min(std::abs(height_map(i, j)/4000.f), 1.f);

            if (i>20 && i<40 && j>20 && j<40)
            {
                m_params.S_vx(i,j) = 5.0f ;
                m_params.S_vy(i,j) = 0.f ;
            }

//            if (i>200 && i<230 && j>200 && j<230)
//            {
//                m_params.S_vx(i,j) = -1.0f ;
//                m_params.S_vy(i,j) = 0.f ;
//            }

//            if (i%40>30)
//                m_params.S_vx(i,j) = 1.0f ;
//            else if (i%40>20)
//                m_params.S_vx(i,j) = 0.0f ;
//            else if (i%40>10)
//                m_params.S_vx(i,j) = -1.f ;
//            else
//                m_params.S_vx(i,j) = 0.0f

            m_transp.conc(i,j) = 0.f;
//
            if (i>180 && i<220 && j>150 && j<200)
            {
                m_transp.conc(i,j) = 1.f;
            }

            if (i>20&& i<80 && j>20 && j<80)
            {
                m_transp.conc(i,j) = 1.f;
            }

//            m_transp.conc(i,j) = (float)(rand()%100)/100.f;
//
//            m_transp.conc(i,j) = std::min(std::abs(height_map(i, j)/4000.f), 1.f);

        }
    }
    std::cout << "done!\n";

    m_params.h_g = &height_map;
}

void Weather::step()
{
    m_calc.max_div = 0.0;


    for (int i = 0; i<m_field.v_x.getDimension(); i++)
    {
        for (int j = 0; j<m_field.v_x.getDimension(); j++)
        {
//            Advection (central difference, consider upwinding)
            float dvx_dt_adv = -m_previous->v_x(i,j) * d_dx(m_previous->v_x, i, j)
                               -m_previous->v_y(i,j) * d_dy(m_previous->v_x, i, j);

            m_next->v_x(i,j) = m_previous->v_x(i,j) + m_simParams.dt * dvx_dt_adv;

            float dvy_dt_adv = -m_previous->v_x(i,j) * d_dx(m_previous->v_y, i, j)
                               -m_previous->v_y(i,j) * d_dy(m_previous->v_y, i, j);

            m_next->v_y(i,j) = m_previous->v_y(i,j) + m_simParams.dt * dvy_dt_adv;

            //Advection (upwinding)
//            m_next->v_x(i,j) = m_next->v_x(i-(int)(m_simParams.dt/m_simParams.dl*m_previous->v_y(i,j)),
//                                           j-(int)(m_simParams.dt/m_simParams.dl*m_previous->v_x(i,j)));
//            m_next->v_y(i,j) = m_next->v_y(i-(int)(m_simParams.dt/m_simParams.dl*m_previous->v_y(i,j)),
//                                           j-(int)(m_simParams.dt/m_simParams.dl*m_previous->v_x(i,j)));

            //Diffusion (consider implicit/stable)
            float dvx_dt_diff = m_params.viscosity/m_params.density *
                              ( d2_dx2(m_previous->v_x, i, j)
                              + d2_dy2(m_previous->v_x, i, j) );

            m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_diff;

            float dvy_dt_diff = m_params.viscosity/m_params.density *
                              ( d2_dx2(m_previous->v_y, i, j)
                              + d2_dy2(m_previous->v_y, i, j) );

            m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_diff;

            //Velocity forcing term
            float dvx_dt_src = 0.5*(-m_previous->v_x(i,j)+m_params.S_vx(i,j));
//            float dvx_dt_src = (m_time<3.f) ? m_params.S_vx(i,j) : 0.f;

            m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_src;
//            m_next->v_x(i,j) = m_params.S_vx(i,j);

            float dvy_dt_src = 0.5*(-m_previous->v_y(i,j)+m_params.S_vy(i,j));
//            float dvy_dt_src = (m_time<3.f) ? m_params.S_vy(i,j) : 0.f;

            m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_src;
//            m_next->v_y(i,j) = m_params.S_vy(i,j);

            m_calc.div(i,j) = d_dx(m_next->v_x, i, j) + d_dy(m_next->v_y, i, j);

            if (std::abs(m_calc.div(i,j))>m_calc.max_div) m_calc.max_div = std::abs(m_calc.div(i,j));
        }
    }

    // do some iterations of pressure correction
    for (int k = 0; k<50; k++)
    {
        for (int i = 0; i<m_field.v_x.getDimension(); i++)
        {
            for (int j = 0; j<m_field.v_x.getDimension(); j++)
            {
                m_next->p(i,j) = ( m_next->p(i-1,j) + m_next->p(i+1,j)
                                 + m_next->p(i,j-1) + m_next->p(i,j+1)
                                 - m_simParams.dl*m_simParams.dl*m_calc.div(i,j)*m_params.density/m_simParams.dl ) / 4.f;
            }
        }
    }


    for (int i = 0; i<m_field.v_x.getDimension(); i++)
    {
        for (int j = 0; j<m_field.v_x.getDimension(); j++)
        {
            //pressure grad
            float dvx_dt_press = -1.f/m_params.density * d_dx(m_next->p, i, j);

            m_next->v_x(i,j) = m_next->v_x(i,j) + m_simParams.dt * dvx_dt_press;

            float dvy_dt_press = -1.f/m_params.density * d_dy(m_next->p, i, j);

            m_next->v_y(i,j) = m_next->v_y(i,j) + m_simParams.dt * dvy_dt_press;

            //non-coupled transport (upwinding)
//            m_transp.conc(i,j) = m_transp.conc(i-(int)(m_simParams.dt/m_simParams.dl*m_next->v_x(i,j)),
//                                               j-(int)(m_simParams.dt/m_simParams.dl*m_next->v_y(i,j)));
//
            //non-coupled transport (central difference)
            float dconc_dt_adv = -m_next->v_x(i,j) * d_dx(m_transp.conc, i, j)
                                 -m_next->v_y(i,j) * d_dy(m_transp.conc, i, j);

            m_transp.conc(i,j) = std::max(m_transp.conc(i,j) + m_simParams.dt * dconc_dt_adv, 0.f);
        }
    }

    Field* temp = m_previous;
    m_previous = m_next;
    m_next = temp;

    m_time+=m_simParams.dt;

    std::cout << "max divergence: " << m_calc.max_div << "\n";

}

float Weather::d_dx(ClosedMap<float>& var, unsigned int i, unsigned int j)
{
    return (var(i,j+1)-var(i,j-1))/(2*m_simParams.dl);
}

float Weather::d_dy(ClosedMap<float>& var, unsigned int i, unsigned int j)
{
    return (var(i+1,j)-var(i-1,j))/(2*m_simParams.dl);
}


float Weather::d2_dx2(ClosedMap<float>& var, unsigned int i, unsigned int j)
{
    return (var(i,j+1)-2*var(i,j)+var(i,j-1))/(m_simParams.dl * m_simParams.dl);
}

float Weather::d2_dy2(ClosedMap<float>& var, unsigned int i, unsigned int j)
{
    return (var(i+1,j)-2*var(i,j)+var(i-1,j))/(m_simParams.dl * m_simParams.dl);
}

Weather::~Weather()
{
    //dtor
}
