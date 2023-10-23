#ifndef __MAIN_H__
#define __MAIN_H__

extern "C" {
#include "ttt.h"
}

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <type_traits>

#ifndef DBUTTONCOLOR
#define DBUTTONCOLOR wxColor(100, 100, 100)
#endif

#ifndef P1COLOR
#define P1COLOR wxColor(100, 0, 0)
#endif

#ifndef P2COLOR
#define P2COLOR wxColor(0, 0, 100)
#endif

class TTTButtom;
class TTTFrame;

using buttonMap_t = std::map<int, TTTButtom*>;

struct Player : public ox_player {
    const wxColor color;
    inline Player(const wxColor& color);
};

struct Game : public ox_game {
    Game(
        unsigned int seed = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count());
    inline ox_gameid gameplay(const ox_player* p1, ox_player* p2, unsigned int val);
};

static_assert(std::is_trivially_constructible<ox_game>() && std::is_standard_layout<ox_game>(), "struct ox_game is not a trivially constructible struct && POD");
static_assert(std::is_standard_layout<Game>(), "struct Game is not a POD");
static_assert(sizeof(Game) == sizeof(ox_game), "Game != ox_game");

struct Ai {
    inline static int ai(ox_game* game, const ox_player* p1, const ox_player* p2);
};

class TTTButtom : public wxButton {
public:
    const int val;
    const wxColor defaultButtonColor;

    inline TTTButtom(wxWindow* parent, wxWindowID id, const wxSize& size, int val, const wxColor defaultButtonColor = DBUTTONCOLOR);
};

