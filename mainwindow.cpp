#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myOpenGL = new GLWidget();

    ui->verticalLayout->addWidget(myOpenGL);

    ui->Edit_RPosition_x->setText("-150");
    ui->Edit_RPosition_y->setText("-150");
    ui->Edit_BallPosition_x->setText("150");
    ui->Edit_BallPosition_y->setText("150");

    ui->spinBox_ObstacleNumber->setRange(0,10);
    ui->spinBox_ObstacleNumber->setValue(2);
    ui->spinBox_NodeNumber->setRange(2,20);
    ui->spinBox_NodeNumber->setValue(2);

    ui->Edit_GA_Gen->setText("0");
    ui->Edit_GA_Population->setText("10");
    ui->Edit_GA_CO_Rate->setText("0.6");
    ui->Edit_GA_MU_Rate->setText("0.1");
    ui->Edit_GA_Generation->setText("10");
    dx[0] = 1; dx[1] = 1; dx[2] = 0; dx[3] = -1; dx[4] = -1; dx[5] = -1; dx[6] =  0; dx[7] =  1;
    dy[0] = 0; dy[1] = 1; dy[2] = 1; dy[3] =  1; dy[4] =  0; dy[5] = -1; dy[6] = -1; dy[7] = -1;

    ui->statusBar->showMessage(tr("Mobile Robot Path Planning Simulator - PENS 2017_____Ready!"),120000);
    srand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool operator<(const node & a, const node & b)
{
    return a.getPriority() > b.getPriority();
}

