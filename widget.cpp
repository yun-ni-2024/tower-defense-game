#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);

    fileRead();
    catalogueInit();
    editWindowInit();
    setUpInit();
    extenInit();

    bgm = new QSoundEffect(this);
    bgm->setSource(QUrl::fromLocalFile(":/resource/TheDawn.wav"));
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->play();

    bgm1 = new QSoundEffect(this);
    bgm1->setSource(QUrl::fromLocalFile(":/resource/GetUp.wav"));
    bgm2 = new QSoundEffect(this);
    bgm2->setSource(QUrl::fromLocalFile(":/resource/MistyMoon.wav"));

    sound1 = new QSoundEffect(this);
    sound1->setSource(QUrl::fromLocalFile(":/resource/melee.wav"));
    sound2 = new QSoundEffect(this);
    sound2->setSource(QUrl::fromLocalFile(":/resource/archer.wav"));

    QLabel* homepage = new QLabel(this);
    homepage->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);
    homepage->setPixmap(QPixmap(":/resource/homepage.jpg"));
    homepage->move(0, 0);
    homepage->show();

    QLabel* title = new QLabel(homepage);
    title->setFixedSize(200, 50);
    title->setText("塔防游戏");
    title->setAlignment(Qt::AlignCenter);
    QFont ft;
    ft.setPixelSize(50);
    title->setFont(ft);
    title->move((DISPLAY_WID + STATUS_WID - 200) / 2, 150);
    title->show();

    QPushButton* Start = new QPushButton(homepage);
    Start->setFixedSize(200, 50);
    Start->setText("开始");
    Start->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 120) / 2);
    Start->show();

    connect(Start, &QPushButton::clicked, this, [=](){
        Catalogue->show();
        Catalogue->raise();
    });

    QPushButton* Edit = new QPushButton(homepage);
    Edit->setFixedSize(200, 50);
    Edit->setText("编辑地图");
    Edit->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 120) / 2 + 70);
    Edit->show();

    connect(Edit, &QPushButton::clicked, this, [=](){
        bgm->stop();
        bgm1->setLoopCount(QSoundEffect::Infinite);
        bgm1->play();

        EditWindow->show();
        EditWindow->raise();
    });

    QPushButton* SetUp = new QPushButton(homepage);
    SetUp->setFixedSize(200, 50);
    SetUp->setText("设置");
    SetUp->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 120) / 2 + 70 * 2);
    SetUp->show();

    connect(SetUp, &QPushButton::clicked, this, [=](){
        SetUpWindow->show();
        SetUpWindow->raise();
    });

    QPushButton* Exten = new QPushButton(homepage);
    Exten->setFixedSize(200, 50);
    Exten->setText("拓展部分");
    Exten->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 120) / 2 + 70 * 3);
    Exten->show();

    connect(Exten, &QPushButton::clicked, this, [=](){
        ExtenWindow->show();
        ExtenWindow->raise();
    });

    QPushButton* Quit = new QPushButton(homepage);
    Quit->setFixedSize(200, 50);
    Quit->setText("退出");
    Quit->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 120) / 2 + 70 * 4);
    Quit->show();

    connect(Quit, &QPushButton::clicked, this, [=](){
        this->close();
    });
}

Widget::~Widget()
{
    delete ui;

    fileWrite();

    for (Map* map: maps)
    {
        delete map;
        map = NULL;
    }
    maps.clear();

    for (int i = 0; i < N; ++i)
    {
        delete[] editData[i];
        editData[i] = NULL;
    }
    delete[] editData;
    editData = NULL;
}

void Widget::fileRead()
{
    qDebug() << "Readling file";

    ifstream in_file("map.txt", ios::in);
    if(!in_file)
    {
        ENEMY_NUM = 20;
        AFFIX1_PROB = 5;
        AFFIX2_PROB = 5;
        AFFIX3_PROB = 5;
        INIT_AFFIX = 0;
        qDebug() << "Readling empty file over";
        return;
    }

    string ss;
    in_file >> ss >> ss >> ss >> ENEMY_NUM;
    in_file >> ss >> ss >> AFFIX1_PROB;
    in_file >> ss >> ss >> AFFIX2_PROB;
    in_file >> ss >> ss >> AFFIX3_PROB;
    in_file >> ss >> ss >> INIT_AFFIX;

    int mapNum;
//    qDebug() << mapNum;
    in_file >> ss >> ss >> ss >> mapNum;
//    qDebug() << ss.length();
//    qDebug() << mapNum;
    for (int i = 0; i < mapNum; ++i)
    {
        int pathNum;
        string mapName;
        in_file >> ss >> mapName >> ss >> ss >> ss >> pathNum;
        name.push_back(mapName);
        vector <Path> paths;
        for (int j = 0; j < pathNum; ++j)
        {
            int len;
            in_file >> ss >> len;
            Path p;
            for (int k = 0; k < len; ++k)
            {
                int x, y;
                in_file >> ss >> ss >> x >> ss >> y;
                p.push_back(make_pair(x, y));
            }
            in_file >> ss;
            paths.push_back(p);
        }
        Map* map = new Map(this, N, M, paths);
        maps.push_back(map);
    }

    qDebug() << "Readling file over";
}

void Widget::fileWrite()
{
    ofstream out_file("map.txt", ios::out);
    if (!out_file) exit(-1);
    out_file << "number of enemies: " << ENEMY_NUM << endl;
    out_file << "affix1 probability: " << AFFIX1_PROB << endl;
    out_file << "affix2 probability: " << AFFIX2_PROB << endl;
    out_file << "affix3 probability: " << AFFIX3_PROB << endl;
    out_file << "initial affix: " << INIT_AFFIX << endl << endl;

    out_file << "number of maps: " << maps.size() << endl << endl;
    for (size_t i = 0; i < maps.size(); ++i)
    {
        out_file << "map" << i + 1 << ": " << name[i] << endl;
        out_file << "number of paths: " << maps[i]->paths.size() << endl;
        for (size_t j = 0; j < maps[i]->paths.size(); ++j)
        {
            out_file << "path" << j + 1 << "(length= " << maps[i]->paths[j].size() << " ): ";
            for (Coor pos: maps[i]->paths[j])
                out_file << "( " << pos.first << " , " << pos.second << " ) ";
            out_file << endl;
        }
        out_file << endl;
    }
}

void Widget::catalogueInit()
{
    Catalogue = new QLabel(this);
    Catalogue->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);
    Catalogue->setPixmap(QPixmap(":/resource/catalogue.jpg"));
    Catalogue->move(0, 0);
    Catalogue->close();

    const int totRow = (maps.size() + 4) / 5;
    for (size_t i = 0; i < maps.size(); ++i)
    {
        const int curRow = (i + 1 + 4) / 5, curCol = (i + 1) - 5 * (curRow - 1);
        const int totCol = (curRow == totRow) ? maps.size() - 5 * (totRow - 1) : 5;
        QPushButton* chs = new QPushButton(Catalogue);
        chs->setText(QString::fromStdString(name[i]));
        const int len = 70, inr = 20;
        chs->setFixedSize(len, len);
        const int x = (DISPLAY_WID + STATUS_WID - totCol * (len + inr) + inr) / 2 + (curCol - 1) * (len + inr);
        const int y = (DISPLAY_HEI - totRow * (len + inr) - inr) / 2 + (curRow - 1) * (len + inr);
        chs->move(x, y + 20);

        connect(chs, &QPushButton::clicked, this, [=](){
            qDebug() << "start load";

            bgm->stop();
            bgm2->setLoopCount(QSoundEffect::Infinite);
            bgm2->play();

            load(maps[i]);
        });
        connect(chs, &QPushButton::clicked, Catalogue, &QPushButton::close);
    }

    QPushButton* ret = new QPushButton(Catalogue);
    ret->setFixedSize(80, 25);
    ret->setText("返回");
    ret->move(15, 15);
    ret->show();
    connect(ret, &QPushButton::clicked, [=]()mutable{
        Catalogue->close();
    });
}

void Widget::catalogueUpdate()
{
    delete Catalogue;
    catalogueInit();
}

