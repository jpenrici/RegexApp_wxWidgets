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
        wxTheApp->CallOnInit();
        wxTheApp->OnRun();
        wxEntryCleanup();
    }
    else {
        std::cerr << "There was something wrong\n";
    }

    return 0;
}
