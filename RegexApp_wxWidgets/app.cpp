#include "app.h"

bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    AppFrame *frame = new AppFrame("Regex Search wxWidgets App", wxSize(800, 600));
    frame->Show();

    SetTopWindow(frame);

    return true;
}

AppFrame::AppFrame(const wxString &title, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(1, 1), size)
{
    // Menu File
    fileMenu = new wxMenu;
    fileMenu->Append(ID_Menu_OnLoad, "&Load file\tCtrl-O", "Load Text File");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);

    helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &AppFrame::OnLoad, this, ID_Menu_OnLoad);
    Bind(wxEVT_MENU, &AppFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, [ = ](wxCommandEvent &) { Close(true); }, wxID_EXIT);

    // Color
    regexBkgColor  = wxColor(255, 255, 127, 0);
    regexTextColor = wxColor(255, 0, 0, 0);

    // Font
    wxFont font(14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    // Text
    label = new wxStaticText(this, ID_wxLabel, "Regular Expression:");
    label->SetFont(font);

    fileText = new wxTextCtrl(this, ID_wxTxtCtrl_File, wxEmptyString,
                              wxDefaultPosition, wxDefaultSize,
                              wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY,
                              wxDefaultValidator, wxTextCtrlNameStr);
    fileText->SetEditable(false);
    fileText->SetFont(font);

    regexText  = new wxTextCtrl(this, ID_wxTxtCtrl_Regex, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_PROCESS_ENTER);
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

    Bind(wxEVT_CHAR_HOOK, &AppFrame::OnKeyDown, this);

    // Operating system information
    auto osInfo = wxGetOsDescription().Upper();

#ifdef __LINUX__
    osInfo = wxGetLinuxDistributionInfo().Description.Upper();
#endif

#ifdef WIN32
    SetBackgroundColour(wxColour(*wxWHITE));
    statusBar->SetBackgroundColour(wxColour(*wxWHITE));
#endif

    // Example
    currentText = "\t------------------------------\n"
                  "\tWelcome to Simple RegEx Search!\n"
                  "\t------------------------------\n\n"
                  "\tExample:\n\n"
                  "\t1) Click SEARCH or press ENTER.\n"
                  "\t2) Click RESET or press DEL to clear regular expression.\n"
                  "\t3) Enter the expression in the input box and search:\n\n"
                  "\t\t3.1) \\w+\n"
                  "\t\t3.2) \\d\\)\n"
                  "\t\t3.3) [0-9]\n\n"
                  "\t4) Load a .txt file by selecting Load from the Menu.\n\n"
                  "\tOperating system:\n"
                  "\t" + osInfo + "\n";

    // Update
    currentRegex = "";
    currentFilename = "";

    fileText->SetValue(currentText);
    regexText->SetValue("search");
}

void AppFrame::OnLoad(wxCommandEvent &event)
{
    wxFileDialog *openFileDialog;
    openFileDialog = new wxFileDialog(this,
                                      "Open File Text", wxGetHomeDir(), "",
                                      "Open (*.txt)|*.txt",
                                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog->ShowModal() == wxID_CANCEL) {
        SetStatusText(fileText->IsEmpty() ? "Nothing to do!" : "Keep current text!");
        return;
    }

    auto path = openFileDialog->GetPath();
    auto filename = std::filesystem::path(std::string(path)).filename();
    if (currentFilename == filename) {
        wxMessageBox(currentFilename + " is open!", GetTitle(),
                     wxOK | wxCENTRE | wxDIALOG_NO_PARENT | wxICON_EXCLAMATION);
        return;
    }

    currentFilename = filename.string();
    currentText = "";
    int count = 0;

    try {
        wxFileInputStream input(path);
        wxTextInputStream txtInput(input);
        while (input.IsOk() && !input.Eof()) {
            currentText += Concat({std::string(txtInput.ReadLine()), "\n"});
            count += !input.Eof() ? 1 : 0;
        }
        if (count > 0) {
            wxMessageBox("Loaded " + std::to_string(count) + " lines.");
            SetStatusText(Concat({"Open: ", filename.string()}));
            fileText->SetValue(currentText);
            currentRegex = "";
        }
    }
    catch (...) {
        SetStatusText(Concat({"Failed to open ", filename.string()}));
    }
}

void AppFrame::OnAbout(wxCommandEvent &event)
{
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->Show(true);
}

void AppFrame::OnSearch(wxCommandEvent &event)
{
    Search(regexText->GetValue());
}

void AppFrame::OnReset(wxCommandEvent &event)
{
    ResetAll();
}

void AppFrame::OnKeyDown(wxKeyEvent &event)
{
    auto keyCode = event.GetKeyCode();
    //SetStatusText(std::to_string(keyCode));

    if (keyCode == 127) { // Delete
        ResetAll();
    }
    if (keyCode == 13) { // Enter
        Search(regexText->GetValue());
    }

    event.Skip();
}

