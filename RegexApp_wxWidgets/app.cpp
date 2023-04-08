#include "app.h"

#if wxUSE_FILE
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#endif

#include <filesystem>
#include <iterator>
#include <regex>
#include <vector>

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
    Bind(wxEVT_MENU, [ = ](wxCommandEvent &) {Close(true);}, wxID_EXIT);

    // Color
    normalBkgColor = GetBackgroundColour();
    highlightedFont = wxColor(255, 0, 0, 0);

    // Font
    wxFont font(14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    // Text
    label = new wxStaticText(this, ID_wxLabel, "Regular Expression:");
    label->SetFont(font);

    fileText = new wxTextCtrl(this, ID_wxTxtCtrl_Regex, wxEmptyString,
                              wxDefaultPosition, wxDefaultSize,
                              wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY,
                              wxDefaultValidator, wxTextCtrlNameStr);
    fileText->SetBackgroundColour(normalBkgColor);
    fileText->SetEditable(false);
    fileText->SetFont(font);

    regexText = new wxTextCtrl(this, ID_wxTxtCtrl_File);
    regexText->SetFocus();
    regexText->SetFont(font);
    regexText->SetMinSize(wxSize(380, wxDefaultCoord));

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

    hBox2->AddSpacer(5);
    hBox2->Add(label);
    hBox2->AddSpacer(5);
    hBox2->Add(regexText);
    hBox2->AddSpacer(10);
    hBox2->Add(searchBtn);
    hBox2->Add(resetBtn);

    vBox->Add(hBox1, 1, wxEXPAND);
    vBox->AddSpacer(10);
    vBox->Add(hBox2);
    vBox->AddSpacer(10);

    // Status Bar
    statusBar = new wxStatusBar(this, ID_StatuBar);
    statusBar->SetFont(font);
    statusBar->SetMinHeight(30);

    // Frame
    SetSizer(vBox);
    SetMinSize(size);
    SetStatusBar(statusBar);
    SetStatusText("Welcome!");

    // Example
    text = "\t------------------------------\n"
           "\tWelcome to Simple RegEx Search!\n"
           "\t------------------------------\n\n"
           "\tExample:\n\n"
           "\t1) Click SEARCH.\n\n"
           "\t2) Click RESET.\n\n"
           "\t3) Enter the expression in the input box and search:\n\n"
           "\t\t3.1) \\w+\n"
           "\t\t3.2) \\d\\)\n"
           "\t\t3.3) [0-9]\n\n\n"
           "\t4) Load a .txt file by selecting Load from the Menu.\n";

    // Update
    fileText->SetValue(text);
    regexText->SetValue("search");
}

void AppFrame::OnLoad(wxCommandEvent &event)
{
    wxFileDialog *openFileDialog;
    openFileDialog = new wxFileDialog(this,
                                      "Open File Text", ".", "",
                                      "Open (*.txt)|*.txt",
                                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog->ShowModal() == wxID_CANCEL) {
        SetStatusText(fileText->IsEmpty() ? "Nothing to do!" : "Keep current text!");
        return;
    }

    auto path = openFileDialog->GetPath();
    auto filename = std::filesystem::path(std::string(path)).filename();
    int count = 0;
    text = "";

    try {
        wxFileInputStream input(path);
        wxTextInputStream txtInput(input);
        while (input.IsOk() && !input.Eof()) {
            text += Concat({std::string(txtInput.ReadLine()), "\n"});
            count += !input.Eof() ? 1 : 0;
        }
        if (count > 0) {
            wxMessageBox("Loaded " + ToWxStr(count) + " lines.");
            SetStatusText(Concat({"Open: ", std::string(filename)}));
            fileText->SetBackgroundColour(normalBkgColor);
            fileText->SetValue(text);
        }
    }
    catch (...) {
        SetStatusText(Concat({"Failed to open ", filename}));
    }
}

void AppFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(WXSTR_ABOUT, "About " + title, wxOK | wxICON_INFORMATION);
}

void AppFrame::OnSearch(wxCommandEvent &event)
{
    Search(regexText->GetValue());
}

void AppFrame::OnReset(wxCommandEvent &event)
{
    regexText->Clear();

    fileText->Clear();
    fileText->SetBackgroundColour(normalBkgColor);
    fileText->SetValue(text);

    SetStatusText(wxString(wxEmptyString));
}

void AppFrame::Search(const wxString regExpression)
{
    if (regExpression.IsEmpty()) {
        SetStatusText("Regex is empty!");
        return;
    }
    SetStatusText("Wait ...");

    unsigned count = 0;
    const std::string pattern(regExpression);
    std::vector<bool> matches(text.size(), false);

    try {
        auto flags = std::regex_constants::ECMAScript | std::regex_constants::icase;
        std::regex regEx(pattern, flags);

        if (std::regex_search(text, regEx)) {
            auto words_begin = std::sregex_iterator(text.begin(), text.end(), regEx);
            auto words_end = std::sregex_iterator();
            count = std::distance(words_begin, words_end);
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch m = *i;
                auto pos = m.position();
                auto size = pos + m.length();
                for (unsigned j = pos; j < size; j++) {
                    matches[j] = true;
                }
            }
        }
    }
    catch (...) {
        wxMessageBox("Invalid regex!");
    }

    // Refresh
    fileText->Clear();
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i]) {
            fileText->SetBackgroundColour(highlightedFont);
        }
        else {
            fileText->SetBackgroundColour(normalBkgColor);
        }
        fileText->AppendText(text[i]);
    }

    SetStatusText(Concat({"Found ", ToStr(count), " matches."}));
}

wxString AppFrame::Concat(std::vector<std::string> strings)
{
    for (int i = 1; i < strings.size(); i++) {
        strings.front() += strings[i];
    }

    return strings.empty() ? wxString(wxEmptyString) : wxString(strings.front());
}

template <typename T>
wxString AppFrame::ToWxStr(T value)
{
    wxString wstr;
    wstr << value;

    return wstr;
};

template <typename T>
std::string AppFrame::ToStr(T value)
{
    std::stringstream ss;
    std::string str;
    ss << value;
    ss >> str;

    return str;
}
