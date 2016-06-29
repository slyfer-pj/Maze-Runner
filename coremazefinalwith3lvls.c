#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<stdlib.h>
#include<GL/glut.h>
#define true 1
#define false 0
GLfloat rotation = 90.0;
float posX = 0, posY = 0, posZ = 0;     //for player movement
static int collI=0, collJ=0;       //to track the current matrix position player is in for ez collision detection
static int m=0;         //to keep track of which matrix level you on
typedef struct          //structure of maze with 4 walls, east, west , north, south
{
    int e,w,n,s;
}maze;
int i,j;
maze maze_struct[3][100][100];  //3D array in which info about maze walls resides in
//int move[]={0.0,0.0};
void init(){
    // set clear color to black
    glClearColor(1.0, 0.0, 0.0, 0.0);

    // set fill color to white
    glColor3f(1.0, 1.0, 1.0);

    //set up standard orthogonal view with clipping
    //box as cube of side 2 centered at origin
    //This is the default view and these statements could be removed
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    gluOrtho2D(0.0,150.0,0.0,150.0);
}
void reshape(int width, int heigth){
    // window ro reshape when made it bigger or smaller

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //clip the windows so its shortest side is 2.0
    if (width < heigth) {
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)heigth / (GLfloat)width, 2.0 * (GLfloat)heigth / (GLfloat)width, 2.0, 2.0);
    }
    else{
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)width / (GLfloat)heigth, 2.0 * (GLfloat)width / (GLfloat)heigth,2.0 , 2.0);
    }
    // set viewport to use the entire new window
    glViewport(0, 0, width, heigth);
}
void spawn_exit()                       //spawns the blue colored tile to which the player has to move
{
           glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(115.0,115.0);
    glVertex2f(125.0,115.0);
    glVertex2f(125.0,125.0);
    glVertex2f(115.0,125.0);
    glEnd();
}
void draw_line()                    //used to render vertical walls of maze
{
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(25.0,25.0);
        glVertex2f(25.0,35.0);
    glEnd();
    glFlush();
}
void draw_line_horizontal()         //used to render horizontal walls of maze
{
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(25.0,25.0);
        glVertex2f(35.0,25.0);
    glEnd();
    glFlush();
}
void draw_player();
void render_maze(maze maze_struct[][100][100])      //rendering function of the maze
{
    for(i=0;i<10;i++)
        for(j=0;j<10;j++)
        {
           if(maze_struct[m][i][j].e==true)
            {
                glColor3f(0.0,0.0,0.0);
                glPushMatrix();
                glTranslatef((i+1)*10,(j)*10,0);
                draw_line();
                glPopMatrix();
            }
           if(maze_struct[m][i][j].w==true)
            {
                glColor3f(0.0,0.0,0.0);
                glPushMatrix();
                glTranslatef(i*10,j*10,0);
                draw_line();
                glPopMatrix();
            }
            if(maze_struct[m][i][j].n==true)
            {
                glColor3f(0.0,0.0,0.0);
                glPushMatrix();
                glTranslatef((i)*10,(j+1)*10,0);
                draw_line_horizontal();
                glPopMatrix();
            }
             if(maze_struct[m][i][j].s==true)
            {
                glColor3f(0.0,0.0,0.0);
                glPushMatrix();
                glTranslatef((i)*10,(j)*10,0);
                draw_line_horizontal();
                glPopMatrix();
            }
        }

}
void reset_player()             //resets the players position when he moves on to the next level
{
    collI=0;
    collJ=0;

    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 0.0);
        glVertex2f(25.0,25.0);
        glVertex2f(35.0,25.0);
        glVertex2f(35.0,35.0);
        glVertex2f(25.0,35.0);
    glEnd();
}

