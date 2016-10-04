#ifndef RE_OPENGLSHADER_HPP
#define RE_OPENGLSHADER_HPP

namespace RadonExample {

class OpenGLShader
{
public:
    void Generate();
    void Bind();
    void Unbind();    
private:
    RF_Type::UInt32 shader_programme = 0;
};

}

#endif //!RE_OPENGLSHADER_HPP