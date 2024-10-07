#include "bits/stdc++.h"
#include "bitmap_image.hpp"
using namespace std;

#define pi (2*acos(0.0))
#define epsilon (1.0e-6

class homogeneous_point
{
public:
    double x, y, z, w;

    // set the three coordinates, set w to 1
    homogeneous_point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }

    /*
    default constructor. does nothing. allows declarations like below:
        matrix m;
    therefore, usage is dangerous
    */
    homogeneous_point() {
    }

    // constructs a homogeneous point with given coordinates. forces w to be 1.0
    // if w is zero, raises error
    homogeneous_point(double x, double y, double z, double w)
    {
        assert (w != 0);
        this->x = x/w;
        this->y = y/w;
        this->z = z/w;
        this->w = 1;
    }

    // adds two points. returns a point forcing w to be 1.0
    homogeneous_point operator+ (const homogeneous_point& point)
    {
        double x = this->x + point.x;
        double y = this->y + point.y;
        double z = this->z + point.z;
        double w = this->w + point.w;
        homogeneous_point p(x, y, z, w);
        return p;
    }

    // subtracts one point from another. returns a point forcing w to be 1.0
    homogeneous_point operator- (const homogeneous_point& point)
    {
        double x = this->x - point.x;
        double y = this->y - point.y;
        double z = this->z - point.z;
        double w = this->w - point.w;
        homogeneous_point p(x, y, z, w);
        return p;
    }


    bool operator<( const homogeneous_point& val ) const {
        if (y == val.y){
            return x < val.x;
        }
        return y < val.y;
    }

    // Print the coordinates of a point. exists for testing purpose.
    void print()
    {
        cout << "Point: " << endl;
        cout << x << " " << y << " " << z << " " << w << endl<<endl<<endl;
    }

};

class Vector
{
public:
    double x, y, z;

    // constructs a vector with given components
    Vector(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // keeps the direction same. recalculates the vector to be unit.
    void normalize()
    {
        double r = sqrt(x*x + y*y + z*z);
        x = x / r;
        y = y / r;
        z = z / r;
    }

    // add two vectors
    Vector operator+(const Vector& v)
    {
        Vector v1(x+v.x, y+v.y, z+v.z);
        return v1;
    }

    // subtract one vector from another
    Vector operator-(const Vector& v)
    {
        Vector v1(x-v.x, y-v.y, z-v.z);
        return v1;
    }

    // scale a vector with a given coefficient
    Vector operator* (double m)
    {
        Vector v(x*m, y*m, z*m);
        return v;
    }

    // get the dot product of two vectors
    static double dot(Vector a, Vector b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    // get the cross product of two vectors
    static Vector cross(Vector a, Vector b)
    {
        Vector v(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);
        return v;
    }

    // print a vector. only for testing purposes.
    void print ()
    {
        cout << "Vector" << endl;
        cout << x << " " << y << " " << z << endl;
    }
};

class color {
public:
    double r, g, b;
    color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color() {
    }

    void print(){
        printf("%f\t%f\t%f\n",r,g,b);
    }
};

/*
The matrices are forced to be 4x4. This is because in this assignment, we will deal with points in triangles.
Maximum # of points that we will deal with at once is 3. And all the standard matrices are 4x4 (i.e. scale, translation, rotation etc.)
*/
class matrix
{
public:
    double values[4][4];
    int num_rows, num_cols;

    // only set the number of rows and cols
    matrix(int rows, int cols)
    {
        assert (rows <= 4 && cols <= 4);
        num_rows = rows;
        num_cols = cols;
    }

    // prepare an nxn square matrix
    matrix(int n)
    {
        assert (n <= 4);
        num_rows = num_cols = n;
    }

    matrix(Vector a, Vector b, Vector c,int value){

        // value == 1 if i want to create matrix for triangle and value == -1 if i want to create matrix for rotation in stage 2
        num_rows = 4;
        num_cols = 4;

        values[0][0] = a.x , values[0][1] = a.y , values[0][2] = a.z , values[0][3] = 0;
        values[1][0] = b.x , values[1][1] = b.y , values[1][2] = b.z , values[1][3] = 0;
        values[2][0] = value * c.x , values[2][1] = value * c.y , values[2][2] = value * c.z , values[2][3] = 0;
        values[3][0] = 0 , values[3][1] = 0 , values[3][2] = 0 , values[3][3] = 1;
    }

    // prepare and return an identity matrix of size nxn
    static matrix make_identity(int n)
    {
        assert (n <= 4);
        matrix m(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == j)
                    m.values[i][j] = 1;
                else
                    m.values[i][j] = 0;
            }
        }
        return m;
    }

