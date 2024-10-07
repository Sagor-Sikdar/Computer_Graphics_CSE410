#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))


double cameraHeight;
double cameraAngle;
int drawaxes;
double rotation_angle=pi/60;
double Rs,rs,h;
bool checkU,checkL,checkC,checkM;
double angleX=0,angleY=0,angleU=0,angleR=0;


double slice;


struct point
{
	double x,y,z;

    void initialise(double a, double b, double c){
        x=a;
        y=b;
        z=c;
    }
};


point pos,l,r,u;

void move_forwardOrBackward(int direction){
    //direction value tells me where to go ,either in forward or backward direction

    if(direction){
        pos.x+=l.x;
        pos.y+=l.y;
        pos.z+=l.z;
    }

    else{
        pos.x-=l.x;
        pos.y-=l.y;
        pos.z-=l.z;
    }

}

void move_RightOrLeft(int direction){
    //direction value tells me where to go ,either in right or left direction

    if(direction){
        pos.x+=r.x;
        pos.y+=r.y;
        pos.z+=r.z;
    }

    else{
        pos.x-=r.x;
        pos.y-=r.y;
        pos.z-=r.z;
    }
}

void move_UpDown(int direction){
    //direction value tells me where to go ,either in up or down direction

    if(direction){
        pos.x+=u.x;
        pos.y+=u.y;
        pos.z+=u.z;
    }

    else{
        pos.x-=u.x;
        pos.y-=u.y;
        pos.z-=u.z;
    }

}

void rotate_LeftOrRight(int direction){

    if (direction){
        double a=r.x;
        double b=r.y;
        double c=r.z;

        r.x=r.x*cos(rotation_angle)-l.x*sin(rotation_angle);
        r.y=r.y*cos(rotation_angle)-l.y*sin(rotation_angle);
        r.z=r.z*cos(rotation_angle)-l.z*sin(rotation_angle);

        l.x=l.x*cos(rotation_angle)+a*sin(rotation_angle);
        l.y=l.y*cos(rotation_angle)+b*sin(rotation_angle);
        l.z=l.z*cos(rotation_angle)+c*sin(rotation_angle);
    }
    else{
        double a=r.x;
        double b=r.y;
        double c=r.z;

        r.x=r.x*cos(rotation_angle)+l.x*sin(rotation_angle);
        r.y=r.y*cos(rotation_angle)+l.y*sin(rotation_angle);
        r.z=r.z*cos(rotation_angle)+l.z*sin(rotation_angle);

        l.x=l.x*cos(rotation_angle)-a*sin(rotation_angle);
        l.y=l.y*cos(rotation_angle)-b*sin(rotation_angle);
        l.z=l.z*cos(rotation_angle)-c*sin(rotation_angle);

    }
}

void rotate_UpOrDown(int direction){


    if (direction){
        double a=l.x;
        double b=l.y;
        double c=l.z;

        l.x=l.x*cos(rotation_angle)-u.x*sin(rotation_angle);
        l.y=l.y*cos(rotation_angle)-u.y*sin(rotation_angle);
        l.z=l.z*cos(rotation_angle)-u.z*sin(rotation_angle);


        u.x=u.x*cos(rotation_angle)+a*sin(rotation_angle);
        u.y=u.y*cos(rotation_angle)+b*sin(rotation_angle);
        u.z=u.z*cos(rotation_angle)+c*sin(rotation_angle);
    }
    else{
        double a=l.x;
        double b=l.y;
        double c=l.z;

        l.x=l.x*cos(rotation_angle)+u.x*sin(rotation_angle);
        l.y=l.y*cos(rotation_angle)+u.y*sin(rotation_angle);
        l.z=l.z*cos(rotation_angle)+u.z*sin(rotation_angle);

        u.x=u.x*cos(rotation_angle)-a*sin(rotation_angle);
        u.y=u.y*cos(rotation_angle)-b*sin(rotation_angle);
        u.z=u.z*cos(rotation_angle)-c*sin(rotation_angle);
    }
}



