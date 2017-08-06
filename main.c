#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#define NO_OF_MINUTES_IN_AN_HOUR 60
#define NO_OF_LAMPS 15
#define PI 3.14157
#define true 1
#define false 0

unsigned int find_total_number_of_minutes();
unsigned int get_weight_of_lamp(short);
void get_weight_of_every_lamp(unsigned int[]);
void make_corresponding_bitmask(unsigned int, short[], unsigned int[]);
void display_bitmask(short []);
void display_bitmask_in_triangle(short[]);
void keyboard_action(unsigned char, int, int);

void hollow_circle(float, float, float);

short bitmask[NO_OF_LAMPS]; //Here, B[i] = 1, if the i-th lamp is on and 0 if it is off
unsigned int total_number_of_minutes, weight_of_position[NO_OF_LAMPS];

void display_text(char string[],int x,int y,int z, short lamp)
{
	  int i;
	  glPushMatrix();
	  glTranslatef(x, y, z);
	  glScalef(0.5, 0.5, 1.5);

      if(0 <=lamp  && lamp < 5)         glColor3f(1.0, 0.489, 0.0);
      else if(5 <=lamp  && lamp < 9)    glColor3f(0.53, 0.12, 0.47);
      else if(9 <=lamp  && lamp < 12)   glColor3f(0.6, 0.196178, 0.8);
      else if(12 <=lamp  && lamp < 14)  glColor3f(0.35, 0.20, 0.19);
      else if(lamp == 14)               glColor3f(0.53, 0.12, 0.47);

	  for (i = 0; string[i] != '\0'; i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , string[i]);
	  }
	  glPopMatrix();
}

void display_clock_heading(char string[], int x, int y, int z)
{
    int i;
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.5, 0.5, 1.5);

    glColor3f(1.0, 0.25, 0.0);
    for (i = 0; string[i] != '\0'; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , string[i]);
    }
    glPopMatrix();
}

void display_menu(char string[], int x, int y, int z)
{
    int i;
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.20, 0.20, 0.5);

    glColor3f(1.0, 0.25, 0.75);
    for (i = 0; string[i] != '\0'; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , string[i]);
    }
    glPopMatrix();
}

void hollow_circle(float h, float k, float r)
{
    float delta_theta = 0.01;

    glBegin( GL_LINE_LOOP ); // OR GL_LINE_LOOP

    for( float angle = 0; angle < 2*PI; angle += delta_theta )
    {
        glVertex3f(h+ r*cos(angle), k+r*sin(angle), 0 );
    }

    glEnd();
}

void filled_circle(float h, float k, float r)
{
    float delta_theta = 0.01;

    glBegin( GL_POLYGON ); // OR GL_LINE_LOOP

    for( float angle = 0; angle < 2*PI; angle += delta_theta )
    {
        glVertex3f(h+ r*cos(angle), k+r*sin(angle), 0 );
    }

    glEnd();
}

void get_text(short lamp, char text[])
{
    switch(lamp)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4: strcpy(text, "1M"); //First five lamps are all worth 1 minute
                break;
        case 5:
        case 6:
        case 7:
        case 8: strcpy(text, "6M"); //Next Four lamps are all worth 6 minutes
                break;
        case 9:
        case 10:
        case 11:strcpy(text, "30"); //Next 3 are worth 30 minutes
                break;
        case 12:
        case 13:strcpy(text, "2H"); //Next two lamps are worth 2 hours
                break;
        case 14: strcpy(text, "6H");//Next lamp is worth 6 hours
                 break;
    }
}
//REturns the 'level' or floor where the i-the lamp is
short get_y_level(short i)
{
    switch(i)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4: return 0; //First five lamps are all worth 1 minute
                break;
        case 5:
        case 6:
        case 7:
        case 8: return 1; //Next Four lamps are all worth 6 minutes
                break;
        case 9:
        case 10:
        case 11:return 2; //Next 3 are worth 30 minutes
                break;
        case 12:
        case 13:return 3; //Next two lamps are worth 2 hours
                break;
        case 14: return 4;//Next lamp is worth 6 hours
                 break;
        default : return -1;
    }
}

