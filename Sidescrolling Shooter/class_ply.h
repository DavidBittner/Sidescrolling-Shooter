#ifndef CLASS_PLY_H_INCLUDED
#define CLASS_PLY_H_INCLUDED

#include <te.h>
#include <te_class_particle.h>
#include "class_bullet.h"
#include "struct_camera.h"

TE_SPRITE BULLET_IND;
GLuint bullet;

struct weapon
{

    int bulletspeed;
    int bulletspread;
    int bulletdamage;

    int bulletspershot;
    int firerate;

    int clipsize;
    int ammo;

    int automat;

    GLuint tex;

    void CreateWep( int speed, int spread, int damage, int ampershot, int amperclip, int isauto, int rps );

};

void weapon::CreateWep( int speed, int spread, int damage, int ampershot, int amperclip, int isauto, int rps )
{

    bulletspeed = speed;
    bulletspread = spread;
    bulletdamage = damage;
    bulletspershot = ampershot;
    clipsize = amperclip;
    automat = isauto;
    firerate = rps;

    ammo = 5000;

}

class Player
{

    public:
        Player();
        void Create();

        void Draw();
        void DrawGUI();
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

        bool hasCollidedHori;
        bool hasCollidedVert;

        double xvel, yvel;
        float propelx, propely;
        float tarxvel;

        int maxhorispeed;

        bool jump;

        vector<Bullet> bullets;

        int animframe;
        bool dir;

        void Propel( float ang, float strength );

        camera cam;

        TE_RECT plyrect;
        TE_RECT colrect;
        TE_SPRITE plysprite;
        TE_SPRITE armSprite;
        float armAng;
        int armx, army;

        float health;

        TE_SOUND SOUND_collide;
        TE_SOUND SOUND_step;

        TE_SOUND SOUND_BULLET_shot;

        bool HAS_RIFLE;
        bool HAS_PISTOL;
        bool HAS_MACHINE_GUN;
        bool HAS_SHOT_GUN;
        weapon WEAPON_RIFLE;
        weapon WEAPON_PISTOL;
        weapon WEAPON_MACHINE_GUN;
        weapon WEAPON_SHOT_GUN;
        int AMMO_PIST;
        int AMMO_RIFLE;
        int AMMO_SHOT;
        int AMMO_MACHIN;

        float lastshot;

        int curclip;
        weapon *curwep;
        void SwitchWep( weapon *wep );
        void Reload();

        TE_SPRITE parttex;

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

    lastshot = TE_CUR_SECOND;

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
    hasCollidedHori = true;

}

void Player::StopYVel()
{

    plyrect.y = colrect.y;

    if( abs(yvel) > 100.0f )
    {

        SOUND_collide.Play( abs(yvel*5.0) );

    }

    float damage;

    if( abs(yvel) > 2000.0f ){ damage = (abs(yvel))/8000.0f; cout << health - damage*100 << endl; }
    else damage = 0.0f;

    health-=damage*100;

    yvel = 0.0f;
    propely = 0.0f;
    hasCollidedVert = true;

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

    static TE_SPRITE temp;
    temp.Create( 0, 1, 1 );
    temp.LoadThruFunc( "texs/game/tile.png", 256, 256, GL_NEAREST );

    plysprite.Create( 0, 2, 1 );
    plysprite.LoadThruFunc( "texs/game/playersheet.png", 256, 256, GL_NEAREST );
    armSprite.Create( 0, 1, 1 );
    armSprite.LoadThruFunc( "texs/game/arm.png", 32, 8, GL_NEAREST );
    bullet = LoadTexture( "texs/game/bullet.png", 16, 16, GL_NEAREST, GL_NEAREST );

    BULLET_IND.Create( 0, 1, 1 );
    BULLET_IND.LoadThruFunc( "texs/game/ammo_counter.png", 32, 8, GL_NEAREST );

    SOUND_BULLET_shot.initSound();

    SOUND_step.initSound();
    SOUND_collide.initSound();

    health = 100.0f;

    // Speed, Spread, Damage, Bullets per shot, Clip size, Is Auto, Shots per Second

    WEAPON_PISTOL.CreateWep(        2000, 6, 5, 1, 13, 0, 10  );
    WEAPON_SHOT_GUN.CreateWep(      2000, 40, 5, 6, 6, 0, 2   );
    WEAPON_RIFLE.CreateWep(         3500, 2, 100, 1, 10, 0, 1 );
    WEAPON_MACHINE_GUN.CreateWep(   2000, 10, 5, 1, 30, 1, 10 );

    curwep = &WEAPON_PISTOL;
    curclip = curwep->clipsize;

    parttex.Create( 0, 1, 1 );
    parttex.LoadThruFunc( "texs/game/spark.png", 128, 128, GL_LINEAR );

}

