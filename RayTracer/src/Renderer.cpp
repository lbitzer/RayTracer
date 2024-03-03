#include "Renderer.h"
#include "Walnut/Random.h"


void Renderer::OnResize(uint32_t width, uint32_t height)
{

    if (m_FinalImage)
    {
        if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
            return;

        m_FinalImage->Resize(width, height);
    }
    else
    {
        m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] m_ImageData;
    m_ImageData = new uint32_t[width * height];
    
}

void Renderer::Render()
{
    for (uint32_t i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++)
    {
        m_ImageData[i] = Walnut::Random::UInt();
        m_ImageData[i] |= 0xff000000;

        // check if the point satisfys the equation with 1 or more intersections
        // 1. Equation of the sphere, take the current position of the pixel, transform into (x,y) coordinate
        // 2. Equation of the ray - Has to do with the camera? Every pixel can be used to create a ray
        // 3. Discriminant to see if the ray intersects with the sphere or not
    }

    m_FinalImage->SetData(m_ImageData);

}
