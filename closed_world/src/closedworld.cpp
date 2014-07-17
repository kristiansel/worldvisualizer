#include "closedworld.h"

double rand_range(double a, double b)
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

//template <class Func>
//void debugPrint(Func func, int inds_abs_norm, )

ClosedWorld::ClosedWorld(unsigned int size_exponent) :
    m_size_exponent(size_exponent),
    m_dimension((int)pow(2,size_exponent)),
    m_height_map(size_exponent),
    m_cellSize(1000.0),
    m_dayNum(0) // start day zero
{
    //ctor
    m_seed = std::time(0);
//    m_seed = 1398706648;
    //  1398237758 "strait island"
    //  1398237868 "two continents"
    //  1398333390 "two oceans"
    //  1398237320 "western reaches"
    //  1398706648 "tongue of the world"
    srand(m_seed);

    { // height map generation (to be moved)

        double range = 8000.0; // m

        // find the first (middle point) m_dimension/2
        m_height_map(m_dimension/2, m_dimension/2) = rand_range(-range, range);

//        std::cout << "first point: " << m_height_map(m_dimension/2, m_dimension/2) << "\n";

        // find the first corner
        m_height_map(0,0) = m_height_map(m_dimension/2, m_dimension/2) + rand_range(-range/2.0, range/2.0);

//        std::cout << "corner point: " << m_height_map(m_dimension, m_dimension) << "\n";
//        std::cout << "corner point: " << m_height_map(0, 0) << "\n";

        // find the first edge points
        m_height_map(m_dimension/2, 0) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);
        m_height_map(0, m_dimension/2) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);

