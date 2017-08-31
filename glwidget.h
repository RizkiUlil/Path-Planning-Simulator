#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class GLWidget : public QGLWidget
{
public:
    GLWidget();

    GLUquadricObj *Qobj;

    struct Position{
        double x;
        double y;
    };

    Position Ball;
    Position Path[100];
    Position MyTeam[3];
    Position Others[20];
    Position viaPoint[100];
    Position AStar[5000];

    int numPlayer, numObstacle, numNode, numPath, numAStar;

    void resizeGL(int width, int height);

    void drawNode(void);
    void drawPath(void);
    void drawField(void);
    void drawBall(void);
    void drawRobot(int x, int y, double angle, int mode, int number);
    void drawTeam(void);
    void drawObstacle(void);
    void drawAStarPath(void);
protected:
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);

private:


};

#endif // GLWIDGET_H
