#ifndef MAP_H
#define MAP_H


#pragma once
#include <QObject>
#include "Function.h"

class Map : public QObject
{
    Q_OBJECT

private:
    friend class Simulation;
    friend class Widget;
    int** data; //地图n行m列具体信息：0表示空地，1表示敌人路径
    int row, col;

    vector <Path> paths;

    QWidget* parent;

public:
    explicit Map(QObject *parent = nullptr);
    Map(QWidget* par, int n, int m, vector <Path> p);
    Map(QWidget* par, int n, int m);
    ~Map();
    void addAll(vector <Path> p);
    int*& operator[](int i) { return *(data + i); };

signals:

};

#endif // MAP_H
