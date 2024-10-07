#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <glut.h>
#include "iostream"
#include <vector>
#include "bitmap_image.hpp"
#include <fstream>
#include <vector>
#define INF 10000000.0
#define pi (2*acos(0.0))

using namespace std;

double resolution, fovY, aspectRatio, nearPlane, farPlane;
double radian = pi / 180.0;
double rotation_angle=pi/60;
int recursionLevel;




ifstream description;

void drawSquare(double a)
{
	glBegin(GL_QUADS);{
        glVertex3f( 0, 0,0);
		glVertex3f( a, 0,0);
		glVertex3f( a, a,0);
		glVertex3f( 0, a,0);

	}glEnd();
}


class Color{

public:
    double r,g,b;
    Color(double x, double y, double z){
        r = x; g = y; b = z;
    }

    Color(){
    }


    Color operator+ (const Color& color)
    {
        return Color(this->r + color.r , this->g + color.g , this->b + color.b);
    }

    Color operator- (const Color& color)
    {
        return Color(this->r - color.r , this->g - color.g , this->b - color.b);
    }

    Color operator* (double m)
    {
        return Color( r * m , g * m , b * m);
    }


    Color clip(){
        if ( r < 0 ) r = 0.0;
        else if (r > 1.0) r = 1.0;

        if ( g < 0 ) g = 0.0;
        else if (g > 1.0) g = 1.0;

        if ( b < 0 ) b = 0.0;
        else if (b > 1.0) b = 1.0;

        return Color(r, g, b);
    }

    void print(){
        printf("Red = %f , Green = %f , Blue = %f\n",r,g,b);
    }

};


class Vector{
public:
    double x, y, z;

    Vector(double px, double py, double pz){
        x = px; y = py; z = pz;
    }

    Vector(){
    }

    Vector operator+ (const Vector& point)
    {
        return Vector(this->x + point.x,  this->y + point.y, this->z + point.z);
    }

    Vector operator- (const Vector& point)
    {
        return Vector(this->x - point.x,  this->y - point.y, this->z - point.z);
    }

    Vector operator* (double m)
    {
        return Vector(x*m, y*m, z*m);
    }

    Vector normalize(){
        double a = sqrt ( x*x + y*y + z*z );
        return Vector( x/a , y/a , z/a );
    }

    double dot(Vector b){
        return x * b.x + y * b.y + z * b.z ;
    }

    static Vector cross(Vector a, Vector b)
    {
        Vector v(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);
        return v;
    }

    double value(){
        return sqrt(x*x + y*y + z*z);
    }

    void print(){
        printf("Vector : (%f , %f , %f)\n",x,y,z);
    }
};

Vector pos(19.098528 , 178.326810 , 19.403327), l (-0.104860 , -0.993286 , 0.048860), r(-0.994395 , 0.104054 , -0.018756), u (-0.013546 , 0.050553 , 0.998630);


void rotation(int command){
    Vector R = r;
    Vector L = l;

    switch(command){

        case 1 :

            r = r * cos(rotation_angle) + l * sin(rotation_angle);
            l = l * cos(rotation_angle) - R * sin(rotation_angle);

            l.normalize(); u.normalize(); r.normalize();

            break;

        case 2 :
            r = r * cos(rotation_angle) - l * sin(rotation_angle);
            l = l * cos(rotation_angle) + R * sin(rotation_angle);

            l.normalize(); u.normalize(); r.normalize();

            break;


        case 3 :
            l = l * cos(rotation_angle) + u * sin(rotation_angle);
            u = u * cos(rotation_angle) - L * sin(rotation_angle);

             l.normalize(); u.normalize(); r.normalize();

            break;

        case 4 :
            l = l * cos(rotation_angle) - u * sin(rotation_angle);
            u = u * cos(rotation_angle) + L * sin(rotation_angle);

            l.normalize(); u.normalize(); r.normalize();

            break;

        case 5 :
            r = r * cos(rotation_angle) - u * sin(rotation_angle);
            u = u * cos(rotation_angle) + R * sin(rotation_angle);

            l.normalize(); u.normalize(); r.normalize();

            break;


        case 6 :
            r = r * cos(rotation_angle) + u * sin(rotation_angle);
            u = u * cos(rotation_angle) - R * sin(rotation_angle);

            l.normalize(); u.normalize(); r.normalize();
            break;

        default :
            break;

    }
}



