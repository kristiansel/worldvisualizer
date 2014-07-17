#include "heightmap.h"

HeightMap::HeightMap(unsigned int size_exponent) :
    m_map_data(size_exponent),
    m_cellSize(1000.0) // 1000 meters per cell
{
    //ctor
}

float HeightMap::sample(float x, float y, Interpolation interp)
{
    return m_map_data(x/m_cellSize, y/m_cellSize);
}

float HeightMap::sampleI(unsigned int i, unsigned int j, Interpolation interp)
{
    return m_map_data(i,j);
}

HeightMap::~HeightMap()
{
    //dtor
}
