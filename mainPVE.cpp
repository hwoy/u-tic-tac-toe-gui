#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.h"

class TTT_pve_Frame : public TTTFrame {
protected:
    int __OnButton__(TTTButtom* selectedButton);
    void __ComTurn__();

public:
    TTT_pve_Frame();
    virtual void OnNewGame(wxCommandEvent& event) override;
    virtual void OnP1First(wxCommandEvent& event) override;
    virtual void OnButton(wxCommandEvent& event) override;
};

void TTT_pve_Frame::OnNewGame(wxCommandEvent& event)
{
    OnP1First(event);
}

TTT_pve_Frame::TTT_pve_Frame()
    : TTTFrame("u-Tic-Tac-Toe PVE", wxPoint(0, 0), wxGetDisplaySize())
{
    wxCommandEvent event;
    OnP1First(event);
}

void TTT_pve_Frame::OnP1First(wxCommandEvent& event)
{
    TTTFrame::OnP1First(event);
    __ComTurn__();
}

int TTT_pve_Frame::__OnButton__(TTTButtom* selectedButton)
{
    auto* rival = (currentPlayer == &p1) ? &p2 : &p1;
    const auto gameid = game.gameplay(rival, currentPlayer, selectedButton->val);
    switch (gameid) {
    case ffi::ox_idgame:
    case ffi::ox_idwin:
    case ffi::ox_iddraw:
        selectedButton->SetBackgroundColour(currentPlayer->color);
        selectedButton->Disable();
        if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
            wxMessageBox(gameid == ffi::ox_idwin ? (currentPlayer == &p1 ? "P1(Com) wins" : "P2(You) win") : "Draw!", "Game Over!", wxOK | wxICON_INFORMATION);
        }
    default:
        break;
    }

    return gameid;
}

void TTT_pve_Frame::__ComTurn__()
{
    const auto select = Ai::ai(game, p2, p1);

    TTTButtom* selectedButton = std::find_if(mapButton.begin(), mapButton.end(),
        [select](const auto& buttonPair) {
            return buttonPair.second->val == select;
        })->second;

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        wxCommandEvent event;
        OnP2First(event);
        return;
    }

    currentPlayer = &p2;
    indexButton->SetBackgroundColour(currentPlayer->color);
}

void TTT_pve_Frame::OnButton(wxCommandEvent& event)
{

    TTTButtom* selectedButton = mapButton.at(event.GetId());

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        OnP1First(event);
        return;
    }

    currentPlayer = &p1;
    indexButton->SetBackgroundColour(currentPlayer->color);

    __ComTurn__();
}

wxIMPLEMENT_APP(TTTApp<TTT_pve_Frame>);