void Player::SwitchWep( weapon *wep )
{

    curwep->ammo+=curclip;
    curclip = 0;
    curwep = wep;

    if( curwep->ammo > 0 )
    {
        curwep->ammo-=curwep->clipsize;
        curclip = curwep->clipsize;
    }

}

void Player::Reload()
{

    if( curwep->ammo > 0 )
    {

        if( curwep->ammo > curwep->clipsize )
        {
            curwep->ammo-=(curwep->clipsize - curclip);
            curclip = curwep->clipsize;
        }else
        {
            curclip = curwep->ammo;
            curwep->ammo-=curclip;
        }
    }

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

void Player::DrawGUI()
{

    float healthwid = 32;
    TE_RECT healthbar;
    healthbar.x = TE_WINDOW_WIDTH - 32;
    healthbar.w = healthwid;
    healthbar.h = health*2;
    healthbar.y = 32 + healthbar.h/2.0f;
    healthbar.Draw( 1.0f-( health/100.0f), health/100.0f, 0.0f, 0.0f );

    float bulletscale = TE_WINDOW_HEIGHT/curwep->clipsize;

    for( int i = 0; i < curclip; i++ )
    {

        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 1.0f, 1.0f, 1.0f, 0.4f );

        BULLET_IND.Draw( 0, 32, i*bulletscale + bulletscale/2.0f, 64, bulletscale, 0.0f );

        glPopAttrib();

    }

}

TE_RECT *Player::GetRect()
{

    return &colrect;

}

void Player::Propel( float ang, float strength )
{

    float pushstrength = curwep->bulletspeed*curwep->bulletspershot*(curwep->firerate/5.0f);
    pushstrength/=2.0f;

    propelx = -degcos( ang )*pushstrength;
    propely = -degsin( ang )*pushstrength;

}