void Widget::editWindowInit()
{
    EditWindow = new QLabel(this);
    EditWindow->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);
    EditWindow->setPixmap(QPixmap(":/resource/editwindow.jpg"));
    EditWindow->move(0, 0);
    EditWindow->close();

    QLabel* label = new QLabel(EditWindow);
    label->setFixedSize(80, 25);
    label->setText("地图名称:");
    label->move(DISPLAY_WID, 25);
    label->show();

    QTextEdit* text = new QTextEdit(EditWindow);
    text->setFixedSize(80, 27);
    text->setText("地图" + QString::number(maps.size() + 1, 10));
    text->move(DISPLAY_WID, 50);

    editData = new int* [N];
    for (int i = 0; i < N; ++i)
    {
        editData[i] = new int[M];
        memset(editData[i], 0, M * sizeof(int));
    }
    QPushButton*** land = new QPushButton** [N];
    for (int i = 0; i < N; ++i)
    {
        land[i] = new QPushButton* [M];
        for (int j = 0; j < M; ++j)
        {
            land[i][j] = new QPushButton(EditWindow);
            land[i][j]->setFixedSize(LEN, LEN);
            land[i][j]->setIcon(MAP_BLANK_GRAPH);
            land[i][j]->setIconSize(QSize(LEN - 1, LEN - 1));
            land[i][j]->move(LEN * i + OFFSET, LEN * j + OFFSET);
            land[i][j]->show();
        }
    }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            connect(land[i][j], &QPushButton::clicked, [=]()mutable{
                if (tmpPath.size() && abs(i - tmpPath[tmpPath.size() - 1].first) + abs(j - tmpPath[tmpPath.size() - 1].second) != 1)
                    return;

                tmpPath.push_back(make_pair(i, j));

                editData[i][j] = 1;
                land[i][j]->setIcon(MAP_PATH_GRAPH);

                int d[5] = { 0, 1, 0, -1, 0 };
                for (int k = 0; k < 4; ++k)
                {
                    int x = i + d[k], y = j + d[k + 1];
                    if (x >= 0 && x < N && y >= 0 && y < M && editData[x][y] == 0)
                    {
                        editData[x][y] = 2;
                        land[x][y]->setIcon(MAP_SIDE_GRAPH);
                    }
                }
            });

    QPushButton* addPath = new QPushButton(EditWindow);
    addPath->setFixedSize(80, 25);
    addPath->setText("添加路径");
    addPath->move(DISPLAY_WID, 96);
    addPath->show();
    connect(addPath, &QPushButton::clicked, [=]()mutable{
        if (tmpPath.size() == 0)
            return;

        tmpPaths.push_back(tmpPath);
        tmpPath.clear();
    });

    Map* map = NULL;
    QPushButton* createMap = new QPushButton(EditWindow);
    createMap->setFixedSize(80, 25);
    createMap->setText("创建地图");
    createMap->move(DISPLAY_WID, 141);
    createMap->show();
    connect(createMap, &QPushButton::clicked, [=]()mutable{
        map = new Map(this, N, M, tmpPaths);
        tmpPaths.clear();
        maps.push_back(map);
        name.push_back(text->toPlainText().toStdString());
        QMessageBox::information(this, "提示", "创建成功");

        text->setText("地图" + QString::number(maps.size() + 1, 10));
        map = NULL;
        for (int i = 0; i < N; ++i)
            memset(editData[i], 0, M * sizeof(int));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                land[i][j]->setIcon(MAP_BLANK_GRAPH);

        catalogueUpdate();
    });

    QPushButton* ret = new QPushButton(EditWindow);
    ret->setFixedSize(80, 25);
    ret->setText("返回");
    ret->move(DISPLAY_WID, 465);
    ret->show();
    connect(ret, &QPushButton::clicked, [=]()mutable{
        tmpPath.clear();
        tmpPaths.clear();

        for (int i = 0; i < N; ++i)
            memset(editData[i], 0, M * sizeof(int));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                land[i][j]->setIcon(MAP_BLANK_GRAPH);

        EditWindow->close();

        bgm1->stop();
        bgm->setLoopCount(QSoundEffect::Infinite);
        bgm->play();
    });
}

