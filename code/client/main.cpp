#include <RadonFramework/precompiled.hpp>
// Radon Framework heavily use forward declaration to reduce compile/linker time.
// You can put the most used header into the precompiled header which will be
// generated by a Radon CMake framework based project.
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/IO/Log.hpp>
#include <RadonFramework/IO/LogConsole.hpp>
#include <RadonFramework/Diagnostics/Appender.hpp>
#include <RadonFramework/Threading/ThreadPool.hpp>

// Radon Framework namespace are associated to the folder structure.
using namespace RadonFramework;
using namespace RadonFramework::IO;
using namespace RadonFramework::Collections;
using namespace RadonFramework::Core::Types;

void main()
{ 
    // This will enforce to dispatch all system functions and register services.
    // You can work without this, system function will dispatch itself on first 
    // call and services can register by your self.
    Radon framework;

    // attach console log output
    Memory::AutoPointer<Diagnostics::Appender> console(new LogConsole);
    Log::AddAppender(console);

    // log some text types
    LogInfo("Hello world!");
    LogError("Hello error!");
    LogDebug("Hello debug notice!");
    LogFatalError("Hello urgent error!");

    // wait till the thread pool processed all log messages
    RF_Pattern::Singleton<Threading::ThreadPool>::GetInstance().WaitTillDone();
    return;
}