double GetDistance(float x1, float y1, float x2, float y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

void MainWindow::on_Button_Generate_Data_clicked()
{
    bool ok = true;

    myOpenGL->MyTeam[0].x = MyRobotPosition[0].x = ui->Edit_RPosition_x->text().toInt();
    myOpenGL->MyTeam[0].y = MyRobotPosition[0].y = ui->Edit_RPosition_y->text().toInt();
    myOpenGL->Ball.x = MyBallPosition.x = ui->Edit_BallPosition_x->text().toInt();
    myOpenGL->Ball.y = MyBallPosition.y = ui->Edit_BallPosition_y->text().toInt();
    myObstacleNumber = ui->spinBox_ObstacleNumber->text().toInt();

    ObstacleCounter = 0;
    for(int ii=0; ii<100; ii++){
        ok = true;
        float distanceObs = 0, distanceRbt = 0, distanceBall = 0;

        int xPos = (rand()%600) - 300;
        int yPos = (rand()%400) - 200;

        for(int jj=0; jj<ObstacleCounter;jj++){
            distanceObs = GetDistance(xPos,yPos,MyObstacle[jj].x,MyObstacle[jj].y);
            if(distanceObs < 50){ok = false; break;}
        }

        distanceRbt = GetDistance(xPos,yPos, MyRobotPosition[0].x, MyRobotPosition[0].y);
        if(distanceRbt < 50){ok = false;}

        distanceBall = GetDistance(xPos,yPos, MyBallPosition.x, MyBallPosition.y);
        if(distanceBall < 50){ok = false;}

        if(ok && myObstacleNumber > 0){
            myOpenGL->Others[ObstacleCounter].x = MyObstacle[ObstacleCounter].x = xPos;
            myOpenGL->Others[ObstacleCounter].y = MyObstacle[ObstacleCounter].y = yPos;
            ObstacleCounter++;
        }

        if(ObstacleCounter >= myObstacleNumber) break;
    }

    myOpenGL->numObstacle = ObstacleCounter;
    myOpenGL->numPlayer = 1;
}

void MainWindow::on_Button_Generate_viaPoint_clicked()
{
    myNodeNumber = ui->spinBox_NodeNumber->text().toInt();

    if(myNodeNumber > 2){
        int deltaX = 0, deltaY = 0, step = 0, xOne = 0, yOne = 0, xPos = 0, yPos = 0, dX = 0, dY = 0;
        float distance = 0;
        bool ok = true;

        clock_t start = clock();
        deltaX = MyBallPosition.x - MyRobotPosition[0].x;
        deltaY = MyBallPosition.y - MyRobotPosition[0].y;

        if(abs(deltaX) < 100){
            deltaX = 150;
            if(MyRobotPosition[0].x < MyBallPosition.x)
                xOne = MyRobotPosition[0].x - 50;
            else
                xOne = MyRobotPosition[0].x + 50;
        }else{
            xOne = MyRobotPosition[0].x;
        }

        if(abs(deltaY) < 100){
            deltaY = 150;
            if(MyRobotPosition[0].y < MyBallPosition.y)
                yOne = MyRobotPosition[0].y - 50;
            else
                yOne = MyRobotPosition[0].y + 50;
        }else{
            yOne = MyRobotPosition[0].y;
        }

        step = deltaX / (myNodeNumber-1);

        NodeCounter = 1;
        for(int ii=1; ii<100; ii++){
            ok = true;

            xPos = xOne + (ii+1%(myNodeNumber-2))*step;

            if(MyRobotPosition[0].y < MyBallPosition.y) yPos = yOne + (rand() % abs(deltaY));
            else                                        yPos = yOne - (rand() % abs(deltaY));

            for(int jj=0; jj<myObstacleNumber; jj++){
                distance = GetDistance(xPos, yPos, MyObstacle[jj].x, MyObstacle[jj].y);
                dX = abs(xPos - MyObstacle[jj].x);
                dY = abs(yPos - MyObstacle[jj].y);

                if((dX <= 10 && dY <= 10) && distance <= 50){ ok = false; break;}
            }

            if(ok){
                for(int jj=0; jj<NodeCounter; jj++){
                    distance = GetDistance(xPos, yPos, MyNode[jj].x, MyNode[jj].y);
                    dX = abs(xPos - MyNode[jj].x);
                    dY = abs(yPos - MyNode[jj].y);

                    if((dX <= 5 && dY <= 5) && distance <= 20){ ok = false; break;}
                }
            }

            if(ok){
                myOpenGL->viaPoint[NodeCounter].x = MyNode[NodeCounter].x = xPos;
                myOpenGL->viaPoint[NodeCounter].y = MyNode[NodeCounter].y = yPos;
                NodeCounter++;
            }

            if(NodeCounter > myNodeNumber-1) break;
        }
        clock_t stop = clock();

        MyNode[0].x = MyRobotPosition[0].x;
        MyNode[0].y = MyRobotPosition[0].y;
        MyNode[NodeCounter-1].x = MyBallPosition.x;
        MyNode[NodeCounter-1].y = MyBallPosition.y;
        myOpenGL->numNode = NodeCounter;

        QString xx;
        QTextStream(&xx)<<"Execution Time : "<<((double)(stop - start)*1000/CLOCKS_PER_SEC)<<" ms";
        ui->Label_Execution_viaPoint->setText(xx);

        xx.clear();
        ui->List_viaPoint->clear();
        for(int ii=0; ii<NodeCounter; ii++){
            xx.clear();
            QTextStream(&xx)<<"Point["<<ii<<"] : "<<MyNode[ii].x<<","<<MyNode[ii].y;
            ui->List_viaPoint->addItem(xx);
        }

        ui->Edit_GA_Gen->setText(QString::number(NodeCounter));
    }else{
        QMessageBox::warning(this,"via Point","Not enough via Point",QMessageBox::Ok);
    }
}

void MainWindow::on_Button_Generate_GA_Parameter_clicked()
{
    UkuranKromosom = ui->Edit_GA_Gen->text().toDouble();
    UkuranPopulasi = ui->Edit_GA_Population->text().toDouble();
    ProbabilitasPenyilangan = ui->Edit_GA_CO_Rate->text().toDouble();
    ProbabilitasMutasi = ui->Edit_GA_MU_Rate->text().toDouble();
    BanyakGenerasi = ui->Edit_GA_Population->text().toDouble();

    MaxObjectif = myOpenGL->numPath = cacah = 0;

    clock_t start = clock();
    Inisialisasi(UkuranKromosom);
    for(int Generasi = 1; Generasi <= BanyakGenerasi; Generasi++){
        Evaluasi(Generasi);
        Historis(Generasi);
        Seleksi(Generasi);
        Penyilangan();
        Mutasi();
    }
    clock_t stop = clock();
    qDebug()<<"Execution Time : "<<(double)(stop - start)*1000/CLOCKS_PER_SEC<<" ms";    
    qDebug()<<"Jarak : "<<BestPathDistance;
}

void MainWindow::on_Button_Generate_Astar_clicked()
{
    int xA, yA, xG, yG;    
    xA = (MyRobotPosition[0].x + 370)/5;
    yA = (MyRobotPosition[0].y + 270)/5;
    xG = (MyBallPosition.x + 370)/5;
    yG = (MyBallPosition.y + 270)/5;
    clearMap();
    initMap();
    clock_t start = clock();
    string x = AstarPathFinding(xA,yA, xG, yG);
    clock_t stop = clock();
    qDebug()<<"Execution Time : "<<(double)(stop - start)*1000/CLOCKS_PER_SEC<<" ms";
    qDebug()<<"Jarak : "<<x.length()*5;
    QString ii = QString::fromStdString(x);
    int xPos, yPos, counterAStar=0;
    if(x.length() > 0){
        xPos = xA;
        yPos = yA;
        for(size_t i = 0; i < x.length(); i++){
            char c = x.at(i);
            int j = atoi(&c);
            xPos = xPos+dx[j];
            yPos = yPos+dy[j];

            myOpenGL->AStar[counterAStar].x = (xPos*5)-370;
            myOpenGL->AStar[counterAStar].y = (yPos*5)-270;
            counterAStar++;
        }
    }
    myOpenGL->numAStar = counterAStar;
    qDebug()<<ii;
}

float MainWindow::AcakRill()
{
    return rand()/float(RAND_MAX);
}

int MainWindow::circleLineIntersect(int x1, int y1, int x2, int y2, int cx, int cy, int cr)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    float a = pow(dx,2) + pow(dy,2);
    float b = 2 * (dx * (x1 - cx) + dy * (y1 - cy));
    float c = pow(cx,2) + pow(cy,2);

    c += pow(x1,2) + pow(y1,2);
    c -= 2 * (cx * x1 + cy * y1);
    c -= pow(cr,2);

    float bb4ac = pow(b,2) - 4 * a * c;

    if(bb4ac < 0)   return 0;
    else            return 1;
}

