#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum ID {ID_Menu_OnLoad, ID_wxTxtCtrl_Regex, ID_wxTxtCtrl_File, ID_wxLabel,
         ID_Button_Search, ID_Button_Reset, ID_StatuBar};

static const char *WXSTR_TITLE = "wxWidgets Simple Regex Search";
static const char *WXSTR_ABOUT = "Application for text search by RegEx.";

class App : public wxApp {
public:
    virtual bool OnInit() wxOVERRIDE;
};

class AppFrame : public wxFrame {
public:
    AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    ~AppFrame() {};

private:
    std::string text;

    wxString    title;
    wxColor     normalBkgColor, highlightedFont;

    wxMenuBar    *menuBar;
    wxStatusBar  *statusBar;
    wxMenu       *fileMenu, *helpMenu;
    wxButton     *resetBtn, *searchBtn;
    wxBoxSizer   *vBox, *hBox1, *hBox2;
    wxTextCtrl   *fileText, *regexText;
    wxStaticText *label;

    void OnAbout(wxCommandEvent &event);
    void OnLoad(wxCommandEvent &event);
    void OnReset(wxCommandEvent &event);
    void OnSearch(wxCommandEvent &event);
    void Search(const wxString regExpression);

    wxString Concat(std::vector<std::string> strings);

    template<typename T>
    wxString ToWxStr(T value);

    template<typename T>
    std::string ToStr(T value);
};