class TTTFrame : public wxFrame {
public:
    TTTFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    inline const buttonMap_t& getmapButton() const;

protected:
    virtual void OnNewGame(wxCommandEvent& event);
    virtual void OnP1First(wxCommandEvent& event);
    void OnP2First(wxCommandEvent& event);
    void OnHint(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    virtual void OnButton(wxCommandEvent& event) = 0;

    Player* newgame(Player* wf);

    wxSize sqSize() const;
    wxSize sqindexSize() const;

    Game game;
    Player p1;
    Player p2;
    Player* currentPlayer;

    wxPanel* panel;
    const buttonMap_t mapButton;
    wxButton* indexButton;

    wxDECLARE_EVENT_TABLE();
};
enum {
    ID_BUTTON0 = 0,
    ID_BUTTON1 = ID_BUTTON0 + 1,
    ID_BUTTON2 = ID_BUTTON0 + 2,
    ID_BUTTON3 = ID_BUTTON0 + 3,
    ID_BUTTON4 = ID_BUTTON0 + 4,
    ID_BUTTON5 = ID_BUTTON0 + 5,
    ID_BUTTON6 = ID_BUTTON0 + 6,
    ID_BUTTON7 = ID_BUTTON0 + 7,
    ID_BUTTON8 = ID_BUTTON0 + 8,
    ID_NEWGAME = ID_BUTTON0 + 9,
    ID_P1 = ID_BUTTON0 + 10,
    ID_P2 = ID_BUTTON0 + 11,
    ID_HINT = ID_BUTTON0 + 12
};

//// *******  BEGIN METHOD DEF ********** ////

inline Player::Player(const wxColor& color)
    : color(color)
{
}

Game::Game(unsigned int seed)
    : ox_game(ox_creatgame(seed))
{
}

inline ox_gameid Game::gameplay(const ox_player* p1, ox_player* p2, unsigned int val)
{
    return ox_gameplay(this, p1, p2, val);
}

inline int Ai::ai(ox_game* game, const ox_player* p1, const ox_player* p2)
{
    return ox_ai(game, p1, p2);
}

inline TTTButtom::TTTButtom(wxWindow* parent, wxWindowID id, const wxSize& size, int val, const wxColor defaultButtonColor)
    : wxButton(parent, id, "", wxPoint(0, 0), size)
    , val(val)
    , defaultButtonColor(defaultButtonColor)
{
}

TTTFrame::TTTFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN)
    , p1(P1COLOR)
    , p2(P2COLOR)
    , panel(new wxPanel(this, -1))
    , mapButton({ { ID_BUTTON0, new TTTButtom(panel, ID_BUTTON0, sqSize(), 0) },
          { ID_BUTTON1, new TTTButtom(panel, ID_BUTTON1, sqSize(), 1) },
          { ID_BUTTON2, new TTTButtom(panel, ID_BUTTON2, sqSize(), 2) },

          { ID_BUTTON3, new TTTButtom(panel, ID_BUTTON3, sqSize(), 3) },
          { ID_BUTTON4, new TTTButtom(panel, ID_BUTTON4, sqSize(), 4) },
          { ID_BUTTON5, new TTTButtom(panel, ID_BUTTON5, sqSize(), 5) },

          { ID_BUTTON6, new TTTButtom(panel, ID_BUTTON6, sqSize(), 6) },
          { ID_BUTTON7, new TTTButtom(panel, ID_BUTTON7, sqSize(), 7) },
          { ID_BUTTON8, new TTTButtom(panel, ID_BUTTON8, sqSize(), 8) } })
    , indexButton(new wxButton(panel, wxID_ANY, "", wxPoint(0, 0), sqindexSize()))
{
    wxMenu* menuGame = new wxMenu;
    menuGame->Append(ID_NEWGAME, "&New Game...\tCtrl-N", "Start a new game");

    menuGame->AppendSeparator();
    {
        auto* menuItemP1 = new wxMenuItem(nullptr, ID_P1, "&P1 goes first...\tCtrl-C", "Start a new game and P1 goes first");
        menuItemP1->SetTextColour(p1.color);
        menuGame->Append(menuItemP1);

        auto* menuItemP2 = new wxMenuItem(nullptr, ID_P2, "&P2 goes first...\tCtrl-Y", "Start a new game and P2 goes first");
        menuItemP2->SetTextColour(p2.color);
        menuGame->Append(menuItemP2);
    }

    menuGame->AppendSeparator();
    menuGame->Append(ID_HINT, "&Hint...\tCtrl-H", "Hint move");
    menuGame->AppendSeparator();
    menuGame->Append(wxID_EXIT);
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuGame, "&Game");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to u-Tic-Tac-Toe Game!");

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox_N = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* hbox_S = new wxBoxSizer(wxHORIZONTAL);

    hbox_N->Add(new wxPanel(panel, -1));
    hbox_S->Add(new wxPanel(panel, -1));

    indexButton->Disable();
    hbox_N->Add(indexButton);
    vbox->Add(hbox_N, 0, wxALIGN_CENTER, 0);

    hbox1->Add(mapButton.at(ID_BUTTON0));
    hbox1->Add(mapButton.at(ID_BUTTON1));
    hbox1->Add(mapButton.at(ID_BUTTON2));
    hbox2->Add(mapButton.at(ID_BUTTON3));
    hbox2->Add(mapButton.at(ID_BUTTON4));
    hbox2->Add(mapButton.at(ID_BUTTON5));
    hbox3->Add(mapButton.at(ID_BUTTON6));
    hbox3->Add(mapButton.at(ID_BUTTON7));
    hbox3->Add(mapButton.at(ID_BUTTON8));

    vbox->Add(hbox1, 0, wxALIGN_CENTER, 0);
    vbox->Add(hbox2, 0, wxALIGN_CENTER, 0);
    vbox->Add(hbox3, 0, wxALIGN_CENTER, 0);

    vbox->Add(hbox_S, 1, wxEXPAND);

    panel->SetSizer(vbox);
}