int MainWindow::GetIntersection(int populasi)
{
    int intersection = 0;
    Point temp;
    temp.x = MyNode[0].x;
    temp.y = MyNode[0].y;

    for(int i = 2; i <= UkuranKromosom; i++){
        if(Kromosom[populasi][i]){
            for(int j=0; j < ObstacleCounter; j++){
                intersection += circleLineIntersect(temp.x, temp.y, MyNode[i-1].x, MyNode[i-1].y, MyObstacle[j].x, MyObstacle[j].y, 40);
            }
            temp.x = MyNode[i-1].x;
            temp.y = MyNode[i-1].y;
        }
    }
    return intersection;
}

float MainWindow::GetPathDistance(int populasi)
{
    float distance;
    Point temp;
    temp.x = MyNode[0].x;
    temp.y = MyNode[0].y;

    for(int i = 2; i <= UkuranKromosom; i++){
        if(Kromosom[populasi][i]){
            distance += GetDistance(temp.x, temp.y, MyNode[i-1].x, MyNode[i-1].y);
            temp.x = MyNode[i-1].x;
            temp.y = MyNode[i-1].y;
        }
    }
    return distance;
}

void MainWindow::Inisialisasi(int LebarKromosom)
{
    for(int i=1; i <= UkuranPopulasi; i++){
        for(int j=1; j<= 16; j++){
            if(j==1 || j==LebarKromosom)    Kromosom[i][j] = 1;
            else if(j > LebarKromosom)      Kromosom[i][j] = 0;
            else{
                if(AcakRill() < 0.5)        Kromosom[i][j] = 0;
                else                        Kromosom[i][j] = 1;
            }
        }
    }
}