    static matrix translate(double tx,double ty,double tz){
        matrix translation= make_identity(4);

        translation.values[0][3] = tx;
        translation.values[1][3] = ty;
        translation.values [2][3] = tz;

        return translation;

    }

     static matrix scaling(double tx,double ty,double tz){
        matrix scale= make_identity(4);

        scale.values[0][0] = tx;
        scale.values[1][1] = ty;
        scale.values [2][2] = tz;

        return scale;

    }

    static matrix rotation(double angle , double ax, double ay, double az){

        Vector a = Vector(ax,ay,az);
        a.normalize();

        ax = a.x ; ay = a.y ; az = a.z;

        Vector vec =  Vector(ax,ay,az);
        Vector i = Vector(1,0,0);
        Vector j = Vector(0,1,0);
        Vector k =  Vector(0,0,1);


        Vector c1 = i*(cos(angle)) + vec * (ax-ax*cos(angle)) + (Vector::cross(vec , i))*(sin(angle));
        Vector c2 = j*(cos( angle)) + vec*(ay-ay*cos(angle)) + (Vector::cross(vec , j))*(sin(angle));
        Vector c3 = k*(cos(angle)) + vec*(az-az*cos(angle)) + (Vector::cross(vec , k))*(sin(angle));


        matrix rotation = make_identity(4);


        rotation.values[0][0] = c1.x; rotation.values[1][0] = c1.y ; rotation.values[2][0] = c1.z;
        rotation.values[0][1] = c2.x; rotation.values[1][1] = c2.y ; rotation.values[2][1] = c2.z;
        rotation.values[0][2] = c3.x; rotation.values[1][2] = c3.y ; rotation.values[2][2] = c3.z;

        return rotation;


    }


    static matrix movetoOrigin(double ex, double ey, double ez){

        matrix m = make_identity(4);

        m.values[0][3] = -ex;
        m.values[1][3] = -ey;
        m.values[2][3] = -ez;

        return m;
    }

