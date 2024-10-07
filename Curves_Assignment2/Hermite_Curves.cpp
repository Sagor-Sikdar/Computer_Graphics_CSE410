#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))


struct point2d
{
    double x, y;

    void constructPoint(double a , double b){
        x=a;
        y=b;
    }

    void initPos(double ax, double bx, double cx, double dx, double ay, double by, double cy , double dy, double delta, int status){

        switch(status){

            case 0 :
                x = dx;
                y= dy;
                break;

            case 1 :
                x=ax*(delta*delta*delta) + bx*(delta*delta) + cx*delta;
                y=ay*(delta*delta*delta) + by*(delta*delta) + cy*delta;
                break;

            case 2 :

                x=6*ax*(delta*delta*delta) + 2*bx*(delta*delta);
                y=6*ay*(delta*delta*delta) + 2*by*(delta*delta);
                break;

            default:

                x=6*ax*(delta*delta*delta);
                y=6*ay*(delta*delta*delta);
                break;
        }
    }


    void update(double x1,double y1){
        x+=x1;
        y+=y1;
    }
};

struct point2d cp[200];
struct point2d position[200];
struct point2d initial;
struct point2d firstDerivative;
struct point2d secondDerivative;
struct point2d thirdDerivative;
struct point2d generatedPoints[100][101];

double a[2],b[2],c[2],d[2];
int curveNo,curvepoint;

double traverseX,traverseY;

int status,toggleView=1;
int selectedpoint;
int check;



int cpidx;

void getCoefficient(double p1,double p4, double r1 , double r4,int place){
    a[place] = 2*p1 - 2*p4 + r1 + r4;
    b[place] = -3*p1 + 3*p4 - 2*r1 - r4;
    c[place] = r1;
    d[place] = p1;
}

double pointInitialization(){
    initial.initPos(a[0],b[0],c[0],d[0],a[1],b[1],c[1],d[1],0.01,0);
    firstDerivative.initPos(a[0],b[0],c[0],d[0],a[1],b[1],c[1],d[1],0.01,1);
    secondDerivative.initPos(a[0],b[0],c[0],d[0],a[1],b[1],c[1],d[1],0.01,2);
    thirdDerivative.initPos(a[0],b[0],c[0],d[0],a[1],b[1],c[1],d[1],0.01,3);
}

void getGeneratedPoints(){

    double delta = 0.01;

    for (int i=0;i<cpidx;i+=2){

        if (i==cpidx-2){
            getCoefficient(position[i].x, position[0].x , position[i+1].x , position[1].x,0);
            getCoefficient(position[i].y, position[0].y , position[i+1].y , position[1].y,1);
        }

        else{
            getCoefficient(position[i].x, position[i+2].x , position[i+1].x , position[i+3].x,0);
            getCoefficient(position[i].y, position[i+2].y , position[i+1].y , position[i+3].y,1);
        }

        pointInitialization();

        generatedPoints[i/2][0].constructPoint(initial.x,initial.y);

        for (int j=0;j<100;j++){
            initial.update(firstDerivative.x,firstDerivative.y);
            firstDerivative.update(secondDerivative.x,secondDerivative.y);
            secondDerivative.update(thirdDerivative.x,thirdDerivative.y);

            generatedPoints[i/2][j+1].constructPoint(initial.x,initial.y);
        }

    }


}


void getCP(double x,double y){
    selectedpoint=-1;
    double minDist=1000;

    double dist;

    for(int i=0;i<cpidx;i++){
        dist=sqrt((cp[i].x-x)*(cp[i].x-x) + (cp[i].y - y)* (cp[i].y - y));

        if(dist<minDist){
            minDist=dist;
            selectedpoint=i;
        }
    }
}

void updateCP(double x, double y){

    if (selectedpoint%2){
        cp[selectedpoint].x=x;
        cp[selectedpoint].y=y;

        position[selectedpoint].x=cp[selectedpoint].x-cp[selectedpoint-1].x;
        position[selectedpoint].y=cp[selectedpoint].y-cp[selectedpoint-1].y;
    }

    else{
        position[selectedpoint].x=cp[selectedpoint].x=x;
        position[selectedpoint].y=cp[selectedpoint].y=y;

        position[selectedpoint+1].x=cp[selectedpoint+1].x-cp[selectedpoint].x;
        position[selectedpoint+1].y=cp[selectedpoint+1].y-cp[selectedpoint].y;
    }


}



void drawSquare()
{
    glBegin(GL_QUADS);
    {


        glVertex3d( 3,  3, 0);
        glVertex3d( 3, -3, 0);
        glVertex3d(-3, -3, 0);
        glVertex3d(-3,  3, 0);
    }
    glEnd();
}

