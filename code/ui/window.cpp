#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>
#include "window.hpp"
#include <RadonFramework/System/Drawing/OpenGL.hpp>

namespace RadonExample {


void OpenGLRenderer::SimpleGLShader::Generate()
{
    const char* vertex_shader =
        "#version 420\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "layout(std140, binding = 2) uniform SharedTransformUniforms"
        "{"
        "    mat4 ModelView;"
        "    mat4 ModelViewProjection;"
        "};"
        "void main () {"
        "  gl_Position = ModelViewProjection* vec4(vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 420\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4 (0.5, 0.5, 0.5, 1.0);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    glCreateBuffers(1, &m_SharedUBO);
    glNamedBufferData(m_SharedUBO, sizeof(SharedTransformUniforms), &m_SharedTransformUniforms, GL_STREAM_DRAW);
}

void OpenGLRenderer::SimpleGLShader::Bind()
{
    glUseProgram(shader_programme);
}

void OpenGLRenderer::SimpleGLShader::Unbind()
{
    glUseProgram(0);
}

void OpenGLRenderer::SimpleGLShader::Update()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_SharedUBO);
    glNamedBufferSubData(m_SharedUBO, 0, sizeof(SharedTransformUniforms), &m_SharedTransformUniforms);
}

BasicWindow::BasicWindow()
:m_Canvas(*this)
,m_Label1(&m_Canvas)
{
    m_Label1.SetText(":)"_rfs);
    m_OpenGLRenderer.SetCanvas(m_Canvas);
    m_Canvas.SetRenderer(m_OpenGLRenderer);
}

RF_Form::Label& BasicWindow::GetLabel1()
{
    return m_Label1;
}

void OpenGLRenderer::StartFrame()
{
    m_Canvas->Clear();
    m_BasicShader.Bind();
    m_BasicShader.m_SharedTransformUniforms.ModelView = m_Camera.GetMatrix();
    m_BasicShader.m_SharedTransformUniforms.ModelViewProjection = m_Projection.GetMatrix(RF_Geo::Viewtype::View2D);
    m_BasicShader.Update();
}

void OpenGLRenderer::EndFrame()
{

    m_Canvas->SwapBuffer();
}

void OpenGLRenderer::Process(const RF_Draw::Path2D& Path)
{    
    auto hash = Path.GetHash();
    void* data;
    if(!m_Objects.Get(hash, data))
    {
        Object obj;
        Path.Visit(m_Tesselator);
        auto vertices = m_Tesselator.GetVertices();
        auto colors = m_Tesselator.GetVertexColors();

        glGenBuffers(2, obj.vbos);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.Count() * sizeof(RF_Geo::Vec3f), &vertices(0), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, colors.Count() * sizeof(RF_Geo::Vec3f), &colors(0), GL_STATIC_DRAW);

        glGenVertexArrays(1, &obj.vao);
        glBindVertexArray(obj.vao);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbos[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbos[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        obj.vertices = vertices.Count();
        m_ObjectPool.Resize(m_ObjectPool.Count()+1);
        m_ObjectPool(m_ObjectPool.Count()-1) = obj;
        m_Objects.Add(hash, &m_ObjectPool(m_ObjectPool.Count() - 1));
    }
}

void OpenGLRenderer::Draw(const RF_Draw::Path2D& Path)
{
    for (RF_Type::Size i = 0; i < m_ObjectPool.Count(); ++i)
    {
        glBindVertexArray(m_ObjectPool(i).vao);
        glDrawArrays(GL_TRIANGLES, 0, m_ObjectPool(i).vertices);
    }
}

void OpenGLRenderer::SetCanvas(RF_Draw::Canvas3D& Canvas)
{
    m_Canvas = &Canvas;
    m_BasicShader.Generate();
}

}

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    RF_Collect::Array<RadonExample::BasicWindow> windows;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    app->ShowConsole(false);

    windows.Resize(screens.Count());

    for(RF_Type::Size i = 0; i < screens.Count(); i++)
    {
        windows(i).Position(screens[i].Position());
        RF_Type::UInt32 dpiX = screens[i].DPIX();
        RF_Type::UInt32 dpiY = screens[i].DPIY();
        RF_Geo::Size2D<RF_Type::Int32> windowSize((windows(i).Width()*dpiX)/96,(windows(i).Height()*dpiY)/96);
        windows(i).Resize(windowSize);
        windows(i).Size(windowSize);
        windows(i).Title(RF_Type::String::Format("X=%ddpi Y=%ddpi"_rfs, dpiX, dpiY));
    }

    app->Run(&windows(0));
    
    return;
}