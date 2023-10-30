#ifndef __MAIN_H__
#define __MAIN_H__

namespace ffi {
extern "C" {
#include "ttt.h"
}
}

#include <algorithm>
#include <functional>
#include <map>

#if __cplusplus >= 201703L
#include <optional>
#endif

#include <random>
#include <string>
#include <type_traits>

#include <wx/wx.h>

#ifndef DBUTTONCOLOR
#define DBUTTONCOLOR wxColor(100, 100, 100)
#endif

#ifndef P1COLOR
#define P1COLOR wxColor(100, 0, 0)
#endif

#ifndef P2COLOR
#define P2COLOR wxColor(0, 0, 100)
#endif

#define PINNED(TYPE)                       \
    TYPE(const TYPE&) = delete;            \
    TYPE(TYPE&&) = delete;                 \
    TYPE& operator=(const TYPE&) = delete; \
    TYPE& operator=(TYPE&&) = delete;

class TTTButtom;
class TTTFrame;

using buttonMap_t = std::map<int, TTTButtom*>;

struct Player : public ffi::ox_player {
    const wxColor color;
    inline Player(const wxColor& color);
};

struct Game : public ffi::ox_game {
    Game(
        unsigned int seed);
    inline ffi::ox_gameid gameplay(const Player& p1, Player& p2, unsigned int val) const;

    static unsigned int seed();
};

struct Ai {
    inline static int ai(Game& game, const Player& p1, const Player& p2);
};

class TTTButtom : public wxButton {
public:
    const int val;
    const wxColor defaultButtonColor;

    inline TTTButtom(wxWindow* parent, wxWindowID id, const wxSize& size, int val, const wxColor defaultButtonColor = DBUTTONCOLOR);
};

class TTTFrame : public wxFrame {
public:
    TTTFrame(const wxString& title, unsigned int seed, const wxColor& p1color, const wxColor& p2olor, const wxPoint& pos, const wxSize& size);

