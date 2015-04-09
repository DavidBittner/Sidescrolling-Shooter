#ifndef CLASS_PLY_H_INCLUDED
#define CLASS_PLY_H_INCLUDED

#include <te.h>
#include "struct_camera.h"

class Player
{

    public:
        Player();
        void Create();

        void Draw();
        void Move();

        void StopXVel( bool left );
        void StopYVel();
        void ResetJump();

        float getYVel();

        rect *GetRect();
        camera *GetCam();

    private:
        float mass;
        double xvel, yvel;
        float tarxvel;

        bool jump;

        int animframe;
        bool dir;

        bool vertColSound, horizColSound;

        camera cam;

        rect plyrect;
        rect colrect;
        Sprite plysprite;

        TE_SOUND SOUND_collide;
        TE_SOUND SOUND_step;

};

Player::Player():
    SOUND_collide( "sounds/game/collision.wav" ),
    SOUND_step( "sounds/game/footstep.wav" )
{

    vertColSound = false;
    horizColSound = false;

    dir = false;

    jump = true;

    animframe = 0;

    mass = 60; //kilograms
    xvel = 0.0f; yvel = 0.0f;

    plyrect.w = 128.0f;
    plyrect.h = 128.0f;

    colrect.w = 88.0f;
    colrect.h = 128.0f;

    plyrect.x = plyrect.w+128.0f;
    plyrect.y = plyrect.w+128.0f;
    colrect.x = plyrect.x+20.0f;
    colrect.y = plyrect.y;

}

float Player::getYVel()
{

    return yvel;

}

void Player::StopXVel( bool left )
{

    plyrect.x = colrect.x-20.0f;

    if( xvel > 1.0f or xvel < -1.0f )
    {

        SOUND_collide.Play( abs(xvel*10.0) );

    }
    xvel = 0.0f;
    tarxvel = 0.0f;

}
void Player::StopYVel()
{

    plyrect.y = colrect.y;

    if( yvel > 0.5f or yvel < -0.5f )
    {

        SOUND_collide.Play( abs(yvel*5.0) );

    }
    yvel = 0.0f;
}
void Player::ResetJump()
{
    jump = true;
}

camera *Player::GetCam()
{

    return &cam;

}

void Player::Create()
{

    plysprite.Create( 0, 2, 1 );
    plysprite.LoadThruFunc( "texs/game/playersheet.png", 256, 256, GL_NEAREST );

}

void Player::Draw()
{

    if(!dir)plysprite.Draw( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f );
    if(dir)plysprite.DrawFlipped( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f );
    cam.x = plyrect.x;
    cam.y = plyrect.y;

}

rect *Player::GetRect()
{

    return &colrect;

}

void Player::Move()
{

    //extern float RUN_FRAMES_PER_SECOND;

    bool allowFrameChange = false;

    extern int frame;
    if( frame%20/((keyMods[GLFW_MOD_SHIFT]+1)) == 0 )
    {

        allowFrameChange = true;

    }

    xvel = round( xvel*10 )/10;

    yvel -= 0.5f;

    if( keyStates[GLFW_KEY_A] and jump )
    {
        tarxvel = -5.0f * (keyMods[GLFW_MOD_SHIFT]+1);
        if(allowFrameChange){animframe++; SOUND_step.Play( 5 ); }
        dir = true;
    }else if( keyStates[GLFW_KEY_D] and jump )
    {
        tarxvel = 5.0f * (keyMods[GLFW_MOD_SHIFT]+1);
        if(allowFrameChange){animframe++; SOUND_step.Play( 5 ); }
        dir = false;
    }else if( jump )
    {
        tarxvel = 0.0f;
    }

    if( !jump and keyStates[GLFW_KEY_A] and xvel > -10.0f )
    {
        tarxvel-=0.2f;
    }else if( !jump and keyStates[GLFW_KEY_D] and xvel < 10.0f )
    {
        tarxvel+=0.2f;
    }

    if( keyDownState[GLFW_KEY_SPACE] and jump )
    {
        yvel = 15.0f;
        jump = !jump;
    }

    if( xvel < tarxvel )
    {

        xvel += 1.0f;

    }else if( xvel > tarxvel )
    {

        xvel -= 1.0f;

    }

    plyrect.x += xvel;
    plyrect.y += yvel;
    colrect.x = plyrect.x+20.0f;
    colrect.y = plyrect.y;

    if( plyrect.y < plyrect.w/2 )
    {

        if( yvel > 0.5f or yvel < -0.5f )
        {

            SOUND_collide.Play( abs(yvel*5.0) );

        }

        jump = true;
        plyrect.y = colrect.h/2;
        yvel = 0.0f;

    }

    if( animframe > 1 )
        animframe = 0;

}

#endif // CLASS_PLY_H_INCLUDED
