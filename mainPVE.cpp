#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.h"

class TTT_pve_Frame : public TTTFrame {
protected:
    ffi::ox_gameid __OnButton__(TTTButtom& selectedButton);
    void __ComTurn__();

public:
    TTT_pve_Frame();

    TTT_pve_Frame(const TTT_pve_Frame&) = delete;
    TTT_pve_Frame(TTT_pve_Frame&&) = delete;

    TTT_pve_Frame& operator=(const TTT_pve_Frame&) = delete;
    TTT_pve_Frame& operator=(TTT_pve_Frame&&) = delete;

    virtual void OnNewGame(wxCommandEvent& event) override;
    virtual void OnP1First(wxCommandEvent& event) override;
    virtual void OnButton(wxCommandEvent& event) override;
};

void TTT_pve_Frame::OnNewGame(wxCommandEvent& event)
{
    ffi::ox_random(&game, 0, 1) ? OnP1First(event) : OnP2First(event);
}

TTT_pve_Frame::TTT_pve_Frame()
    : TTTFrame("u-Tic-Tac-Toe PVE", wxPoint(0, 0), wxGetDisplaySize())
{
    wxCommandEvent event;
    OnNewGame(event);
}

void TTT_pve_Frame::OnP1First(wxCommandEvent& event)
{
    TTTFrame::OnP1First(event);
    __ComTurn__();
}

ffi::ox_gameid TTT_pve_Frame::__OnButton__(TTTButtom& selectedButton)
{
    auto& rival = &currentPlayer.get() == &p1 ? p2 : p1;
    const auto gameid = game.gameplay(rival, currentPlayer.get(), selectedButton.val);
    switch (gameid) {
    case ffi::ox_idgame:
    case ffi::ox_idwin:
    case ffi::ox_iddraw:
        selectedButton.SetBackgroundColour(currentPlayer.get().color);
        selectedButton.Disable();
        if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
            wxMessageBox(gameid == ffi::ox_idwin ? (&currentPlayer.get() == &p1 ? "P1(Com) wins" : "P2(You) win") : "Draw!", "Game Over!", wxOK | wxICON_INFORMATION);
        }
    default:
        break;
    }

    return gameid;
}

void TTT_pve_Frame::__ComTurn__()
{
    const auto select = Ai::ai(game, p2, p1);

    TTTButtom& selectedButton = *std::find_if(mapButton.begin(), mapButton.end(),
        [select](const auto& buttonPair) {
            return buttonPair.second->val == select;
        })->second;

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        wxCommandEvent event;
        OnP2First(event);
        return;
    }

    currentPlayer = p2;
    indexButton.get().SetBackgroundColour(currentPlayer.get().color);
}

void TTT_pve_Frame::OnButton(wxCommandEvent& event)
{

    TTTButtom& selectedButton = *mapButton.at(event.GetId());

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        OnP1First(event);
        return;
    }

    currentPlayer = p1;
    indexButton.get().SetBackgroundColour(currentPlayer.get().color);

    __ComTurn__();
}

wxIMPLEMENT_APP(TTTApp<TTT_pve_Frame>);

/* =================  TEST ================= */
static_assert(SELF_REFERENCE_TEST<TTT_pve_Frame>(), "TTT_pve_Frame is not compatible with self reference");
