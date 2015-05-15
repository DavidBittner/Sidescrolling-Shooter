#ifndef STATE_TITLE_H_INCLUDED
#define STATE_TITLE_H_INCLUDED

#include <te.h>
#include <vector>

#include "state_game.h"

static vector< TE_BUTTON > titlebuttons;
static TE_SOUND SOUND_titlemusic( "sounds/gui/maintheme.ogg" );

void STATE_TITLE_LOAD()
{

    for( int i = 0; i < 3; i++ )
    {

        titlebuttons.push_back( TE_BUTTON() );
        titlebuttons[i].Create( 0, i*128 + 40*i + 40, 512, 128, 1, 4, "texs/gui/title_sheet.png" );
        titlebuttons[i].SetAttrib( TE_BUTTON_CENTERX );

    }

    SOUND_titlemusic.initSound();
    SOUND_titlemusic.Play( 80, true );

}

void STATE_TITLE_RUN()
{

    for( unsigned i = 0; i < titlebuttons.size(); i++ )
    {

        titlebuttons[i].MouseIsOver();
        titlebuttons[i].Draw( i );

    }

    extern int GAME_STATE;
    if( titlebuttons[0].IsClicked() ){ GAME_STATE = 1; STATE_GAME_LOAD(); SOUND_titlemusic.Stop(); }

}

#endif // STATE_TITLE_H_INCLUDED
