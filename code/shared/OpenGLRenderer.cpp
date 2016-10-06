#include <RadonFramework/precompiled.hpp>
#include "OpenGLRenderer.hpp"
#include <RadonFramework/System/Drawing/OpenGL.hpp>
#include <RadonFramework/Drawing/Canvas3D.hpp>

namespace RadonExample {

OpenGLRenderer::OpenGLRenderer()
:m_NextId(1)
{
    m_ShaderPool.Resize(1);
    m_ShaderPool(0).Generate();

    glCreateBuffers(1, &m_SharedUBO);
    glNamedBufferData(m_SharedUBO, sizeof(SharedTransformUniforms), &m_SharedTransformUniforms, GL_STREAM_DRAW);
}

void OpenGLRenderer::StartFrame()
{
    glClearColor(0.2,0.2,0.2,1.0);
    m_Canvas->Clear();

    m_ShaderPool(0).Bind();
}

void OpenGLRenderer::EndFrame()
{
    m_Canvas->SwapBuffer();
}

RF_Type::UInt32 OpenGLRenderer::Process(RF_Type::UInt32 EntityId, const RF_Draw::Path2D& Path)
{
    if(EntityId != 0)
    {
        OpenGLModel* data;
        m_Objects.Get(EntityId, reinterpret_cast<void*&>(data));
        if(data->Hash == Path.GetHash())
        {

        }
        else
        {
            Path.Visit(m_Tesselator);
            auto vertices = m_Tesselator.GetVertices();
            if(vertices.Count())
            {
                data->Update(vertices(0).Value, vertices.Count(), 2);
            }
            data->Hash = Path.GetHash();
        }
    }
    else
    {
        EntityId = m_NextId;
        ++m_NextId;

        Path.Visit(m_Tesselator);
        auto vertices = m_Tesselator.GetVertices();
        if(vertices.Count())
        {
            RF_Mem::AutoPointer<OpenGLModel> obj(new OpenGLModel);
            obj->Generate(vertices(0).Value, vertices.Count(), 2);
            m_ObjectPool.Resize(m_ObjectPool.Count() + 1);
            m_ObjectPool(m_ObjectPool.Count() - 1) = EntityId;
            m_Objects.Add(EntityId, obj.Release());
        }
    }
    return EntityId;
}

void OpenGLRenderer::Draw()
{
    for(RF_Type::Size i = 0; i < m_ObjectPool.Count(); ++i)
    {   
        OpenGLModel* obj = 0;
        m_Objects.Get(m_ObjectPool(i), reinterpret_cast<void*&>(obj));
        obj->Bind();
        obj->Draw();
    }
}

void OpenGLRenderer::SetCanvas(RF_Draw::Canvas3D& Canvas)
{
    m_Canvas = &Canvas;
}

void OpenGLRenderer::ResizedViewport()
{
    m_Canvas->MakeCurrent();
    m_SharedTransformUniforms.ModelView = m_Camera.GetMatrix();
    m_SharedTransformUniforms.ModelViewProjection = m_Projection.GetMatrix(RF_Geo::Viewtype::View3D) * m_SharedTransformUniforms.ModelView;
    m_SharedTransformUniforms.UIProjection = m_Projection.GetMatrix(RF_Geo::Viewtype::View2D);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_SharedUBO);
    glNamedBufferSubData(m_SharedUBO, 0, sizeof(SharedTransformUniforms), &m_SharedTransformUniforms);
}

}