void Widget::setUpInit()
{
    const int X = 500, Y = 400;
    SetUpWindow = new QLabel(this);
    SetUpWindow->setFixedSize(X, Y);
    SetUpWindow->move((DISPLAY_WID + STATUS_WID - X) / 2, (DISPLAY_HEI - Y) / 2);
    QString style = "QLabel {\
        border-radius: 20px;\
        background-color:#f0f8ff;\
        border:5px solid #f0f0f0;\
        }";
    SetUpWindow->setStyleSheet(style);
    SetUpWindow->close();

    QString style1 = "QLabel {\
        border-radius: 0px;\
        background-color:#f0f8ff;\
        border:0px;\
        }";
    QString style2 = "QLabel {\
        border-radius: 3px;\
        background-color:#f0f8ff;\
        border:5px solid #f0f0f0;\
        }";


    QLabel* item1 = new QLabel(SetUpWindow);
    item1->setText("敌人数量");
    item1->setFixedSize(150, 25);
    item1->move(25, 25);
    item1->setStyleSheet(style1);
    item1->show();

    QLabel* data1 = new QLabel(SetUpWindow);
    data1->setText(QString::number(ENEMY_NUM, 10));
    data1->setFixedSize(125, 25);
    data1->move(200, 25);
    data1->setStyleSheet(style2);
    data1->setAlignment(Qt::AlignCenter);
    data1->show();

    QPushButton* dec1 = new QPushButton(SetUpWindow);
    dec1->setFixedSize(25, 25);
    dec1->setText("-");
    dec1->move(375, 25);
    dec1->show();

    connect(dec1, &QPushButton::clicked, this, [=]()mutable{
        if (ENEMY_NUM >= 20) ENEMY_NUM -= 10;
        else if (ENEMY_NUM >= 2) ENEMY_NUM--;
        data1->setText(QString::number(ENEMY_NUM, 10));
    });

    QPushButton* inc1 = new QPushButton(SetUpWindow);
    inc1->setFixedSize(25, 25);
    inc1->setText("+");
    inc1->move(425, 25);
    inc1->show();

    connect(inc1, &QPushButton::clicked, this, [=]()mutable{
        if (ENEMY_NUM < 10) ENEMY_NUM++;
        else if (ENEMY_NUM <= 499) ENEMY_NUM += 10;
        data1->setText(QString::number(ENEMY_NUM, 10));
    });


    QLabel* item2 = new QLabel(SetUpWindow);
    item2->setText("[闪现的]词缀出现概率");
    item2->setFixedSize(150, 25);
    item2->move(25, 25 * 3);
    item2->setStyleSheet(style1);
    item2->show();

    QLabel* data2 = new QLabel(SetUpWindow);
    data2->setText(QString::number(AFFIX1_PROB, 10));
    data2->setFixedSize(125, 25);
    data2->move(200, 25 * 3);
    data2->setStyleSheet(style2);
    data2->setAlignment(Qt::AlignCenter);
    data2->show();

    QPushButton* dec2 = new QPushButton(SetUpWindow);
    dec2->setFixedSize(25, 25);
    dec2->setText("-");
    dec2->move(375, 25 * 3);
    dec2->show();

    connect(dec2, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX1_PROB >= 20) AFFIX1_PROB -= 10;
        else if (AFFIX1_PROB >= 1) AFFIX1_PROB--;
        data2->setText(QString::number(AFFIX1_PROB, 10));
    });

    QPushButton* inc2 = new QPushButton(SetUpWindow);
    inc2->setFixedSize(25, 25);
    inc2->setText("+");
    inc2->move(425, 25 * 3);
    inc2->show();

    connect(inc2, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX1_PROB < 10) AFFIX1_PROB++;
        else if (AFFIX1_PROB <= 90) AFFIX1_PROB += 10;
        data2->setText(QString::number(AFFIX1_PROB, 10));
    });


    QLabel* item3 = new QLabel(SetUpWindow);
    item3->setText("[神速的]词缀出现概率");
    item3->setFixedSize(150, 25);
    item3->move(25, 25 * 5);
    item3->setStyleSheet(style1);
    item3->show();

    QLabel* data3 = new QLabel(SetUpWindow);
    data3->setText(QString::number(AFFIX2_PROB, 10));
    data3->setFixedSize(125, 25);
    data3->move(200, 25 * 5);
    data3->setStyleSheet(style2);
    data3->setAlignment(Qt::AlignCenter);
    data3->show();

    QPushButton* dec3 = new QPushButton(SetUpWindow);
    dec3->setFixedSize(25, 25);
    dec3->setText("-");
    dec3->move(375, 25 * 5);
    dec3->show();

    connect(dec3, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX2_PROB >= 20) AFFIX2_PROB -= 10;
        else if (AFFIX2_PROB >= 1) AFFIX2_PROB--;
        data3->setText(QString::number(AFFIX2_PROB, 10));
    });

    QPushButton* inc3 = new QPushButton(SetUpWindow);
    inc3->setFixedSize(25, 25);
    inc3->setText("+");
    inc3->move(425, 25 * 5);
    inc3->show();

    connect(inc3, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX2_PROB < 10) AFFIX2_PROB++;
        else if (AFFIX2_PROB <= 90) AFFIX2_PROB += 10;
        data3->setText(QString::number(AFFIX2_PROB, 10));
    });


    QLabel* item4 = new QLabel(SetUpWindow);
    item4->setText("[远程的]词缀出现概率");
    item4->setFixedSize(150, 25);
    item4->move(25, 25 * 7);
    item4->setStyleSheet(style1);
    item4->show();

    QLabel* data4 = new QLabel(SetUpWindow);
    data4->setText(QString::number(AFFIX3_PROB, 10));
    data4->setFixedSize(125, 25);
    data4->move(200, 25 * 7);
    data4->setStyleSheet(style2);
    data4->setAlignment(Qt::AlignCenter);
    data4->show();

    QPushButton* dec4 = new QPushButton(SetUpWindow);
    dec4->setFixedSize(25, 25);
    dec4->setText("-");
    dec4->move(375, 25 * 7);
    dec4->show();

    connect(dec4, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX3_PROB >= 20) AFFIX3_PROB -= 10;
        else if (AFFIX3_PROB >= 1) AFFIX3_PROB--;
        data4->setText(QString::number(AFFIX3_PROB, 10));
    });

    QPushButton* inc4 = new QPushButton(SetUpWindow);
    inc4->setFixedSize(25, 25);
    inc4->setText("+");
    inc4->move(425, 25 * 7);
    inc4->show();

    connect(inc4, &QPushButton::clicked, this, [=]()mutable{
        if (AFFIX3_PROB < 10) AFFIX3_PROB++;
        else if (AFFIX3_PROB <= 90) AFFIX3_PROB += 10;
        data4->setText(QString::number(AFFIX3_PROB, 10));
    });


    QLabel* item5 = new QLabel(SetUpWindow);
    item5->setText("初始词缀数量");
    item5->setFixedSize(150, 25);
    item5->move(25, 25 * 9);
    item5->setStyleSheet(style1);
    item5->show();

    QLabel* data5 = new QLabel(SetUpWindow);
    data5->setText(QString::number(INIT_AFFIX, 10));
    data5->setFixedSize(125, 25);
    data5->move(200, 25 * 9);
    data5->setStyleSheet(style2);
    data5->setAlignment(Qt::AlignCenter);
    data5->show();

    QPushButton* dec5 = new QPushButton(SetUpWindow);
    dec5->setFixedSize(25, 25);
    dec5->setText("-");
    dec5->move(375, 25 * 9);
    dec5->show();

    connect(dec5, &QPushButton::clicked, this, [=]()mutable{
        if (INIT_AFFIX >= 20) INIT_AFFIX -= 10;
        else if (INIT_AFFIX >= 1) INIT_AFFIX--;
        data5->setText(QString::number(INIT_AFFIX, 10));
    });

    QPushButton* inc5 = new QPushButton(SetUpWindow);
    inc5->setFixedSize(25, 25);
    inc5->setText("+");
    inc5->move(425, 25 * 9);
    inc5->show();

    connect(inc5, &QPushButton::clicked, this, [=]()mutable{
        if (INIT_AFFIX < 10) INIT_AFFIX++;
        else if (INIT_AFFIX <= 90) INIT_AFFIX += 10;
        data5->setText(QString::number(INIT_AFFIX, 10));
    });


    QPushButton* ret = new QPushButton(SetUpWindow);
    ret->setFixedSize(80, 25);
    ret->setText("返回");
    ret->move((X - 80) / 2, Y - 50);
    ret->show();

    connect(ret, &QPushButton::clicked, this, [=](){
        SetUpWindow->close();
    });
}

void Widget::extenInit()
{
    const int X = DISPLAY_WID + STATUS_WID - 50, Y = DISPLAY_HEI - 50;
    ExtenWindow = new QLabel(this);
    ExtenWindow->setFixedSize(X, Y);
    QString style = "QLabel {\
        border-radius: 20px;\
        background-color:#f0f8ff;\
        border:5px solid #f0f0f0;\
        }";
    ExtenWindow->setStyleSheet(style);
    QFont ft;
    ft.setPixelSize(25);
    ExtenWindow->setFont(ft);
    ExtenWindow->move(25, 25);
    ExtenWindow->close();

    ExtenWindow->setText("\
        拓展部分介绍：\n\
        ————————————————————————————————\n\
        游戏资源设定部分：\n\
        ·增加了自定义参数功能\n\
        ————————————————————————————————\n\
        游戏内容部分：\n\
        ·增加了5种词缀\n\
        ·增加了防御塔的拆除操作及相应资源设定\n\
        ————————————————————————————————\n\
        游戏音画部分：\n\
        ·增加了各种攻击特效\n\
        ·增加了各种负面状态特效\n\
        ·增加了背景音乐和攻击的音效\n\
        ————————————————————————————————\n\
        其它部分：\n\
        ·增加了地图编辑功能\n\
    ");

    QPushButton* ret = new QPushButton(ExtenWindow);
    ret->setFixedSize(200, 50);
    ret->setText("返回");
    ret->move((X - 200) / 2, Y - 80);
    ret->show();

    connect(ret, &QPushButton::clicked, this, [=](){
        ExtenWindow->close();
    });
}

void Widget::load(Map* map)
{
    qDebug() << "load S";
    S = new Simulation(this, map);

    qDebug() << "load bg";

    background = new QLabel(this);
    background->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_WID);
    background->setPixmap(QPixmap(":/resource/background.jpg"));
    background->move(0, 0);
    background->show();

