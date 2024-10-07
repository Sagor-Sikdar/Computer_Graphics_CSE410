#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double R,r;


struct point
{
	double x,y,z;

	void init(double x1,double y1){
        x=x1;
        y=y1;
	}

	void update(double x1,double y1){
        x+=x1;
        y+=y1;
	}

	double value(){
        return sqrt(x*x+y*y);
	}


};


point point1,point2,pv1,pv2;

double dot_product(point a,point b){
    return (a.x*b.x)+(a.y*b.y);
}

void Rotate(double theta){
    point pv_perpendicular;

    pv_perpendicular.init(pv1.y,-pv1.x);

    double cos_Theta=cos(theta);
    double sin_Theta=sin(theta);

    pv1.init(pv1.x*cos_Theta+pv_perpendicular.x*sin_Theta,pv1.y*cos_Theta+pv_perpendicular.y*sin_Theta);

}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}




void drawCircle(double radius,int segments,double centreX,double centreY)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=centreX+radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=centreY+radius*sin(((double)i/(double)segments)*2*pi);

    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }




}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}

void drawLine(){

    glColor3f(1.0,1.0,1.0);
    double val=pv1.value();
    point pnt;

    pnt.init(pv1.x/val,pv1.y/val);

    glBegin(GL_LINES);
    {
        glVertex3f(point1.x,point1.y,0);
        glVertex3f(point1.x+r*pnt.x,point1.y+r*pnt.y,0);
    }
    glEnd();

    point perpen1,perpen2;

    perpen1.init(-pv1.y/sqrt(pv1.x*pv1.x+pv1.y*pv1.y),pv1.x/sqrt(pv1.x*pv1.x+pv1.y*pv1.y));
    perpen2.init(pv1.y/sqrt(pv1.x*pv1.x+pv1.y*pv1.y),-pv1.x/sqrt(pv1.x*pv1.x+pv1.y*pv1.y));

    double a1= point1.x+pnt.x*(r-3);
    double b1=point1.y+pnt.y*(r-3);

    double x1=a1+4*perpen1.x;
    double y1=b1+4*perpen1.y;

    double x2=a1+4*perpen2.x;
    double y2=b1+4*perpen2.y;


    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0,0.0,0.0);
        glVertex3f(point1.x+r*pnt.x,point1.y+r*pnt.y,0);
        glVertex3f(x1,y1,0);
        glVertex3f(x2,y2,0);
    }
    glEnd();




}

void drawGreenLine(){
    glColor3f(1.0,1.0,1.0);
    double val=pv2.value();
    point pnt;

    pnt.init(pv2.x/val,pv2.y/val);

    glBegin(GL_LINES);
    {
        glVertex3f(point2.x,point2.y,0);
        glVertex3f(point2.x+r*pnt.x,point2.y+r*pnt.y,0);
    }
    glEnd();

    point perpen1,perpen2;

    perpen1.init(-pv2.y/sqrt(pv2.x*pv2.x+pv2.y*pv2.y),pv2.x/sqrt(pv2.x*pv2.x+pv2.y*pv2.y));
    perpen2.init(pv2.y/sqrt(pv2.x*pv2.x+pv2.y*pv2.y),-pv2.x/sqrt(pv2.x*pv2.x+pv2.y*pv2.y));

    double a1= point2.x+pnt.x*(r-3);
    double b1=point2.y+pnt.y*(r-3);

    double x1=a1+4*perpen1.x;
    double y1=b1+4*perpen1.y;

    double x2=a1+4*perpen2.x;
    double y2=b1+4*perpen2.y;


    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0,0.0,0.0);
        glVertex3f(point2.x+r*pnt.x,point2.y+r*pnt.y,0);
        glVertex3f(x1,y1,0);
        glVertex3f(x2,y2,0);
    }
    glEnd();


}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
		    Rotate(pi/45.0);
			//cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			Rotate(-pi/45.0);
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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


	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	glColor3f(1.0,1.0,1.0);
	drawCircle(R,70,0,0);
	glColor3f(1.0,1.0,0.0);
	drawCircle(r,70,point1.x,point1.y);
	drawLine();
	glColor3f(0.0,1.0,0.0);
	drawCircle(r,70,point2.x,point2.y);
	drawGreenLine();




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	double x_distance=point1.x-point2.x;
	double y_distance= point1.y-point2.y;
	double distance=sqrt(x_distance*x_distance+y_distance*y_distance);


	if(point1.x*point1.x  + point1.y*point1.y > (R-r)*(R-r)){
        point unit_vec;
        unit_vec.init(-point1.x,-point1.y);
        double val= unit_vec.value();
        unit_vec.init(-point1.x/val,-point1.y/val);

        double dot_value= dot_product(pv1,unit_vec);

        pv1.update(-2*dot_value*unit_vec.x,-2*dot_value*unit_vec.y);

	}



	else if(point2.x*point2.x  + point2.y*point2.y > (R-r)*(R-r)){
        point unit_vec;
        unit_vec.init(-point2.x,-point2.y);
        double val= unit_vec.value();
        unit_vec.init(-point2.x/val,-point2.y/val);


        double dot_value= dot_product(pv2,unit_vec);

        pv2.update(-2*dot_value*unit_vec.x,-2*dot_value*unit_vec.y);

	}


	else if(distance <=2*r){
        point unit_vect;
        unit_vect.init(point1.x-point2.x,point1.y-point2.y);
        double val=unit_vect.value();
        unit_vect.init((point1.x-point2.x)/val,(point1.y-point2.y)/val);

        double dot_value= dot_product(pv1,unit_vect);

        pv1.update(-2*dot_value*unit_vect.x,-2*dot_value*unit_vect.y);

        dot_value= dot_product(pv2,unit_vect);
        pv2.update(-2*dot_value*unit_vect.x,-2*dot_value*unit_vect.y);

	}

    point1.update(pv1.x,pv1.y);
	point2.update(pv2.x,pv2.y);

	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
    R=100;
    r=R/10;

	point1.init(4,10);
	point2.init(-20,-10);
	pv1.init(-0.06,0.02);
	pv2.init(-0.02,0.07);


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
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
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
