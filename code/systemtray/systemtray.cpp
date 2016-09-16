#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/Drawing/SystemTray.hpp>

class AppLogic: public RF_Pattern::IObserver
{
public:
    enum Type
    {
        ToggleHideCmd = 1,
        Exit,
        OpenControlCenter
    };

    AppLogic()
    :ExitProgram(false)
    ,HideConsole(false)
    {

    }

    void OnAction(RF_Type::UInt32 ActionId)
    {
        switch(ActionId)
        {
            case ToggleHideCmd:
                HideConsole = !HideConsole;
                RF_Form::WindowServiceLocator::Default().Application()->ShowConsole(!HideConsole);
                break;
            case Exit:
                ExitProgram = true;
                break;
        }
    }

    RF_Type::Bool ExitProgram;
    RF_Type::Bool HideConsole;
};

void main()
{ 
    RadonFramework::Radon framework;
    AppLogic logic;

    RF_Type::String appDir = RF_IO::Directory::ApplicationDirectory()->Location().OriginalString();

    RF_Draw::SystemTray& tray = RF_Draw::SystemTray::GetInstance();
    RF_Mem::AutoPointer<RF_Draw::TrayIcon> icon(new RF_Draw::TrayIcon);
    icon->Tooltip = "System tray example!";
    icon->Icon = appDir+"rn.ico";
    icon->Notification = "Radon system tray\nIs running now!";
    icon->Menu.Items.Resize(3);
    icon->Menu.Items(0).Name = "Hide commandline";
    icon->Menu.Items(0).CanChecked = true;
    icon->Menu.Items(0).IsChecked = false;
    icon->Menu.Items(0).ActionId = AppLogic::ToggleHideCmd;
    icon->Menu.Items(1).IsSeperator = true;
    icon->Menu.Items(2).Name = "Exit";
    icon->Menu.Items(2).ActionId = AppLogic::Exit;
    icon->OnAction += logic.Connector<>(&AppLogic::OnAction);
    tray.Add(icon);

    RF_Mem::AutoPointer<RF_Diag::Appender> console(new RF_IO::LogConsole);
    RF_IO::Log::AddAppender(console);

    RF_Time::TimeSpan napTime = RF_Time::TimeSpan::CreateByTime(0, 0, 0, 0, 1);
    while(logic.ExitProgram == false)
    {
        RF_Form::WindowServiceLocator::Default().Application()->Run(0);
        RF_Thread::Thread::Sleep(napTime);
    }
    return;
}