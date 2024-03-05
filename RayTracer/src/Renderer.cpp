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
    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {

            glm::vec2 coord = {x / static_cast<float>(m_FinalImage->GetWidth()), y / static_cast<float>(m_FinalImage->GetHeight())};
            
            m_ImageData[y*m_FinalImage->GetWidth() + x] =  PerPixel(coord);

            // check if the point satisfys the equation with 1 or more intersections
            // 1. Equation of the sphere, take the current position of the pixel, transform into (x,y) coordinate
            // 2. Equation of the ray - Has to do with the camera? Every pixel can be used to create a ray
            // 3. Discriminant to see if the ray intersects with the sphere or not
        }
    }
    m_FinalImage->SetData(m_ImageData);

}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
    uint8_t r = (uint8_t)(coord.x * 255.0f);
    uint8_t g = (uint8_t)(coord.y * 255.0f);

    return 0xff000000 | (g << 8) | r;
}
