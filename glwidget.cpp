#include "glwidget.h"
#include <QDebug>
#include <math.h>

#define PI	22/7
#define TIM 0
#define OBS 1

GLfloat LightAmbient[]=		{ 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat LightDiffuse[]=		{ 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat LightPosition[]=	{ 10.0f, -600.0f,410.0f, 1.0f };

GLWidget::GLWidget()
{
    startTimer( 10 ); //64-65fps
    Ball.x = 0;
    Ball.y = 0;
    numNode = numPlayer = numObstacle = numPath = 0;
}

void GLWidget::initializeGL()
{
    //initialization of OpenGL

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClearDepth(1.0f);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
    glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);	// Position The Light
    glEnable(GL_LIGHT0);								// Enable Light One

    //glEnable(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    resizeGL( 400 , 300 );   
}

void drawCircle(float x, float y, float r, int mode){
    if(mode==0)			glBegin(GL_POLYGON);
    else if(mode == 1)	glBegin(GL_LINE_LOOP);
    for(int i=0;i<360;i++){
        float theta = 2.0f * PI * float(i) / 360;
        float x1=r*cos(theta);
        float y1=r*sin(theta);
        glVertex2f(x1 + x, y1 + y);
    }
    glEnd();
}

void drawCircle3f(float x, float y, float z, float r, int mode){
    if(mode==0)			glBegin(GL_POLYGON);
    else if(mode == 1)	glBegin(GL_LINE_LOOP);
    for(int i=0;i<360;i++){
        float theta = 2.0f * PI * float(i) / 360;
        float x1=r*cos(theta);
        float y1=r*sin(theta);
        glVertex3f(x1 + x, y1 + y, z + 1);
    }
    glEnd();
}

void drawCircleAngle(float x, float y, float r, int min, int max){
    glBegin(GL_POLYGON);
    for(int i=min;i<max;i++){
        float theta = 2.0f * PI * float(i) / 360;
        float x1=r*cos(theta);
        float y1=r*sin(theta);
        glVertex2f(x1 + x, y1 + y);
    }
    glEnd();
}

void GLWidget::drawField(void){
    glPushMatrix();
        //Draw Field
        glColor3ub(0,155,0);
        glBegin(GL_QUADS);
            glVertex2f(-370, 270);
            glVertex2f(-370, -270);
            glVertex2f(370, -270);
            glVertex2f(370, 270);
        glEnd();
        //Draw Field Border
        glColor3ub(255,255,255);
        glLineWidth(4.0f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-300, 200, 0.5);
            glVertex3f(-300, -200, 0.5);
            glVertex3f(300, -200, 0.5);
            glVertex3f(300, 200, 0.5);
        glEnd();
        //Draw Goal Line
        glBegin(GL_LINE_LOOP);
            glVertex3f(-300, 110, 0.5);
            glVertex3f(-300, -110, 0.5);
            glVertex3f(-240, -110, 0.5);
            glVertex3f(-240, 110, 0.5);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex3f(300, 110,0.5);
            glVertex3f(300, -110, 0.5);
            glVertex3f(240, -110, 0.5);
            glVertex3f(240, 110, 0.5);
        glEnd();
        //Draw Circle
        drawCircle(0,0,60,1);
        //Draw Center Line
        glBegin(GL_LINES);
            glVertex2f(0, 200);
            glVertex2f(0, -200);
        glEnd();

        //Draw Goal
        /*glPushMatrix();
            glColor3ub(0,0,255);
            glTranslatef(-300,-75, 0);
            gluCylinder(Qobj, 10.0, 10.0, 80.0, 32, 32);
            glTranslatef(0.0, 0.0, 80);
            gluSphere(Qobj, 10, 360, 32);
            glRotatef(-90,1,0,0);
            gluCylinder(Qobj, 10.0, 10.0, 150.0, 32, 32);
            glTranslatef(0.0, 0.0, 150);
            gluSphere(Qobj, 10, 360, 32);
            glRotatef(-90,1,0,0);
            gluCylinder(Qobj, 10.0, 10.0, 80.0, 32, 32);
        glPopMatrix();
        glPushMatrix();
            glColor3ub(255,255,0);
            glTranslatef(300,75, 0);
            gluCylinder(Qobj, 10.0, 10.0, 80.0, 32, 32);
            glTranslatef(0.0, 0.0, 80);
            gluSphere(Qobj, 10, 360, 32);
            glRotatef(90,1,0,0);
            gluCylinder(Qobj, 10.0, 10.0, 150.0, 32, 32);
            glTranslatef(0.0, 0.0, 150);
            gluSphere(Qobj, 10, 360, 32);
            glRotatef(90,1,0,0);
            gluCylinder(Qobj, 10.0, 10.0, 80.0, 32, 32);
        glPopMatrix();*/
        glLineWidth(20.0f);
        glBegin(GL_LINES);
            glColor3ub(0,0,255);
            glVertex3f(-300, 100,2.0);
            glVertex3f(-300, -100,2.0);
        glEnd();
        glBegin(GL_LINES);
            glColor3ub(255,255,0);
            glVertex3f(300, 100,2.0);
            glVertex3f(300, -100,2.0);
        glEnd();
    glPopMatrix();
}

void GLWidget::drawBall()
{
    /*glPushMatrix();
        glColor3ub(246, 64, 8);
        glTranslatef(Ball.x,Ball.y,5.0);
        gluSphere(Qobj, 5, 360, 32);
    glPopMatrix();*/
    glPushMatrix();
        glColor3ub(0, 255, 255);
        //glColor3ub(246, 64, 8);
        drawCircle3f(Ball.x,Ball.y,2.0,8,0);
    glPopMatrix();
}

void GLWidget::drawRobot(int x, int y, double angle, int mode, int number)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle,0,0,1);
        glPushMatrix();          

            //Draw Top and Bottom
            glTranslatef(0,0,1);            
            glColor3ub(255,0,0);
            drawCircleAngle(0,0,23,30,330);

            //Draw Dot Team
            glTranslatef(0,0,1);
            if(mode == TIM) glColor3ub(0,0,255);
            else            glColor3ub(255,255,0);
            drawCircle(0,0,6,0);

            //Draw Dot Player Number and Heading
            for(int i = 30; i < 330; i++){
                float theta = 2.0f * PI * float(i) / 360;
                float x1=17*cos(theta);
                float y1=17*sin(theta);
                if(i == 55){
                    glColor3ub(0,255,0);
                    drawCircle3f(x1,y1,0,4,0);
                }else if(i == 155){
                    if(number == 4 || number == 5 || number == 7){
                        glColor3ub(154,4,144);
                        drawCircle3f(x1,y1,0,4,0);
                    }
                }else if(i == 205){
                    if(number == 2 || number == 3 || number == 7){
                        glColor3ub(154,4,144);
                        drawCircle3f(x1,y1,0,4,0);
                    }
                }else if(i == 305){
                    if(number == 1 || number == 3 || number == 5 || number == 7){
                        glColor3ub(154,4,144);
                        drawCircle3f(x1,y1,0,4,0);
                    }
                }
            }
        glPopMatrix();
        glPopMatrix();
}

