#ifndef CLASS_DUMMYENEMY_H_INCLUDED
#define CLASS_DUMMYENEMY_H_INCLUDED

#include <te.h>

class DumEnem
{

    public:
        void Draw();
        void Move();
        TE_RECT *getRect();
        void Create( int x, int y );

        void hit( float damage, float force );

    private:
        TE_SPRITE enemysprite;
        TE_RECT enemrect;
        float xvel, yvel;

        TE_PARTICLE_EMITTER parts;
        TE_SPRITE partsprite;

        float health;

};

void DumEnem::Create( int x, int y )
{

    enemrect.x = x;
    enemrect.y = y;
    enemrect.w = 100;
    enemrect.h = 200;

    health = 100.0f;

    enemysprite.Create( 0, 1, 1 );
    enemysprite.LoadThruFunc( "texs/game/enemsprite.png", 256, 256, GL_LINEAR );

    partsprite.Create( 0, 1, 1 );
    partsprite.LoadThruFunc( "texs/game/spark.png", 32, 32, GL_NEAREST );
    parts.Create( &partsprite, 16, 16, 60, 360, 200 );

}

void DumEnem::Move()
{

    yvel -= 1000.0f*TE_DELTA_TIME;

    enemrect.y+=yvel*TE_DELTA_TIME;

    if( enemrect.y < enemrect.h/2 )
    {

        enemrect.y = enemrect.h/2;
        yvel = 0.0f;

    }

    int x;
    if( health > 0.0f )x = rand()%((int)health);
    else x = 0;

    if( health >= 100.0f ) x = 1;

    parts.Move( enemrect.x, enemrect.y, 0.0f, (x==0) );

}

void DumEnem::hit( float damage, float force )
{

    health-=damage;
    cout << health << endl;

}

TE_RECT *DumEnem::getRect()
{

    return &enemrect;

}

void DumEnem::Draw()
{

    enemysprite.Draw( 0, enemrect.x, enemrect.y, enemrect.w, enemrect.h, 0.0f );

    parts.Draw();
}

#endif // CLASS_DUMMYENEMY_H_INCLUDED