void AppFrame::ResetAll()
{
    regexText->Clear();
    currentRegex = "";

    fileText->Clear();
    fileText->SetValue(currentText);

    SetStatusText(wxString(wxEmptyString));
}

void AppFrame::Search(const wxString regExpression)
{
    if (regExpression.IsEmpty()) {
        SetStatusText("Regex is empty!");
        return;
    }

    if (currentRegex != regExpression) {
        currentRegex = regExpression;
    }
    else {
        SetStatusText("This RegEx has already been searched.");
        return;
    }

    fileText->Clear();
    fileText->SetValue(currentText);
    SetStatusText("Wait ...");

    unsigned count = 0;
    const std::string pattern(regExpression);

    try {
        auto flags = std::regex_constants::ECMAScript | std::regex_constants::icase;
        std::regex regEx(pattern, flags);

        if (std::regex_search(currentText, regEx)) {
            auto words_begin = std::sregex_iterator(currentText.begin(),
                                                    currentText.end(), regEx);
            auto words_end = std::sregex_iterator();
            count = std::distance(words_begin, words_end);
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch m = *i;
                fileText->SetStyle(m.position(), m.position() + m.length(),
                                   wxTextAttr(regexTextColor, regexBkgColor));
            }
        }
    }
    catch (...) {
        wxMessageBox("Invalid regex!", GetTitle(),
                     wxOK | wxCENTRE | wxDIALOG_NO_PARENT | wxICON_EXCLAMATION);
    }

    SetStatusText(Concat({"Found ", std::to_string(count), " matches."}));
}

wxString AppFrame::Concat(std::vector<std::string> strings)
{
    for (int i = 1; i < strings.size(); i++) {
        strings.front() += strings[i];
    }

    return strings.empty() ? wxString(wxEmptyString) : wxString(strings.front());
}

AppFrame::AboutDialog::AboutDialog()
    : wxDialog(NULL, wxID_ANY, "About", wxDefaultPosition, wxSize(450, 300))
{
    // Box
    vBox = new wxBoxSizer(wxVERTICAL);
    hBox[0] = new wxBoxSizer(wxHORIZONTAL);
    hBox[1] = new wxBoxSizer(wxHORIZONTAL);
    hBox[2] = new wxBoxSizer(wxHORIZONTAL);
    hBox[3] = new wxBoxSizer(wxHORIZONTAL);

    // Label
    label = new wxStaticText(this, wxID_ANY, ABOUT, wxDefaultPosition,
                             wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
    label->SetFont(wxFont(14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                          wxFONTWEIGHT_NORMAL));

    // Hyperlink
    hyperlink1 = new wxHyperlinkCtrl(this, wxID_ANY,
                                     ABOUT_HLINK1, ABOUT_HLINK1,
                                     wxDefaultPosition, wxDefaultSize,
                                     wxHL_DEFAULT_STYLE);
    hyperlink1->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                               wxFONTWEIGHT_NORMAL));

    hyperlink2 = new wxHyperlinkCtrl(this, wxID_ANY,
                                     ABOUT_HLINK2, ABOUT_HLINK2,
                                     wxDefaultPosition, wxDefaultSize,
                                     wxHL_DEFAULT_STYLE);
    hyperlink2->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                               wxFONTWEIGHT_NORMAL));

    // Button
    okBtn = new wxButton(this, wxID_ANY, "Ok", wxDefaultPosition, wxSize(70, 30));
    okBtn->Bind(wxEVT_BUTTON, &AboutDialog::OnOK, this);
    okBtn->SetFocus();

    // Dialog
    hBox[0]->Add(label, 1);
    hBox[1]->Add(hyperlink1, 1);
    hBox[2]->Add(hyperlink2, 1);
    hBox[3]->Add(okBtn, 1);

    vBox->AddSpacer(10);
    vBox->Add(hBox[0], 1, wxALIGN_CENTRE_HORIZONTAL);
    vBox->AddSpacer(10);
    vBox->Add(hBox[1], 1, wxALIGN_CENTRE_HORIZONTAL);
    vBox->Add(hBox[2], 1, wxALIGN_CENTRE_HORIZONTAL);
    vBox->AddSpacer(10);
    vBox->Add(hBox[3], 1, wxALIGN_CENTRE_HORIZONTAL);

    SetSizer(vBox);

#ifdef WIN32
    SetBackgroundColour(wxColour(*wxWHITE));
#endif

    Centre();
    ShowModal();

    // Exit
    Destroy();
}

void AppFrame::AboutDialog::OnOK(wxCommandEvent &event)
{
    Close();
}
