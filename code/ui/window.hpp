#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

#include <RadonFramework/Drawing/AbstractRenderer.hpp>
#include <RadonFramework/Drawing/Path2DTriangulation.hpp>

namespace RadonExample {

class OpenGLRenderer: public RF_Draw::AbstractRenderer
{
public:
    virtual void StartFrame() override;
    virtual void EndFrame() override;
    virtual void Process(const RF_Draw::Path2D& Path) override;
    virtual void Draw(const RF_Draw::Path2D& Path) override;
    void SetCanvas(RF_Draw::Canvas3D& Canvas);
    OpenGLRenderer& operator=(const OpenGLRenderer& Other)
    { 
        return *this;
    }
private:
    RF_Draw::Canvas3D* m_Canvas;
    RF_Draw::Path2DTriangulation m_Tesselator;

    struct Object
    {
        RF_Type::UInt32 vbos[2];
        RF_Type::UInt32 vao, vertices;
    };

    class SimpleGLShader
    {
    public:
        void Generate();
        void Bind();
        void Unbind();
        void Update();
        struct SharedTransformUniforms
        {
            RF_Geo::Mat4f ModelView;
            RF_Geo::Mat4f ModelViewProjection;
        } m_SharedTransformUniforms;
    private:
        RF_Type::UInt32 shader_programme = 0;
        RF_Type::UInt32 m_SharedUBO = 0;
    };

    RF_Collect::Array<Object> m_ObjectPool;
    RF_Collect::HashList m_Objects;
    SimpleGLShader m_BasicShader;
    RF_Draw::Camera m_Camera;
    RF_Geo::ProjectionMatrix m_Projection;
};

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();

    RF_Form::Label& GetLabel1();
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::Label m_Label1;
    OpenGLRenderer m_OpenGLRenderer;
};

}

#endif // RE_WINDOW_HPP