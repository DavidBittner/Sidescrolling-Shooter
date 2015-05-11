#include <GL/glew.h>
#include <GL/glfw.h>
#include <irrKlang/irrKlang.h>

#include "state_game.h"
#include <string>
#include <sstream>

#include <te_class_fonts.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

    //Loading the game state.
	STATE_GAME_LOAD();
	mainFont.Create( 16, 8, "texs/gui/font.png" );

    int windwid, windhei;

	while( !glfwWindowShouldClose( window ) )
	{
        float stime = glfwGetTime();

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

		glPushMatrix();

        glfwGetWindowSize( window, &windwid, &windhei );

        float xtrans, ytrans;
        xtrans = MainCam->x - (TE_WINDOW_WIDTH/2.0);
        ytrans = MainCam->y - (windhei/2.0);

        glTranslatef( -xtrans, -(ytrans), -1.0f );

		STATE_GAME_RUN();
        TE_RESET_KEYS();
		glPopMatrix();

		//UI Elements
		glPopMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f );

        glfwPollEvents();
        glfwSwapBuffers( window );

		TE_CALC_FRAMES( stime );
		cout << TE_CUR_SECOND << endl;


	}

    glfwTerminate();
    exit(0);

    return 0;
}
