#include <RadonFramework/precompiled.hpp>
#include "OpenGLModel.hpp"
#include <RadonFramework/System/Drawing/OpenGL.hpp>

namespace RadonExample {

void OpenGLModel::Generate(RF_Type::Float32* Vertices, RF_Type::Size VertexCount,
                           RF_Type::Size ElementsPerVertex)
{
    glGenBuffers(1, m_VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(RF_Type::Float32) * 
                 ElementsPerVertex, Vertices, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    m_VertexCount = VertexCount;
    glVertexAttribPointer(0, ElementsPerVertex, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLModel::Bind()
{
    glBindVertexArray(m_VAO);
}

void OpenGLModel::Unbind()
{
    glBindVertexArray(0);
}

void OpenGLModel::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
}

void OpenGLModel::Update(RF_Type::Float32 * Vertices, RF_Type::Size VertexCount,
                         RF_Type::Size ElementsPerVertex)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(RF_Type::Float32) *
                 ElementsPerVertex, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexCount * sizeof(RF_Type::Float32) *
                    ElementsPerVertex, Vertices);
}

}