void MainWindow::Evaluasi(int IndexGenerasi)
{
    for(int i = 1; i <= UkuranPopulasi; i++){
        Jarak[IndexGenerasi][i] = GetPathDistance(i);
        Perpotongan[IndexGenerasi][i] = GetIntersection(i)+1;
        Objektif[IndexGenerasi][i] = 1/(GetPathDistance(i) * (GetIntersection(i)+1));
    }
}

void MainWindow::Historis(int IndexGenerasi)
{
    for(int i = 0; i <= UkuranPopulasi; i++){
        if(MaxObjectif < Objektif[IndexGenerasi][i]){
            MaxObjectif = Objektif[IndexGenerasi][i];
            BestPathDistance = Jarak[IndexGenerasi][i];
            BestIntersection = Perpotongan[IndexGenerasi][i];
            int myNumPath = 0;

            for(int j = 1; j <= UkuranKromosom; j++){
                KromosomMax[j] = Kromosom[i][j];
                if(Kromosom[i][j]){
                    myOpenGL->Path[myNumPath].x = MyNode[j-1].x;
                    myOpenGL->Path[myNumPath].y = MyNode[j-1].y;
                    myNumPath++;
                }
            }

            myOpenGL->numPath = myNumPath;
            GenerasiMax = IndexGenerasi;
        }
    }
}

void MainWindow::Seleksi(int IndexGenerasi)
{
    float JumlahProb[101], Fitness[101], Prob[101];
    int Terseleksi[101];
    float JumlahFitness = 0, Acak = 0;
    Vektor KromosomSeleksi[101];

    JumlahFitness = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Fitness[i] = Objektif[IndexGenerasi][i];
        JumlahFitness += Fitness[i];
    }
    JumlahProb[0] = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Prob[i] = Fitness[i]/JumlahFitness;
        JumlahProb[i] = JumlahProb[i-1]+Prob[i];
    }
    int j=0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Acak = AcakRill();
        j = 1;
        while(Acak>JumlahProb[j])
            j++;
        Terseleksi[i] = j;
    }
    for(int i = 1; i <= UkuranPopulasi; i++){
        for(int j = 0; j <= UkuranKromosom; j++){
            KromosomSeleksi[i][j] = Kromosom[Terseleksi[i]][j];
        }
    }
}

void MainWindow::PilihInduk(MainWindow::Pasangan &Ortu, int &CacahInduk)
{
    CacahInduk = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        if(AcakRill() <= ProbabilitasPenyilangan){
            cacah++;
            CacahInduk = CacahInduk + 1;
            if(CacahInduk%2 == 1)   Ortu[1][(CacahInduk/2)+1] = i;
            else                    Ortu[2][(CacahInduk/2)]   = i;
        }
    }
    if(CacahInduk%2==1)
        CacahInduk = CacahInduk - 1;
}

void MainWindow::Ganti(int nOrtu, int nGen, MainWindow::Pasangan Ortu)
{
    for(int k = 1; k <= nOrtu/2; k++){
        for(int j = 1; j <= nGen; j++){
            Kromosom[Ortu[1][k]][j] = KromosomSilang[2*k-1][j];
            Kromosom[Ortu[2][k]][j] = KromosomSilang[2*k][j];
        }
    }
}

void MainWindow::Penyilangan()
{
    Pasangan Induk;
    int Posisi = 0, BanyakInduk = 0;

    PilihInduk(Induk, BanyakInduk);

    if(BanyakInduk > 0){
        Posisi = rand()%(UkuranKromosom-1)+1;

        for(int i=1; i<=BanyakInduk/2; i++){
            for(int j=1; j<=Posisi; j++){
                KromosomSilang[2*i-1][j] = Kromosom[Induk[2][i]][j];
                KromosomSilang[2*i][j] = Kromosom[Induk[1][i]][j];
            }

            for(int j=Posisi+1; j<=UkuranKromosom; j++){
                KromosomSilang[2*i-1][j] = Kromosom[Induk[1][i]][j];
                KromosomSilang[2*i][j] = Kromosom[Induk[2][i]][j];
            }
        }
        Ganti(BanyakInduk, UkuranKromosom, Induk);
    }
}

