#include "Map.h"

Map::Map(QObject *parent) : QObject(parent)
{

}

Map::Map(QWidget* par, int n, int m, vector <Path> p)
{
    row = n, col = m;
    data = new int* [row];
    for (int i = 0; i < row; ++i)
    {
        data[i] = new int[col];
        memset(data[i], 0, col * sizeof(int));
    }

    paths = p;

    for (size_t i = 0; i < paths.size(); ++i)
    {
        for (size_t j = 0; j < paths[i].size(); ++j)
            data[paths[i][j].first][paths[i][j].second] = 1;
    }

    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
        {
            if (data[i][j] == 0 &&
                ((i - 1 >= 0 && data[i - 1][j] == 1) ||
                 (i + 1 < row && data[i + 1][j] == 1) ||
                 (j - 1 >= 0 && data[i][j - 1] == 1) ||
                 (j + 1 < col && data[i][j + 1] == 1))
               )
            data[i][j] = 2;
        }

    parent = par;
}

Map::Map(QWidget* par, int n, int m)
{
    row = n, col = m;
    data = new int* [row];
    for (int i = 0; i < row; ++i)
    {
        data[i] = new int[col];
        memset(data[i], 0, col * sizeof(int));
    }
    parent = par;
}

Map::~Map()
{
    for (int i = 0; i < row; ++i)
    {
        delete[] data[i];
        data[i] = NULL;
    }
    delete[] data;
    data = NULL;

    for (size_t i = 0; i < paths.size(); ++i)
        paths[i].clear();
    paths.clear();

    parent = NULL;
}

void Map::addAll(vector <Path> p)
{
    paths = p;

    for (size_t i = 0; i < paths.size(); ++i)
    {
        for (size_t j = 0; j < paths[i].size(); ++j)
            data[paths[i][j].first][paths[i][j].second] = 1;
    }

    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
        {
            if (data[i][j] == 0 &&
                ((i - 1 >= 0 && data[i - 1][j] == 1) ||
                 (i + 1 < row && data[i + 1][j] == 1) ||
                 (j - 1 >= 0 && data[i][j - 1] == 1) ||
                 (j + 1 < col && data[i][j + 1] == 1))
               )
            data[i][j] = 2;
        }
}