    // print the matrix. exists for testing purposes
    void print()
    {
        cout << "Matrix:" << endl;
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                cout << values[i][j] << "\t";
            }
            cout << endl;
        }
    }

    // add the two matrices. Raise error if dimension mismatches
    matrix operator+ (const matrix& m)
    {
        assert (this->num_rows == m.num_rows);
        assert (this->num_cols == m.num_cols);

        matrix m1(num_rows, num_cols);
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                m1.values[i][j] = values[i][j] + m.values[i][j];
            }
        }
        return m1;
    }

    // subtract a matrix from another. raise error if dimension mismatches
    matrix operator- (const matrix& m)
    {
        assert (this->num_rows == m.num_rows);
        assert (this->num_cols == m.num_cols);

        matrix m1(num_rows, num_cols);
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                m1.values[i][j] = values[i][j] - m.values[i][j];
            }
        }
        return m1;
    }

    // multiply two matrices. allows statements like m1 = m2 * m3; raises error is dimension mismatches
    matrix operator* (const matrix& m)
    {
        assert (this->num_cols == m.num_rows);
        matrix m1(this->num_rows, m.num_cols);

        for (int i = 0; i < m1.num_rows; i++) {
            for (int j = 0; j < m1.num_cols; j++) {
                double val = 0;
                for (int k = 0; k < this->num_cols; k++) {
                    val += this->values[i][k] * m.values[k][j];
                }
                m1.values[i][j] = val;
            }
        }
        return m1;
    }

    // multiply a matrix with a constant
    matrix operator* (double m)
    {
        matrix m1(this->num_rows, this->num_cols);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                m1.values[i][j] = m * this->values[i][j];
            }
        }
        return m1;
    }

    // multiply a 4x4 matrix with a homogeneous point and return the resulting point.
    // usage: homogeneous_point p = m * p1;
    // here, m is a 4x4 matrix, intended to be the transformation matrix
    // p1 is the point on which the transformation is being made
    // p is the resulting homogeneous point
    homogeneous_point operator* (const homogeneous_point& p)
    {
        assert (this->num_rows == this->num_cols && this->num_rows == 4);

        matrix m(4, 1);
        m.values[0][0] = p.x;
        m.values[1][0] = p.y;
        m.values[2][0] = p.z;
        m.values[3][0] = p.w;

        matrix m1 = (*this)*m;
        homogeneous_point p1(m1.values[0][0], m1.values[1][0], m1.values[2][0], m1.values[3][0]);
        return p1;
    }

    // return the transpose of a matrix
    matrix transpose()
    {
        matrix m(num_cols, num_rows);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                m.values[j][i] = values[i][j];
            }
        }
        return m;
    }

    static matrix projectionMatrix(double Near, double Far, double FovY, double AspectRatio){
        double FovX = FovY * AspectRatio;
        double t = Near * tan((pi/360)*FovY);
        double r = Near * tan((pi/360)*FovX);

        matrix mat = make_identity(4);

        mat.values[0][0] = Near / r;
        mat.values[1][1] = Near / t;
        mat.values[2][2] = (Far + Near) / (Near - Far);
        mat.values[2][3] = (2* Far * Near) / (Near - Far);
        mat.values[3][2] = -1;
        mat.values[3][3] = 0;

        return mat;
    }

};

class polygon{
public:
    homogeneous_point p1,p2,p3;
    color colors;

    polygon(homogeneous_point a, homogeneous_point b, homogeneous_point c,color col){
        p1 = a;
        p2 = b;
        p3 = c;
        colors = col;
    }

    polygon convertedCoord(double w, double h){
        double i,j,k,l,m,n;

        i = ((1 + p1.x)*w - 1)/2;
        j = ((1 - p1.y)*h - 1)/2;

        k = ((1 + p2.x)*w - 1)/2;
        l = ((1 - p2.y)*h - 1)/2;

        m = ((1 + p3.x)*w - 1)/2;
        n = ((1 - p3.y)*h - 1)/2;

        p1.x = i;
        p1.y = j;
        p2.x = k;
        p2.y = l;
        p3.x = m;
        p3.y = n;

        vector<homogeneous_point> val;
        val.push_back(p1);
        val.push_back(p2);
        val.push_back(p3);

        sort(val.begin(),val.end());

        return polygon(val[0], val[1],val[2],colors);
    }

    void print(){
        colors.print();
        cout << p1.x << " " << p1.y << " " << p1.z << " " << p1.w << endl;
        cout << p2.x << " " << p2.y << " " << p2.z << " " << p2.w << endl;
        cout << p3.x << " " << p3.y << " " << p3.z << " " << p3.w << endl<<endl<<endl;
    }

};


double eye_x, eye_y, eye_z;
double look_x, look_y, look_z;
double up_x, up_y, up_z;
double fov_x, fov_y, aspectRatio, near, far;
color backgroud;
int screen_x, screen_y;
stack <matrix> st;
vector<matrix> triangles;
vector <color> colors;
vector <int> colorID;
vector<polygon>polygons;

color** pixels;
double** zs;

ifstream scene;
ofstream out1;


homogeneous_point intersection(homogeneous_point p1, homogeneous_point p2, double clip){

    double t = (clip - p1.z) / (p2.z - p1.z);

    homogeneous_point point(p1.x + t*(p2.x-p1.x) , p1.y + t*(p2.y-p1.y), p1.z + t*(p2.z-p1.z));

    return point;


}


