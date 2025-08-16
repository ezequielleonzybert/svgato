#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "svgparser.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Canvas *canvas;
    QWidget *sideBar;
    QString filePath = "";
    QPushButton *btnOpen;
    SvgParser *parser;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFile();

};
#endif // MAINWINDOW_H
