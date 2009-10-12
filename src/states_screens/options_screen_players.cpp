//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "states_screens/options_screen_input.hpp"
#include "states_screens/options_screen_av.hpp"
#include "states_screens/options_screen_players.hpp"

#include "config/player.hpp"
#include "config/device_config.hpp"
#include "graphics/irr_driver.hpp"
#include "guiengine/engine.hpp"
#include "guiengine/screen.hpp"
#include "guiengine/widget.hpp"
#include "states_screens/dialogs/enter_player_name_dialog.hpp"
#include "states_screens/dialogs/player_info_dialog.hpp"
#include "states_screens/state_manager.hpp"


#include <iostream>
#include <sstream>

using namespace GUIEngine;
using namespace irr::core;
using namespace irr::gui;

OptionsScreenPlayers::OptionsScreenPlayers() : Screen("options_players.stkgui")
{
}
// -----------------------------------------------------------------------------
void OptionsScreenPlayers::init()
{
    RibbonWidget* ribbon = this->getWidget<RibbonWidget>("options_choice");
    if (ribbon != NULL) ribbon->select( "players", GUI_PLAYER_ID );
    
    ListWidget* players = this->getWidget<ListWidget>("players");
    assert(players != NULL);
    
    const int playerAmount = UserConfigParams::m_all_players.size();
    for(int n=0; n<playerAmount; n++)
    {
        players->addItem( UserConfigParams::m_all_players[n].getName() );
    }
}

/**
 * Adds a new player (if 'player' is NULL) or renames an existing player (if 'player' is not NULL)
 */
void OptionsScreenPlayers::gotNewPlayerName(const stringw& newName, PlayerProfile* player)
{
    stringc newNameC( newName );
    ListWidget* players = this->getWidget<ListWidget>("players");
    if(players == NULL) return;
    
    // ---- Add new player
    if(player == NULL)
    {
        UserConfigParams::m_all_players.push_back( new PlayerProfile(newNameC.c_str()) );
        
        players->addItem( newNameC.c_str() );
    }
    else // ---- Rename existing player
    {
        player->setName( newNameC.c_str() );
        
        // refresh list display
        players->clear();
        const int playerAmount =  UserConfigParams::m_all_players.size();
        for(int n=0; n<playerAmount; n++)
        {
            players->addItem(UserConfigParams::m_all_players[n].getName());
        }
        
    }
    // TODO : need to re-save user config here?
}

void OptionsScreenPlayers::deletePlayer(PlayerProfile* player)
{
    UserConfigParams::m_all_players.erase(player);
    
    // refresh list display
    ListWidget* players = this->getWidget<ListWidget>("players");
    if(players == NULL) return;
    players->clear();
    
    const int playerAmount =  UserConfigParams::m_all_players.size();
    for(int n=0; n<playerAmount; n++)
    {
        players->addItem(UserConfigParams::m_all_players[n].getName());
    }
    
    // TODO : need to re-save user config here?
}


// -----------------------------------------------------------------------------
void OptionsScreenPlayers::tearDown()
{
}

// -----------------------------------------------------------------------------
// main call (from StateManager); dispatches the call to a specialissed function as needed
void OptionsScreenPlayers::eventCallback(Widget* widget, const std::string& name)
{    
    if (name == "options_choice")
    {
        std::string selection = ((RibbonWidget*)widget)->getSelectionIDString(GUI_PLAYER_ID).c_str();
        
        if (selection == "audio_video") StateManager::get()->replaceTopMostScreen(OptionsScreenAV::getInstance());
        else if (selection == "players") StateManager::get()->replaceTopMostScreen(OptionsScreenPlayers::getInstance());
        else if (selection == "controls") StateManager::get()->replaceTopMostScreen(OptionsScreenInput::getInstance());
    }
    else if (name == "back")
    {
        StateManager::get()->escapePressed();
    }
    else if (name == "addplayer")
    {
        new EnterPlayerNameDialog(0.5f, 0.4f);
    }
    else if (name == "players")
    {
        // Find which player in the list was clicked
        ListWidget* players = this->getWidget<ListWidget>("players");
        assert(players != NULL);
        
        std::string selectedPlayer = players->getSelectionName();
        const int playerAmount = UserConfigParams::m_all_players.size();
        for(int n=0; n<playerAmount; n++)
        {
            if(UserConfigParams::m_all_players[n].getName() == selectedPlayer)
            {
                new PlayerInfoDialog( &UserConfigParams::m_all_players[n], 0.5f, 0.6f );
                return;
            }
        } // end for
    }
    
}

