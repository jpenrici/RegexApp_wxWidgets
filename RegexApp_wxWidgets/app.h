#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if wxUSE_HYPERLINKCTRL
#include <wx/hyperlink.h>
#endif

#if wxUSE_FILE
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#endif

#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class App : public wxApp {
public:
    virtual bool OnInit() wxOVERRIDE;

};

class AppFrame : public wxFrame {
public:
    AppFrame(const wxString &title, const wxSize &size);
    ~AppFrame() {};

private:

    enum ID {
        ID_AboutDialog,
        ID_Button_Reset,
        ID_Button_Search,
        ID_Menu_OnLoad,
        ID_StatuBar,
        ID_wxLabel,
        ID_wxTxtCtrl_File,
        ID_wxTxtCtrl_Regex
    };

    std::string currentText;
    std::string currentRegex;
    std::string currentFilename;

    wxColor     regexBkgColor;
    wxColor     regexTextColor;

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
    void OnKeyDown(wxKeyEvent &event);
    void Search(const wxString regExpression);
    void ResetAll();

    wxString Concat(std::vector<std::string> strings);

    class AboutDialog : public wxDialog {
    public:
        AboutDialog();
        ~AboutDialog() {};

    private:
        const wxString ABOUT = "Application for text search by RegEx.\n\n"
                               "References:\n";
        const wxString ABOUT_HLINK1 = "https://www.wxwidgets.org";
        const wxString ABOUT_HLINK2 = "https://cplusplus.com/reference/regex";

        wxButton *okBtn;
        wxStaticText *label;
        wxHyperlinkCtrl *hyperlink1, *hyperlink2;
        wxBoxSizer *vBox, *hBox[4];

        void OnOK(wxCommandEvent &event);
    };
};
