#ifndef CLASS_BULLET_H_INCLUDED
#define CLASS_BULLET_H_INCLUDED

#include <te.h>

class Bullet
{

    public:
        void Shoot( float ang );
        void Move();
        void Draw();
        void Create( GLuint sprite, int x, int y, float speed );

        Bullet();

        TE_RECT *GetRect();
        bool IsAlive();

    private:
        bool isAlive;

        float bulang;
        float bulspeed;

        TE_SOUND SOUND_shot;

        TE_RECT bulrect;
        TE_RECT colrect;
        TE_SPRITE bullet;

};

Bullet::Bullet():SOUND_shot( "sounds/game/gunshot.wav" )
{



}

void Bullet::Create( GLuint sprite, int x, int y, float speed )
{

    bulrect.x = x;
    bulrect.y = y;
    bulrect.w = 16;
    bulrect.h = 16;

    colrect.x = bulrect.x;
    colrect.y = bulrect.y+3;
    colrect.w = 16;
    colrect.h = 10;

    bulspeed = speed;

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

        bulrect.x += (cos( bulang*(pi/180) )*bulspeed)*TE_SPEED_PROPORTION;
        bulrect.y += (sin( bulang*(pi/180) )*bulspeed)*TE_SPEED_PROPORTION;

        colrect.x = bulrect.x;
        colrect.y = bulrect.y+4;

    }


}

void Bullet::Shoot( float ang )
{

    isAlive = true;
    bulang = ang;
    SOUND_shot.Play( 20 );

}

#endif // CLASS_BULLET_H_INCLUDED