class Object{

public:
    double ambient, diffuse, specular, reflection, shininess;
    Color color;

    Object(){
        ambient = 0.0; diffuse = 0.0; reflection = 0.0; specular = 0.0; shininess = 0.0;
        color = Color(0.0, 0.0, 0.0);
    }

    Object(Color color, double ambient, double diffuse, double specular, double reflection, double shininess){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this-> reflection = reflection;
        this->specular = specular;
        this->shininess = shininess;
        this->color = color;
    }

    virtual void drawObject(){}
    virtual double getT(Vector Ro , Vector Rd){}
    virtual Color getColor(Vector intersectingPoint){}
    virtual Vector getNormal(Vector intersection){}
   // virtual print(){}
};

class checkerBoard:public Object{
public:

    double cellWidth;

    checkerBoard():Object(){cellWidth = 10.0;}
    checkerBoard(double Width, double a, double d, double r):Object(Color(0.0, 0.0, 0.0), a, d, 0.0, r, 0.0)
    {
        cellWidth = Width;
    }

    void drawObject(){
        double flip = 1.0;
        double start = flip;

        for (int i = (-farPlane); i<=farPlane; i+=cellWidth){

            flip = start;

            for (int j=(-farPlane); j<=farPlane; j+=cellWidth){

                glPushMatrix();
                {

                    glTranslatef(i, j, 0);
                    glColor3f(flip, flip, flip);
                    drawSquare(cellWidth);

                }

                glPopMatrix();
                flip = 1-flip;
            }
            start = 1 - start;
        }
    }

    double getT(Vector Ro , Vector Rd){
        if (abs(Ro.z) < 0.0001) return INF;
        if (abs(Rd.z) < 0.00000001) return INF;

        double t = -(Ro.z/Rd.z);



        if (t < 0.0) return INF;
        return t;
    }

    Vector getNormal(Vector intersection){
        return Vector(0.0, 0.0, 1.0).normalize();
    }

    Color getColor(Vector intersectingPoint){

        int i = floor((intersectingPoint.x - 1000.0) / cellWidth);
        int j = floor((intersectingPoint.y - 1000.0)/ cellWidth);

        if ((i+j) % 2 ) return Color(0.0,0.0,0.0);
        else return Color(1.0,1.0,1.0);
    }

};


class Sphere:public Object{
public:

    Vector centre;
    double radius;

    Sphere():Object(){}

    Sphere(Vector centre, double radius, Color color, double a,double d,double sp, double r, double sh):Object(color, a, d, sp, r, sh){
        this->centre = centre;
        this->radius = radius;
    }

    Sphere(Vector centre, double radius, Color color):Object(color, 0.0, 0.0, 0.0, 0.0, 0.0){
        this->centre = centre;
        this->radius = radius;
    }

