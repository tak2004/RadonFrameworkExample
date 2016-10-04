#ifndef RE_OPENGLRENDERER_HPP
#define RE_OPENGLRENDERER_HPP

#include <RadonFramework/Collections/Array.hpp>
#include <RadonFramework/Collections/HashList.hpp>
#include <RadonFramework/Drawing/AbstractRenderer.hpp>
#include <RadonFramework/Drawing/Path2DTriangulation.hpp>
#include <RadonFramework/Drawing/Camera.hpp>
#include <RadonFramework/Math/Geometry/Matrix.hpp>
#include <RadonFramework/Math/Geometry/ProjectionMatrix.hpp>
#include "OpenGLModel.hpp"
#include "OpenGLShader.hpp"

namespace RadonFramework { namespace Drawing { class Canvas3D; } }

namespace RadonExample {

class OpenGLRenderer: public RF_Draw::AbstractRenderer
{
public:
    OpenGLRenderer();
    virtual void StartFrame() override;
    virtual void EndFrame() override;
    virtual RF_Type::UInt32 Process(RF_Type::UInt32 EntityId, const RF_Draw::Path2D& Path) override;
    virtual void Draw() override;
    void SetCanvas(RF_Draw::Canvas3D& Canvas);
    OpenGLRenderer& operator=(const OpenGLRenderer& Other)
    {
        return *this;
    }
    virtual void ResizedViewport() override;
private:
    RF_Draw::Canvas3D* m_Canvas;
    RF_Draw::Path2DTriangulation m_Tesselator;
    RF_Collect::Array<RF_Type::UInt32> m_ObjectPool;
    RF_Collect::Array<OpenGLShader> m_ShaderPool;
    RF_Collect::HashList m_Objects;
    RF_Draw::Camera m_Camera;
    RF_Geo::ProjectionMatrix m_Projection;
    struct SharedTransformUniforms
    {
        RF_Geo::Mat4f ModelView;
        RF_Geo::Mat4f ModelViewProjection;
        RF_Geo::Mat4f UIProjection;
    } m_SharedTransformUniforms;
    RF_Type::UInt32 m_SharedUBO;
    RF_Type::UInt32 m_NextId;
};

}

#endif //!RE_OPENGLRENDERER_HPP