//    display = new QPushButton(background);
//    display->setFixedSize(DISPLAY_LEN, DISPLAY_LEN);
//    display->setIcon(QIcon(":/background.ico"));
//    display->setIconSize(QSize(DISPLAY_LEN, DISPLAY_LEN));
//    display->move(0, 0);
//    display->show();

    qDebug() << "load map";

    graph = new QPushButton** [map->row];
    for (int i = 0; i < map->row; ++i)
    {
        graph[i] = new QPushButton* [map->col];
        for (int j = 0; j < map->col; ++j)
        {
            graph[i][j] = new QPushButton(background);
            graph[i][j]->setFixedSize(LEN, LEN);
            graph[i][j]->move(LEN * i + OFFSET, LEN * j + OFFSET);
            switch((*map)[i][j])
            {
            case 0:
                graph[i][j]->setIcon(MAP_BLANK_GRAPH);
                graph[i][j]->setIconSize(QSize(LEN, LEN));
                break;
            case 1:
                graph[i][j]->setIcon(MAP_PATH_GRAPH);
                graph[i][j]->setIconSize(QSize(LEN, LEN));
                break;
            case 2:
                graph[i][j]->setIcon(MAP_SIDE_GRAPH);
                graph[i][j]->setIconSize(QSize(LEN, LEN));
                break;
            default:
                break;
            }
            graph[i][j]->show();
        }
    }

    qDebug() << "load status";

    chooseBoard = new QPushButton(background);
    chooseBoard->close();

    for (int i = 0; i < map->row; ++i)
        for (int j = 0; j < map->col; ++j)
        {
            switch((*map)[i][j])
            {
            case 1:
            {
                connect(graph[i][j], &QPushButton::clicked, this, [=]()mutable{
                    delete chooseBoard;
                    chooseBoard = new QPushButton(background);
                    chooseBoard->setFixedSize(CHOOSE_BUTTON_LEN * 2, CHOOSE_BUTTON_LEN);
                    chooseBoard->move(LEN * i - (CHOOSE_BUTTON_LEN - LEN) / 2 + OFFSET, LEN * j - (CHOOSE_BUTTON_LEN - LEN) / 2 + OFFSET);
                    chooseBoard->raise();
                    chooseBoard->show();

                    QPushButton* opt = new QPushButton(chooseBoard);
                    opt->setFixedSize(CHOOSE_BUTTON_LEN, CHOOSE_BUTTON_LEN);
                    opt->setText("放置\n近战塔\n$500");
                    opt->move(0, 0);
                    opt->show();
                    connect(opt, &QPushButton::clicked, this, [=]()mutable{
                        S->meleeClicked(i, j);
                        chooseBoard->close();
                    });

                    QPushButton* can = new QPushButton(chooseBoard);
                    can->setFixedSize(CHOOSE_BUTTON_LEN, CHOOSE_BUTTON_LEN);
                    can->setText("取消");
                    can->move(CHOOSE_BUTTON_LEN, 0);
                    can->show();
                    connect(can, &QPushButton::clicked, this, [=](){
                        chooseBoard->close();
                    });
                });
                break;
            }
            case 2:
            {
                connect(graph[i][j], &QPushButton::clicked, this, [=]()mutable{
                    delete chooseBoard;
                    chooseBoard = new QPushButton(background);
                    chooseBoard->setFixedSize(CHOOSE_BUTTON_LEN * 2, CHOOSE_BUTTON_LEN);
                    chooseBoard->move(LEN * i - (CHOOSE_BUTTON_LEN - LEN / 2) + OFFSET, LEN * j - (CHOOSE_BUTTON_LEN / 2 - LEN / 2) + OFFSET);
                    chooseBoard->raise();
                    chooseBoard->show();

                    QPushButton* opt = new QPushButton(chooseBoard);
                    opt->setFixedSize(CHOOSE_BUTTON_LEN, CHOOSE_BUTTON_LEN);
                    opt->setText("放置\n远程塔\n$1000");
                    opt->move(0, 0);
                    opt->show();
                    connect(opt, &QPushButton::clicked, this, [=](){
                        S->archerClicked(i, j);
                        chooseBoard->close();
                    });

                    QPushButton* can = new QPushButton(chooseBoard);
                    can->setFixedSize(CHOOSE_BUTTON_LEN, CHOOSE_BUTTON_LEN);
                    can->setText("取消");
                    can->move(CHOOSE_BUTTON_LEN, 0);
                    can->show();
                    connect(can, &QPushButton::clicked, this, [=](){
                        chooseBoard->close();
                    });
                });
            }
            default:
                break;
            }
        }