void MainWindow::Mutasi()
{
    for(int i=1; i<=UkuranPopulasi; i++){
        for(int j=1; j<=UkuranKromosom; j++){
            if(j==1 || j==UkuranKromosom){
                Kromosom[i][j] = 1;
            }else{
                if(AcakRill() <= ProbabilitasMutasi){
                    if(Kromosom[i][j]==1)	Kromosom[i][j] = 0;
                    else					Kromosom[i][j] = 1;
                }
            }
        }
    }
}

void MainWindow::clearMap()
{
    for(int x = 0; x < 74; x++){
        for(int y = 0; y < 54; y++){
            map[x][y] = closed_nodes_map[x][y] = 0;
            open_nodes_map[x][y] = dir_map[x][y] = 0;
        }
    }
}

void MainWindow::initMap()
{
    for(int Obs = 0; Obs < ObstacleCounter; Obs++){
        int x = (MyObstacle[Obs].x + 370)/5;
        int y = (MyObstacle[Obs].y + 270)/5;
        qDebug()<<x<<"-"<<y;
        drawObstacleMap(x, y, 3);
    }
}

void MainWindow::drawObstacleMap(int x, int y, int r)
{
    int xMin = 0, xMax = 0,yMin = 0, yMax = 0;

    xMin = x - r; xMax = x + r;
    yMin = y - r; yMax = y + r;

    (xMin < 0 ? xMin = 0 : 0);
    (xMax > 740/5 ? xMax = 740/5 : 0);

    (yMin < 0 ? yMin = 0 : 0);
    (yMax > 540/5 ? yMax = 540/5 : 0);

    for(int xMap = xMin; xMap < xMax; xMap++)
        for(int yMap = yMin; yMap < yMax; yMap++)
            map[xMap][yMap] = 1;
}

string MainWindow::AstarPathFinding(const int & xStart, const int & yStart, const int & xGoal, const int & yGoal)
{
    priority_queue<node> pq[2];
    int pqi = 0, x = 0, y = 0, i = 0, j = 0, xdx = 0, ydy = 0;
    char c = 0;
    node* n0;
    node* m0;

    n0 = new node(xStart, yStart,0,0);
    n0->updatePriority(xGoal, yGoal);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]= n0->getPriority();

    while(!pq[pqi].empty()){        
        n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), pq[pqi].top().getLevel(), pq[pqi].top().getPriority());
        x = n0->getxPos(); y = n0->getyPos();

        pq[pqi].pop();
        open_nodes_map[x][y] = 0;
        closed_nodes_map[x][y] = 1;

        if(x == xGoal && y == yGoal){
            string path = "";
            while(!(x == xStart && y == yStart)){                
                j = dir_map[x][y];
                c = '0' + (j+4)%8;
                path = c + path;
                x += dx[j];
                y += dy[j];
            }
            delete n0;
            while(!pq[pqi].empty()) pq[pqi].pop();
            return path;
        }

        for(i = 0; i < 8; i++){            
            xdx = x+dx[i]; ydy = y+dy[i];

            if(!(xdx<0 || xdx>185-1 || ydy<0 || ydy>135-1 || map[xdx][ydy]==1 || closed_nodes_map[xdx][ydy]==1)){
                m0 = new node(xdx, ydy, n0->getLevel(), n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xGoal, yGoal);

                if(open_nodes_map[xdx][ydy]==0){
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    dir_map[xdx][ydy] = (i+4)%8;
                }else if(open_nodes_map[xdx][ydy]>m0->getPriority()){
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    dir_map[xdx][ydy]=(i+4)%8;

                    while(!pq[pqi].top().getxPos()==xdx && pq[pqi].top().getyPos()==ydy){
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop();

                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty()){
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0);
                }else delete m0;
            }
        }
        delete n0;
    }
    return "";
}
