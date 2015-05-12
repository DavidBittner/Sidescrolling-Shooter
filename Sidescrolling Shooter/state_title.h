#ifndef STATE_TITLE_H_INCLUDED
#define STATE_TITLE_H_INCLUDED

#include <te.h>
#include <vector>

#include "state_game.h"

static vector< TE_BUTTON > titlebuttons;
static TE_SOUND SOUND_titlemusic( "sounds/gui/titlesong.ogg" );

void STATE_TITLE_LOAD()
{

    titlebuttons.push_back( TE_BUTTON() );
    titlebuttons[0].Create( 0, 400, 512, 128, 1, 4, "texs/gui/title_buttonsheet.png" );
    titlebuttons[0].SetAttrib( TE_BUTTON_CENTERX );
    titlebuttons[0].SetAttrib( TE_BUTTON_SCALEX );
    //titlebuttons[0].SetAttrib( TE_BUTTON_SCALEY );

    SOUND_titlemusic.initSound();
    SOUND_titlemusic.Play( 80, true );

}

void STATE_TITLE_RUN()
{

    for( unsigned i = 0; i < titlebuttons.size(); i++ )
    {

        titlebuttons[i].MouseIsOver();
        titlebuttons[i].Draw();

    }

    extern int GAME_STATE;
    if( titlebuttons[0].IsClicked() ){ GAME_STATE = 1; STATE_GAME_LOAD(); SOUND_titlemusic.Stop(); }

}

#endif // STATE_TITLE_H_INCLUDED