//    QPushButton* status = new QPushButton(background);
//    status->setFixedSize(STATUS_WID, DISPLAY_WID);
//    status->setDisabled(true);
//    status->move(DISPLAY_WID, 0);
//    status->show();

    messageBoard = new QPushButton(background);
    messageBoard->setFixedSize(200, 50);
    messageBoard->setDisabled(true);
    messageBoard->move((DISPLAY_WID - 200) / 2, 35);
    messageBoard->close();

    moneyIcon = new QPushButton(background);
    moneyIcon->setFixedSize(80, 25);
    moneyIcon->setIcon(MONEY_GRAPH);
    moneyIcon->setDisabled(true);
    moneyIcon->setText("2000");
    moneyIcon->move(DISPLAY_WID, 25);
    moneyIcon->show();

    heartIcon = new QPushButton(background);
    heartIcon->setFixedSize(80, 25);
    heartIcon->setIcon(HEART_GRAPH);
    heartIcon->setDisabled(true);
    heartIcon->setText("3");
    heartIcon->move(DISPLAY_WID, 70);
    heartIcon->show();

    affixIcon[1] = new QPushButton(background);
    affixIcon[1]->setFixedSize(90, 25);
    affixIcon[1]->setDisabled(true);
    affixIcon[1]->setText("[狂暴的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[1]->move(DISPLAY_WID, 200);
    affixIcon[1]->show();
    affixIcon[2] = new QPushButton(background);
    affixIcon[2]->setFixedSize(90, 25);
    affixIcon[2]->setDisabled(true);
    affixIcon[2]->setText("[冰系的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[2]->move(DISPLAY_WID, 240);
    affixIcon[2]->show();
    affixIcon[3] = new QPushButton(background);
    affixIcon[3]->setFixedSize(90, 25);
    affixIcon[3]->setDisabled(true);
    affixIcon[3]->setText("[群伤的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[3]->move(DISPLAY_WID, 280);
    affixIcon[3]->show();
    affixIcon[4] = new QPushButton(background);
    affixIcon[4]->setFixedSize(90, 25);
    affixIcon[4]->setDisabled(true);
    affixIcon[4]->setText("[放血的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[4]->move(DISPLAY_WID, 320);
    affixIcon[4]->show();
    affixIcon[5] = new QPushButton(background);
    affixIcon[5]->setFixedSize(90, 25);
    affixIcon[5]->setDisabled(true);
    affixIcon[5]->setText("[隐匿的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[5]->move(DISPLAY_WID, 360);
    affixIcon[5]->show();
    affixIcon[6] = new QPushButton(background);
    affixIcon[6]->setFixedSize(90, 25);
    affixIcon[6]->setDisabled(true);
    affixIcon[6]->setText("[迟缓的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[6]->move(DISPLAY_WID, 400);
    affixIcon[6]->show();
    affixIcon[7] = new QPushButton(background);
    affixIcon[7]->setFixedSize(90, 25);
    affixIcon[7]->setDisabled(true);
    affixIcon[7]->setText("[自愈的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[7]->move(DISPLAY_WID, 440);
    affixIcon[7]->show();
    affixIcon[8] = new QPushButton(background);
    affixIcon[8]->setFixedSize(90, 25);
    affixIcon[8]->setDisabled(true);
    affixIcon[8]->setText("[必杀的]" + QString::number(INIT_AFFIX, 10));
    affixIcon[8]->move(DISPLAY_WID, 480);
    affixIcon[8]->show();

    QPushButton* start = new QPushButton(background);
    start->setFixedSize(DISPLAY_WID, DISPLAY_HEI);
    start->setText("点击任意位置开始游戏");
    start->setFlat(true);
    start->move(0, 0);
    start->show();
    connect(start, &QPushButton::clicked, S, &Simulation::Start);
    connect(start, &QPushButton::clicked, start, &QPushButton::close);

    connect(S, &Simulation::createEnemy, this, &Widget::createEnemy);
    connect(S, &Simulation::enemyMoveTo, this, &Widget::enemyMoveTo);
    connect(S, &Simulation::createMelee, this, &Widget::createMelee);
    connect(S, &Simulation::createArcher, this, &Widget::createArcher);
    connect(S, &Simulation::eliminateDefense, this, &Widget::eliminateDefense);
    connect(S, &Simulation::eliminateEnemy, this, &Widget::eliminateEnemy);

    connect(S, &Simulation::createDefenseBullet, this, &Widget::createDefenseBullet);
    connect(S, &Simulation::defenseBulletMoveTo, this, &Widget::defenseBulletMoveTo);
    connect(S, &Simulation::eliminateDefenseBullet, this, &Widget::eliminateDefenseBullet);

    connect(S, &Simulation::createDefenseClaw, this, &Widget::createDefenseClaw);
    connect(S, &Simulation::defenseClawResize, this, &Widget::defenseClawResize);
    connect(S, &Simulation::eliminateDefenseClaw, this, &Widget::eliminateDefenseClaw);

    connect(S, &Simulation::createEnemyBullet, this, &Widget::createEnemyBullet);
    connect(S, &Simulation::enemyBulletMoveTo, this, &Widget::enemyBulletMoveTo);
    connect(S, &Simulation::eliminateEnemyBullet, this, &Widget::eliminateEnemyBullet);

    connect(S, &Simulation::createEnemyClaw, this, &Widget::createEnemyClaw);
    connect(S, &Simulation::enemyClawResize, this, &Widget::enemyClawResize);
    connect(S, &Simulation::eliminateEnemyClaw, this, &Widget::eliminateEnemyClaw);

    connect(S, &Simulation::installAffix, this, &Widget::installAffix);
    connect(S, &Simulation::removeAffix, this, &Widget::removeAffix);
    connect(S, &Simulation::updateEnemyHp, this, &Widget::updateEnemyHp);
    connect(S, &Simulation::updateDefenseHp, this, &Widget::updateDefenseHp);
    connect(S, &Simulation::updateMoney, this, &Widget::updateMoney);
    connect(S, &Simulation::moneyNotEnough, this, &Widget::moneyNotEnough);
    connect(S, &Simulation::affixNotEnough, this, &Widget::affixNotEnough);
    connect(S, &Simulation::alreadyInstalled, this, &Widget::alreadyInstalled);
    connect(S, &Simulation::blocked, this, &Widget::blocked);
    connect(S, &Simulation::closeMessage, this, &Widget::closeMessage);
    connect(S, &Simulation::updateHeart, this, &Widget::updateHeart);
    connect(S, &Simulation::updateAffix, this, &Widget::updateAffix);
    connect(S, &Simulation::freeze, this, &Widget::freeze);
    connect(S, &Simulation::unfreeze, this, &Widget::unfreeze);
    connect(S, &Simulation::bleed, this, &Widget::bleed);
    connect(S, &Simulation::unbleed, this, &Widget::unbleed);
    connect(S, &Simulation::slow, this, &Widget::slow);
    connect(S, &Simulation::unslow, this, &Widget::unslow);
    connect(S, &Simulation::makeSound, this, &Widget::makeSound);
    connect(S, &Simulation::lose, this, &Widget::lose);
    connect(S, &Simulation::win, this, &Widget::win);
    connect(S, &Simulation::remake, this, &Widget::remake);

    enemyGraph = new QPushButton* [MAX_ENEMY_NUM];
    defenseGraph = new QPushButton* [MAX_DEFENSE_NUM];
    enemyHpGraph = new QPushButton* [MAX_ENEMY_NUM];
    defenseHpGraph = new QPushButton* [MAX_DEFENSE_NUM];
    defenseBulletGraph = new QPushButton** [MAX_DEFENSE_NUM];
    for (int i = 0; i < MAX_DEFENSE_NUM; ++i)
        defenseBulletGraph[i] = new QPushButton* [MAX_BULLET_NUM];
    defenseClawGraph = new QPushButton** [MAX_DEFENSE_NUM];
    for (int i = 0; i < MAX_DEFENSE_NUM; ++i)
        defenseClawGraph[i] = new QPushButton* [MAX_CLAW_NUM];
    enemyBulletGraph = new QPushButton** [MAX_ENEMY_NUM];
    for (int i = 0; i < MAX_ENEMY_NUM; ++i)
        enemyBulletGraph[i] = new QPushButton* [MAX_BULLET_NUM];
    enemyClawGraph = new QPushButton** [MAX_ENEMY_NUM];
    for (int i = 0; i < MAX_ENEMY_NUM; ++i)
        enemyClawGraph[i] = new QPushButton* [MAX_CLAW_NUM];
    bloodGraph = new QPushButton* [MAX_ENEMY_NUM];
    slowGraph = new QPushButton* [MAX_ENEMY_NUM];
    aff[0] = new QPushButton* [MAX_DEFENSE_NUM];
    aff[1] = new QPushButton* [MAX_DEFENSE_NUM];
}

void Widget::createEnemy(int num, Coor pos, int affix)
{
    enemyGraph[num] = new QPushButton(background);
    enemyGraph[num]->setFixedSize(ENEMY_LEN, ENEMY_LEN);
    if (affix / 4 % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH3);
    else if (affix / 2 % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH2);
    else if (affix % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH1);
    else
        enemyGraph[num]->setIcon(ENEMY_GRAPH);

    enemyGraph[num]->setIconSize(QSize(ENEMY_LEN, ENEMY_LEN));
    enemyGraph[num]->move(pos.first + ENEMY_OFFSET, pos.second + ENEMY_OFFSET);
    enemyGraph[num]->show();

    enemyHpGraph[num] = new QPushButton(enemyGraph[num]);
    QString style = "QPushButton {\
            background-color:#ff0000;\
        }";
    enemyHpGraph[num]->setStyleSheet(style);
    enemyHpGraph[num]->resize(ENEMY_LEN, 5);
    enemyHpGraph[num]->move(0, 0);
    enemyHpGraph[num]->show();

    bloodGraph[num] = new QPushButton(enemyGraph[num]);
    bloodGraph[num]->setFixedSize(20, 20);
    bloodGraph[num]->setIcon(BLOOD_GRAPH);
    bloodGraph[num]->move(0, 5);
    bloodGraph[num]->close();

    slowGraph[num] = new QPushButton(enemyGraph[num]);
    slowGraph[num]->setFixedSize(20, 20);
    slowGraph[num]->setIcon(SLOW_GRAPH);
    slowGraph[num]->move(ENEMY_LEN - 20, 5);
    slowGraph[num]->close();

//    qDebug() << affix;
    const int AFFIX_LEN = 20;
    QFont ft;
    ft.setPixelSize(12);
    if (affix % 2)
    {
        QPushButton* affix = new QPushButton(enemyGraph[num]);
        affix->setFixedSize(AFFIX_LEN, AFFIX_LEN);
        affix->setText("闪");
        affix->setFont(ft);
        affix->setDisabled(true);
        affix->move(0, ENEMY_LEN - AFFIX_LEN);
        affix->show();
    }
    if (affix / 2 % 2)
    {
        QPushButton* affix = new QPushButton(enemyGraph[num]);
        affix->setFixedSize(AFFIX_LEN, AFFIX_LEN);
        affix->setText("速");
        affix->setFont(ft);
        affix->setDisabled(true);
        affix->move(ENEMY_LEN - AFFIX_LEN, ENEMY_LEN - AFFIX_LEN);
        affix->show();
    }
    if (affix / 4 % 2)
    {
        QPushButton* affix = new QPushButton(enemyGraph[num]);
        affix->setFixedSize(AFFIX_LEN, AFFIX_LEN);
        affix->setText("远");
        affix->setFont(ft);
        affix->setDisabled(true);
        affix->move((ENEMY_LEN - AFFIX_LEN) / 2, ENEMY_LEN - AFFIX_LEN);
        affix->show();
    }

//    connect(enemyHpGraph[num], &QPushButton::clicked, [=](){ enemyMoveTo(num, make_pair(0, 0)); });
}

void Widget::enemyMoveTo(int num, Coor pos)
{
    enemyGraph[num]->move(pos.first + ENEMY_OFFSET, pos.second + ENEMY_OFFSET);
}

void Widget::createMelee(int num, Coor pos)
{
    defenseGraph[num] = new QPushButton(background);
    defenseGraph[num]->setFixedSize(DEFENSE_LEN, DEFENSE_LEN);
    defenseGraph[num]->setIcon(MELEE_GRAPH);
    defenseGraph[num]->setIconSize(QSize(DEFENSE_LEN, DEFENSE_LEN));
    defenseGraph[num]->move(pos.first + DEFENSE_OFFSET, pos.second + DEFENSE_OFFSET);
    defenseGraph[num]->show();

    defenseHpGraph[num] = new QPushButton(defenseGraph[num]);
    QString style = "QPushButton {\
            background-color:#ff0000;\
        }";
    defenseHpGraph[num]->setStyleSheet(style);
    defenseHpGraph[num]->resize(DEFENSE_LEN, 5);
    defenseHpGraph[num]->move(0, 0);
    defenseHpGraph[num]->show();

    aff[0][num] = new QPushButton(defenseGraph[num]);
    aff[0][num]->setFixedSize(25, 25);
    aff[0][num]->move(0, DEFENSE_LEN - 25);
    aff[0][num]->show();
    aff[1][num] = new QPushButton(defenseGraph[num]);
    aff[1][num]->setFixedSize(25, 25);
    aff[1][num]->move(DEFENSE_LEN - 25, DEFENSE_LEN - 25);
    aff[1][num]->show();

    for (int i = 0; i <= 1; ++i)
    {
        connect(aff[i][num], &QPushButton::clicked, this, [=]()mutable{
            delete chooseBoard;
            chooseBoard = new QPushButton(background);
            chooseBoard->resize(80, 25 * 6);
            chooseBoard->move(pos.first + DEFENSE_OFFSET + DEFENSE_LEN, pos.second + DEFENSE_OFFSET);
            chooseBoard->raise();
            chooseBoard->show();

            QPushButton* title = new QPushButton(chooseBoard);
            title->setFixedSize(80, 25);
            title->setText("安装词缀");
            title->setDisabled(true);
            title->move(0, 0);
            title->show();

            QPushButton* affix1 = new QPushButton(chooseBoard);
            affix1->setFixedSize(80, 25);
            affix1->setText("[狂暴的]");
            affix1->move(0, 25);
            affix1->show();
            connect(affix1, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 1);
                chooseBoard->close();
            });

            QPushButton* affix2 = new QPushButton(chooseBoard);
            affix2->setFixedSize(80, 25);
            affix2->setText("[冰系的]");
            affix2->move(0, 50);
            affix2->show();
            connect(affix2, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 2);
                chooseBoard->close();
            });

            QPushButton* affix3 = new QPushButton(chooseBoard);
            affix3->setFixedSize(80, 25);
            affix3->setText("[群伤的]");
            affix3->move(0, 75);
            affix3->show();
            connect(affix3, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 3);
                chooseBoard->close();
            });

            QPushButton* affix4 = new QPushButton(chooseBoard);
            affix4->setFixedSize(80, 25);
            affix4->setText("[自愈的]");
            affix4->move(0, 100);
            affix4->show();
            connect(affix4, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 7);
                chooseBoard->close();
            });

            QPushButton* can = new QPushButton(chooseBoard);
            can->setFixedSize(80, 25);
            can->setText("取消");
            can->move(0, 125);
            can->show();
            connect(can, &QPushButton::clicked, this, [=](){
                chooseBoard->close();
            });
        });
    }

    connect(defenseGraph[num], &QPushButton::clicked, this, [=]()mutable{
        delete chooseBoard;
        chooseBoard = new QPushButton(background);
        chooseBoard->resize(80, 25 * 3);
        chooseBoard->move(pos.first + DEFENSE_OFFSET + DEFENSE_LEN, pos.second + DEFENSE_OFFSET);
        chooseBoard->raise();
        chooseBoard->show();

        QPushButton* title = new QPushButton(chooseBoard);
        title->setFixedSize(80, 25);
        title->setText("拆除塔");
        title->setDisabled(true);
        title->move(0, 0);
        title->show();

        QPushButton* remove = new QPushButton(chooseBoard);
        remove->setFixedSize(80, 25);
        remove->setText("确定");
        remove->move(0, 25);
        remove->show();
        connect(remove, &QPushButton::clicked, this, [=](){
            S->removeClicked(num, 0);
            chooseBoard->close();
        });

        QPushButton* can = new QPushButton(chooseBoard);
        can->setFixedSize(80, 25);
        can->setText("取消");
        can->move(0, 50);
        can->show();
        connect(can, &QPushButton::clicked, this, [=](){
            chooseBoard->close();
        });
    });
}

