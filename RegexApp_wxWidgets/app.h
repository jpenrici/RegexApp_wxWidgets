#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum ID {ID_Menu_OnLoad, ID_wxTxtCtrl_Regex, ID_wxTxtCtrl_File, ID_Button_Search,
         ID_Button_Reset, ID_StatuBar
        };

class App : public wxApp {
public:
    virtual bool OnInit() wxOVERRIDE;

private:
    const wxString WXSTR_TITLE = "wxWidgets Regex";
};

class AppFrame : public wxFrame {
public:
    AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    ~AppFrame() {};

private:
    wxString text;
    wxString title;
    wxButton *resetBtn;
    wxButton *searchBtn;
    wxMenu *fileMenu;
    wxMenu *helpMenu;
    wxMenuBar *menuBar;
    wxStatusBar *statusBar;
    wxTextCtrl *fileText;
    wxTextCtrl *regexText;
    wxBoxSizer *vBox;
    wxBoxSizer *hBox1;
    wxBoxSizer *hBox2;

    void OnAbout(wxCommandEvent &event);
    void OnLoad(wxCommandEvent &event);
    void OnReset(wxCommandEvent &event);
    void OnSearch(wxCommandEvent &event);

    const wxString WXSTR_ABOUT = "This is a\n"
                                 "test";
};