    PINNED(TTTFrame);

protected:
    virtual void OnNewGame(wxCommandEvent& event);
    virtual void OnP1First(wxCommandEvent& event);
    void OnP2First(wxCommandEvent& event);
    void OnHint(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    virtual void OnButton(wxCommandEvent& event) = 0;

#if __cplusplus >= 201703L
    Player& newgame(std::optional<std::reference_wrapper<Player>> wf);
#else
    Player& newgame(Player* wf);
#endif

    wxSize sqSize() const;
    wxSize sqindexSize() const;

    Game game;
    Player p1;
    Player p2;
    std::reference_wrapper<Player> currentPlayer;

    wxPanel* panel;
    const buttonMap_t mapButton;
    std::reference_wrapper<wxButton> indexButton;

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
    : ffi::ox_game(ffi::ox_creatgame(seed))
{
}

unsigned int Game::seed()
{
    return std::random_device {}();
}

inline ffi::ox_gameid Game::gameplay(const Player& p1, Player& p2, unsigned int val) const
{
    return ffi::ox_gameplay(this, &p1, &p2, val);
}

inline int Ai::ai(Game& game, const Player& p1, const Player& p2)
{
    return ffi::ox_ai(&game, &p1, &p2);
}

inline TTTButtom::TTTButtom(wxWindow* parent, wxWindowID id, const wxSize& size, int val, const wxColor defaultButtonColor)
    : wxButton(parent, id, "", wxPoint(0, 0), size)
    , val(val)
    , defaultButtonColor(defaultButtonColor)
{
}

TTTFrame::TTTFrame(const wxString& title, unsigned int seed = Game::seed(), const wxColor& p1color = P1COLOR, const wxColor& p2color = P2COLOR, const wxPoint& pos = wxPoint(0, 0), const wxSize& size = wxGetDisplaySize())
    : wxFrame(NULL, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN)
    , game(seed)
    , p1(p1color)
    , p2(p2color)
    , currentPlayer(p1)
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
    , indexButton(*(new wxButton(panel, wxID_ANY, "", wxPoint(0, 0), sqindexSize())))
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

    indexButton.get().Disable();
    hbox_N->Add(&indexButton.get());
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

Player& TTTFrame::newgame(
#if __cplusplus >= 201703L
    std::optional<std::reference_wrapper<Player>>
        wf
#else
    Player* wf
#endif
)
{
    for (const auto& buttonPair : mapButton) {
        buttonPair.second->SetBackgroundColour(buttonPair.second->defaultButtonColor);
        buttonPair.second->Enable();
    }

    ffi::ox_init(&game, ffi::WINLIST, ffi::TRILIST, NWIN, NELEMENT, NTRI, NTRIELEMENT, &p1, &p2);

    currentPlayer = (!wf) ? (ffi::ox_random(&game, 0, 1) ? p1 : p2) :
#if __cplusplus >= 201703L
                          wf->get();
#else
                          *wf;
#endif

    indexButton.get().SetBackgroundColour(currentPlayer.get().color);

    return currentPlayer;
}

void TTTFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}
void TTTFrame::OnAbout(wxCommandEvent&)
{
    wxMessageBox("This is an unbeatable Tic-Tac-Toe Game.\n\nContact me:\n\nE-mail:bosskillerz@gmail.com\nFaceBook:dead-root\nGitHub:github.com/hwoy", "About u-Tic-Tac-Toe", wxOK | wxICON_INFORMATION);
}
void TTTFrame::OnNewGame(wxCommandEvent&)
{
    newgame(
#if __cplusplus >= 201703L
        std::nullopt
#else
        nullptr
#endif
    );
}

void TTTFrame::OnP1First(wxCommandEvent&)
{
    newgame(
#if __cplusplus >= 201703L
        p1
#else
        &p1
#endif
    );
}
void TTTFrame::OnP2First(wxCommandEvent&)
{
    newgame(
#if __cplusplus >= 201703L
        p2
#else
        &p2
#endif
    );
}

void TTTFrame::OnHint(wxCommandEvent&)
{
    const auto& rival = &currentPlayer.get() == &p1 ? p2 : p1;
    const auto pHint = Ai::ai(game, rival, currentPlayer.get());

    TTTButtom& hintButton = *std::find_if(mapButton.begin(), mapButton.end(),
        [pHint](const

#if __cplusplus >= 201402L
            auto
#else
            std::pair<const int, TTTButtom*>
#endif

                & buttonPair) {
            return buttonPair.second->val == pHint;
        })->second;

    hintButton.SetBackgroundColour(currentPlayer.get().color);

    wxMessageBox(("\n\nHint: " + std::to_string(pHint)).c_str(), "Hint a best move", wxOK | wxICON_INFORMATION);

    hintButton.SetBackgroundColour(hintButton.defaultButtonColor);
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

template <typename F>
class TTTApp : public wxApp {

public:
    virtual bool OnInit() override;
};

template <typename F>
bool TTTApp<F>::OnInit()
{
    F& frame = *(new F());
    if (!frame.IsMaximized())
        frame.Maximize(true);
    frame.Show(true);
    return true;
}

template <typename T>
constexpr bool SELF_REFERENCE_TEST()
{
    return !(std::is_copy_constructible<T>() || std::is_move_constructible<T>()
        || std::is_copy_assignable<T>() || std::is_move_assignable<T>());
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

    /* =================  TEST ================= */

    static_assert(std::is_trivially_constructible<ffi::ox_game>() && std::is_standard_layout<ffi::ox_game>(), "struct ffi::ox_game is not a trivially constructible struct && standard layout");
static_assert(std::is_standard_layout<Game>(), "struct Game is not a standard layout");
static_assert(sizeof(Game) == sizeof(ffi::ox_game), "Game != ffi::ox_game");

static_assert(SELF_REFERENCE_TEST<TTTFrame>(), "TTTFrame is not compatible with self reference");

#endif
