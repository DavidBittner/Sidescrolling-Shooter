#ifndef CLASS_WALL_H_INCLUDED
#define CLASS_WALL_H_INCLUDED

#include <te.h>

class Wall
{

    public:
        void Draw();
        void Create( GLuint tex, int x, int y );

        TE_COORD GetZone();

        TE_RECT *GetRect();

    private:
        TE_RECT wallrect;
        TE_SPRITE wall;

};

void Wall::Create( GLuint tex, int x, int y )
{

    wall.Create( tex, 1, 1 );

    wallrect.x = x;
    wallrect.y = y;
    wallrect.w = 128;
    wallrect.h = 128;

}

void Wall::Draw()
{

    wall.Draw( 0, wallrect.x, wallrect.y, wallrect.w, wallrect.w, 0.0f );

}

TE_COORD Wall::GetZone()
{

    TE_COORD temp;
    temp.x = wallrect.x;
    temp.y = wallrect.y;

    return temp;

}

TE_RECT *Wall::GetRect()
{

    return &wallrect;

}

#endif // CLASS_WALL_H_INCLUDED
