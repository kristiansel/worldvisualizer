#include "comprgassim.h"

ComprGasSim::ComprGasSim(unsigned int dim, float* height_data, unsigned int x_res, unsigned int y_res) :
    m_params(
                 {
                    0.1 //viscosity
                 }
            ),
    m_simParams(
                 {
                     0.3, //dl
                 }
            ),
    m_timeStep(0),
    m_time(0.0)
{
    //ctor

    unsigned int n_valsPerCell = 4;

    float* source = new float [n_valsPerCell*dim*dim];
    float* vx_vy_p_rho = new float [n_valsPerCell*dim*dim];
    //float* transp = new float [n_valsPerCell*dim*dim]; // transported quantities

    for (int i = 0; i<dim; i++)
    {
        for (int j = 0; j<dim; j++)
        {
            float* s_node = &source[n_valsPerCell*i*dim+n_valsPerCell*j];
            s_node[0] = 0.0; s_node[1] = 0.0; s_node[2] = 0.0; s_node[3] = 0.0;

            float* vp_node = &vx_vy_p_rho[n_valsPerCell*i*dim+n_valsPerCell*j];
            vp_node[0] = 0.0; vp_node[1] = 0.0; vp_node[2] = 1.0; vp_node[3] = 0.0; // set pressure to one

            //float* tr_node = &transp[n_valsPerCell*i*dim+n_valsPerCell*j];
            //tr_node[0] = 0.0; tr_node[1] = 0.0; tr_node[2] = 0.0; tr_node[3] = 0.0; // Transported quantities

            if (i/(float)(dim) > 0.0 && i/(float)(dim) < 0.2 &&
                j/(float)(dim) > 0.2 && j/(float)(dim) < 0.4)
            {
                //s_node[0] = 1.f;
                //s_node[1] = 1.f;

                vp_node[0] = 3.f;
                vp_node[1] = 3.f;
                //
                vp_node[3] = 1.f; // set transported quantity 1

                //tr_node[0] = 1.f;
                //tr_node[1] = 1.f;
            }
        }
    }

    // initialize and connect the shaders
    m_gasJacobiShader.init(dim, vx_vy_p_rho, source, // Create the source texture
                         m_params.viscosity,  // use as uniform
                         m_simParams.dl);  // use as uniform

    m_gasVisualShader.init(dim, m_simParams.dl);

    delete [] source;
    delete [] vx_vy_p_rho;

    m_dim = dim;
    m_xres = x_res;
    m_yres = y_res;


}

void ComprGasSim::step(float dt)
{
    glViewport(0, 0, m_dim, m_dim);

    GLuint solution_tex = m_gasJacobiShader.step(dt, 50, true); // iterations
//
//    // glViewport(0, 0, m_xres, m_xres);
//
//    // m_gasVisualShader.draw(solution_tex);

}

ComprGasSim::~ComprGasSim()
{
    //dtor
}
