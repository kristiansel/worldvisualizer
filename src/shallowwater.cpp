#include "shallowwater.h"

ShallowWater::ShallowWater(unsigned int dim, float* height_data, unsigned int x_res, unsigned int y_res) :
    m_params(
                 {
                    -9.81 //gravity
                 }
            ),
    m_simParams(
                 {
                     40, //dl
                 }
            ),
    m_timeStep(0),
    m_time(0.0),
    last_kicked(0)
{
    //ctor

    unsigned int n_valsPerCell = 4;

    float* u_v_h = new float [n_valsPerCell*dim*dim];

    for (int i = 0; i<dim; i++)
    {
        for (int j = 0; j<dim; j++)
        {

            float* vp_node = &u_v_h[n_valsPerCell*i*dim+n_valsPerCell*j];
            float* height_node = &height_data[i*dim+j];
            //vp_node[0] = 0.0; vp_node[1] = 0.0; vp_node[2] = std::max(10-*height_node, 0.f); vp_node[3] = 0.0; // set pressure to one
            vp_node[0] = 0.0; vp_node[1] = 0.0; vp_node[2] = 0.000; vp_node[3] = 0.0; // set pressure to one

            if (i/(float)(dim) > 0.4 && i/(float)(dim) < 0.6 &&
                j/(float)(dim) > 0.4 && j/(float)(dim) < 0.6)
            {
                vp_node[0] = 0.f;
                vp_node[1] = 0.f;
                //
                //vp_node[2] = (vp_node[2] > 0.0001) ? vp_node[2] + 3 : 0.0; // set initial fluid column height to 0.5
            }
        }
    }

    // initialize and connect the shaders
    m_shallowWaterShader.init(dim, u_v_h, height_data, // Create the source texture
                         m_params.gravity,  // use as uniform
                         m_simParams.dl);  // use as uniform

    m_waterDisplayShader.init(m_shallowWaterShader.getHeightTex());

    delete [] u_v_h;

    m_dim = dim;
    m_xres = x_res;
    m_yres = y_res;


}

void ShallowWater::step(float dt)
{
    glViewport(0, 0, m_dim, m_dim);

//    int n_div=1;
    GLuint solution_tex;
//    for (int i = 0; i<n_div; i++)
//    {
//        float dt_div = dt/(float)(n_div);
//        solution_tex = m_shallowWaterShader.step(dt_div, false); // iterations
//    }

    solution_tex = m_shallowWaterShader.step(dt, false); // iterations

    glViewport(0, 0, m_xres, m_xres);

    m_waterDisplayShader.draw(solution_tex);

    m_time = m_time+dt;
    int get_stride = 10;
    if(int(m_time)%get_stride == 0 && last_kicked!=int(m_time))
    {
        std::cout << "kicked \n";
        std::cout << "time = " << m_time << "\n";

        last_kicked = int(m_time);

        unsigned int n_valsPerCell = 4;
        float* u_v_h = new float [n_valsPerCell*m_dim*m_dim];

        m_shallowWaterShader.writeBackResults(m_dim, u_v_h);

        unsigned int dim = m_dim;

        float total_water = 0.f;

        for (int i = 0; i<dim; i++)
        {
            for (int j = 0; j<dim; j++)
            {
                float* vp_node = &u_v_h[n_valsPerCell*i*dim+n_valsPerCell*j];
                total_water+=vp_node[2];
            }
        }

        std::cout << "water content: " << total_water << "\n";
        std::cout << "relative     : " << total_water/(100*dim*dim) << "\n";

        delete [] u_v_h;
    }
}

void ShallowWater::stirWater(float rel_x, float rel_y)
{
    m_shallowWaterShader.stirWater(rel_x, rel_y);
}


ShallowWater::~ShallowWater()
{
    //dtor
}
