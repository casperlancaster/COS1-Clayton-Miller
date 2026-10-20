#include "ConsoleTheme.h"
#include "TruckzSimApp.h"

int main()
{
    ConsoleTheme::Initialize();

    TruckzSimApp app;
    app.Run();

    return 0;
}