void Widget::createArcher(int num, Coor pos)
{
    defenseGraph[num] = new QPushButton(background);
    defenseGraph[num]->setFixedSize(DEFENSE_LEN, DEFENSE_LEN);
    defenseGraph[num]->setIcon(ARCHER_GRAPH);
    defenseGraph[num]->setIconSize(QSize(DEFENSE_LEN, DEFENSE_LEN));
    defenseGraph[num]->move(pos.first + DEFENSE_OFFSET, pos.second + DEFENSE_OFFSET);
    defenseGraph[num]->show();

    defenseHpGraph[num] = new QPushButton(defenseGraph[num]);
    QString style = "QPushButton {\
            background-color:#ff0000;\
        }";
    defenseHpGraph[num]->setStyleSheet(style);
    defenseHpGraph[num]->resize(DEFENSE_LEN, 5);
    defenseHpGraph[num]->move(0, 0);
    defenseHpGraph[num]->show();

    aff[0][num] = new QPushButton(defenseGraph[num]);
    aff[0][num]->setFixedSize(25, 25);
    aff[0][num]->move(0, DEFENSE_LEN - 25);
    aff[0][num]->show();
    aff[1][num] = new QPushButton(defenseGraph[num]);
    aff[1][num]->setFixedSize(25, 25);
    aff[1][num]->move(DEFENSE_LEN - 25, DEFENSE_LEN - 25);
    aff[1][num]->show();

    for (int i = 0; i <= 1; ++i)
    {
        connect(aff[i][num], &QPushButton::clicked, this, [=]()mutable{
            delete chooseBoard;
            chooseBoard = new QPushButton(background);
            chooseBoard->resize(80, 25 * 6);
            chooseBoard->move(pos.first + DEFENSE_OFFSET + DEFENSE_LEN, pos.second + DEFENSE_OFFSET);
            chooseBoard->raise();
            chooseBoard->show();

            QPushButton* title = new QPushButton(chooseBoard);
            title->setFixedSize(80, 25);
            title->setText("安装词缀");
            title->setDisabled(true);
            title->move(0, 0);
            title->show();

            QPushButton* affix1 = new QPushButton(chooseBoard);
            affix1->setFixedSize(80, 25);
            affix1->setText("[放血的]");
            affix1->move(0, 25);
            affix1->show();
            connect(affix1, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 4);
                chooseBoard->close();
            });

            QPushButton* affix2 = new QPushButton(chooseBoard);
            affix2->setFixedSize(80, 25);
            affix2->setText("[隐匿的]");
            affix2->move(0, 50);
            affix2->show();
            connect(affix2, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 5);
                chooseBoard->close();
            });

            QPushButton* affix3 = new QPushButton(chooseBoard);
            affix3->setFixedSize(80, 25);
            affix3->setText("[迟缓的]");
            affix3->move(0, 75);
            affix3->show();
            connect(affix3, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 6);
                chooseBoard->close();
            });

            QPushButton* affix4 = new QPushButton(chooseBoard);
            affix4->setFixedSize(80, 25);
            affix4->setText("[必杀的]");
            affix4->move(0, 100);
            affix4->show();
            connect(affix4, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, i, 8);
                chooseBoard->close();
            });

            QPushButton* can = new QPushButton(chooseBoard);
            can->setFixedSize(80, 25);
            can->setText("取消");
            can->move(0, 125);
            can->show();
            connect(can, &QPushButton::clicked, this, [=](){
                chooseBoard->close();
            });
        });
    }

    connect(defenseGraph[num], &QPushButton::clicked, this, [=]()mutable{
        delete chooseBoard;
        chooseBoard = new QPushButton(background);
        chooseBoard->resize(80, 25 * 3);
        chooseBoard->move(pos.first + DEFENSE_OFFSET + DEFENSE_LEN, pos.second + DEFENSE_OFFSET);
        chooseBoard->raise();
        chooseBoard->show();

        QPushButton* title = new QPushButton(chooseBoard);
        title->setFixedSize(80, 25);
        title->setText("拆除塔");
        title->setDisabled(true);
        title->move(0, 0);
        title->show();

        QPushButton* remove = new QPushButton(chooseBoard);
        remove->setFixedSize(80, 25);
        remove->setText("确定");
        remove->move(0, 25);
        remove->show();
        connect(remove, &QPushButton::clicked, this, [=](){
            S->removeClicked(num, 0);
            chooseBoard->close();
        });

        QPushButton* can = new QPushButton(chooseBoard);
        can->setFixedSize(80, 25);
        can->setText("取消");
        can->move(0, 50);
        can->show();
        connect(can, &QPushButton::clicked, this, [=](){
            chooseBoard->close();
        });
    });
}

void Widget::eliminateDefense(int num)
{
    defenseGraph[num]->close();
}

void Widget::eliminateEnemy(int num)
{
    enemyGraph[num]->close();
}

