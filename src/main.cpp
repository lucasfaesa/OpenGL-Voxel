#include "Engine.h"
#include "Application.h"

int main()
{
    Application app;
    Engine engine(&app);
    engine.Run();
    return 0;
}