//        std::cout << "edge point: " << m_height_map(m_dimension, m_dimension) << "\n";

        unsigned int max_size = m_dimension+1;
        unsigned int depth = 0;
        unsigned int length_scale = (max_size-1)/pow(2, depth);

        while (length_scale > 2) // each layer
        {
//            std::cout << "level===================================\n";

            length_scale = (max_size-1)/pow(2, depth);
//            std::cout << "length_scale = " << length_scale << "\n";
            unsigned int num_quads = (max_size-1)/length_scale;
//            std::cout << "num_quads = " << num_quads << "\n";

            int scale_factor = pow(2, depth);

            for (int j = 0; j<num_quads; j++)
            {
//                std::cout << "yrow========================\n";
                // update y
                unsigned int low_y = j*length_scale;
                unsigned int high_y = (j+1)*length_scale;
                for (int i = 0; i<num_quads; i++)
                {
//                    std::cout << "xrow============\n";
                    unsigned int low_x = i*length_scale;
                    unsigned int high_x = (i+1)*length_scale;

                    // fill in edges
//                    if (length_scale==2 && low_x == 32)
//                    {
//                        std::cout << "low_x = " << low_x << "\n";
//                        std::cout << "high_x = " << high_x << "\n";
//                        std::cout << "low_y = " << low_y << "\n";
//                        std::cout << "high_y = " << high_y << "\n";
//                    }


                    m_height_map((low_x+high_x)/2, low_y) = (m_height_map(low_x, low_y)+m_height_map(high_x, low_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_height_map((low_x+high_x)/2, high_y) = (m_height_map(low_x, high_y)+m_height_map(high_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_height_map(low_x, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_height_map(high_x, (low_y+high_y)/2) = (m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);

                    // middle
//                    m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0 +
//                                                 rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);

                    // Need to change this to make smoother middle (take all 9 surrounding into account with different weights)
                    m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = 0.414213562*(m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0
                                                 + 0.585786438*(m_height_map((low_x+high_x)/2, low_y) + m_height_map((low_x+high_x)/2, high_y) + m_height_map(low_x, (low_y+high_y)/2) + m_height_map(high_x, (low_y+high_y)/2))/4.0
                                                 + rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);
                }

            }
        depth++;
        }

        // adjust water lvl
        float av_height;
        float num_cells = m_dimension*m_dimension;

        for (int i = 0; i<m_dimension; i++)
        {
            for (int j = 0; j<m_dimension; j++)
            {
                av_height+=m_height_map(i,j)/num_cells;
            }
        }

        for (int i = 0; i<m_dimension; i++)
        {
            for (int j = 0; j<m_dimension; j++)
            {
                m_height_map(i,j)=m_height_map(i,j)-av_height;
            }
        }


        // debug height
        unsigned int x_side = 79 < m_dimension? 79 : m_dimension;
        unsigned int y_side = 46 < m_dimension? 46 : m_dimension;

//        std::cout << "seed = " << m_seed << ":\n";
        for (int j = 0; j<y_side; j++)
        {
            for (int i = 0; i<x_side; i++)
            {
                unsigned int x_ind = int((float)(i)/(float)(x_side)*m_dimension);
                unsigned int y_ind = int((float)(j)/(float)(y_side)*m_dimension);

                float height = m_height_map(y_ind, x_ind);
                if (height < 0.0)
                {
                    std::cout << "-";
    //                std::printf("\x1b[34m"     "-"     "\x1b[0m" "\n");
                }
                else if (height < 500.0)
                {
                    std::cout << "=";
                }
                else if (height < 1500.0)
                {
                    std::cout << "o";
                }
                else if (height < 3000.0)
                {
                    std::cout << "U";
                }
                else
                {
                    std::cout << "A";
                }
            }
            std::cout << "\n";
        } // finished printing debug map


//        std::cout << "\n";
////        std::cout << "seed = " << m_seed << ":\n";
//        for (int j = 0; j<y_side; j++)
//        {
//            for (int i = 0; i<x_side; i++)
//            {
//                float x = (float)(i)/(float)(x_side);
//                float y = (float)(j)/(float)(y_side);
//
//                float irad = IncidentRadiation(x, y, 0.3f);
//
//                if (irad < 0.2)
//                {
//                    std::cout << ".";
//    //                std::printf("\x1b[34m"     "-"     "\x1b[0m" "\n");
//                }
//                else if (irad < 0.4)
//                {
//                    std::cout << "-";
//                }
//                else if (irad < 0.6)
//                {
//                    std::cout << "=";
//                }
//                else if (irad < 0.8)
//                {
//                    std::cout << "x";
//                }
//                else
//                {
//                    std::cout << "X";
//                }
//            }
//            std::cout << "\n";
//        } // finished printing debug map

        // set initial pressures and fluid velocity
//        m_weatherMap.initialize()
    }
}

float* ClosedWorld::getRadiationData(float t)
{
    float* radiationData = new float [m_dimension*m_dimension];

    for (int i = 0; i<m_dimension; i++)
    {
        for (int j = 0; j<m_dimension; j++)
        {
            float x = j/(float)(m_dimension);
            float y = i/(float)(m_dimension);
            radiationData[m_dimension*i+j] = IncidentRadiation(x, y, t);
        }
    }

    return radiationData;
}




float ClosedWorld::IncidentRadiation(float x, float y, float t, Coords coords)
{
    float x_norm;
    float y_norm;
    float t_norm;
//    if (coords == Absolute)
//    {
//        // sizes
//        float world_scale = m_dimensions*m_cellSize;
//
//        // map x and y within the limits
//        if (x > world_scale
//
//        x_norm = x/(m_dimension*m_cellSize);
//        y_norm = y/(m_dimension*m_cellSize);
//        t =
//    }
    if (coords == Normalized)
    {
        x_norm = x;
        y_norm = y;
        t_norm = t;
    }
    else
    {
        std::cerr << "error in incident radiation\n";
    }

    float period_factor = 2*3.141592;
    float wave_function = 0.5+0.5*(0.5*(1-cos(period_factor*(y_norm+t_norm))));
    float pole_attenuation = 0.5+0.5*(0.5*(1-cos(period_factor*(y_norm))));

    return pole_attenuation*wave_function;
}

ClosedWorld::~ClosedWorld()
{
    //dtor
}

void ClosedWorld::stepWorld()
{
    m_dayNum++;
}

void ClosedWorld::getYearMonthDay(int& year, int& month, int& day)
{
    year = m_dayNum/daysInYear;
    int yearDay = m_dayNum%daysInYear;

    month = yearDay/daysInMonth + 1;
    day = m_dayNum%daysInMonth + 1;
}

unsigned int ClosedWorld::getDimension()
{
    return m_dimension;
}