void Widget::createDefenseBullet(int defenseNum, int bulletNum, Coor pos, int typ)
{
    defenseBulletGraph[defenseNum][bulletNum] = new QPushButton(background);
    QString style;
    switch(typ)
    {
    case 0:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ccff66;\
                border:1px solid #ccff00;\
                }";
        break;
    case 1:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ff0066;\
                border:1px solid #ff3366;\
                }";
        break;
    case 2:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#00ccff;\
                border:1px solid #0099ff;\
                }";
        break;
    case 3:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#cc33ff;\
                border:1px solid #cc00ff;\
                }";
        break;
    case 4:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ffff00;\
                border:1px solid #ffcc00;\
                }";
        break;
    case 5:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#663333;\
                border:1px solid #660033;\
                }";
        break;
    default:
        break;
    }

    defenseBulletGraph[defenseNum][bulletNum]->setStyleSheet(style);
    defenseBulletGraph[defenseNum][bulletNum]->setFixedSize(BULLET_LEN, BULLET_LEN);
    defenseBulletGraph[defenseNum][bulletNum]->move(pos.first + BULLET_OFFSET, pos.second + BULLET_OFFSET);
    defenseBulletGraph[defenseNum][bulletNum]->show();
    defenseBulletGraph[defenseNum][bulletNum]->raise();
    qDebug() << "defense bullet " << defenseNum << ' ' << bulletNum << " show";
}

void Widget::defenseBulletMoveTo(int defenseNum, int bulletNum, Coor pos)
{
    defenseBulletGraph[defenseNum][bulletNum]->move(pos.first + BULLET_OFFSET, pos.second + BULLET_OFFSET);
}

void Widget::eliminateDefenseBullet(int defenseNum, int bulletNum)
{
    defenseBulletGraph[defenseNum][bulletNum]->close();
}

void Widget::createDefenseClaw(int defenseNum, int clawNum, Coor pos, int typ)
{
    defenseClawGraph[defenseNum][clawNum] = new QPushButton(background);
    QString style = typ
        ? "QPushButton {\
            border-radius: 2px;\
            background-color:#66ccff;\
            border:1px solid #6699ff;\
        }"
        : "QPushButton {\
            border-radius: 2px;\
            background-color:#ff9900;\
            border:1px solid #ff3366;\
        }";
    defenseClawGraph[defenseNum][clawNum]->setStyleSheet(style);
    defenseClawGraph[defenseNum][clawNum]->resize(0, CLAW_WID);
    defenseClawGraph[defenseNum][clawNum]->move(pos.first + CLAW_OFFSET_X, pos.second + CLAW_OFFSET_Y);
    defenseClawGraph[defenseNum][clawNum]->show();
}

void Widget::defenseClawResize(int defenseNum, int clawNum, int len)
{
    defenseClawGraph[defenseNum][clawNum]->resize(len, CLAW_WID);
}

void Widget::eliminateDefenseClaw(int defenseNum, int clawNum)
{
    defenseClawGraph[defenseNum][clawNum]->close();
}

void Widget::createEnemyBullet(int enemyNum, int bulletNum, Coor pos, int typ)
{
    enemyBulletGraph[enemyNum][bulletNum] = new QPushButton(background);
    QString style;
    switch(typ)
    {
    case 0:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ccff66;\
                border:1px solid #ccff00;\
                }";
        break;
    case 1:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ff0066;\
                border:1px solid #ff3366;\
                }";
        break;
    case 2:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#00ccff;\
                border:1px solid #0099ff;\
                }";
        break;
    case 3:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#cc33ff;\
                border:1px solid #cc00ff;\
                }";
        break;
    case 4:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#ffff00;\
                border:1px solid #ffcc00;\
                }";
        break;
    case 5:
        style = "QPushButton {\
                border-radius: 7px;\
                background-color:#663333;\
                border:1px solid #660033;\
                }";
        break;
    default:
        break;
    }

    enemyBulletGraph[enemyNum][bulletNum]->setStyleSheet(style);
    enemyBulletGraph[enemyNum][bulletNum]->setFixedSize(BULLET_LEN, BULLET_LEN);
    enemyBulletGraph[enemyNum][bulletNum]->move(pos.first + BULLET_OFFSET, pos.second + BULLET_OFFSET);
    enemyBulletGraph[enemyNum][bulletNum]->show();
}

void Widget::enemyBulletMoveTo(int enemyNum, int bulletNum, Coor pos)
{
    enemyBulletGraph[enemyNum][bulletNum]->move(pos.first + BULLET_OFFSET, pos.second + BULLET_OFFSET);
}

void Widget::eliminateEnemyBullet(int enemyNum, int bulletNum)
{
    enemyBulletGraph[enemyNum][bulletNum]->close();
}

void Widget::createEnemyClaw(int enemyNum, int clawNum, Coor pos, int typ)
{
    enemyClawGraph[enemyNum][clawNum] = new QPushButton(background);
    QString style = typ
        ? "QPushButton {\
            border-radius: 2px;\
            background-color:#66ccff;\
            border:1px solid #6699ff;\
        }"
        : "QPushButton {\
            border-radius: 2px;\
            background-color:#ff9900;\
            border:1px solid #ff3366;\
        }";
    enemyClawGraph[enemyNum][clawNum]->setStyleSheet(style);
    enemyClawGraph[enemyNum][clawNum]->resize(0, CLAW_WID);
    enemyClawGraph[enemyNum][clawNum]->move(pos.first + CLAW_OFFSET_X, pos.second + CLAW_OFFSET_Y);
    enemyClawGraph[enemyNum][clawNum]->show();
}

void Widget::enemyClawResize(int enemyNum, int clawNum, int len)
{
    enemyClawGraph[enemyNum][clawNum]->resize(len, CLAW_WID);
}

void Widget::eliminateEnemyClaw(int enemyNum, int clawNum)
{
    enemyClawGraph[enemyNum][clawNum]->close();
}

void Widget::installAffix(int num, int affNum, int typ)
{
    switch(typ)
    {
    case 1:
    {
        aff[affNum][num]->setText("狂");
        break;
    }
    case 2:
    {
        aff[affNum][num]->setText("冰");
        break;
    }
    case 3:
    {
        aff[affNum][num]->setText("群");
        break;
    }
    case 4:
    {
        aff[affNum][num]->setText("血");
        break;
    }
    case 5:
    {
        aff[affNum][num]->setText("隐");
        break;
    }
    case 6:
    {
        aff[affNum][num]->setText("缓");
        break;
    }
    case 7:
    {
        aff[affNum][num]->setText("愈");
        break;
    }
    case 8:
    {
        aff[affNum][num]->setText("必");
        break;
    }
    default:
        break;
    }

    connect(aff[affNum][num], &QPushButton::clicked, this, [=]()mutable{
        delete chooseBoard;
        chooseBoard = new QPushButton(background);
        chooseBoard->resize(80, 25 * 3);
        chooseBoard->move(S->defenses[num]->pos.first + DEFENSE_OFFSET + DEFENSE_LEN, S->defenses[num]->pos.second + DEFENSE_OFFSET);
        chooseBoard->raise();
        chooseBoard->show();

        QPushButton* title = new QPushButton(chooseBoard);
        title->setFixedSize(80, 25);
        title->setText("拆卸词缀");
        title->setDisabled(true);
        title->move(0, 0);
        title->show();

        QPushButton* remove = new QPushButton(chooseBoard);
        remove->setFixedSize(80, 25);
        remove->setText("确定");
        remove->move(0, 25);
        remove->show();
        connect(remove, &QPushButton::clicked, this, [=](){
            S->removeAffixClicked(num, affNum, typ);
            chooseBoard->close();
        });

        QPushButton* can = new QPushButton(chooseBoard);
        can->setFixedSize(80, 25);
        can->setText("取消");
        can->move(0, 50);
        can->show();
        connect(can, &QPushButton::clicked, this, [=](){
            chooseBoard->close();
        });
    });
}

