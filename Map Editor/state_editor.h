#ifndef STATE_EDITOR_H_INCLUDED
#define STATE_EDITOR_H_INCLUDED
#include <te.h>
#include "struct_camera.h"

Sprite Floor;

void STATE_EDITOR_LOAD()
{

	Floor.Create( 0, 1, 1 );
	Floor.LoadThruFunc( "floor.png", 512, 512, GL_LINEAR );

}

void STATE_LOAD_RUN()
{

	for( int i = -2; i < 2; i++ )
	{

        int tempx = 0;
        Floor.Draw( 0, )

	}

}

#endif // STATE_EDITOR_H_INCLUDED
