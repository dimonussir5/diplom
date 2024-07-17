#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <windows.h>  // Добавьте это

#include "PCANBasic.h"  // Убедитесь, что файл находится в вашем проекте

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUploadAlgorithmClicked();
    void onFindOldAlgorithmClicked();
    void onUpdateAlgorithmClicked();

private:
    QPushButton *uploadButton;
    QPushButton *findButton;
    QPushButton *updateButton;

    bool sendDataOverCAN(const QByteArray &data);
    bool receiveDataOverCAN(QByteArray &data);
};

#endif // MAINWINDOW_H
