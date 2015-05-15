#ifndef CLASS_PLY_H_INCLUDED
#define CLASS_PLY_H_INCLUDED

#include <te.h>
#include "class_bullet.h"
#include "struct_camera.h"

GLuint bullet;

class Player
{

    public:
        Player();
        void Create();

        void Draw();
        void Move();

        //Stopping various velocities for collisions, and resetting the jump boolean.
        void StopXVel( bool left );
        void StopYVel();
        void ResetJump();

        //Returning the y velocity so the program can determine the vertical direction of movement.
        float getYVel();

        TE_RECT *GetRect();
        camera *GetCam();

        Bullet *GetBullets( unsigned int *am );

    private:
        float mass;
        double xvel, yvel;
        float tarxvel;

        int maxhorispeed;

        bool jump;

        vector<Bullet> bullets;

        int animframe;
        bool dir;

        camera cam;

        TE_RECT plyrect;
        TE_RECT colrect;
        TE_SPRITE plysprite;
        TE_SPRITE armSprite;
        float armAng;
        int armx, army;

        TE_SOUND SOUND_collide;
        TE_SOUND SOUND_step;

        TE_SOUND SOUND_BULLET_shot;

};


//Beginning of the constructor runs the sounds constructors.
Player::Player():
    SOUND_collide( "sounds/game/collision.ogg" ),
    SOUND_step( "sounds/game/footstep.ogg" ),
    SOUND_BULLET_shot( "sounds/game/gunshot.ogg" )
{

    armx = -30;
    army = -12;

    //Direction
    dir = false;

    jump = true;

    animframe = 0;

    xvel = 0.0f; yvel = 0.0f;

    //The players image rectangle versus the players collision rectangle.
    //Different sizes due to the player not taking up the full image.
    plyrect.w = 128.0f;
    plyrect.h = 128.0f;

    colrect.w = 85.0f;
    colrect.h = 120.0f;

    plyrect.x = plyrect.w+128.0f;
    plyrect.y = plyrect.w+128.0f;
    colrect.x = plyrect.x+24.0f;
    colrect.y = plyrect.y;

    maxhorispeed = 600;

}

Bullet *Player::GetBullets( unsigned int *am )
{

    *am = bullets.size();
    return bullets.data();

}

float Player::getYVel()
{

    return yvel;

}

void Player::StopXVel( bool left )
{

    plyrect.x = colrect.x-20.0f;

    if( xvel > 300.0f or xvel < -300.0f )
    {

        SOUND_collide.Play( abs(xvel*10.0) );

    }
    xvel = 0.0f;
    tarxvel = 0.0f;

}

void Player::StopYVel()
{

    plyrect.y = colrect.y;

    if( (yvel > 30.0f or yvel < -30.0f) )
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
    armSprite.Create( 0, 1, 1 );
    armSprite.LoadThruFunc( "texs/game/arm.png", 32, 8, GL_NEAREST );
    bullet = LoadTexture( "texs/game/bullet.png", 16, 16, GL_NEAREST, GL_NEAREST );

    SOUND_BULLET_shot.initSound();

    SOUND_step.initSound();
    SOUND_collide.initSound();

}

void Player::Draw()
{
    static bool last;

    //Whether or not to draw the player facing left or right.

    if( tarxvel > 0 ){ plysprite.Draw( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f ); last = true; armx = 30; army = -8; }
    else if( tarxvel < 0 ){ plysprite.DrawFlipped( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f ); last = false; armx = -30; army = -8;  }
    else if( last ){ plysprite.Draw( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f ); }
    else if( !last ){ plysprite.DrawFlipped( animframe, plyrect.x, plyrect.y, plyrect.w, plyrect.w, 0.0f ); }
    cam.x = plyrect.x;
    cam.y = plyrect.y;

    for( unsigned int i = 0; i < bullets.size(); i++ )
    {

        bullets[i].Draw();

    }

    armSprite.Draw( 0, plyrect.x+armx, plyrect.y+army, 64, 8, armAng );

}

TE_RECT *Player::GetRect()
{

    return &colrect;

}

void Player::Move()
{

    //The frame and if-statement that control whether or not to change the players animation frame.

    bool shift = glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT );
    bool allowFrameChange = false;

    extern int frame;
    if( frame%(10/(shift+1)) == 0 )
    {

        allowFrameChange = true;

    }

    yvel -= 1000.0f*TE_DELTA_TIME;

    //The If statements that control movement targets. Basically, just checking key-presses.

    if( TE_KEYSTATES[GLFW_KEY_A] )
    {

        tarxvel = -maxhorispeed * (shift+1);
        if( allowFrameChange and jump )
        {

            animframe++;
            SOUND_step.Play( 50 );

        }

    }else if( TE_KEYSTATES[GLFW_KEY_D] )
    {

        tarxvel = maxhorispeed * (shift+1);
        if( allowFrameChange and jump )
        {

            animframe++;
            SOUND_step.Play( 50 );

        }

    }else
    {

        tarxvel = 0.0f;
        if( xvel < 0.0f and xvel > -100.0f ) xvel = 0.0f;
        else if( xvel > 0.0f and xvel < 100.0f ) xvel = 0.0f;

        animframe = 0;

    }

    if( TE_KEYPRESS[GLFW_KEY_SPACE] and jump )
    {

        yvel = 600.0f;
        jump = !jump;

    }

    //Getting the actual velocity up to the velocity target for gradual motion instead of sudden motion.

    if( xvel < tarxvel )
    {

        xvel += 4000.0f*TE_DELTA_TIME;
        if( xvel > tarxvel ) xvel = tarxvel;

    }
    if( xvel > tarxvel )
    {

        xvel -= 4000.0f*TE_DELTA_TIME;

        if( xvel < tarxvel ) xvel = tarxvel;

    }

    plyrect.x += xvel*TE_DELTA_TIME;
    plyrect.y += yvel*TE_DELTA_TIME;
    colrect.x = plyrect.x+20.0f;
    colrect.y = plyrect.y;

    if( plyrect.y < plyrect.w/2 )
    {

        if( yvel > 30.0f or yvel < -30.0f )
        {

            SOUND_collide.Play( abs(yvel*5.0) );

        }

        jump = true;
        plyrect.y = colrect.h/2;
        yvel = 0.0f;

    }

    if( animframe > 1 )
        animframe = 0;

    armAng = TE_GET_INCLIN( plyrect.x+armx, plyrect.y+army,
                           TE_MOUSE_POS.x+( plyrect.x  - (TE_WINDOW_WIDTH/2.0) ),
                           TE_MOUSE_POS.y+( plyrect.y  - (TE_WINDOW_HEIGHT/2.0) ) );

    if( TE_MOUSECLICK[GLFW_MOUSE_BUTTON_LEFT] )
    {

        int randnum = rand()%5;
        randnum = randnum-2;

        bullets.push_back( Bullet( &SOUND_BULLET_shot ) );
        bullets.back().Shoot( armAng+randnum, 2500 );
        bullets.back().Create( bullet, plyrect.x+armx, plyrect.y+army, 3000 );

    }

    for( unsigned int i = 0; i < bullets.size(); i++ )
    {

        bullets[i].Move();

    }

}

#endif // CLASS_PLY_H_INCLUDED