vector<homogeneous_point> clipping(homogeneous_point p1, homogeneous_point p2, homogeneous_point p3, double Near, double Far){
////sequence 1 2 3
    vector<homogeneous_point> points;
    vector <homogeneous_point> output;
    points.push_back(p1); points.push_back(p2); points.push_back(p3);

    double clipPoint[2] = {-Near, -Far};

    for (int i=0;i<2;i++){

        double z = clipPoint[i];

        homogeneous_point S = p1;
        homogeneous_point D = p3;

        if (i==1){
            points.clear();
            for (int i=0;i<output.size();i++) points.push_back(output[i]);
            output.clear();
        }

        for (int j=0;j<points.size();j++){

            S = D;
            D = points[j];

            // S , P both are inside the clip edge
            if ((S.z <= z && D.z <=z && i==0) || (S.z >= z && D.z >= z && i==1)) {
                output.push_back(D);
            }

            //S inside , P outside
            else if ((S.z <= z && i==0) || (S.z >= z && i==1)){
                homogeneous_point intersect = intersection(S,D,z);
                output.push_back(intersect);
            }

            // D inside , S outside
            else if ((D.z <= z && i==0) || (D.z >= z && i==1))
            {
                homogeneous_point intersect = intersection(S,D,z);

                output.push_back(intersect);
                output.push_back(D);
            }
        }
    }

    return output;

}


void stage1()
{
    st.push(matrix::make_identity(4));

    string command;

    st.push(matrix::make_identity(4));

    while(true){

        scene >> command;

        if (command == "triangle"){

            double a,b,c;

            for (int i=0;i<3;i++){
                scene >> a >> b >> c ;
                homogeneous_point p(a,b,c);

                homogeneous_point ans = st.top() * p;

                out1 << ans.x << "\t" << ans.y << "\t" << ans.z << "\n";
            }
            out1 << "\n";

            scene >> a >> b >> c ;
            colors.push_back(color(a,b,c));

        }

        else if (command == "translate"){

            double tx,ty,tz;
            scene >> tx >> ty >> tz;

            matrix top = st.top()*matrix::translate(tx,ty,tz);

            st.pop();
            st.push(top);

        }

        else if (command == "scale"){

            double sx,sy,sz;
            scene >> sx >> sy >> sz;

            matrix top = st.top()*matrix::scaling(sx,sy,sz);

            st.pop();
            st.push(top);

        }

        else if (command == "rotate"){

            double angle,rx,ry,rz;
            scene >> angle >> rx >> ry >> rz;

            matrix top = st.top()*(matrix::rotation((pi/180)*angle,rx,ry,rz));

            st.pop();
            st.push(top);

        }

        else if (command=="push"){
            st.push(st.top());
        }

        else if (command=="pop"){
            st.pop();
        }

        else if (command == "end"){
            break;
        }
    }

    scene.close();
    out1.close();

}

void stage2(){

    ifstream in1;
    ofstream out2;
    in1.open("stage1.txt");
    out2.open("stage2.txt");
    out2 << std::fixed;
    out2 << std::setprecision(8);


    Vector l = Vector(look_x,look_y,look_z) - Vector(eye_x,eye_y,eye_z);
    l.normalize();
    Vector r = Vector::cross(l,Vector(up_x,up_y,up_z));
    r.normalize();
    Vector u = Vector::cross(r,l);

    matrix T = matrix::movetoOrigin(eye_x,eye_y,eye_z);
    matrix R = matrix(r,u,l,-1); // -1 indicates that it is a matrix for rotation

    matrix V = R * T ; // view transformation matrix
    double a,b,c;

    for (int i=0;i<colors.size(); i++){

        for (int j=0;j<3;j++){
            in1 >> a >> b >> c;
            homogeneous_point p1(a,b,c);
            homogeneous_point transformed = V * p1;

            out2 << transformed.x << "\t" << transformed.y << "\t" << transformed.z << endl;
        }
        out2 << "\n";
    }
    in1.close();
    out2.close();

}

