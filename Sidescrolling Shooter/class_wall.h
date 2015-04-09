#ifndef CLASS_WALL_H_INCLUDED
#define CLASS_WALL_H_INCLUDED

#include <te.h>

class Wall
{

    public:
        void Draw();
        void Create( GLuint tex, int x, int y );

        coord GetZone();

        rect *GetRect();

    private:
        rect wallrect;
        Sprite wall;

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

coord Wall::GetZone()
{

    coord temp;
    temp.x = wallrect.x;
    temp.y = wallrect.y;

    return temp;

}

rect *Wall::GetRect()
{

    return &wallrect;

}

#endif // CLASS_WALL_H_INCLUDED