Player* TTTFrame::newgame(Player* wf)
{
    for (const auto& buttonPair : mapButton) {
        buttonPair.second->SetBackgroundColour(buttonPair.second->defaultButtonColor);
        buttonPair.second->Enable();
    }

    ox_init(&game, WINLIST, TRILIST, NWIN, NELEMENT, NTRI, NTRIELEMENT, &p1, &p2);

    currentPlayer = (!wf) ? (ox_random(&game, 0, 1) ? &p1 : &p2) : wf;
    indexButton->SetBackgroundColour(currentPlayer->color);

    return currentPlayer;
}

void TTTFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void TTTFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is an unbeatable Tic-Tac-Toe Game.\n\nContact me:\n\nE-mail:bosskillerz@gmail.com\nFaceBook:dead-root\nGitHub:github.com/hwoy", "About u-Tic-Tac-Toe", wxOK | wxICON_INFORMATION);
}
void TTTFrame::OnNewGame(wxCommandEvent& event)
{
    newgame(nullptr);
}

void TTTFrame::OnP1First(wxCommandEvent& event)
{
    newgame(&p1);
}
void TTTFrame::OnP2First(wxCommandEvent& event)
{
    newgame(&p2);
}

void TTTFrame::OnHint(wxCommandEvent& event)
{
    auto* rival = (currentPlayer == &p1) ? &p2 : &p1;
    const auto pHint = Ai::ai(&game, rival, currentPlayer);

    TTTButtom* hintButton = std::find_if(mapButton.begin(), mapButton.end(),
        [pHint](const auto& buttonPair) {
            return buttonPair.second->val == pHint;
        })->second;

    hintButton->SetBackgroundColour(currentPlayer->color);

    wxMessageBox(("\n\nHint: " + std::to_string(pHint)).c_str(), "Hint a best move", wxOK | wxICON_INFORMATION);

    hintButton->SetBackgroundColour(hintButton->defaultButtonColor);
}
wxSize TTTFrame::sqSize() const
{
    const auto sq = std::min(GetClientSize().GetWidth(), GetClientSize().GetHeight()) / 4;

    return wxSize(sq, sq);
}

wxSize TTTFrame::sqindexSize() const
{
    const auto sq = std::min(GetClientSize().GetWidth(), GetClientSize().GetHeight()) / 8;

    return wxSize(sq, sq);
}

inline const buttonMap_t& TTTFrame::getmapButton() const
{
    return mapButton;
}

template <typename F>
class TTTApp : public wxApp {
    F frame;

public:
    virtual bool OnInit() override;
};

template <typename F>
bool TTTApp<F>::OnInit()
{
    if (!frame.IsMaximized())
        frame.Maximize(true);
    frame.Show(true);
    return true;
}

wxBEGIN_EVENT_TABLE(TTTFrame, wxFrame)

    EVT_MENU(ID_NEWGAME, TTTFrame::OnNewGame)
        EVT_MENU(ID_P2, TTTFrame::OnP2First)
            EVT_MENU(ID_P1, TTTFrame::OnP1First)
                EVT_MENU(ID_HINT, TTTFrame::OnHint)
                    EVT_MENU(wxID_EXIT, TTTFrame::OnExit)
                        EVT_MENU(wxID_ABOUT, TTTFrame::OnAbout)

                            EVT_BUTTON(ID_BUTTON0, TTTFrame::OnButton)
                                EVT_BUTTON(ID_BUTTON1, TTTFrame::OnButton)
                                    EVT_BUTTON(ID_BUTTON2, TTTFrame::OnButton)

                                        EVT_BUTTON(ID_BUTTON3, TTTFrame::OnButton)
                                            EVT_BUTTON(ID_BUTTON4, TTTFrame::OnButton)
                                                EVT_BUTTON(ID_BUTTON5, TTTFrame::OnButton)

                                                    EVT_BUTTON(ID_BUTTON6, TTTFrame::OnButton)
                                                        EVT_BUTTON(ID_BUTTON7, TTTFrame::OnButton)
                                                            EVT_BUTTON(ID_BUTTON8, TTTFrame::OnButton)

                                                                wxEND_EVENT_TABLE()

#endif
