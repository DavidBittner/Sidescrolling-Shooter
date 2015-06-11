#ifndef STATE_GAME_H_INCLUDED
#define STATE_GAME_H_INCLUDED

#include "class_ply.h"
#include "class_wall.h"

#include <te.h>
#include <vector>

Player ply;

camera *MainCam;

TE_SPRITE ground;
TE_SPRITE background;

vector<Wall> Walls;

float fade = 1.5f;

TE_SOUND SOUND_gamesong( "sounds/game/song_game.ogg" );
TE_SOUND SOUND_bullethit( "sounds/game/bullethit.ogg" );
TE_SOUND SOUND_deathsong( "sounds/game/song_death.ogg" );
TE_SOUND SOUND_winsong( "sounds/game/song_win.ogg" );

void STATE_GAME_LOAD()
{

    //Loading textures.
    ground.Create( 0, 1, 1 );
    ground.LoadThruFunc( "texs/game/floortile.png", 512, 512, GL_LINEAR );

    background.Create( 0, 1, 1 );
    background.LoadThruFunc( "texs/game/backgroundtile.png", 256, 256, GL_LINEAR );

    MainCam = ply.GetCam();

	ply.Create();

    GLuint walltex = LoadTexture( "texs/game/wall.png", 256, 256, GL_LINEAR, GL_NEAREST );

    for( int i = 0; i < 20; i++ )
    {

        Walls.push_back( Wall() );
        Walls.back().Create( walltex, -256, 64 + (i*128 ), 1 );
        if( i == 19 ) Walls.back().Create( walltex, -256, 64+(i*128), 3 );

    }

    SOUND_bullethit.initSound();
    SOUND_gamesong.initSound();
    SOUND_deathsong.initSound();
    SOUND_winsong.initSound();
    SOUND_gamesong.Play( 5, true );

}

bool STATE_GAME_ACT_ON_COLLISION( TE_RECT *a, TE_RECT *b, int type )
{

    if( TE_AABB( a, b ) )
    {

        //Declaring variables that are the coordinates of the two centers of the rectangles.
        float acentx = a->x+(a->w/2.0);
        float bcentx = b->x+(b->w/2.0);

        float acenty = a->y+(a->h/2.0);
        float bcenty = b->y+(b->h/2.0);

        float horzdist = abs( abs(acentx) - abs(bcentx) );
        float vertdist = abs( abs(acenty) - abs(bcenty) );

        if( horzdist > vertdist )
        {

            //If it is a horizontal collision, I.E. sides. (yes, they are in orbit)

            if( acentx > bcentx )
            {

                a->x = b->x+b->w;
                ply.StopXVel( false );

            }else if( acentx < bcentx )
            {

                a->x = b->x-a->w;
                ply.StopXVel( true );

            }

        }else if( horzdist < vertdist and ( type == 0 or type == 2 or type == 3 or type == 4 ) )
        {

            //If it's a vertical collision I.E. top or bottoms.
            if( acenty > bcenty and ply.getYVel() < 0 and ( type == 0 or type == 2 or type == 3 ) )
            {

                a->y = b->y+b->h;
                ply.StopYVel();
                ply.ResetJump();


            }else if( acenty < bcenty and ply.getYVel() > 0 and ( type == 0 or type != 3 ))
            {

                a->y = b->y-a->h;
                ply.StopYVel(); ply.ResetJump();

            }

        }

        return true;

    }

    return false;

}

bool CHECK_BULLET_COLLISION( TE_RECT *a, TE_RECT *b )
{

    TE_RECT temp;
    temp = *b;
    temp.x = temp.x - temp.w/2;
    temp.y = temp.y - temp.h/2;

    temp.w = temp.w-10;
    temp.x = temp.x+5;

    temp.h = temp.h-10;
    temp.y = temp.y+5;

    return TE_AABB( a, &temp );

}

void STATE_GAME_RUN()
{

    glPushAttrib( GL_CURRENT_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, fade );

	if( ply.getHealth() > 0.0f ) ply.Move();

    for( int y = -5; y < 6; y++ )
    {

        for( int x = -8; x < 9; x++ )
        {

            int tempx = MainCam->x;
            int tempy = MainCam->y;
            tempx = tempx/128;
            tempy = tempy/128;

            background.Draw( 0, (tempx*128) + (128*x), (tempy*128) + (128*y)+64, 128, 128, 0.0f );

        }

    }

    //Draws the ground so that it stays within the screen.

    for( int i = -3; i < 4; i++ )
    {

        int temp = MainCam->x;
        temp = temp/512;
        ground.Draw( 0, temp*512 + ( 512*i ), -256, 512, 512, 0.0f );

    }

    //Code that deals with collisions with the player and the walls.

    unsigned int am = 0;

    for( unsigned i = 0; i < Walls.size(); i++ )
    {

        STATE_GAME_ACT_ON_COLLISION( ply.GetRect(), Walls[i].GetRect(), Walls[i].GetAlignType() );

        Bullet *temp;
        temp = ply.GetBullets( &am );

        for( unsigned int j = 0; j < am; j++ )
        {

            if( temp[j].IsAlive() and CHECK_BULLET_COLLISION( temp[j].GetRect(), Walls[i].GetRect() ) )
            {

                temp[j].Kill();
                SOUND_bullethit.Play(20);

            }


        }

    }

    for( unsigned i = 0; i < Walls.size(); i++ )
    {

        Walls[i].Draw();

    }

    glPopAttrib();

    if( ply.getHealth() > 0.0f )
    {

        ply.Draw();

    }

}

void STATE_GAME_DRAW_GUI()
{

    //GUI methods here.
    if( ply.getHealth() > 0.0f ) ply.DrawGUI();
    else { fade-=0.01f; SOUND_gamesong.Stop(); }

}

#endif // STATE_GAME_H_INCLUDED