void Widget::removeAffix(int num, int affNum, int defenseType)
{
    aff[affNum][num]->setText("");
    switch(defenseType)
    {
    case 0:
        connect(aff[affNum][num], &QPushButton::clicked, this, [=]()mutable{
            delete chooseBoard;
            chooseBoard = new QPushButton(background);
            chooseBoard->resize(80, 25 * 6);
            chooseBoard->move(S->defenses[num]->pos.first + DEFENSE_OFFSET + DEFENSE_LEN, S->defenses[num]->pos.second + DEFENSE_OFFSET);
            chooseBoard->raise();
            chooseBoard->show();

            QPushButton* title = new QPushButton(chooseBoard);
            title->setFixedSize(80, 25);
            title->setText("安装词缀");
            title->setDisabled(true);
            title->move(0, 0);
            title->show();

            QPushButton* affix1 = new QPushButton(chooseBoard);
            affix1->setFixedSize(80, 25);
            affix1->setText("[狂暴的]");
            affix1->move(0, 25);
            affix1->show();
            connect(affix1, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 1);
                chooseBoard->close();
            });

            QPushButton* affix2 = new QPushButton(chooseBoard);
            affix2->setFixedSize(80, 25);
            affix2->setText("[冰系的]");
            affix2->move(0, 50);
            affix2->show();
            connect(affix2, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 2);
                chooseBoard->close();
            });

            QPushButton* affix3 = new QPushButton(chooseBoard);
            affix3->setFixedSize(80, 25);
            affix3->setText("[群伤的]");
            affix3->move(0, 75);
            affix3->show();
            connect(affix3, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 3);
                chooseBoard->close();
            });

            QPushButton* affix4 = new QPushButton(chooseBoard);
            affix4->setFixedSize(80, 25);
            affix4->setText("[自愈的]");
            affix4->move(0, 100);
            affix4->show();
            connect(affix4, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 7);
                chooseBoard->close();
            });

            QPushButton* can = new QPushButton(chooseBoard);
            can->setFixedSize(80, 25);
            can->setText("取消");
            can->move(0, 125);
            can->show();
            connect(can, &QPushButton::clicked, this, [=](){
                chooseBoard->close();
            });
        });
        break;
    case 1:
        connect(aff[affNum][num], &QPushButton::clicked, this, [=]()mutable{
            delete chooseBoard;
            chooseBoard = new QPushButton(background);
            chooseBoard->resize(80, 25 * 6);
            chooseBoard->move(S->defenses[num]->pos.first + DEFENSE_OFFSET + DEFENSE_LEN, S->defenses[num]->pos.second + DEFENSE_OFFSET);
            chooseBoard->raise();
            chooseBoard->show();

            QPushButton* title = new QPushButton(chooseBoard);
            title->setFixedSize(80, 25);
            title->setText("安装词缀");
            title->setDisabled(true);
            title->move(0, 0);
            title->show();

            QPushButton* affix1 = new QPushButton(chooseBoard);
            affix1->setFixedSize(80, 25);
            affix1->setText("[放血的]");
            affix1->move(0, 25);
            affix1->show();
            connect(affix1, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 4);
                chooseBoard->close();
            });

            QPushButton* affix2 = new QPushButton(chooseBoard);
            affix2->setFixedSize(80, 25);
            affix2->setText("[隐匿的]");
            affix2->move(0, 50);
            affix2->show();
            connect(affix2, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 5);
                chooseBoard->close();
            });

            QPushButton* affix3 = new QPushButton(chooseBoard);
            affix3->setFixedSize(80, 25);
            affix3->setText("[迟缓的]");
            affix3->move(0, 75);
            affix3->show();
            connect(affix3, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 6);
                chooseBoard->close();
            });

            QPushButton* affix4 = new QPushButton(chooseBoard);
            affix4->setFixedSize(80, 25);
            affix4->setText("[必杀的]");
            affix4->move(0, 100);
            affix4->show();
            connect(affix4, &QPushButton::clicked, this, [=](){
                S->affixClicked(num, affNum, 8);
                chooseBoard->close();
            });

            QPushButton* can = new QPushButton(chooseBoard);
            can->setFixedSize(80, 25);
            can->setText("取消");
            can->move(0, 125);
            can->show();
            connect(can, &QPushButton::clicked, this, [=](){
                chooseBoard->close();
            });
        });
        break;
    default:
        break;
    }
}

void Widget::updateEnemyHp(int num, int hp_len)
{
    enemyHpGraph[num]->resize(hp_len, 5);
}

void Widget::updateDefenseHp(int num, int hp_len)
{
    defenseHpGraph[num]->resize(hp_len, 5);
}

void Widget::updateMoney(int money)
{
    moneyIcon->setText(QString::number(money, 10));
}

void Widget::moneyNotEnough()
{
    messageBoard->setText("金币不足！");
    messageBoard->show();
    messageBoard->raise();
}

void Widget::blocked()
{
    messageBoard->setText("路径被敌人挡住！");
    messageBoard->show();
    messageBoard->raise();
}

void Widget::affixNotEnough()
{
    messageBoard->setText("该词缀数量不足！");
    messageBoard->show();
    messageBoard->raise();
}

void Widget::alreadyInstalled()
{
    messageBoard->setText("已装备相同词缀！");
    messageBoard->show();
    messageBoard->raise();
}

void Widget::closeMessage()
{
    messageBoard->close();
}

void Widget::updateHeart(int heart)
{
    heartIcon->setText(QString::number(heart, 10));
}

void Widget::updateAffix(int typ, int num)
{
    QString name[10] = { "WRONG", "[狂暴的]", "[冰系的]", "[群伤的]", "[放血的]", "[隐匿的]", "[迟缓的]", "[自愈的]", "[必杀的]"};
    affixIcon[typ]->setText(name[typ] + QString::number(num, 10));
}

void Widget::freeze(int num)
{
    enemyGraph[num]->setIcon(FROZEN_GRAPH);
}

void Widget::unfreeze(int num, int affix)
{
    if (affix / 4 % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH3);
    else if (affix / 2 % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH2);
    else if (affix % 2)
        enemyGraph[num]->setIcon(ENEMY_GRAPH1);
    else
        enemyGraph[num]->setIcon(ENEMY_GRAPH);
}

void Widget::bleed(int num)
{
    bloodGraph[num]->show();
}

void Widget::unbleed(int num)
{
    bloodGraph[num]->close();
}

void Widget::slow(int num)
{
    slowGraph[num]->show();
}

void Widget::unslow(int num)
{
    slowGraph[num]->close();
}

void Widget::makeSound(int typ)
{
    switch(typ)
    {
    case 0:
        sound1->play();
        break;
    case 1:
        sound2->play();
        break;
    default:
        break;
    }
}

void Widget::lose()
{
    QPushButton* info = new QPushButton(background);
    info->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);
    info->setDisabled(true);
    info->move(0, 0);
    info->show();
    info->setText("你输了");
}

void Widget::win()
{
    QPushButton* info = new QPushButton(background);
    info->setFixedSize(DISPLAY_WID + STATUS_WID, DISPLAY_HEI);
    info->setDisabled(true);
    info->move(0, 0);
    info->show();
    info->setText("你赢了");
}

void Widget::remake()
{
    QPushButton* info = new QPushButton(background);
    info->setFixedSize(200, 50);
    info->move((DISPLAY_WID + STATUS_WID - 200) / 2, (DISPLAY_HEI - 50) / 2);
    info->show();
    info->setText("点击继续");
    connect(info, &QPushButton::clicked, this, [=](){
        qDebug() << "Delete graph";
        for (int i = 0; i < N; ++i)
        {
//            for (int j = 0; j < M; ++j) { delete graph[i][j]; graph[i][j] = NULL; }
            delete[] graph[i]; graph[i] = NULL;
        }
        delete[] graph; graph = NULL;
        qDebug() << "Delete enemyGraph";
        delete[] enemyGraph; enemyGraph = NULL;
        qDebug() << "Delete defenseGraph";
        delete[] defenseGraph; defenseGraph = NULL;
        qDebug() << "Delete enemyHpGraph";
        delete[] enemyHpGraph; enemyHpGraph = NULL;
        qDebug() << "Delete defenseHpGraph";
        delete[] defenseHpGraph; defenseHpGraph = NULL;
        qDebug() << "Delete aff";
        delete[] aff[0]; aff[0] = NULL;
        delete[] aff[1]; aff[1] = NULL;
        qDebug() << "Delete bulletGraph";
        delete[] defenseBulletGraph; defenseBulletGraph = NULL;
        delete[] enemyBulletGraph; enemyBulletGraph = NULL;
        qDebug() << "Delete clawGraph";
        delete[] defenseClawGraph; defenseClawGraph = NULL;
        delete[] enemyClawGraph; enemyClawGraph = NULL;
        qDebug() << "Delete bloodGraph";
        delete[] bloodGraph; bloodGraph = NULL;
        qDebug() << "Delete slowGraph";
        delete[] slowGraph; slowGraph = NULL;
        qDebug() << "Delete background";
        delete background; background = NULL;
        qDebug() << "Delete S";
        delete S; S = NULL;
        qDebug() << "Done";

        bgm2->stop();
        bgm->setLoopCount(QSoundEffect::Infinite);
        bgm->play();
    });
}