void drawLineAndTriangle(double a , double b , double c, double d)
{
        glBegin(GL_LINES);
        {
            glColor3f(1,1,1);
            glVertex3f(a,b,0);
            glVertex3f(a+c*0.8,b+d*0.8,0);

        }
        glEnd();

        double unitx=c*0.8;
        double unity=d*0.8;


        glBegin(GL_TRIANGLES);
        {
            glColor3f(1,0,0);
            glVertex3f(a+c, b+d, 0);
            glVertex3f(a + unitx - unity*0.05, b + unity + unitx*0.05,0);
            glVertex3f(a + unitx + unity*0.05, b + unity - unitx*0.05,0);
        }
        glEnd();

}


void drawCurve(){

    getGeneratedPoints();

    int cntCurve=cpidx/2;

    for (int i=0;i<cntCurve;i++){

        for(int j=0;j<100;j++){
            glBegin(GL_LINES);
            {
               // printf("%f %f %f %f\n",generatedPoints[i][j].x,generatedPoints[i][j].y,generatedPoints[i][j+1].x,generatedPoints[i][j+1].y);
                glColor3f(1,1,1);
                glVertex3f(generatedPoints[i][j].x,generatedPoints[i][j].y,0);
                glVertex3f(generatedPoints[i][j+1].x,generatedPoints[i][j+1].y,0);

            }
            glEnd();

        }

    }

}


void drawTemp(double cx,double cy,double radius){
    glColor3f(0,1,1);

    glBegin(GL_POLYGON);

    for (int i=0;i<360;i++)
    {
        float theta=(i*pi)/180.0;
        glVertex3f(cx+radius*cos(theta),cy+radius*sin(theta),0);
    }
    glEnd();

}








void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        case 'g':
            toggleView=1-toggleView;
			break;

        case 'u':
            if(status) status=2;
            break;

        case 'a':
            if (status==1) {
                status=4;
                traverseX=generatedPoints[0][0].x;
                traverseY=generatedPoints[0][0].y;
            }
            else if(status==4) status=1;

            curveNo=0;
            curvepoint=0;

            break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			break;
		case GLUT_KEY_UP:		// up arrow key
			break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
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

                std::cout << x << " " << 600-y <<  std::endl;
                switch(status){
                    case 0 :
                        if (cpidx % 2){
                            cp[cpidx].x = (double)x ;
                            cp[cpidx].y = (double)(600 - y);

                            position[cpidx].x=cp[cpidx].x - position[cpidx-1].x;
                            position[cpidx].y=cp[cpidx].y - position[cpidx-1].y;
                        }
                        else{
                            cp[cpidx].x = (double)x;
                            cp[cpidx].y = (double)(600 - y);

                            position[cpidx].x=cp[cpidx].x;
                            position[cpidx].y=cp[cpidx].y;
                        }
                        cpidx++;
                        break;

                    case 2:
                        getCP(x,600-y);
                        status=3;
                        break;

                    case 3:
                        updateCP(x,600-y);
                        status=1;
                        break;

                    default:
                        break;
                }
			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if (!status && cpidx % 2 == 0) status=1;
		    else if (status) status=1;
			break;

		case GLUT_MIDDLE_BUTTON:
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

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,0,	0,0,-1,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	int i;

	if(toggleView){

        for (i = 0; i < cpidx; i++)
        {
            if(i%2) glColor3f(1,1,0);
            else glColor3f(0,1,0);

            glPushMatrix();
            {
                glTranslatef(cp[i].x, cp[i].y, 0);
                drawSquare();
            }
            glPopMatrix();
        }


        for(int i=1; i<cpidx; i+=2){
            drawLineAndTriangle(position[i-1].x , position[i-1].y ,  position[i].x , position[i].y );
        }
	}


    if (status){
        drawCurve();
    }

    if(status==3){
        drawTemp(cp[selectedpoint].x,cp[selectedpoint].y,10);
    }

    if (status==4){
        drawTemp(traverseX,traverseY,5);
    }
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

    if (status==4){
        if (check>10){
            curvepoint++;

            if(curvepoint>100) {
                curvepoint=0;
                curveNo++;
            }

            if (curveNo==cpidx/2){
                curveNo=0;
            }

            traverseX=generatedPoints[curveNo][curvepoint].x;
            traverseY=generatedPoints[curveNo][curvepoint].y;
            check=0;
        }
        check++;
    }

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	cpidx = 0;

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
	gluOrtho2D(0, 800, 0, 600);
	//gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("1405054");

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
