// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.
#include <GL/glew.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>

using namespace std;

class Vector {
  float values[4];
  public:
  Vector(float a=0.0,float b=0.0,float c=0.0,float d=0.0){
    values[0]=a; values[1]=b; values[2]=c; values[3]=d;
  }
  float operator *(const Vector &other){ // really the dot product
    float total=0.0;
    for (int i=0;i<4;i++)
      total+=values[i]*other.values[i];
    return total;
  }
  float x(){return values[0];}
  float y(){return values[1];}
  float z(){return values[2];}
  float w(){return values[3];}
  float r(){return values[0];}
  float g(){return values[1];}
  float b(){return values[2];}
  float a(){return values[3];}
};

class Triangle {
  Vector a,b,c,color;
  public:

};

void addTriangle(Vector a, Vector b, Vector c, Vector color) {
  glBegin(GL_POLYGON);
    glColor3f(color.r(),color.g(),color.b()); glVertex3f(a.x(),a.y(),a.z());
    glColor3f(color.b(),color.r(),color.g()); glVertex3f(b.x(),b.y(),b.z());
    glColor3f(color.g(),color.b(),color.r()); glVertex3f(c.x(),c.y(),c.z());
  glEnd();
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);
  addTriangle(Vector(-0.6, -0.75, 0.5),
              Vector(0.6, -0.75, 0),
              Vector(0, 0.75, 0),
              Vector(1,0,0));
  addTriangle(Vector(-0.6, -0.75, 1.5),
              Vector(0.6, -0.75, 1),
              Vector(0, 0.75, 1),
              Vector(1,0,0));

  // glBegin(GL_POLYGON);
  //   glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
  //   glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
  //   glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
  // glEnd();

  // glBegin(GL_POLYGON);
  //   glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 1.5);
  //   glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 1);
  //   glColor3f(0, 0, 1); glVertex3f(0, 0.75, 1);
  // glEnd();

  glFlush();
}

//int Loc1,Loc2;
int Loc3;

  // We want to be able to write Linear Algebra statement similar to Shader Language GLSL
  // n=m*p;  Matrix n,m,p;
class Matrix {
  float values[4][4];
  public:
  /*
  Matrix(const Matrix &m) {
    for (int i=0;i<4;i++)
      for (int j=0;j<4;j++)
        values[i][j]=m.values[i][j];
  } 
  */
  Matrix(float a=0.0,float b=0.0, float c=0.0,float d=0.0,
         float e=0.0,float f=0.0,float g=0.0, float h=0.0,
         float i=0.0,float j=0.0,float k=0.0,float l=0.0,
         float m=0.0,float n=0.0,float o=0.0,float p=0.0){
          values[0][0]=a;values[0][1]=b;values[0][2]=c;values[0][3]=d;
          values[1][0]=e;values[1][1]=f;values[1][2]=g;values[1][3]=h;
          values[2][0]=i;values[2][1]=j;values[2][2]=k;values[2][3]=l;
          values[3][0]=m;values[3][1]=n;values[3][2]=o;values[3][3]=p;
         }
  static Matrix rotView(float alpha,float gamma,float s) {
   return s*Matrix::rotX(alpha)*Matrix::rotY(gamma);  
  }
  void setUniform(int loc){
     glUniformMatrix4fv(loc,16,false,(const float *) &values);
  }
  static Matrix rotX(float angle) {
    return Matrix(1.0,        0.0,       0.0,     0.0,
                  0.0, cos(angle),sin(angle),0.0,
                  0.0,-sin(angle),cos(angle),0.0,
                  0.0,        0.0,       0.0,     1.0);
  }
  static Matrix rotY(float angle) {
    return Matrix(cos(angle), 0.0,sin(angle),0.0,
                  0.0,          1.0,          0.0,0.0,
                  -sin(angle),0.0,cos(angle),0.0,
                  0.0,        0.0,            0.0,1.0);
  } 
  static Matrix rotZ(float angle) {
    return Matrix(); // To be completed by student
  }
  static Matrix shift(float dx=0.0,float dy=0.0,float dz=0.0) {
    return Matrix(); // To be completed by student
  }  
  Matrix & operator *(float s) const { 
    static Matrix res;
    for (int i=0;i<4;i++)
      for (int j=0;j<4;j++)
        res.values[i][j]=s*values[i][j];
    return res;
  }
  friend Matrix & operator *(float s,const Matrix &m){
      return m*s;
  }
  Vector row(int i) const {
    if (i<0 || i>=4) return Vector();
    return Vector(values[i][0],values[i][1],values[i][2],values[i][3]);
  }
  Vector col(int j) const {
    if (j<0 || j>=4) return Vector();
    return Vector(values[0][j],values[1][j],values[2][j],values[3][j]);
  }
  Matrix & operator *(const Matrix &other)const {
     static Matrix res;
     for (int i=0;i<4;i++)
      for (int j=0;j<4;j++)
        res.values[i][j]=row(i)*other.col(j);
     return res;
  }
  friend ostream & operator <<(ostream &out,const Matrix &m){
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        out << setw(4) << fixed << setprecision(2) << m.values[i][j] << ' ';
      }
      out << endl;
    }
  }
};


void mouseMove(int button, int state,int x, int y){
  float t=(float)x/800.0;
  float alpha=(90 - -90)*t+-90;
  // Assuming 0.0<=t<=1.0
  // lineary interpolate between start and end(end-start)*t+start;
  t=(float)y/600.0;
  float gamma=(-90 - 90)*t+90;
  cout << gamma << ' ' << alpha << endl;

 // glUniform1f(Loc1,gamma);
  //glUniform1f(Loc2,alpha);
  Matrix rx=Matrix::rotX(alpha);
  cout << "Rot X" << endl << rx;
  Matrix ry=Matrix::rotY(gamma);
  cout << "Rot Y" << endl << ry;

  Matrix m=Matrix::rotView(alpha,gamma,1.0);
  cout << m;
  m.setUniform(Loc3);

  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 600);
  glutCreateWindow("A Simple Triangle");

  GLenum err = glewInit();
  if (err != GLEW_OK)
    exit(1); // or handle the error in a nicer way

  glutDisplayFunc(display);
  glutMouseFunc(mouseMove);

    ifstream inFile;
    stringstream strStream;

    inFile.open("vertex.glsl"); //open the input file
    strStream << inFile.rdbuf(); //read the file
    string s=strStream.str();

    inFile.close();
  const char *vertexShaderSource=s.c_str();
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
  }
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    inFile.open("fragment.glsl"); //open the input file
    strStream.str("");
    strStream << inFile.rdbuf(); //read the file
    s=strStream.str();
    inFile.close();
  const char *fragmentShaderSource=s.c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      cout << "ERROR::SHADER::Linking::COMPILATION_FAILED\n" << infoLog << endl;
  }
  //Loc1=glGetUniformLocation(shaderProgram, "gammaIn");
  //Loc2=glGetUniformLocation(shaderProgram, "alphaIn");
  Loc3=glGetUniformLocation(shaderProgram, "viewMat");
  //cout << "Locations "<<Loc1 <<',' << Loc2<< endl;
  glUseProgram(shaderProgram);
  
  // Back to old example
  glutMainLoop();

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader); 
}