void GLWidget::drawTeam()
{
    for(int i = 0; i < numPlayer; i++){
        glPushMatrix();
            drawRobot(MyTeam[i].x, MyTeam[i].y, 0, TIM, i+1);
        glPopMatrix();
    }
}

void GLWidget::drawObstacle()
{
    for(int i = 0; i < numObstacle; i++){
        glPushMatrix();
            drawRobot(Others[i].x, Others[i].y, 180, OBS, i+1);
            //qDebug()<<"Draw "<<Others[i].x<<" , "<<Others[i].y;
        glPopMatrix();
    }
}

void GLWidget::drawNode()
{
    //qDebug()<<"Num Node : "<<numNode;
    if(numNode > 0){
        glPushMatrix();
            glColor3ub(255,0,0);
            glPointSize(7.0f);
            for(int i=1; i<numNode-1; i++){
                glBegin(GL_POINTS);
                    glVertex2f(viaPoint[i].x, viaPoint[i].y);
                glEnd();
            }
        glPopMatrix();
    }
}

void GLWidget::drawPath()
{
    //if(numPath > 0){
        glPushMatrix();
            glColor3ub(255,255,255);
            glLineWidth(3.0);
            for(int i=1; i<numPath; i++){
                glBegin(GL_LINES);
                    glVertex3d(Path[i-1].x, Path[i-1].y,5);
                    glVertex3d(Path[i].x, Path[i].y,5);
                glEnd();
            }
        glPopMatrix();
    //}
}

void GLWidget::drawAStarPath(void){
    if(numAStar){
        glPushMatrix();
            glColor3ub(255,255,0);
            glPointSize(3.0f);
            for(int i=1; i<numAStar-1; i++){
                glBegin(GL_POINTS);
                    glVertex2f(AStar[i].x, AStar[i].y);
                glEnd();
            }
        glPopMatrix();
    }
}

void drawCOba(){
    glPushMatrix();
        glBegin(GL_LINES);
            glVertex2d(10,10);
            glVertex2d(-100,100);
        glEnd();
    glPopMatrix();
    drawCircle(110,20,100,1);
    /*glPushMatrix();
    for(int i = 0; i < 360; i++){
        for(int j = 0; j < 255; j++){
            int ptX = j * cos((float)(i/4)*3.14/180);
            int ptY = j * sin((float)(i/4)*3.14/180);
            if(j < 35)
                glColor3ub(0,0,0);
            else
                glColor3ub(0,0,255);
            glPointSize(2.0);
            glBegin(GL_POINTS);
                glVertex2d(ptX, ptY);
            glEnd();
        }
    }
    glPopMatrix();*/
}

void GLWidget::paintGL()
{
    //Draw Scene Here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Qobj = gluNewQuadric();

    //gluLookAt(0,-600,410,0,0,0,0,2,0);
    gluLookAt(0,0,650,0,0,0,0,2,0);

    drawField();
    drawPath();
    drawAStarPath();
    drawNode();
    drawBall();
    drawObstacle();
    drawTeam();
}

void GLWidget::resizeGL(int width, int height)
{
    //proces resize keep good aspect ratio for 3D scene
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    int side = qMin(width, height);
    glViewport(0, 0, width, height);

   // glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.01f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0,width,0,width);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  //proces mouse events for rotate/move inside 3D scene
    int dx = event->x();
    int dy = event->y();
    //qDebug()<<"X : "<<dx<<" - Y : "<<dy;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
 //proces keyboard events
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    updateGL();
}
