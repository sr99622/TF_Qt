#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "panel.h"

#include <QtWidgets/QMainWindow>

#define MW dynamic_cast<MainWindow *>(mainWindow)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Panel *panel;
    QTextEdit *messages;
};
#endif // MAINWINDOW_H
