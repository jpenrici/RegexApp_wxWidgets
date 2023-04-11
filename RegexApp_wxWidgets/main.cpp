/*
 * Simple regular expression search app.
 *
 * References:
 *
 *      https://www.wxwidgets.org/
 *      https://cplusplus.com/reference/regex/
 *
 */

#include "app.h"

#include <iostream>

IMPLEMENT_APP_NO_MAIN(App)

int main(int argc, char *argv[])
{
    wxApp *pApp = new App();
    App::SetInstance(pApp);

    if (pApp != nullptr) {
        wxEntryStart(argc, argv);
        if (wxTheApp->OnInit()) {
            wxMessageOutputDebug().Printf("%s is open!", pApp->GetAppName().Upper());
            wxTheApp->OnRun();
        };
        wxMessageOutputDebug().Printf("%s is closed!", pApp->GetAppName().Upper());
        wxEntryCleanup();
    }
    else {
        std::cerr << "There was something wrong\n";
    }

    return 0;
}
