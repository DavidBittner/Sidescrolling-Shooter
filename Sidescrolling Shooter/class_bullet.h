#ifndef CLASS_BULLET_H_INCLUDED
#define CLASS_BULLET_H_INCLUDED

#include <te.h>

class Bullet
{

    public:
        void Shoot( float ang, float bulspeedval, float buldamage );
        void Move();
        void Draw();
        void Create( GLuint sprite, int x, int y );

        void Kill();

        Bullet( TE_SOUND *gunshot );

        TE_RECT *GetRect();
        bool IsAlive();

    private:
        bool isAlive;

        float bulang;
        float bulspeed;

        float xvel;
        float yvel;

        float buldamage;

        TE_SOUND *SOUND_shot;

        TE_RECT bulrect;
        TE_RECT colrect;
        TE_SPRITE bullet;

};

Bullet::Bullet( TE_SOUND *gunshot )
{

    SOUND_shot = gunshot;

}

void Bullet::Create( GLuint sprite, int x, int y )
{

    bulrect.x = x;
    bulrect.y = y;
    bulrect.w = 16;
    bulrect.h = 16;

    colrect.x = bulrect.x;
    colrect.y = bulrect.y+3;
    colrect.w = 16;
    colrect.h = 10;

    bullet.Create( sprite, 1, 1 );

}

void Bullet::Draw()
{

    if( isAlive ) bullet.Draw( 0, bulrect.x, bulrect.y, 16, 16, bulang );

}
void Bullet::Move()
{

    if( isAlive )
    {

        yvel-=500.0f*TE_DELTA_TIME;

        bulrect.x += xvel*TE_DELTA_TIME;
        bulrect.y += yvel*TE_DELTA_TIME;

        colrect.x = bulrect.x;
        colrect.y = bulrect.y+4;

        if( bulrect.y < 0 ) isAlive = false;

    }


}

void Bullet::Shoot( float ang, float bulspeedval, float damage )
{

    isAlive = true;
    bulang = ang;
    SOUND_shot->Play( 80 );

    bulspeed = bulspeedval;
    buldamage = damage;

    xvel = cos( bulang*(pi/180) )*bulspeed;
    yvel = sin( bulang*(pi/180) )*bulspeed;

}

TE_RECT *Bullet::GetRect()
{

    return &colrect;

}

void Bullet::Kill()
{

    isAlive = false;

}

#endif // CLASS_BULLET_H_INCLUDED