void display()
{
    for(i=0;i<10;i++)                                   //maze 1 structure
        for(j=0;j<10;j++)
        {
            maze_struct[0][i][j].e=true;
            maze_struct[0][i][j].w=true;
            maze_struct[0][i][j].n=true;
            maze_struct[0][i][j].s=true;
        }
        maze_struct[0][0][9].s=false;maze_struct[0][0][8].n=false;
        maze_struct[0][0][9].e=false;maze_struct[0][1][9].w=false;
        maze_struct[0][1][9].s=false;maze_struct[0][1][8].n=false;
        maze_struct[0][2][9].e=false;maze_struct[0][3][9].w=false;
        maze_struct[0][3][9].e=false;maze_struct[0][4][9].w=false;
        maze_struct[0][4][9].s=false;maze_struct[0][4][8].n=false;
        maze_struct[0][5][9].s=false;maze_struct[0][5][8].n=false;
        maze_struct[0][5][9].e=false;maze_struct[0][6][9].w=false;
        maze_struct[0][6][9].e=false;maze_struct[0][7][9].w=false;
        maze_struct[0][7][9].e=false;maze_struct[0][8][9].w=false;
        maze_struct[0][8][9].e=false;maze_struct[0][9][9].w=false;
        maze_struct[0][9][9].s=false;maze_struct[0][9][8].n=false;
        maze_struct[0][0][8].s=false;maze_struct[0][0][7].n=false;
        maze_struct[0][1][8].e=false;maze_struct[0][2][8].w=false;
        maze_struct[0][2][8].e=false;maze_struct[0][3][8].w=false;
        maze_struct[0][3][8].e=false;maze_struct[0][4][8].w=false;
        maze_struct[0][4][8].s=false;maze_struct[0][4][7].n=false;
        maze_struct[0][5][8].s=false;maze_struct[0][5][7].n=false;
        maze_struct[0][6][8].s=false;maze_struct[0][6][7].n=false;
        maze_struct[0][6][8].e=false;maze_struct[0][7][8].w=false;
        maze_struct[0][8][8].s=false;maze_struct[0][8][7].n=false;
        maze_struct[0][8][8].e=false;maze_struct[0][9][8].w=false;
        maze_struct[0][0][7].s=false;maze_struct[0][0][6].n=false;
        maze_struct[0][1][7].s=false;maze_struct[0][1][6].n=false;
        maze_struct[0][2][7].s=false;maze_struct[0][2][6].n=false;
        maze_struct[0][2][7].e=false;maze_struct[0][3][7].w=false;
        maze_struct[0][3][7].e=false;maze_struct[0][4][7].w=false;
        maze_struct[0][4][7].e=false;maze_struct[0][5][7].w=false;
        maze_struct[0][5][7].s=false;maze_struct[0][5][6].n=false;
        maze_struct[0][6][7].s=false;maze_struct[0][6][6].n=false;
        maze_struct[0][7][7].s=false;maze_struct[0][7][6].n=false;
        maze_struct[0][7][7].e=false;maze_struct[0][8][7].w=false;
        maze_struct[0][8][7].s=false;maze_struct[0][8][6].n=false;
        maze_struct[0][9][7].s=false;maze_struct[0][9][6].n=false;
        maze_struct[0][0][6].s=false;maze_struct[0][0][5].n=false;
        maze_struct[0][0][6].e=false;maze_struct[0][1][6].w=false;
        maze_struct[0][1][6].s=false;maze_struct[0][1][5].n=false;
        maze_struct[0][2][6].s=false;maze_struct[0][2][5].n=false;
        maze_struct[0][3][6].s=false;maze_struct[0][3][5].n=false;
        maze_struct[0][3][6].e=false;maze_struct[0][4][6].w=false;
        maze_struct[0][5][6].s=false;maze_struct[0][5][5].n=false;
        maze_struct[0][6][6].s=false;maze_struct[0][6][5].n=false;
        maze_struct[0][7][6].s=false;maze_struct[0][7][5].n=false;
        maze_struct[0][8][6].s=false;maze_struct[0][8][5].n=false;
        maze_struct[0][8][6].e=false;maze_struct[0][9][6].w=false;
        maze_struct[0][0][5].s=false;maze_struct[0][0][4].n=false;
        maze_struct[0][1][5].s=false;maze_struct[0][1][4].n=false;
        maze_struct[0][2][5].s=false;maze_struct[0][2][4].n=false;
        maze_struct[0][3][5].s=false;maze_struct[0][3][4].n=false;
        maze_struct[0][4][5].s=false;maze_struct[0][4][4].n=false;
        maze_struct[0][4][5].e=false;maze_struct[0][5][5].w=false;
        maze_struct[0][6][5].e=false;maze_struct[0][7][5].w=false;
        maze_struct[0][7][5].s=false;maze_struct[0][7][4].n=false;
        maze_struct[0][8][5].s=false;maze_struct[0][8][4].n=false;
        maze_struct[0][8][5].e=false;maze_struct[0][9][5].w=false;
        maze_struct[0][0][4].s=false;maze_struct[0][0][3].n=false;
        maze_struct[0][2][4].e=false;maze_struct[0][3][4].w=false;
        maze_struct[0][4][4].e=false;maze_struct[0][5][4].w=false;
        maze_struct[0][5][4].e=false;maze_struct[0][6][4].w=false;
        maze_struct[0][6][4].s=false;maze_struct[0][6][3].n=false;
        maze_struct[0][7][4].s=false;maze_struct[0][7][3].n=false;
        maze_struct[0][8][4].e=false;maze_struct[0][9][4].w=false;
        maze_struct[0][9][4].s=false;maze_struct[0][9][3].n=false;
        maze_struct[0][0][3].s=false;maze_struct[0][0][2].n=false;
        maze_struct[0][0][3].e=false;maze_struct[0][1][3].w=false;
        maze_struct[0][1][3].e=false;maze_struct[0][2][3].w=false;
        maze_struct[0][2][3].e=false;maze_struct[0][3][3].w=false;
        maze_struct[0][3][3].e=false;maze_struct[0][4][3].w=false;
        maze_struct[0][4][3].e=false;maze_struct[0][5][3].w=false;
        maze_struct[0][6][3].s=false;maze_struct[0][6][2].n=false;
        maze_struct[0][8][3].s=false;maze_struct[0][8][2].n=false;
        maze_struct[0][8][3].e=false;maze_struct[0][9][3].w=false;
        maze_struct[0][0][2].s=false;maze_struct[0][0][1].n=false;
        maze_struct[0][0][2].e=false;maze_struct[0][1][2].w=false;
        maze_struct[0][2][2].s=false;maze_struct[0][2][1].n=false;
        maze_struct[0][2][2].e=false;maze_struct[0][3][2].w=false;
        maze_struct[0][3][2].e=false;maze_struct[0][4][2].w=false;
        maze_struct[0][4][2].e=false;maze_struct[0][5][2].w=false;
        maze_struct[0][5][2].e=false;maze_struct[0][6][2].w=false;
        maze_struct[0][7][2].s=false;maze_struct[0][7][1].n=false;
        maze_struct[0][7][2].e=false;maze_struct[0][8][2].w=false;
        maze_struct[0][8][2].e=false;maze_struct[0][9][2].w=false;
        maze_struct[0][0][1].e=false;maze_struct[0][1][1].w=false;
        maze_struct[0][1][1].s=false;maze_struct[0][1][0].n=false;
        maze_struct[0][2][1].s=false;maze_struct[0][2][0].n=false;
        maze_struct[0][3][1].s=false;maze_struct[0][3][0].n=false;
        maze_struct[0][3][1].e=false;maze_struct[0][4][1].w=false;
        maze_struct[0][4][1].s=false;maze_struct[0][4][0].n=false;
        maze_struct[0][5][1].e=false;maze_struct[0][6][1].w=false;
        maze_struct[0][6][1].s=false;maze_struct[0][6][0].n=false;
        maze_struct[0][7][1].e=false;maze_struct[0][8][1].w=false;
        maze_struct[0][8][1].e=false;maze_struct[0][9][1].w=false;
        maze_struct[0][9][1].s=false;maze_struct[0][9][0].n=false;
        maze_struct[0][0][0].e=false;maze_struct[0][1][0].w=false;
        maze_struct[0][2][0].e=false;maze_struct[0][3][0].w=false;
        maze_struct[0][4][0].e=false;maze_struct[0][5][0].w=false;
        maze_struct[0][6][0].e=false;maze_struct[0][7][0].w=false;
        maze_struct[0][7][0].e=false;maze_struct[0][8][0].w=false;
        maze_struct[0][8][0].e=false;maze_struct[0][9][0].w=false;

         for(i=0;i<10;i++)                                      //maze 2 structure
        for(j=0;j<10;j++)
        {
            maze_struct[1][i][j].e=true;
            maze_struct[1][i][j].w=true;
            maze_struct[1][i][j].n=true;
            maze_struct[1][i][j].s=true;
        }
        maze_struct[1][0][9].s=false;maze_struct[1][0][8].n=false;
        maze_struct[1][1][9].s=false;maze_struct[1][1][8].n=false;
        maze_struct[1][1][9].e=false;maze_struct[1][2][9].w=false;
        maze_struct[1][2][9].e=false;maze_struct[1][3][9].w=false;
        maze_struct[1][3][9].e=false;maze_struct[1][4][9].w=false;
        maze_struct[1][4][9].s=false;maze_struct[1][4][8].n=false;
        maze_struct[1][5][9].s=false;maze_struct[1][5][8].n=false;
        maze_struct[1][5][9].e=false;maze_struct[1][6][9].w=false;
        maze_struct[1][6][9].s=false;maze_struct[1][6][8].n=false;
        maze_struct[1][6][9].e=false;maze_struct[1][7][9].w=false;
        maze_struct[1][7][9].e=false;maze_struct[1][8][9].w=false;
        maze_struct[1][8][9].e=false;maze_struct[1][9][9].w=false;
        maze_struct[1][9][9].s=false;maze_struct[1][9][8].n=false;
        maze_struct[1][0][8].s=false;maze_struct[1][0][7].n=false;
        maze_struct[1][1][8].s=false;maze_struct[1][1][7].n=false;
        maze_struct[1][2][8].s=false;maze_struct[1][2][7].n=false;
        maze_struct[1][2][8].e=false;maze_struct[1][3][8].w=false;
        maze_struct[1][3][8].s=false;maze_struct[1][3][7].n=false;
        maze_struct[1][4][8].s=false;maze_struct[1][4][7].n=false;
        maze_struct[1][4][8].e=false;maze_struct[1][5][8].w=false;
        maze_struct[1][6][8].e=false;maze_struct[1][7][8].w=false;
        maze_struct[1][7][8].e=false;maze_struct[1][8][8].w=false;
        maze_struct[1][8][8].s=false;maze_struct[1][8][7].n=false;
        maze_struct[1][0][7].s=false;maze_struct[1][0][6].n=false;
        maze_struct[1][0][7].e=false;maze_struct[1][1][7].w=false;
        maze_struct[1][2][7].s=false;maze_struct[1][2][6].n=false;
        maze_struct[1][3][7].e=false;maze_struct[1][4][7].w=false;
        maze_struct[1][5][7].s=false;maze_struct[1][5][6].n=false;
        maze_struct[1][5][7].e=false;maze_struct[1][6][7].w=false;
        maze_struct[1][6][7].e=false;maze_struct[1][7][7].w=false;
        maze_struct[1][8][7].e=false;maze_struct[1][9][7].w=false;
        maze_struct[1][9][7].s=false;maze_struct[1][9][6].n=false;
        maze_struct[1][0][6].s=false;maze_struct[1][0][5].n=false;
        maze_struct[1][1][6].s=false;maze_struct[1][1][5].n=false;
        maze_struct[1][1][6].e=false;maze_struct[1][2][6].w=false;
        maze_struct[1][3][6].s=false;maze_struct[1][3][5].n=false;
        maze_struct[1][3][6].e=false;maze_struct[1][4][6].w=false;
        maze_struct[1][4][6].e=false;maze_struct[1][5][6].w=false;
        maze_struct[1][6][6].s=false;maze_struct[1][6][5].n=false;
        maze_struct[1][6][6].e=false;maze_struct[1][7][6].w=false;
        maze_struct[1][7][6].e=false;maze_struct[1][8][6].w=false;
        maze_struct[1][8][6].s=false;maze_struct[1][8][5].n=false;
        maze_struct[1][8][6].e=false;maze_struct[1][9][6].w=false;
        maze_struct[1][9][6].s=false;maze_struct[1][9][5].n=false;
        maze_struct[1][0][5].s=false;maze_struct[1][0][4].n=false;
        maze_struct[1][1][5].e=false;maze_struct[1][2][5].w=false;
        maze_struct[1][2][5].s=false;maze_struct[1][2][4].n=false;
        maze_struct[1][3][5].e=false;maze_struct[1][4][5].w=false;
        maze_struct[1][4][5].s=false;maze_struct[1][4][4].n=false;
        maze_struct[1][5][5].e=false;maze_struct[1][6][5].w=false;
        maze_struct[1][6][5].s=false;maze_struct[1][6][4].n=false;
        maze_struct[1][6][5].e=false;maze_struct[1][7][5].w=false;
        maze_struct[1][7][5].s=false;maze_struct[1][7][4].n=false;
        maze_struct[1][8][5].s=false;maze_struct[1][8][4].n=false;
        maze_struct[1][9][5].s=false;maze_struct[1][9][4].n=false;
        maze_struct[1][0][4].s=false;maze_struct[1][0][3].n=false;
        maze_struct[1][1][4].s=false;maze_struct[1][1][3].n=false;
        maze_struct[1][2][4].s=false;maze_struct[1][2][3].n=false;
        maze_struct[1][2][4].e=false;maze_struct[1][3][4].w=false;
        maze_struct[1][3][4].s=false;maze_struct[1][3][3].n=false;
        maze_struct[1][4][4].s=false;maze_struct[1][4][3].n=false;
        maze_struct[1][5][4].s=false;maze_struct[1][5][3].n=false;
        maze_struct[1][5][4].e=false;maze_struct[1][6][4].w=false;
        maze_struct[1][8][4].s=false;maze_struct[1][8][3].n=false;
        maze_struct[1][0][3].e=false;maze_struct[1][1][3].w=false;
        maze_struct[1][1][3].s=false;maze_struct[1][1][2].n=false;
        maze_struct[1][3][3].s=false;maze_struct[1][3][2].n=false;
        maze_struct[1][4][3].s=false;maze_struct[1][4][2].n=false;
        maze_struct[1][4][3].e=false;maze_struct[1][5][3].w=false;
        maze_struct[1][6][3].e=false;maze_struct[1][7][3].w=false;
        maze_struct[1][7][3].e=false;maze_struct[1][8][3].w=false;
        maze_struct[1][8][3].s=false;maze_struct[1][8][2].n=false;
        maze_struct[1][8][3].e=false;maze_struct[1][9][3].w=false;
        maze_struct[1][0][3].e=false;maze_struct[1][1][3].w=false;
        maze_struct[1][1][3].s=false;maze_struct[1][1][2].n=false;
        maze_struct[1][3][3].s=false;maze_struct[1][3][2].n=false;
        maze_struct[1][4][3].s=false;maze_struct[1][4][2].n=false;
        maze_struct[1][4][3].e=false;maze_struct[1][5][3].w=false;
        maze_struct[1][6][3].e=false;maze_struct[1][7][3].w=false;
        maze_struct[1][7][3].e=false;maze_struct[1][8][3].w=false;
        maze_struct[1][8][3].s=false;maze_struct[1][8][2].n=false;
        maze_struct[1][8][3].e=false;maze_struct[1][9][3].w=false;
        maze_struct[1][9][3].s=false;maze_struct[1][9][2].n=false;
        maze_struct[1][0][2].e=false;maze_struct[1][1][2].w=false;
        maze_struct[1][2][2].s=false;maze_struct[1][2][1].n=false;
        maze_struct[1][2][2].e=false;maze_struct[1][3][2].w=false;
        maze_struct[1][4][2].e=false;maze_struct[1][5][2].w=false;
        maze_struct[1][5][2].e=false;maze_struct[1][6][2].w=false;
        maze_struct[1][6][2].s=false;maze_struct[1][6][1].n=false;
        maze_struct[1][7][2].s=false;maze_struct[1][7][1].n=false;
        maze_struct[1][8][2].s=false;maze_struct[1][8][1].n=false;
        maze_struct[1][0][1].s=false;maze_struct[1][0][0].n=false;
        maze_struct[1][0][1].e=false;maze_struct[1][1][1].w=false;
        maze_struct[1][1][1].e=false;maze_struct[1][2][1].w=false;
        maze_struct[1][3][1].e=false;maze_struct[1][4][1].w=false;
        maze_struct[1][4][1].e=false;maze_struct[1][5][1].w=false;
        maze_struct[1][5][1].e=false;maze_struct[1][6][1].w=false;
        maze_struct[1][7][1].s=false;maze_struct[1][7][0].n=false;
        maze_struct[1][8][1].e=false;maze_struct[1][9][1].w=false;
        maze_struct[1][9][1].s=false;maze_struct[1][9][0].n=false;
        maze_struct[1][0][0].e=false;maze_struct[1][1][0].w=false;
        maze_struct[1][1][0].e=false;maze_struct[1][2][0].w=false;
        maze_struct[1][2][0].e=false;maze_struct[1][3][0].w=false;
        maze_struct[1][3][0].e=false;maze_struct[1][4][0].w=false;
        maze_struct[1][5][0].e=false;maze_struct[1][6][0].w=false;
        maze_struct[1][6][0].e=false;maze_struct[1][7][0].w=false;
        maze_struct[1][8][0].e=false;maze_struct[1][9][0].w=false;
        maze_struct[1][4][0].e=false;maze_struct[1][5][0].w=false;
        maze_struct[1][7][0].e=false;maze_struct[1][8][0].w=false;


         for(i=0;i<10;i++)                                      //maze 3 structure
        for(j=0;j<10;j++)
        {
            maze_struct[2][i][j].e=true;
            maze_struct[2][i][j].w=true;
            maze_struct[2][i][j].n=true;
            maze_struct[2][i][j].s=true;
        }
        maze_struct[2][0][9].s=false;maze_struct[2][0][8].n=false;
        maze_struct[2][0][9].e=false;maze_struct[2][1][9].w=false;
        maze_struct[2][1][9].e=false;maze_struct[2][2][9].w=false;
        maze_struct[2][2][9].e=false;maze_struct[2][3][9].w=false;
        maze_struct[2][3][9].s=false;maze_struct[2][3][8].n=false;
        maze_struct[2][3][9].e=false;maze_struct[2][4][9].w=false;
        maze_struct[2][5][9].s=false;maze_struct[2][5][8].n=false;
        maze_struct[2][5][9].e=false;maze_struct[2][6][9].w=false;
        maze_struct[2][7][9].s=false;maze_struct[2][7][8].n=false;
        maze_struct[2][7][9].e=false;maze_struct[2][8][9].w=false;
        maze_struct[2][8][9].e=false;maze_struct[2][9][9].w=false;
        maze_struct[2][9][9].s=false;maze_struct[2][9][8].n=false;
        maze_struct[2][0][8].s=false;maze_struct[2][0][7].n=false;
        maze_struct[2][1][8].e=false;maze_struct[2][2][8].w=false;
        maze_struct[2][2][8].s=false;maze_struct[2][2][7].n=false;
        maze_struct[2][3][8].s=false;maze_struct[2][3][7].n=false;
        maze_struct[2][3][8].e=false;maze_struct[2][4][8].w=false;
        maze_struct[2][4][8].e=false;maze_struct[2][5][8].w=false;
        maze_struct[2][6][8].s=false;maze_struct[2][6][7].n=false;
        maze_struct[2][6][8].e=false;maze_struct[2][7][8].w=false;
        maze_struct[2][8][8].s=false;maze_struct[2][8][7].n=false;
        maze_struct[2][8][8].e=false;maze_struct[2][9][8].w=false;
        maze_struct[2][0][7].e=false;maze_struct[2][1][7].w=false;
        maze_struct[2][1][7].s=false;maze_struct[2][1][6].n=false;
        maze_struct[2][1][7].e=false;maze_struct[2][2][7].w=false;
        maze_struct[2][3][7].s=false;maze_struct[2][3][6].n=false;
        maze_struct[2][4][7].s=false;maze_struct[2][4][6].n=false;
        maze_struct[2][4][7].e=false;maze_struct[2][5][7].w=false;
        maze_struct[2][5][7].s=false;maze_struct[2][5][6].n=false;
        maze_struct[2][6][7].s=false;maze_struct[2][6][6].n=false;
        maze_struct[2][7][7].e=false;maze_struct[2][8][7].w=false;
        maze_struct[2][9][7].s=false;maze_struct[2][9][6].n=false;
        maze_struct[2][0][6].s=false;maze_struct[2][0][5].n=false;
        maze_struct[2][1][6].e=false;maze_struct[2][2][6].w=false;
        maze_struct[2][2][6].s=false;maze_struct[2][2][5].n=false;
        maze_struct[2][3][6].s=false;maze_struct[2][3][5].n=false;
        maze_struct[2][4][6].s=false;maze_struct[2][4][5].n=false;
        maze_struct[2][5][6].s=false;maze_struct[2][5][5].n=false;
        maze_struct[2][6][6].e=false;maze_struct[2][7][6].w=false;
        maze_struct[2][7][6].s=false;maze_struct[2][7][5].n=false;
        maze_struct[2][7][6].e=false;maze_struct[2][8][6].w=false;
        maze_struct[2][8][6].e=false;maze_struct[2][9][6].w=false;
        maze_struct[2][9][6].s=false;maze_struct[2][9][5].n=false;
        maze_struct[2][0][5].e=false;maze_struct[2][1][5].w=false;
        maze_struct[2][1][5].e=false;maze_struct[2][2][5].w=false;
        maze_struct[2][3][5].e=false;maze_struct[2][4][5].w=false;
        maze_struct[2][5][5].e=false;maze_struct[2][6][5].w=false;
        maze_struct[2][6][5].s=false;maze_struct[2][6][4].n=false;
        maze_struct[2][6][5].e=false;maze_struct[2][7][5].w=false;
        maze_struct[2][8][5].s=false;maze_struct[2][8][4].n=false;
        maze_struct[2][8][5].e=false;maze_struct[2][9][5].w=false;
        maze_struct[2][9][5].s=false;maze_struct[2][9][4].n=false;
        maze_struct[2][0][4].s=false;maze_struct[2][0][3].n=false;
        maze_struct[2][0][4].e=false;maze_struct[2][1][4].w=false;
        maze_struct[2][1][4].s=false;maze_struct[2][1][3].n=false;
        maze_struct[2][2][4].s=false;maze_struct[2][2][3].n=false;
        maze_struct[2][3][4].s=false;maze_struct[2][3][3].n=false;
        maze_struct[2][3][4].e=false;maze_struct[2][4][4].w=false;
        maze_struct[2][4][4].e=false;maze_struct[2][5][4].w=false;
        maze_struct[2][5][4].e=false;maze_struct[2][6][4].w=false;
        maze_struct[2][6][4].s=false;maze_struct[2][6][3].n=false;
        maze_struct[2][7][4].s=false;maze_struct[2][7][3].n=false;
        maze_struct[2][7][4].e=false;maze_struct[2][8][4].w=false;
        maze_struct[2][8][4].s=false;maze_struct[2][8][3].n=false;
        maze_struct[2][9][4].s=false;maze_struct[2][9][3].n=false;
        maze_struct[2][0][3].s=false;maze_struct[2][0][2].n=false;
        maze_struct[2][2][3].s=false;maze_struct[2][2][2].n=false;
        maze_struct[2][2][3].e=false;maze_struct[2][3][3].w=false;
        maze_struct[2][4][3].s=false;maze_struct[2][4][2].n=false;
        maze_struct[2][4][3].e=false;maze_struct[2][5][3].w=false;
        maze_struct[2][5][3].s=false;maze_struct[2][5][2].n=false;
        maze_struct[2][7][3].s=false;maze_struct[2][7][2].n=false;
        maze_struct[2][8][3].s=false;maze_struct[2][8][2].n=false;
        maze_struct[2][9][3].s=false;maze_struct[2][9][2].n=false;
        maze_struct[2][0][2].e=false;maze_struct[2][1][2].w=false;
        maze_struct[2][1][2].s=false;maze_struct[2][1][1].n=false;
        maze_struct[2][1][2].e=false;maze_struct[2][2][2].w=false;
        maze_struct[2][3][2].s=false;maze_struct[2][3][1].n=false;
        maze_struct[2][3][2].e=false;maze_struct[2][4][2].w=false;
        maze_struct[2][5][2].e=false;maze_struct[2][6][2].w=false;
        maze_struct[2][6][2].s=false;maze_struct[2][6][1].n=false;
        maze_struct[2][8][2].s=false;maze_struct[2][8][1].n=false;
        maze_struct[2][0][1].s=false;maze_struct[2][0][0].n=false;
        maze_struct[2][0][1].e=false;maze_struct[2][1][1].w=false;
        maze_struct[2][2][1].s=false;maze_struct[2][2][0].n=false;
        maze_struct[2][3][1].s=false;maze_struct[2][3][0].n=false;
        maze_struct[2][4][1].s=false;maze_struct[2][4][0].n=false;
        maze_struct[2][4][1].e=false;maze_struct[2][5][1].w=false;
        maze_struct[2][5][1].s=false;maze_struct[2][5][0].n=false;
        maze_struct[2][6][1].e=false;maze_struct[2][7][1].w=false;
        maze_struct[2][7][1].s=false;maze_struct[2][7][0].n=false;
        maze_struct[2][8][1].e=false;maze_struct[2][9][1].w=false;
        maze_struct[2][9][1].s=false;maze_struct[2][9][0].n=false;
        maze_struct[2][0][0].e=false;maze_struct[2][1][0].w=false;
        maze_struct[2][1][0].e=false;maze_struct[2][2][0].w=false;
        maze_struct[2][3][0].e=false;maze_struct[2][4][0].w=false;
        maze_struct[2][6][0].e=false;maze_struct[2][7][0].w=false;
        maze_struct[2][7][0].e=false;maze_struct[2][8][0].w=false;
        maze_struct[2][8][0].e=false;maze_struct[2][9][0].w=false;




    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
                                                                //used for player movement
    glTranslatef(posX,posY,posZ);

    draw_player();
    glPopMatrix();


      if(m==0)
        render_maze(maze_struct);
      else if(m==1)
        render_maze(maze_struct);
      else
        render_maze(maze_struct);

    if(m==3)
        exit(0);
  spawn_exit();
    glutSwapBuffers();

}
void draw_player()                              //used to render player tile
{
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 0.0);
        glVertex2f(25.0,25.0);
        glVertex2f(35.0,25.0);
        glVertex2f(35.0,35.0);
        glVertex2f(25.0,35.0);
    glEnd();
}
int check_colli_right()                         //check collision with right wall
{
    if(maze_struct[m][collI][collJ].e==true)
    {
        return 0;
    }
    else
    {
        collI++;
        return 1;
    }
}
int check_colli_left()                          //check collision with left wall
{
    if(maze_struct[m][collI][collJ].w==true)
    {
        return 0;
    }
    else
    {
        collI--;
        return 1;
    }
}
int check_colli_top()                       //check collision with top wall
{
    if(maze_struct[m][collI][collJ].n==true)
    {
        return 0;
    }
    else
    {
        collJ++;
        return 1;
    }
}
int check_colli_bot()                   //check collision with bot wall
{
    if(maze_struct[m][collI][collJ].s==true)
    {
        return 0;
    }
    else
    {
        collJ--;
        return 1;
    }
}
float move_unit = 10.0f;
void keyboardown(int key, int x, int y)                 //control of player tile
{
    int flag=0;
    switch (key){
        case GLUT_KEY_RIGHT:
            flag=check_colli_right();
            if(flag)
            posX+=move_unit;
            break;

        case GLUT_KEY_LEFT:
            flag=check_colli_left();
            if(flag)
            posX-=move_unit;
        break;

        case GLUT_KEY_UP:
            flag=check_colli_top();
            if(flag)
            posY+=move_unit;
            break;

        case GLUT_KEY_DOWN:
            flag=check_colli_bot();
            if(flag)
            posY-=move_unit;
        break;

        default:

         break;
    }
    if(collI==9 && collJ==9)                        //after exit reached, increment m to go to next maze and reset player pos
    {
        m++;
        reset_player();
        posX=0;
        posY=0;
    }
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Practice 1");
    glutDisplayFunc(display);
    init();
    glutSpecialFunc(keyboardown);
    glutMainLoop();
}