void clip (double Near , double Far){
    ifstream in2;
    ofstream temp;
    in2.open("stage2.txt");
    temp.open("temp.txt");
    temp << std::fixed;
    temp << std::setprecision(7);
    double a,b,c;

    for (int i=0;i<colors.size(); i++){
        in2 >> a >> b >> c;
        homogeneous_point p1(a,b,c);

        in2 >> a >> b >> c;
        homogeneous_point p2(a,b,c);

        in2 >> a >> b >> c;
        homogeneous_point p3(a,b,c);

        vector <homogeneous_point> ans = clipping(p1,p2,p3,Near,Far);

        if(ans.size() < 3) continue;

        for (int j=1;j<ans.size()-1;j++){
            temp << ans[0].x << "\t" << ans[0].y << "\t" << ans[0].z << "\n";
            temp << ans[j].x << "\t" << ans[j].y << "\t" << ans[j].z << "\n";
            temp << ans[j+1].x << "\t" << ans[j+1].y << "\t" << ans[j+1].z << "\n";

            colorID.push_back(i);

            temp << "\n";
        }
    }
    in2.close();
    temp.close();
}

void stage3(double Near, double Far){
    clip(Near, Far);

    ifstream in3;
    ofstream out3;
    in3.open("temp.txt");
    out3.open("stage3.txt");
    out3 << std::fixed;
    out3 << std::setprecision(8);

    matrix P = matrix::projectionMatrix(near, far, fov_y, aspectRatio);

    double a,b,c;

    for (int i=0; i<colorID.size(); i++){

        for (int j=0;j<3;j++){

            in3 >> a >> b >> c;
            homogeneous_point p1(a,b,c);
            homogeneous_point transformed = P * p1;

            out3 << transformed.x << "\t" << transformed.y << "\t" << transformed.z << endl;
        }
        out3 << "\n";
    }
    in3.close();
    out3.close();
}


void initializePixel_Zbuffer(){
    pixels = new color*[screen_x];
    zs = new double*[screen_x];

    for (int i=0; i<screen_x ; i++){
        pixels[i] = new color [screen_y]; zs[i] = new double [screen_y];

        for (int j = 0; j < screen_y; j++) {
            pixels[i][j] = backgroud;
            zs[i][j] = 20;
        }
    }
}

void createTriangle(){

    ifstream in3;
    in3.open("stage3.txt");
    double a,b,c,d,e,f,g,h,i;

    for (int tr=0;tr<colorID.size();tr++){
        in3 >> a >> b >> c >> d >> e >> f >> g >> h >> i;

        polygon poly = polygon(homogeneous_point(a,b,c),homogeneous_point(d,e,f),homogeneous_point(g,h,i),colors[colorID[tr]]).convertedCoord(screen_x,screen_y);
        polygons.push_back(poly);
    }
}

bool isValid(int x,int y){

    if (x>=0 && x<screen_x && y>=0 && y<screen_y) return true;
    return false;
}

