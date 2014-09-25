#include "gpuweather.h"

gpuWeather::gpuWeather(unsigned int dim) :
    m_result(ClosedMap<float>(log2(dim))),
    m_params(
                 {
                    ClosedMap<float>(log2(dim)), //S_vx
                    ClosedMap<float>(log2(dim)), //S_vy
                    1.0, //density
                    0.5  //viscosity
                 }
            ),
    m_simParams(
                 {
                     1.0, //dl
                     0.1  //dt
                 }
            ),
    m_timeStep(0),
    m_time(0.0)
{
    //ctor

    unsigned int n_valsPerCell = 3;

    float* source = new float [n_valsPerCell*dim*dim];
    float* vx_vy_p = new float [n_valsPerCell*dim*dim];

    for (int i = 0; i<dim; i++)
    {
        for (int j = 0; j<dim; j++)
        {
            float* s_node = &source[n_valsPerCell*i*dim+n_valsPerCell*j];
            s_node[0] = 0.0; s_node[1] = 0.0; s_node[2] = 0.0;

            float* vp_node = &vx_vy_p[n_valsPerCell*i*dim+n_valsPerCell*j];
            vp_node[0] = 0.0; vp_node[1] = 0.0; vp_node[2] = 1.0; // set pressure to one

            if (i/(float)(dim) > 0.0 && i/(float)(dim) < 0.2 &&
                j/(float)(dim) > 0.2 && j/(float)(dim) < 0.4)
            {
                //s_node[0] = 1.f;
                //s_node[1] = 1.f;

                vp_node[0] = 1.f;
                vp_node[1] = 1.f;
            }


        }
    }

    // initialize and connect the shaders
    m_advdiffShader.init(dim, vx_vy_p, source, // Create the source texture
                         m_params.density, // use as uniform
                         m_params.viscosity,  // use as uniform
                         m_simParams.dl);  // use as uniform

    delete [] source;
//
    m_pressShader.init(dim, m_advdiffShader.getXdashTextureID(),  // use as input texture
                       m_params.density, // use as uniform
                       m_params.viscosity,  // use as uniform
                       m_simParams.dl);  // use as uniform
//
    m_velCorrShader.init(dim, m_advdiffShader.getInitCondTextureID(),
                         m_params.density, // use as uniform
                         m_params.viscosity, // use as uniform
                         m_simParams.dl); // use as uniform
//
    m_fluidDispShader.init();

//    // load some data
//    m_pressShader.initialPressure(dim, /*pressure data*/);
//    m_velCorrShader.initialVelocity(dim, /*vx data*/, /*vy data*/);
//
//    m_advdiffShader.connect(m_velCorrShader.getFinalVelocity);



}

void gpuWeather::step(float dt)
{
    // switch to adv/diffusion shader
    m_advdiffShader.step(dt, false);
    GLuint tex_for_next = m_pressShader.step(dt, 50, false); // iterations
    GLuint solution_tex = m_velCorrShader.step(dt, tex_for_next, false);

    m_fluidDispShader.draw(m_advdiffShader.getInitCondTextureID());

    // extract some info


}

gpuWeather::~gpuWeather()
{
    //dtor
}
