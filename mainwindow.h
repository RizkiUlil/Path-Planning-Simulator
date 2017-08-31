#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QEventLoop>
#include <QMessageBox>

#include "glwidget.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    struct Point{
        int x,y;
    };
    
    Point MyObstacle[20], MyRobotPosition[5], MyBallPosition, MyNode[100];
    int myObstacleNumber, myNodeNumber;
    int ObstacleCounter, NodeCounter;

    //Variabel GA
    typedef int Vektor[101];
    typedef Vektor Solusi[101];
    typedef Vektor Pasangan[3];
    Solusi Kromosom, KromosomSilang;
    float Objektif[1001][101], Jarak[1001][101], Perpotongan[1001][101];
    double MaxObjectif, BestPathDistance, ProbabilitasPenyilangan, ProbabilitasMutasi, cacah;
    int KromosomMax[101];
    int UkuranPopulasi, UkuranKromosom, BanyakGenerasi, GenerasiMax, BestIntersection;

    //Variabel A star
    int map[740][540];
    int closed_nodes_map[740][540];
    int open_nodes_map[740][540];
    int dir_map[740][540];
    int dx[8];// = { 1, 1, 0,-1,-1,-1, 0, 1};
    int dy[8];// = { 0, 1, 1, 1, 0,-1,-1,-1};

private slots:
    void on_Button_Generate_Data_clicked();
    void on_Button_Generate_viaPoint_clicked();
    void on_Button_Generate_GA_Parameter_clicked();
    void on_Button_Generate_Astar_clicked();

private:
    Ui::MainWindow *ui;
    GLWidget *myOpenGL;

    //Algoritma Genetika
    float AcakRill();
    int circleLineIntersect(int x1, int y1, int x2, int y2, int cx, int cy, int cr);
    int GetIntersection(int populasi);
    float GetPathDistance(int populasi);
    void Inisialisasi(int LebarKromosom);
    void Evaluasi(int IndexGenerasi);
    void Historis(int IndexGenerasi);
    void Seleksi(int IndexGenerasi);
    void PilihInduk(Pasangan& Ortu, int& CacahInduk);
    void Ganti(int nOrtu, int nGen, Pasangan Ortu);
    void Penyilangan(void);
    void Mutasi(void);

    //Algoritma A star
    void clearMap();
    void initMap();
    void drawObstacleMap(int x, int y, int r);
    string AstarPathFinding(const int & xStart, const int & yStart, const int & xGoal, const int & yGoal);
};

class node
{
    int xPos, yPos;
    int level;
    int priority;

public:
    node(int xp, int yp, int d, int p){xPos=xp; yPos=yp; level=d; priority=p;}
    int getxPos()   const{return xPos;}
    int getyPos()   const{return yPos;}
    int getLevel()   const{return level;}
    int getPriority()   const{return priority;}
    void clear(){xPos=yPos=level=priority=0;}
    void updatePriority(const int & xDest, const int & yDest){
        priority = level+estimate(xDest, yDest)*10;
    }

    void nextLevel(const int & i) // i: direction
    {
         level+=(i%2==0?10:14);
    }

    const int & estimate(const int & xDest, const int & yDest) const
    {
        static int xd, yd, d;
        xd=xDest-xPos;
        yd=yDest-yPos;
        d=static_cast<int>(sqrt(xd*xd+yd*yd));
        return(d);
    }
};

#endif // MAINWINDOW_H
