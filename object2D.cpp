#include "lab_m1/Tema1/Object2D.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

namespace object2D
{
    Mesh* CreateSquare(const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill)
    {
        glm::vec3 corner = leftBottomCorner;

        std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length, 0), color),
            VertexFormat(corner + glm::vec3(0, length, 0), color)
        };

        Mesh* square = new Mesh(name);
        std::vector<unsigned int> indices = { 0, 1, 2, 3 };

        if (!fill) {
            square->SetDrawMode(GL_LINE_LOOP);
        }
        else {
            indices.push_back(0);
            indices.push_back(2);
        }

        square->InitFromData(vertices, indices);
        return square;
    }

    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill, bool half)
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        vertices.push_back(VertexFormat(center, color));
        if (half) {
            for (int i = 0; i <= 50; i++) {
                float angle = glm::pi<float>() * i / 50;
                float x = radius * cos(angle);
                float y = radius * sin(angle);
                vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
                indices.push_back(i + 1);
            }
            indices.push_back(1);
        }
        else {
            for (int i = 0; i <= 100; i++) {
                float angle = 2 * glm::pi<float>() * i / 100;
                float x = radius * cos(angle);
                float y = radius * sin(angle);
                vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
                indices.push_back(i + 1);
            }
            indices.push_back(1);
        }

        Mesh* circle = new Mesh(name);

        if (!fill) {
            circle->SetDrawMode(GL_LINE_LOOP);
        }
        else {
            circle->SetDrawMode(GL_TRIANGLE_FAN);
        }

        circle->InitFromData(vertices, indices);
        return circle;
    }

    Mesh* CreateTriangle(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill,
        bool start)
    {
        if (start) {
            glm::vec3 corner = leftBottomCorner;
            std::vector<VertexFormat> vertices =
            {
                VertexFormat(corner, color),
                VertexFormat(corner + glm::vec3(length, 0, 0), color),
                VertexFormat(corner + glm::vec3(length / 2, length, 0), color),
                VertexFormat(corner + glm::vec3(0, length, 0), color),
                VertexFormat(corner + glm::vec3(length, length, 0), color),
                VertexFormat(corner + glm::vec3(length / 2, 0, 0), color)
            };
            Mesh* triangle = new Mesh(name);
            std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 0, 3, 2 };
            if (!fill) {
                triangle->SetDrawMode(GL_LINE_LOOP);
            }
            triangle->InitFromData(vertices, indices);
            return triangle;
        }

        glm::vec3 corner = leftBottomCorner;
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length / 2, length, 0), color)
        };
        Mesh* triangle = new Mesh(name);
        std::vector<unsigned int> indices = { 0, 1, 2 };
        if (!fill) {
            triangle->SetDrawMode(GL_LINE_LOOP);
        }
        triangle->InitFromData(vertices, indices);
        return triangle;

    }

}
