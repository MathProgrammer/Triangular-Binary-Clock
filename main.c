#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define NO_OF_MINUTES_IN_AN_HOUR 60
#define NO_OF_LAMPS 15
#define PI 3.14157

unsigned int find_total_number_of_minutes();
unsigned int get_weight_of_lamp(short);
void get_weight_of_every_lamp(unsigned int[]);
void make_corresponding_bitmask(unsigned int, short[], unsigned int[]);
void display_bitmask(short []);
void display_bitmask_in_triangle(short[]);

void hollow_circle(float, float, float);

short bitmask[NO_OF_LAMPS]; //Here, B[i] = 1, if the i-th lamp is on and 0 if it is off

void hollow_circle(float h, float k, float r)
{
    float delta_theta = 0.01,angle;

    glBegin( GL_LINE_LOOP ); // OR GL_LINE_LOOP

    for( angle = 0; angle < 2*PI; angle += delta_theta )
    {
        glVertex3f(h+ r*cos(angle), k+r*sin(angle), 0 );
    }

    glEnd();
}

void filled_circle(float h, float k, float r)
{
    float delta_theta = 0.01,angle;
    glBegin( GL_POLYGON ); // OR GL_LINE_LOOP

    for(angle = 0; angle < 2*PI; angle += delta_theta )
    {
        glVertex3f(h+ r*cos(angle), k+r*sin(angle), 0 );
    }

    glEnd();
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
    short i, y_level, x_offset;
    float first_x = 55.0, first_y = 75.0, current_circle_x, current_circle_y, radius = 50.0;
    float y_difference = sqrt(3)*radius;
    float x_difference = radius;

    for(i = 0; i < NO_OF_LAMPS; i++)
    {
        y_level = get_y_level(i);
        x_offset = get_x_offset(i);
        current_circle_y = first_y + y_level*y_difference;
        current_circle_x = first_x + x_offset*x_difference;
        if(bitmask[i] == 0)
        {
            hollow_circle(current_circle_x, current_circle_y, radius);
        }
        else if(bitmask[i] == 1)
        {
            filled_circle(current_circle_x, current_circle_y, radius);
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
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(0.0,1.0,0.0);
    glPointSize(2.0);
    gluOrtho2D(0.0,750.0,0.0,750.0);
}


int main(int argc, char *argv[])
{

    unsigned int total_number_of_minutes, weight_of_position[NO_OF_LAMPS];

    get_weight_of_every_lamp(weight_of_position);
    total_number_of_minutes = find_total_number_of_minutes(); //Reads input from the user
    make_corresponding_bitmask(total_number_of_minutes, bitmask, weight_of_position);
    display_bitmask(bitmask);
    display_bitmask_in_triangle(bitmask);

    glutInit(&argc, argv);
    glutInitWindowSize(750,750);
    glutCreateWindow("Triangular Binary Clock");

    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
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
                printf("O ");//Indicating switched on bulb
            }
            else
            {
                printf(". ");//Indicating switched off bulb
            }
        }
        printf("\n");
    }
}