short get_x_offset(short i)
{
    switch(i)
    {
        case 0: return 0;
                break;
        case 5 :return 1;
                break;
        case 1:
        case 9: return 2;
                break;
        case 6:
        case 12:return 3;
                break;
        case 2:
        case 10:
        case 14:return 4;
                break;
        case 7:
        case 13: return 5;
                 break;
        case 3:
        case 11:return 6;
                break;
        case 8: return 7;
                break;
        case 4: return 8;
                break;
        default : return -1;
    }
}

void clock()
{
    char text[50], clock_time[10];
    short i, y_level, x_offset;
    int hours, minutes;
    float first_x = 125.0, first_y = 75.0, current_circle_x, current_circle_y, radius = 60.0;
    float y_difference = sqrt(3)*radius;
    float x_difference = radius;

    //Displaying menu
    sprintf(text,"+ Increase Minutes");
    display_menu(text, first_x - 2*radius, first_y + 10.5*radius, 0);
    sprintf(text, "- Decrease Minutes");
    display_menu(text, first_x - 2*radius , first_y + 8*radius, 0);
    sprintf(text, "* Increase Hours");
    display_menu(text, first_x + 6*radius, first_y + 10.5*radius, 0);
    sprintf(text, "/ Decrease Hours");
    display_menu(text, first_x + 6*radius, first_y + 8*radius, 0);

    //Displaying text on top of the screen
    hours = total_number_of_minutes/60; if(hours == 0) hours = 12;
    minutes = total_number_of_minutes%60;
    sprintf(clock_time,"%d : %d",hours, minutes);
    display_clock_heading(clock_time, first_x + 2*radius, first_y + 9*radius, 0);

    for(i = 0; i < NO_OF_LAMPS; i++)
    {
        y_level = get_y_level(i);
        x_offset = get_x_offset(i);
        current_circle_y = first_y + y_level*y_difference;
        current_circle_x = first_x + x_offset*x_difference;
        if(bitmask[i] == 0)
        {
            glColor3f(0.85,0.85, 0.10);
            hollow_circle(current_circle_x, current_circle_y, radius);
        }
        else if(bitmask[i] == 1)
        {
            glColor3f(0.85,0.85, 0.10);
            filled_circle(current_circle_x, current_circle_y, radius);

            get_text(i, text);
            display_text(text, current_circle_x - (3*radius/4), current_circle_y - (radius/2), 0, i);
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    clock();
    glFlush();
}

void myinit()
{
    glClearColor(0.0,0.0,0.0,0.60);
    glPointSize(2.0);
    gluOrtho2D(0.0,750.0,0.0,750.0);
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.6, 50.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard_action(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+' : glClearColor(0.0,0.0,0.0,0.60);
                   total_number_of_minutes = (total_number_of_minutes + 1)%(12*NO_OF_MINUTES_IN_AN_HOUR);
                   make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
                   glutPostRedisplay();
                    break;

        case '*' : glClearColor(0.0,0.0,0.0,0.60);
                   total_number_of_minutes = (total_number_of_minutes + NO_OF_MINUTES_IN_AN_HOUR)%(12*NO_OF_MINUTES_IN_AN_HOUR);
                   make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
                   glutPostRedisplay();
                    break;

        case '-' : glClearColor(0.0,0.0,0.0,0.60);
                   total_number_of_minutes = (total_number_of_minutes - 1 + 12*NO_OF_MINUTES_IN_AN_HOUR)%(12*NO_OF_MINUTES_IN_AN_HOUR);
                   make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
                   glutPostRedisplay();
                    break;

        case '/' :  glClearColor(0.0,0.0,0.0,0.60);
                    total_number_of_minutes = (total_number_of_minutes - NO_OF_MINUTES_IN_AN_HOUR + 12*NO_OF_MINUTES_IN_AN_HOUR)%(12*NO_OF_MINUTES_IN_AN_HOUR);
                    make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
                    glutPostRedisplay();
                    break;
    }
}

int main(int argc, char *argv[])
{

    get_weight_of_every_lamp(weight_of_position);
    total_number_of_minutes = find_total_number_of_minutes(); //Reads input from the user
    make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
    display_bitmask(bitmask);
    display_bitmask_in_triangle(bitmask);

    printf("*****************************Instructions for use *****************************\n");
    printf("1. Press '+' to increase the number of minutes by 1.");
    printf("\n2. Press '*' to increase the number of hours by 1.\n");
    printf("1. Press '*' to decrease the number of minutes by 1.\n");
    printf("4. Press '/' to decrease the number of hours by 1.\n");
    glutInit(&argc, argv);
    glutInitWindowSize(1200,1000);
    glutCreateWindow("Triangular Binary Clock");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_action);
    myinit();
    glutMainLoop();
    glutReshapeFunc(reshape);
    return 0;
}

//Function which returns total number of minutes
unsigned int find_total_number_of_minutes()
{
    short no_of_hours, no_of_minutes;
    unsigned int total_no_of_minutes;

    printf("What is the number of hours in the time you want to see ?\n");
    scanf("%hu",&no_of_hours);
    printf("What is the number of minutes in the time you want to see ?\n");
    scanf("%hu",&no_of_minutes);

    no_of_hours = (no_of_hours%12);
    no_of_minutes = (no_of_minutes%NO_OF_MINUTES_IN_AN_HOUR);
    total_no_of_minutes = no_of_hours*(NO_OF_MINUTES_IN_AN_HOUR) + no_of_minutes;
    return total_no_of_minutes;
}

void get_weight_of_every_lamp(unsigned int weight_of_position[])
{
    short i;

    for(i = 0; i < NO_OF_LAMPS; i++)
    {
        weight_of_position[i] = get_weight_of_lamp(i);
    }
}

//Returns weight of lamp at position
unsigned int get_weight_of_lamp(short lamp)
{
    switch(lamp)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4: return 1; //First five lamps are all worth 1 minute
                break;
        case 5:
        case 6:
        case 7:
        case 8: return 6; //Next Four lamps are all worth 6 minutes
                break;
        case 9:
        case 10:
        case 11:return 30; //Next 3 are worth 30 minutes
                break;
        case 12:
        case 13:return (2*NO_OF_MINUTES_IN_AN_HOUR); //Next two lamps are worth 2 hours
                break;
        case 14: return (6*NO_OF_MINUTES_IN_AN_HOUR);//Next lamp is worth 6 hours
                 break;
        default : return -1;
    }
}

