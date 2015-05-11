#include <GL/glew.h>
#include <GL/glfw.h>
#include <irrKlang/irrKlang.h>

#include "state_game.h"
#include "state_title.h"

#include <string>
#include <sstream>

#include "struct_camera.h"

#include <te_class_fonts.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

unsigned int GAME_STATE = 0;

int frame = 0;

float mouseX, mouseY;

string ToString( int x )
{

	stringstream s;
	s << x;
	return s.str().c_str();

}

int main()
{

    camera temp;
    temp.x = 0;
    temp.y = 0;
    MainCam = &temp;

	bool fullscreen = false;
	Font mainFont;

	glfwInit();
	GLFWwindow *window;
    //To create a window of certain dimensions, full screen or not.
	if(!fullscreen)
		window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Side-scrolling Shooter", NULL, NULL );
	else
		window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Side-scrolling Shooter", glfwGetPrimaryMonitor(), NULL );

	glfwMakeContextCurrent( window );

	glfwSwapInterval( 1 );
	glClearColor( 0.6f, 0.8f, 1.0f, 1.0f );
	TE_INIT( WINDOW_WIDTH, WINDOW_HEIGHT );

	mainFont.Create( 16, 8, "texs/gui/font.png" );

    STATE_TITLE_LOAD();

	while( !glfwWindowShouldClose( window ) )
	{
        float stime = glfwGetTime();

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

		glPushMatrix();

        float xtrans, ytrans;
        xtrans = MainCam->x - (TE_WINDOW_WIDTH/2.0);
        ytrans = MainCam->y - (TE_WINDOW_HEIGHT/2.0);

        glTranslatef( -xtrans, -(ytrans), -1.0f );

        switch( GAME_STATE )
        {

            case 0:
                {

                    break;

                }

            case 1:
                {

                    STATE_GAME_RUN();
                    break;

                }

        }
        TE_RESET_KEYS();
		glPopMatrix();

		//UI Elements
		glPopMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f );

        switch( GAME_STATE )
        {

            case 0:
                {

                    STATE_TITLE_RUN();
                    break;

                }
            case 1:
                {

                    STATE_GAME_DRAW_GUI();
                    break;

                }

        }

        glfwPollEvents();
        glfwSwapBuffers( window );

		TE_CALC_FRAMES( stime );

        frame++;
        if( frame == TE_FRAMES_PER_SECOND ) frame = 0;

	}

    glfwTerminate();
    exit(0);

    return 0;
}
