#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QRectF>
#include <QPainter>
#include <qmath.h>
#include "gamewidget.h"
#include <ctime>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    gridSize(50)
{
    srand(time(NULL));
    color[0] = rand() % 255;
    color[1] = rand() % 255;
    color[2] = rand() % 255;
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()), this, SLOT(newGeneration()));
    clear();
}

GameWidget::~GameWidget()
{
}

void GameWidget::startGame()
{
    timer->start();
}

void GameWidget::stopGame()
{
    timer->stop();
}

void GameWidget::clear()
{
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            grid[i][j] = false;
        }
    }
    srand(time(NULL));
    color[0] = rand() % 255;
    color[1] = rand() % 255;
    color[2] = rand() % 255;
    update();
}


int GameWidget::interval()
{
    return timer->interval();
}

void GameWidget::setInterval(int msec)
{
    timer->setInterval(msec);
}

bool GameWidget::isAlive(int x, int y)
{
    int count = 0;
    int xp = x;
    int xm = x;
    int yp = y;
    int ym = y;
    if((x - 1) == -1)
        xm = gridSize;
    if((y - 1) == -1)
        ym = gridSize;
    if((x + 1) == gridSize)
        xp = -1;
    if((y + 1) == gridSize)
        yp = -1;
    count =   grid[xm - 1][ym - 1] + grid[x][ym - 1] + grid[xp + 1][ym - 1]
            + grid[xm - 1][y]                        + grid[xp + 1][y]
            + grid[xm - 1][yp + 1] + grid[x][yp + 1] + grid[xp + 1][yp + 1];
    if (((grid[x][y] == true) && (count == 2)) || (count == 3))
        return true;
    return false;
}

void GameWidget::newGeneration()
{
    for(int i = 0; i < gridSize; i++)
        for(int j = 0; j < gridSize; j++)
            ngrid[i][j] = isAlive(i, j);
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            grid[i][j] = ngrid[i][j];
        }
    }
    update();
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    paintUniverse(p);
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    double cellWidth = (double)width()/gridSize;
    double cellHeight = (double)height()/gridSize;
    int i = floor(e->y()/cellHeight);
    int j = floor(e->x()/cellWidth);
    grid[i][j] = !grid[i][j];
    update();
}

void GameWidget::mouseMoveEvent(QMouseEvent *e)
{
    double cellWidth = (double)width()/gridSize;
    double cellHeight = (double)height()/gridSize;
    int i = floor(e->y()/cellHeight);
    int j = floor(e->x()/cellWidth);
    if(!grid[i][j]){                //if current cell is empty,fill in it
        grid[i][j] = !grid[i][j];
        update();
    }
}

void GameWidget::paintGrid(QPainter &p)
{
    QRect borders(0, 0, width()-1, height()-1); // borders of the universe
    p.setPen(QColor("White"));
    double cellWidth = (double)width()/gridSize; // width of the widget / number of cells at one row
    for(double k = cellWidth; k <= width(); k += cellWidth)
        p.drawLine(k, 0, k, height());
    double cellHeight = (double)height()/gridSize; // height of the widget / number of cells at one row
    for(double k = cellHeight; k <= height(); k += cellHeight)
        p.drawLine(0, k, width(), k);
    p.drawRect(borders);
}

void GameWidget::paintUniverse(QPainter &p)
{
    double cellHeight = (double)height()/gridSize;
    double cellWidth  = (double)width()/gridSize;
    if(grid[0][0] == true) { // if there is any sense to paint it
        qreal left = (qreal)(0); // margin from left
        qreal top  = (qreal)(0); // margin from top
        QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
        p.fillRect(r, QBrush(QColor(255,128,141,255))); // fill cell with color
    }
        for(int i = 0; i < gridSize; i++)
        for(int j = 0; j < gridSize; j++) {
            if(grid[i][j] == true) { // if there is any sense to paint it
                qreal left = (qreal)(cellWidth*(j+1)  - cellWidth); // margin from left
                qreal top  = (qreal)(cellHeight*(i+1) - cellHeight); // margin from top
                QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                p.fillRect(r, QBrush(QColor((i*i + j*j)%color[0],(i*i * j*j)%color[1],abs(i*i - j*j)%color[2],255))); // fill cell with color
}
}


}