//Function which makes a bitmask
void make_corresponding_bitmask(unsigned int total_number_of_minutes, short bitmask[], unsigned int weight_of_position[])
{
    short i;
    unsigned int current_number_of_minutes = total_number_of_minutes;

    for(i = NO_OF_LAMPS - 1; i >= 0; i--)
    {
        if(weight_of_position[i] <= current_number_of_minutes)
        {
            bitmask[i] = 1;
            current_number_of_minutes = current_number_of_minutes - weight_of_position[i];
        }
        else
        {
            bitmask[i] = 0;
        }
    }
}

void display_bitmask(short bitmask[])
{
    printf("************************\n\nThe Bitmask is \n\n");
    short i;
    for(i = NO_OF_LAMPS - 1; i >= 0; i--)
    {
        printf("%hu\t",bitmask[i]);
    }
    printf("\n\n************************\n\n");
}

void display_bitmask_in_triangle(short bitmask[])
{
    short no_of_row, space_counter, no_of_lamps_in_current_row, visited_lamp = NO_OF_LAMPS - 1;
    for(no_of_row = 0; no_of_row < 5; no_of_row++)
    {
        //Print 5 - i spaces in each line
        for(space_counter = 0; space_counter < 5- no_of_row - 1; space_counter++)
        {
            printf(" ");
        }

        //Printing the lamps
        for(no_of_lamps_in_current_row = 0; no_of_lamps_in_current_row <= no_of_row; no_of_lamps_in_current_row++)
        {
            if(bitmask[visited_lamp--] == 1)
            {
                printf("* ");//Indicating switched on bulb
            }
            else
            {
                printf(". ");//Indicating switched off bulb
            }
        }
        printf("\n");
    }
}
