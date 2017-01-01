#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

namespace RadonExample {

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();

    RF_Form::Label Label1;
    RF_Form::Label Label2;
    RF_Form::Label Label3;
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::VerticalLayouter m_VLayouter;
};

}

#endif // RE_WINDOW_HPP