void tilt(int direction){
    //direction==0 means anticlockwise
    if (direction){
        double a=r.x;
        double b=r.y;
        double c=r.z;

        r.x=r.x*cos(rotation_angle)-u.x*sin(rotation_angle);
        r.y=r.y*cos(rotation_angle)-u.y*sin(rotation_angle);
        r.z=r.z*cos(rotation_angle)-u.z*sin(rotation_angle);


        u.x=u.x*cos(rotation_angle)+a*sin(rotation_angle);
        u.y=u.y*cos(rotation_angle)+b*sin(rotation_angle);
        u.z=u.z*cos(rotation_angle)+c*sin(rotation_angle);
    }
    else{
        double a=r.x;
        double b=r.y;
        double c=r.z;


        r.x=r.x*cos(rotation_angle)+u.x*sin(rotation_angle);
        r.y=r.y*cos(rotation_angle)+u.y*sin(rotation_angle);
        r.z=r.z*cos(rotation_angle)+u.z*sin(rotation_angle);

        u.x=u.x*cos(rotation_angle)-a*sin(rotation_angle);
        u.y=u.y*cos(rotation_angle)-b*sin(rotation_angle);
        u.z=u.z*cos(rotation_angle)-c*sin(rotation_angle);
    }
}




void drawAxes()
{
	if(drawaxes==1)
	{
		//glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 500,0,0);
			glVertex3f(-500,0,0);

			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-500,0);
			glVertex3f(0, 500,0);

            glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 500);
			glVertex3f(0,0,-500);
		}glEnd();
	}
}




void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius/2+radius*sin(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(checkM) {glColor3f(1.0,1.0,1.0);checkM=false;}
		    else {glColor3f(0,0,0); checkM=true;}
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}


void UpperHemisphere(double radius,int slices,int stacks){
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(checkU) {glColor3f(1.0,1.0,1.0);checkU=false;}
		    else {glColor3f(0,0,0); checkU=true;}

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}



void lowerHemisphere(double radius,int slices,int stacks){
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{

		    if(checkL) {glColor3f(1.0,1.0,1.0);checkL=false;}
		    else {glColor3f(0,0,0); checkL=true;}

			glBegin(GL_QUADS);{
			    //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void cylinder(double radius,int slices,int stacks){

    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(checkC) {glColor3f(1.0,1.0,1.0);checkC=false;}
		    else {glColor3f(0,0,0); checkC=true;}

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}





void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			rotate_LeftOrRight(0);
			break;

        case '2':
            rotate_LeftOrRight(1);
            break;

        case '3':
            rotate_UpOrDown(0);
            break;

        case '4':
            rotate_UpOrDown(1);
            break;

        case '5':
            tilt(1);
            break;

        case '6':
            tilt(0);
            break;

        case 'q':
            if(angleY<=45) angleY+=3;
            break;

        case 'w':
            if(angleY>=-45)angleY-=3;
            break;

        case 'e':
            if(angleX<=45)angleX+=3;
            break;
        case 'r':
            if(angleX>=-45) angleX-=3;
            break;

        case 'a':
            if(angleR<=45) angleR+=3;
            break;

        case 's':
            if(angleR>=-45)angleR-=3;
            break;

        case 'd':
            angleU+=3;
            break;
        case 'f':
            angleU-=3;
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
		    move_forwardOrBackward(0);
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
		    move_forwardOrBackward(1);
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
		    move_RightOrLeft(1);
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
		    move_RightOrLeft(0);
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
		    move_UpDown(1);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    move_UpDown(0);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

	glRotatef(angleY,0,1,0);
    glRotatef(angleX,1,0,0);

	glPushMatrix();
    {


        glTranslatef(0,0,4*Rs);
        glRotatef(angleR,1,0,0);
        glTranslatef(0,0,rs);

        glRotatef(angleU,0,0,1);

        lowerHemisphere(rs,slice,80);
        glTranslatef(0,0,rs);

        cylinder(rs,slice,80);

        glTranslatef(0,0,rs);
        drawSphere(rs,slice,80);

    }
    glPopMatrix();




	glTranslatef(0,0,Rs);
    lowerHemisphere(Rs,slice,80);

    glTranslatef(0,0,Rs);
    cylinder(Rs,slice,80);

    glTranslatef(0,0,Rs);
    UpperHemisphere(Rs,slice,80);

    glTranslatef(0,0,Rs);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;

	Rs=10;
	rs=8;
	h=2*Rs;
	slice=40;

	pos.initialise(39.236437,32.350933,-71.335800);
	l.initialise(-0.342658 , -0.445620 , 0.827048);
	u.initialise( -0.221770 , 0.893832 , 0.389721);
	r.initialise(-0.912909 , -0.049873 , -0.405104);


	//clear the screen
	glClearColor(0,0,0,0);



	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();
	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);

}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
