﻿#include "Renderer.h"
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
            coord = coord * 2.0f - 1.0f; // remap from 0->1 to -1->1
            m_ImageData[y*m_FinalImage->GetWidth() + x] =  PerPixel(coord);
        }
    }
    m_FinalImage->SetData(m_ImageData);

}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
    // check if the point satisfys the equation with 1 or more intersections
    // 1. Equation of the sphere, take the current position of the pixel, transform into (x,y) coordinate
    // 2. Equation of the ray - Has to do with the camera? Every pixel can be used to create a ray
    // 3. Discriminant to see if the ray intersects with the sphere or not
    
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    float radius = 0.5f;

    // Equation for sphere: (bx^2 + by^2)t^2 + 2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0

    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    // discriminant: b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant >= 0)
        return 0xffff00ff;

    return 0xff000000;
}