    void drawObject()
    {
        int slices = 25;
        int stacks = 25;
        Vector points[100][100];
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
        //draw quads using generated points
        glPushMatrix();
        {
            glTranslatef(centre.x, centre.y, centre.z);
            for(i=0;i<stacks;i++)
            {
                glColor3f(color.r,color.g,color.b);
                for(j=0;j<slices;j++)
                {
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
        glPopMatrix();
    }


    double getT(Vector Ro, Vector Rd){
        Vector eyetocentre = centre - Ro;

        double length = Rd.dot(eyetocentre);
        Vector pc = Ro + Rd * length;
        double val = (centre - pc).value();

        if (length >= 0.0){

            if (val > radius) return INF; // didn't intersect

            double dist = sqrt (radius*radius - val*val);

            if (eyetocentre.value() > radius) return (pc-Ro).value() - dist; // eye sphere er baire ase

            return (pc-Ro).value() + dist; // eye sphere er vitore ase
        }

        else{
            if (eyetocentre.value() > radius) return INF;
            return 0;
        }
    }

    Vector getNormal(Vector intersectingPoint){
        Vector normal = intersectingPoint - centre;
        return normal.normalize();
    }

    Color getColor(Vector intersectingPoint){
        return color;
    }
};



class triangle : public Object{
public:

    Vector a,b,c;

    triangle(){
    }

    triangle (Vector x, Vector y, Vector z, Color color, double amb,double d,double sp, double r, double sh):Object(color, amb, d, sp, r, sh){
        a = x; b = y; c = z;
    }

    void drawObject(){
        glBegin(GL_TRIANGLES);
        {
            glColor3f(color.r, color.g, color.b);
            glVertex3f(a.x , a.y  , a.z);
            glVertex3f(b.x , b.y  , b.z);
            glVertex3f(c.x , c.y  , c.z);
        }
        glEnd();
    }


    double determinant(Vector p , Vector q, Vector r){
    // here it is assumed that the vectors are in column wise format
        double ans = p.x * ( q.y * r.z - q.z * r.y ) - q.x * ( p.y * r.z - p.z * r.y ) + r.x * ( p.y * q.z - p.z * q.y );
        return ans;
    }


    double getT (Vector Ro , Vector Rd){

        Vector coeffBeta = a - b;
        Vector coeffGamma = a - c;
        Vector constant = a - Ro;

        double base = determinant(coeffBeta, coeffGamma, Rd);

        if (abs(base)<0.0000001) return INF;

        double beta = determinant(constant, coeffGamma, Rd) / base;
        double gamma = determinant(coeffBeta, constant, Rd ) / base;
        double t = determinant(coeffBeta, coeffGamma, constant ) / base ;


        if (beta > 0.0 && gamma > 0.0 && (beta + gamma) < 1.0 && t >= 0.0 ) return t;

        return INF;
    }


    Vector getNormal(Vector intersection){

        Vector v1 = a - b;
        Vector v2 = a - c;

        Vector normal = Vector::cross(v1, v2);

        if ( normal.z < 0.0 ) return (normal * (-1.0));
        return normal.normalize();

    }

    Color getColor(Vector intersectingPoint){
        return color;
    }

};


vector<Object*> objects;
Color background(0.0, 0.0, 0.0);


//##################################################Light Class####################################################
class Light{
public:
    int id; // id = 1 means normal light snd id = 2 means spotlight
    Vector source, look;
    double falloff,  cutoff;

    Light(){}

    Light(Vector positions, double fall){
        id = 1;
        source = positions;
        falloff = fall;
    }


    Light(Vector positions, Vector looking, double falloffs, double cutoffs)
    {
        id = 2;
        source = positions;
        look = looking;
        falloff = falloffs;
        cutoff = cutoffs * radian;

        look = look.normalize();
    }

    void drawLight(){

        if (id == 1) Sphere(source, 8.0, Color(255.0,255.0,255.0)).drawObject();
        else Sphere(source, 5.0, Color(255.0,255.0,255.0)).drawObject();
    }


    bool illuminates(Vector intersection){
        Vector ps = (source - intersection);
        ps = ps.normalize();
        double t = INF;

        intersection = intersection +  ps * 0.01;

        for (int i=0; i<objects.size(); i++){

            double tt= objects[i]->getT(intersection, ps);
            if (tt < t) return false;
        }
        if (id == 1) return true;

        Vector v1 = ps * (-1.0);

        double cutAngle =  acos(v1.dot(look));

        if ( cutAngle > cutoff ) return false;
        return true;
    }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Light Class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

vector<Light> lights;

vector<vector <Vector> > rayGeneration(){
    double fovX = aspectRatio * fovY;

    double height = 2.0 * nearPlane * tan( radian * ( fovY/2 ) );
    double width = 2.0* nearPlane * tan( radian * (fovX/2 ) ) ;

    Vector mid = pos + l * nearPlane;

    double pixelHeight = height/resolution;
    double pixelWidth = width / resolution;

    Vector start = mid - r * (width/2.0) +  u* (height/2.0);

    vector < vector <Vector> > pixelColor;

    for (int i=0; i<resolution ; i++){

        pixelColor.push_back(vector<Vector>());

        for (int j = 0 ; j < resolution ; j++){
            Vector ans = start + r * (i*pixelWidth) - u * ( j * pixelHeight);
            pixelColor[i].push_back(ans);
        }
    }

    return pixelColor;
}



void pyramid(Vector lowestPoint, double width, double height, Color color, double ambient, double diffuse, double specular, double reflection, double shiny){

    Vector right = lowestPoint + Vector(width, 0, 0);
    Vector up = lowestPoint + Vector (0, width, 0);
    Vector diag = lowestPoint + Vector ( width, width, 0);

    Vector normal(0.0,0.0,1.0);

    Vector heightPoint = Vector (lowestPoint.x + width/2.0, lowestPoint.y + width/2.0, lowestPoint.z) + normal * height;

    objects.push_back(new triangle(lowestPoint, right, diag, color, ambient, diffuse, specular, reflection, shiny));
    objects.push_back(new triangle(lowestPoint, diag, up, color, ambient, diffuse, specular, reflection, shiny));
    objects.push_back(new triangle(lowestPoint, heightPoint, up, color, ambient, diffuse, specular, reflection, shiny));
    objects.push_back(new triangle(lowestPoint, heightPoint, right, color, ambient, diffuse, specular, reflection, shiny));
    objects.push_back(new triangle(up, heightPoint, diag, color, ambient, diffuse, specular, reflection, shiny));
    objects.push_back(new triangle(right, diag, heightPoint, color, ambient, diffuse, specular, reflection, shiny));

}



void filereading(){
    double ambient, diffusion , reflection, specular, shininess,  squareLength;
    description >> nearPlane >> farPlane >> fovY >> aspectRatio;
    description >> recursionLevel ;
    description >> resolution;
    description >> squareLength;
    description >> ambient >> diffusion >> reflection ;

    objects.push_back(new checkerBoard (squareLength, ambient, diffusion, reflection));

    int n_objects;

    description >> n_objects;

    for (int i=0; i< n_objects ; i++){

        string type;
        description >> type ;

        if (type == "sphere"){
            double cx, cy, cz, rad, r,g ,b;

            description >> cx >> cy >> cz;
            description >> rad;
            description >> r >> g >> b;
            description >> ambient >> diffusion >> specular >> reflection;
            description >> shininess;

            Object *sphere = new Sphere(Vector(cx,cy,cz), rad ,Color( r,g,b), ambient, diffusion, specular, reflection, shininess);

            objects.push_back(sphere);
        }

        else if (type == "pyramid"){
            double px, py, pz, wide, heights, cr,cg, cb;

            description >> px >> py >> pz;
            description >> wide >> heights;
            description >> cr >> cg >> cb;
            description >> ambient >> diffusion >> specular >> reflection;
            description >> shininess;

            pyramid(Vector(px, py, pz), wide, heights, Color(cr, cg, cb), ambient, diffusion, specular, reflection, shininess);

        }

    }

    int n_light;
    double px,py,pz, foff;

    description >> n_light;


    for (int i=0; i < n_light ; i++){
        description >> px >> py >> pz >> foff ;
        lights.push_back(Light(Vector(px, py, pz), foff));
    }

    int s_light;
    double lx, ly, lz;
    double ang;

    description >> s_light;

    for (int i=0;i<s_light ; i++){

        description >> px >> py >> pz >> foff;
        description >> lx >> ly >> lz >> ang;

        Vector po(px, py, pz), lo(lx - px,ly - py, lz - pz);

        lights.push_back(Light(po , lo , foff, ang));

    }
    description.close();
}

Color getColor( Vector Ro, Vector Rd, double t, int objectId, int depth, Color visualeffect){

    Object* bestObj = objects[objectId];
    Vector intersection = Ro + Rd * t;
    Vector normal = bestObj->getNormal(intersection);
    Vector R, V;

    double lambert = 0.0 , phong = 0.0;

    for (int i=0; i<lights.size(); i++){

        Light light = lights[i];

        if (light.illuminates(intersection)  == false ) {
            continue;
        }

        else{
            Vector toSource = light.source - intersection;
            double distance = toSource.value();
            toSource = toSource.normalize();
            double scalingFactor = exp(-distance * distance * light.falloff);

            lambert += max(toSource.dot(normal), 0.0) * scalingFactor;

            toSource = toSource * (-1.0);
            R =  (toSource - normal * (2.0* normal.dot(toSource))).normalize() ;
            V =  (Ro - intersection).normalize();

            phong += max(pow( R.dot(V), bestObj->shininess), 0.0 ) *scalingFactor;
        }
    }

    visualeffect = visualeffect * (bestObj->ambient + bestObj->diffuse * lambert + bestObj->specular * phong);

    if ( depth <= 0) return visualeffect;

    R = Rd - normal * ((2.0)* Rd.dot(normal));
    Ro = intersection + R * 0.01;

    objectId = objects.size();
    double nt = INF;

    for (int i=0; i<objects.size(); i++){
        double tt = objects[i]->getT(Ro, R); // check it

        if (tt < nt){
            nt = tt;
            objectId = i;
        }
    }

    if (objectId == objects.size()) {
        depth = 0;
        return visualeffect;
    }

    Vector inter = Ro + R * nt;
    return visualeffect + getColor(Ro, R, nt, objectId, depth-1, objects[objectId]->getColor(inter))*bestObj->reflection;
}


void rayCasting(){
    vector<vector <Vector> > ans = rayGeneration();
    bitmap_image image(resolution, resolution);

    for (int i=0; i< resolution ;i++){

        for (int j = 0 ; j< resolution ; j++){

            Color color(0.0,0.0,0.0);

            Vector Rd = (ans[i][j] - pos).normalize();


            /// kono object intersect korse kina ei ray diye, jodi na kore tahole objects er size return koro
            int objectId = objects.size();
            double t = INF;

            for (int i=0; i<objects.size(); i++){
                double tt = objects[i]->getT(ans[i][j], Rd);

                if (tt < t){
                    t = tt;
                    objectId = i;
                }
            }

            if ( objectId == objects.size()) {
                color = background;
            }

            else{
                Vector inter = ans[i][j] + Rd * t;
                color = objects[objectId]->getColor(inter);
                color = getColor(ans[i][j], Rd, t, objectId, recursionLevel, color);
            }

            image.set_pixel(i,j, min(color.r*255.0, 255.0) , min(color.g * 255.0, 255.0), min(color.b * 255.0, 255.0));
        }
    }
    image.save_image("out.bmp");
    cout << "Image Rendered\n";

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
			rotation(1);
			break;

        case '2':
            rotation(2);
            break;

        case '3':
            rotation(3);
            break;

        case '4':
            rotation(4);
            break;

        case '5':
            rotation(5);
            break;

        case '6':
            rotation(6);
            break;

        case '0':
            rayCasting();
            break;

        case '8':
            pos.print();
            l.print();
            r.print();
            u.print();

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
		    pos = pos - l*3;
			break;

		case GLUT_KEY_UP:		// up arrow key
		    pos = pos + l*3;
			break;

		case GLUT_KEY_RIGHT:
		    pos = pos + r*3;
			break;
		case GLUT_KEY_LEFT:
		    pos = pos - r*3;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = pos + u*3;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos = pos - u*3;
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
			}
			break;

		case GLUT_RIGHT_BUTTON:
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,	pos.x + l.x, pos.y + l.y, pos.z + l.z,	u.x, u.y, u.z);
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/

    for (int i=0;i<objects.size();i++){
        objects[i]->drawObject();
    }

    for (int i=0;i<lights.size() ; i++){
        lights[i].drawLight();
    }

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovY,	aspectRatio,	nearPlane,	farPlane);

}

int main(int argc, char **argv){
	glutInit(&argc,argv);

    description.open("description.txt");
	filereading();

	glutInitWindowSize(resolution, resolution);
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
