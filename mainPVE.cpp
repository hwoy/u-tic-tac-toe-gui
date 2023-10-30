#include "main.h"

class TTT_pve_Frame : public TTTFrame {
protected:
    ffi::ox_gameid __OnButton__(TTTButtom& selectedButton);
    void __ComTurn__();

public:
    TTT_pve_Frame();

    virtual void OnNewGame(wxCommandEvent& event) override;
    virtual void OnP1First(wxCommandEvent& event) override;
    virtual void OnButton(wxCommandEvent& event) override;
};

TTT_pve_Frame::TTT_pve_Frame()
    : TTTFrame("u-Tic-Tac-Toe PVE")
{
    wxCommandEvent event;
    OnNewGame(event);
}

void TTT_pve_Frame::OnNewGame(wxCommandEvent& event)
{
    ffi::ox_random(&game, 0, 1) ? OnP1First(event) : OnP2First(event);
}

void TTT_pve_Frame::OnP1First(wxCommandEvent& event)
{
    TTTFrame::OnP1First(event);
    __ComTurn__();
}

ffi::ox_gameid TTT_pve_Frame::__OnButton__(TTTButtom& selectedButton)
{
    const auto rivalindex = playerindex == PlayerIndex::p1 ? PlayerIndex::p2 : PlayerIndex::p1;
    const auto gameid = game.gameplay(player[rivalindex], player[playerindex], selectedButton.val);
    switch (gameid) {
    case ffi::ox_idgame:
    case ffi::ox_idwin:
    case ffi::ox_iddraw:
        selectedButton.SetBackgroundColour(player[playerindex].color);
        selectedButton.Disable();
        if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
            wxMessageBox(gameid == ffi::ox_idwin ? (playerindex == PlayerIndex::p1 ? "P1(Com) wins" : "P2(You) win") : "Draw!", "Game Over!", wxOK | wxICON_INFORMATION);
        }
    default:
        break;
    }

    return gameid;
}

void TTT_pve_Frame::__ComTurn__()
{
    const auto select = Ai::ai(game, player[PlayerIndex::p2], player[PlayerIndex::p1]);

    TTTButtom& selectedButton = *std::find_if(mapButton.begin(), mapButton.end(),
        [select](const

#if __cplusplus >= 201402L
            auto
#else
            std::pair<const int, TTTButtom*>
#endif

                & buttonPair) {
            return buttonPair.second->val == select;
        })->second;

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        wxCommandEvent event;
        OnP2First(event);
        return;
    }

    playerindex = PlayerIndex::p2;
    indexButton.get().SetBackgroundColour(player[playerindex].color);
}

void TTT_pve_Frame::OnButton(wxCommandEvent& event)
{

    TTTButtom& selectedButton = *mapButton.at(event.GetId());

    const auto gameid = __OnButton__(selectedButton);
    if (gameid == ffi::ox_idwin || gameid == ffi::ox_iddraw) {
        OnP1First(event);
        return;
    }

    playerindex = PlayerIndex::p1;
    indexButton.get().SetBackgroundColour(player[playerindex].color);

    __ComTurn__();
}

wxIMPLEMENT_APP(TTTApp<TTT_pve_Frame>);
