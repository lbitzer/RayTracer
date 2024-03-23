#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils
{
    static uint32_t ConvertToRGBA(const glm::vec4& color)
    {
        uint8_t r = (uint8_t)(color.r * 255.0f);
        uint8_t g = (uint8_t)(color.g * 255.0f);
        uint8_t b = (uint8_t)(color.b * 255.0f);
        uint8_t a = (uint8_t)(color.a * 255.0f);

        uint32_t result = (a << 24) | (b <<16) | (g << 8) | r;
        return result;
    }
}

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
            glm::vec4 color = PerPixel(coord);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f)); // manual clamping
            m_ImageData[y*m_FinalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
        }
    }
    m_FinalImage->SetData(m_ImageData);

}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
    // check if the point satisfys the equation with 1 or more intersections
    // 1. Equation of the sphere, take the current position of the pixel, transform into (x,y) coordinate
    // 2. Equation of the ray - Has to do with the camera? Every pixel can be used to create a ray
    // 3. Discriminant to see if the ray intersects with the sphere or not
    
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
    glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
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

    if (discriminant < 0.0f)
    {
        return glm::vec4(0, 0, 0, 1);
    }

    glm::vec3 sphereColor(1, 0, 1);
    
    // Rest of Quadratic Formula

    float t0 = (-b + sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b - sqrt(discriminant)) / (2.0f * a); // Since a is calculated by taking the dot product with itself, it will always be position, therefore this t will always be the smallest and therefore closest one.

    // Calculating Hit Positions (a + bt)
    //glm::vec3 h0 = rayOrigin + rayDirection * t0;
    glm::vec3 hitPoint = rayOrigin + rayDirection * t1;
    glm::vec3 normal = glm::normalize(hitPoint);

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

    float d = glm::dot(normal, -lightDir); // since the dot product of two unit vectors is the same as the cos of the angle between them
    d = glm::max(d, 0.0f); // if the face is facing 90 degrees away from the light source, that's as dark as it will get, since there is no other light source or any surfaces to reflect off of yet, so we don't need to handle any angles greater than 90 degrees.

    //sphereColor = normal * 0.5f + 0.5f; // Converts range from -1 to 1 into 0 to 1
    sphereColor *= d; // I want to use the angle between the light source and the normal to scale the intensity of the color on the sphere
    return glm::vec4(sphereColor, 1.0f);
}
