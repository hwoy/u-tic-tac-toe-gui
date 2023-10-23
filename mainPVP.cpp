#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.h"

class TTT_pvp_Frame : public TTTFrame {
public:
    TTT_pvp_Frame();
    virtual void OnButton(wxCommandEvent& event) override;
};

TTT_pvp_Frame::TTT_pvp_Frame()
    : TTTFrame("u-Tic-Tac-Toe PVP", wxPoint(0, 0), wxGetDisplaySize())
{
    newgame(nullptr);
}

void TTT_pvp_Frame::OnButton(wxCommandEvent& event)
{
    TTTButtom* selectedButton = mapButton.at(event.GetId());

    auto* rival = (currentPlayer == &p1) ? &p2 : &p1;
    switch (const auto gameid = game.gameplay(rival, currentPlayer, selectedButton->val)) {
    case ox_idoutofrange:
    case ox_idvalueexist:
        return;
    case ox_idgame:
    case ox_idwin:
    case ox_iddraw:
        selectedButton->SetBackgroundColour(currentPlayer->color);
        selectedButton->Disable();
        if (gameid == ox_idwin || gameid == ox_iddraw) {
            wxMessageBox(gameid == ox_idwin ? (currentPlayer == &p1 ? "P1 wins" : "P2 wins") : "Draw!", "Game Over!", wxOK | wxICON_INFORMATION);
            newgame(rival);
            return;
        }
    default:
        break;
    }

    currentPlayer = rival;
    indexButton->SetBackgroundColour(currentPlayer->color);
}

wxIMPLEMENT_APP(TTTApp<TTT_pvp_Frame>);
