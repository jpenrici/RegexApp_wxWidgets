#include "app.h"

#if wxUSE_FILE
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#endif

bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    AppFrame *frame = new AppFrame(WXSTR_TITLE, wxPoint(1, 1), wxSize(800, 600));
    frame->Show();

    return true;
}

AppFrame::AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), title(title)
{
    // Menu File
    fileMenu = new wxMenu;
    fileMenu->Append(ID_Menu_OnLoad, "&Load file\tCtrl-O", "Load Text File");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);

    helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);

    menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &AppFrame::OnLoad, this, ID_Menu_OnLoad);
    Bind(wxEVT_MENU, &AppFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, [ = ](wxCommandEvent &) {
        Close(true);
    }, wxID_EXIT);

    // Text
    regexText = new wxTextCtrl(this, ID_wxTxtCtrl_Regex);
    fileText = new wxTextCtrl(this, ID_wxTxtCtrl_File);

    regexText->SetFocus();
    fileText->SetEditable(false);

    // Button
    searchBtn = new wxButton(this, ID_Button_Search, "SEARCH");
    resetBtn = new wxButton(this, ID_Button_Reset, "RESET");

    Bind(wxEVT_BUTTON, &AppFrame::OnSearch, this, ID_Button_Search);
    Bind(wxEVT_BUTTON, &AppFrame::OnReset, this, ID_Button_Reset);

    // Box
    vBox = new wxBoxSizer(wxVERTICAL);
    hBox1 = new wxBoxSizer(wxHORIZONTAL);
    hBox2 = new wxBoxSizer(wxHORIZONTAL);

    hBox1->Add(fileText, 1, wxEXPAND);
    hBox2->Add(regexText);
    hBox2->AddSpacer(5);
    hBox2->Add(searchBtn);
    hBox2->Add(resetBtn);

    vBox->Add(hBox1, 1, wxEXPAND);
    vBox->AddSpacer(10);
    vBox->Add(hBox2);
    vBox->AddSpacer(10);
    SetSizer(vBox);

    // Status Bar
    statusBar = new wxStatusBar(this, ID_StatuBar);
    statusBar->SetMinHeight(30);

    SetStatusBar(statusBar);
    SetStatusText("Welcome!");
}

void AppFrame::OnLoad(wxCommandEvent &event)
{
    wxFileDialog *openFileDialog;
    openFileDialog = new wxFileDialog(this, "Open File Text", ".", "",
                                      "Open (*.txt)|*.txt",
                                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog->ShowModal() == wxID_CANCEL) {
        if (fileText->IsEmpty()) {
            SetStatusText("Nothing to do!");
        }
        else {
            SetStatusText("Keep current text!");
        }
        return;
    }

    auto fileName = openFileDialog->GetPath();
    wxFileInputStream input(fileName);
    wxTextInputStream txtInput(input);

    int count = 0;
    text.Clear();
    while (input.IsOk() && !input.Eof()) {
        text += txtInput.ReadLine();
        count += !input.Eof() ? 1 : 0;
    }

    if (count > 0) {
        // Notify read rows
        wxString str;
        str << count;
        wxMessageBox("Loaded " + str + "lines.");
        // Update text
        if (!text.empty()) {
            fileText->SetValue(text);
            SetStatusText("Оpen: " + fileName);
        }
        else {
            SetStatusText("Wrong: " + fileName);
        }
    }
}

void AppFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(WXSTR_ABOUT, "About " + title, wxOK | wxICON_INFORMATION);
}

void AppFrame::OnSearch(wxCommandEvent &event)
{
    wxMessageBox("Search!");
}

void AppFrame::OnReset(wxCommandEvent &event)
{
    regexText->Clear();
}
