#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

namespace RadonExample {

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();

    virtual void Idle() override;
    RF_Form::Label Label1;
    RF_Form::Label Label2;
    RF_Form::Label Label3;
    RF_Form::Label Label4;
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Draw::Path2D* m_Path;
    RF_Draw::Text2D* m_Text;
};

}

#endif // RE_WINDOW_HPP