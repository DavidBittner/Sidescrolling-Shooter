#include <GL/glew.h>
#include <GL/glfw.h>
#include <irrKlang/irrKlang.h>

#include "state_game.h"
#include <string>
#include <sstream>

#include <te_class_fonts.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float FRAMES_PER_SECOND = 60.0f;
float RUN_FRAMES_PER_SECOND = 0.0f;
int frame = 0;

void Reshape( GLFWwindow *window, int width, int height )
{

    //Generic window reshape function.
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, width, 0.0, height, 1.0f, 5.0f );
    glMatrixMode( GL_MODELVIEW );

}

string ToString( int x )
{

	stringstream s;
	s << x;
	return s.str().c_str();

}

int main()
{

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

	glfwSetWindowSizeCallback( window, Reshape );
	glfwSetKeyCallback( window, KeyFunc );

	glfwSwapInterval( 1 );
	glClearColor( 0.6f, 0.8f, 1.0f, 1.0f );
	TE_INIT();

	Reshape( window, WINDOW_WIDTH, WINDOW_HEIGHT );

    //Loading the game state.
	STATE_GAME_LOAD();
	mainFont.Create( 16, 8, "texs/gui/font.png" );

    int windwid, windhei;

	while( !glfwWindowShouldClose( window ) )
	{

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

        float stime = glfwGetTime();

		glPushMatrix();

        glfwGetWindowSize( window, &windwid, &windhei );

        float xtrans, ytrans;
        xtrans = MainCam->x - (windwid/2.0);
        ytrans = MainCam->y - (windhei/2.0);

        glTranslatef( -xtrans, -(ytrans+128), -1.0f );

		STATE_GAME_RUN();
		glPopMatrix();

		//UI Elements
		glPopMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f );

        glfwPollEvents();
        glfwSwapBuffers( window );

        float etime = glfwGetTime();
		frame++;

		if( etime-stime < 1000.0f/FRAMES_PER_SECOND )
		{

            Sleep( (1000.0f/FRAMES_PER_SECOND) - (etime-stime) );

		}

		frame++;

		if( frame == FRAMES_PER_SECOND )
		{

			frame = 0;
			RUN_FRAMES_PER_SECOND = 1000.0f/(1000*etime-1000*stime);

		}

	}

    glfwTerminate();
    exit(0);

    return 0;
}
