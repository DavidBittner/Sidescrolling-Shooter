#ifndef STATE_GAME_H_INCLUDED
#define STATE_GAME_H_INCLUDED

#include "class_ply.h"
#include "class_wall.h"

#include <te.h>
#include <vector>

Player ply;

camera *MainCam;

Sprite ground;
Sprite background;

TE_SOUND SOUND_music("sounds/game/song.wav" );

vector<Wall> Walls;

using namespace irrklang;

extern ISoundEngine *engine;

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

    for( int i = 0; i < 4; i++ )
    {

        Walls.push_back( Wall() );
        Walls.back().Create( walltex, 64+(i*256), 64 + (i*128 ) );

    }

    SOUND_music.Play( 100 );

}

bool STATE_GAME_ACT_ON_COLLISION( rect *a, rect *b )
{

    if( AABB( a, b ) )
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

        }else if( horzdist < vertdist )
        {

            //If it's a vertical collision I.E. top or bottoms.
            if( acenty > bcenty and ply.getYVel() < 0 )
            {

                a->y = b->y+b->h;
                ply.StopYVel();
                ply.ResetJump();

            }else if( acenty < bcenty and ply.getYVel() > 0 )
            {

                a->y = b->y-a->h;
                ply.StopYVel();

            }

        }

        return true;

    }

    return false;

}

void STATE_GAME_RUN()
{

	ply.Move();

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

    for( unsigned i = 0; i < Walls.size(); i++ )
    {

        Walls[i].Draw();
        STATE_GAME_ACT_ON_COLLISION( ply.GetRect(), Walls[i].GetRect() );

    }

    ply.Draw();

}

#endif // STATE_GAME_H_INCLUDED