float TE_CENTER_ZERO( float a, float stren )
{

    float temp;
    if( a > 0.0f ) temp = a-stren;
    if( a < 0.0f ) temp = a+stren;

    if( abs( temp ) < stren )temp = 0.0f;
    return temp;

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

    //The If statements that control movement targets. Basically, just checking key-presses.

    float ysub = -2000.0f;

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

    if( TE_KEYPRESS[GLFW_KEY_SPACE] and jump and yvel == 0.0f )
    {

        yvel = 1000.0f;
        jump = !jump;

    }

    //Getting the actual velocity up to the velocity target for gradual motion instead of sudden motion.

    if( xvel < tarxvel )
    {

        xvel += 5000.0f*TE_DELTA_TIME;
        if( xvel > tarxvel ) xvel = tarxvel;

    }
    if( xvel > tarxvel )
    {

        xvel -= 5000.0f*TE_DELTA_TIME;

        if( xvel < tarxvel ) xvel = tarxvel;

    }

    extern int GAME_STATE;
    if( health <= 0.0f ) GAME_STATE = 0;

    if( propely <= 0.0f )
    {


        if(!hasCollidedHori or yvel>0 )yvel+=ysub*TE_DELTA_TIME;
        else yvel+=ysub*TE_DELTA_TIME;


    }

    plyrect.x += propelx*TE_DELTA_TIME;
    plyrect.y += propely*TE_DELTA_TIME;

    plyrect.x += xvel*TE_DELTA_TIME;
    plyrect.y += yvel*TE_DELTA_TIME;

    colrect.x = plyrect.x+20.0f;
    colrect.y = plyrect.y;

    propelx = TE_CENTER_ZERO( propelx, 3000.0f*TE_DELTA_TIME );
    propely = TE_CENTER_ZERO( propely, 3000.0f*TE_DELTA_TIME );

    if( plyrect.y < plyrect.w/2 )
    {

        StopYVel();

        jump = true;
        plyrect.y = colrect.h/2;

    }

    if( animframe > 1 )
        animframe = 0;

    float ytrans;
    ytrans = plyrect.y - ( TE_WINDOW_HEIGHT/2.0 );
    if( ytrans < -250 ) ytrans = -250;

    //Just weapon switches and reloading.

         if( TE_KEYPRESS[GLFW_KEY_1] ){ SwitchWep( &WEAPON_PISTOL ); }
    else if( TE_KEYPRESS[GLFW_KEY_2] ){ SwitchWep( &WEAPON_RIFLE ); }
    else if( TE_KEYPRESS[GLFW_KEY_3] ){ SwitchWep( &WEAPON_SHOT_GUN ); }
    else if( TE_KEYPRESS[GLFW_KEY_4] ){ SwitchWep( &WEAPON_MACHINE_GUN ); }

    if( TE_KEYPRESS[GLFW_KEY_R] ){ Reload(); }

    //Calculating the rotation angle for the arm.

    armAng = TE_GET_INCLIN( plyrect.x+armx, plyrect.y+army,
                           TE_MOUSE_POS.x+( plyrect.x  - (TE_WINDOW_WIDTH/2.0) ),
                           TE_MOUSE_POS.y+( ytrans ) );

    //Deals with shooting a shot with all the different curwep attributes.
    bool allowshot = false;
    if( TE_CUR_SECOND - lastshot > (100.0f/curwep->firerate)/100.0f ){ allowshot = true; }

    float propelstren = 500.0f;

    if( TE_MOUSECLICK[GLFW_MOUSE_BUTTON_LEFT] and !curwep->automat and curclip > 0 and allowshot )
    {

        lastshot = TE_CUR_SECOND;
        for( int i = 0; i < curwep->bulletspershot; i++ )
        {

            int randnum = (rand()%(int)curwep->bulletspread) - curwep->bulletspread/2.0;

            bullets.push_back( Bullet( &SOUND_BULLET_shot ) );
            bullets.back().Shoot( armAng+randnum,
                                  curwep->bulletspeed,
                                  curwep->bulletdamage,
                                  i );

            bullets.back().Create( bullet,
                                   plyrect.x+armx,
                                   plyrect.y+army,
                                   &parttex
                                    );
            Propel( armAng, propelstren );

        }
        curclip--;

    }else if( TE_MOUSEBUTTONS[ GLFW_MOUSE_BUTTON_LEFT ] and curwep->automat and curclip > 0 and allowshot )
    {

        lastshot = TE_CUR_SECOND;
        for( int i = 0; i < curwep->bulletspershot; i++ )
        {

            int randnum = (rand()%(int)curwep->bulletspread) - curwep->bulletspread/2.0;

            bullets.push_back( Bullet( &SOUND_BULLET_shot ) );
            bullets.back().Shoot( armAng+randnum,
                                  curwep->bulletspeed,
                                  curwep->bulletdamage,
                                  i );

            bullets.back().Create( bullet,
                                   plyrect.x+armx,
                                   plyrect.y+army,
                                   &parttex );

            Propel( armAng, propelstren );

        }
        curclip--;

    }

    for( unsigned int i = 0; i < bullets.size(); i++ )
    {

        bullets[i].Move();

    }

    hasCollidedHori = false;
    hasCollidedVert = false;

    alListener3f( AL_POSITION, plyrect.x, plyrect.y, 0.0f );

}

#endif // CLASS_PLY_H_INCLUDED