void renderImage(homogeneous_point p1, homogeneous_point p2, homogeneous_point p3,color c){
    int y_min = p1.y, y_mid = p2.y,  y_max = p3.y, x, y;
    double x_a, x_b,z, z_a,z_b;


    /////////////////////////////// y = ymin /////////////////////////////////////
    x = p1.x;
    z = p1.z;

    if(isValid(x,y_min) && z < zs[x][y_min]){
        zs[x][y_min] = z;
        pixels[x][y_min] = c;
    }

    //////////////////////////////// when ymin = ymid ///////////////////////////

    if (y_min == y_mid){
        x_a = p1.x;
        x_b = p2.x;
        z_a = p1.z;
        z_b = p2.z;

        for (x = x_a; x <= int(x_b) ; x++){

            if (!isValid(x,y_min)) continue;

            z = z_b + (z_b - z_a) * ((x-x_b)/(x_b - x_a));

            if (z < zs[x][y_mid]){
                zs[x][y_mid] = z;
                pixels[x][y_mid] = c;
            }
        }
    }

    for (int y = y_min+1 ; y <= y_mid ; y++){

        if (y_mid == y_min) break;

        x_a = p2.x + (p2.x - p1.x) * ( (y - p2.y)/(p2.y-p1.y) );
        x_b = p3.x + (p3.x - p1.x) * ( (y - p3.y)/(p3.y-p1.y) );
        z_a = p2.z + (p2.z - p1.z) * ( (y - p2.y)/(p2.y-p1.y) );
        z_b = p3.z + (p3.z - p1.z) * ( (y - p3.y)/(p3.y-p1.y) );

        if (x_a > x_b){
            swap(x_a , x_b);
            swap(z_a , z_b);
        }

        for (x = x_a; x <= int(x_b) ; x++){

            if (!isValid(x,y)) continue;

            z = z_b + ( z_b - z_a ) * (( x - x_b ) / ( x_b - x_a ));

            if (z < zs[x][y]){
                zs[x][y] = z;
                pixels[x][y] = c;
            }
        }
    }

    if (y_max == y_mid){
        y= y_mid;
        x_a = p2.x; x_b = p3.x; z_a = p2.z; z_b = p3.z;

        for (x = x_a ; x <= int(x_b) ; x++){

            if (!isValid(x,y_max)) continue;

            z = z_b + (z_b - z_a) * ((x - x_b)/(x_b - x_a));

            if (z < zs[x][y]){
                zs[x][y] = z;
                pixels[x][y] = c;
            }
        }
        return;
    }


    for (int y = y_mid+1 ; y < y_max; y++){

        x_a = p2.x + (p2.x - p3.x) * ((y-p2.y)/(p2.y - p3.y));
        x_b = p1.x + (p1.x - p3.x) * ((y-p1.y)/(p1.y - p3.y));
        z_a = p2.z + (p2.z - p3.z) * ((y-p2.y)/(p2.y - p3.y));
        z_b = p1.z + (p1.z - p3.z) * ((y-p1.y)/(p1.y - p3.y));

        if (x_a > x_b){
            swap(x_a , x_b);
            swap(z_a , z_b);
        }

        for (x = x_a; x <= int(x_b) ; x++){

            if (!isValid(x,y)) continue;

            z = (((x - x_a) * (z_a - z_b))/(x_a - x_b)) + z_a;

            if (z < zs[x][y]){
                zs[x][y] = z;
                pixels[x][y] = c;
            }
        }
    }

    x = p3.x; y = y_max;  z = p3.z;

    if(isValid(x,y) && z < zs[x][y] ){
        zs[x][y] = z;
        pixels[x][y] = c;
    }
}

void drawImage(){

    bitmap_image image(screen_x, screen_y);
    for (int x = 0; x < screen_x; x++) {
        for (int y = 0; y < screen_y; y++) {
            image.set_pixel(x, y, pixels[x][y].r, pixels[x][y].g, pixels[x][y].b);
        }
    }
    image.save_image("out.bmp");
}

void stage4(){
    initializePixel_Zbuffer();
    createTriangle();

    for (int i=0;i<polygons.size();i++){
        polygon p = polygons[i];

        renderImage(p.p1,p.p2,p.p3, p.colors);
    }

    drawImage();
}


int main(){

    cout << std::fixed;
    cout << std::setprecision(8);
    scene.open ("scene.txt");
    out1.open ("stage1.txt");
    out1 << std::fixed;
    out1 << std::setprecision(8);

    scene >> eye_x >> eye_y >> eye_z;
    scene >> look_x >> look_y >> look_z;
    scene >> up_x >> up_y >> up_z;
    scene >> fov_y >> aspectRatio >> near >> far;
    scene >> screen_x >> screen_y;
    scene >> backgroud.r >> backgroud.g >> backgroud.b;


    stage1();
    stage2();
    stage3(near,far);
    stage4();

    remove("temp.txt");

    return 0;

}
