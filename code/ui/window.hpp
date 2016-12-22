#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

#include "../shared/OpenGLRenderer.hpp"

namespace RadonExample {

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();

    RF_Form::Label& GetLabel1();
    RF_Form::Image& GetImage();
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::GridLayouter m_Layouter;
    RF_Form::VerticalLayouter m_VLayouter;
    RF_Form::Label m_Label1;
    RF_Form::GPUInfoGraph m_FanRPM;
    RF_Form::Image m_Image;
    OpenGLRenderer m_OpenGLRenderer;
};

}

#endif // RE_WINDOW_HPP