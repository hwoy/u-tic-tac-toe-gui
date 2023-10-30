#include "main.h"

class TTT_pvp_Frame : public TTTFrame {
public:
    TTT_pvp_Frame();

    virtual void OnButton(wxCommandEvent& event) override;
};

TTT_pvp_Frame::TTT_pvp_Frame()
    : TTTFrame("u-Tic-Tac-Toe PVP")
{
    wxCommandEvent event;
    OnNewGame(event);
}

void TTT_pvp_Frame::OnButton(wxCommandEvent& event)
{
    TTTButtom& selectedButton = *mapButton.at(event.GetId());

    const auto rivalindex = playerindex == PlayerIndex::p1 ? PlayerIndex::p2 : PlayerIndex::p1;
    switch (const auto gameid = game.gameplay(player[rivalindex], player[playerindex], selectedButton.val)) {
    case ffi::ox_idoutofrange:
    case ffi::ox_idvalueexist:
        return;
    case ffi::ox_idgame:
    case ffi::ox_idwin:
    case ffi::ox_iddraw:
        selectedButton.SetBackgroundColour(player[playerindex].color);
        selectedButton.Disable();
        if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
            wxMessageBox(gameid == ffi::ox_idwin ? (playerindex == PlayerIndex::p1 ? "P1 wins" : "P2 wins") : "Draw!", "Game Over!", wxOK | wxICON_INFORMATION);
            newgame(rivalindex);
            return;
        }
    default:
        break;
    }

    playerindex = rivalindex;
    indexButton.get().SetBackgroundColour(player[playerindex].color);
}

wxIMPLEMENT_APP(TTTApp<TTT_pvp_Frame>);
