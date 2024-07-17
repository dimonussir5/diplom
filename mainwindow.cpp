#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    uploadButton = new QPushButton("Загрузка алгоритма", this);
    findButton = new QPushButton("Поиск старого алгоритма", this);
    updateButton = new QPushButton("Обновление алгоритма", this);

    // Устанавливаем круглую форму кнопок
    uploadButton->setStyleSheet("border-radius: 50px; padding: 20px;");
    findButton->setStyleSheet("border-radius: 50px; padding: 20px;");
    updateButton->setStyleSheet("border-radius: 50px; padding: 20px;");

    layout->addWidget(uploadButton);
    layout->addWidget(findButton);
    layout->addWidget(updateButton);

    setCentralWidget(centralWidget);

    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadAlgorithmClicked);
    connect(findButton, &QPushButton::clicked, this, &MainWindow::onFindOldAlgorithmClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateAlgorithmClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onUploadAlgorithmClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите алгоритм для загрузки", "", "All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QByteArray algorithmData = file.readAll();
    file.close();

    if (!sendDataOverCAN(algorithmData)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось передать данные через CAN.");
    } else {
        QMessageBox::information(this, "Загрузка алгоритма", "Загрузка алгоритма завершена.");
    }
}

void MainWindow::onFindOldAlgorithmClicked()
{
    QMessageBox::information(this, "Поиск старого алгоритма", "Поиск старого алгоритма начался.");

    QByteArray requestData;  // Создайте запрос для поиска старого алгоритма
    if (!sendDataOverCAN(requestData)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось отправить запрос через CAN.");
        return;
    }

    QByteArray responseData;
    if (!receiveDataOverCAN(responseData)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ через CAN.");
        return;
    }

    QMessageBox::information(this, "Поиск старого алгоритма", "Поиск старого алгоритма завершен.");
}

void MainWindow::onUpdateAlgorithmClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите новый алгоритм для обновления", "", "All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QByteArray newAlgorithmData = file.readAll();
    file.close();

    if (!sendDataOverCAN(newAlgorithmData)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось передать данные через CAN.");
    } else {
        QMessageBox::information(this, "Обновление алгоритма", "Обновление алгоритма завершено.");
    }
}

bool MainWindow::sendDataOverCAN(const QByteArray &data)
{
    TPCANMsg message;
    message.ID = 0x123;  // Замените на необходимый идентификатор CAN
    message.MSGTYPE = PCAN_MESSAGE_STANDARD;
    message.LEN = data.size();
    memcpy(message.DATA, data.data(), message.LEN);

    TPCANStatus status = CAN_Write(PCAN_USBBUS1, &message);
    if (status != PCAN_ERROR_OK) {
        qDebug() << "Ошибка при отправке данных через CAN: " << status;
        return false;
    }

    return true;
}

bool MainWindow::receiveDataOverCAN(QByteArray &data)
{
    TPCANMsg message;
    TPCANTimestamp timestamp;

    TPCANStatus status = CAN_Read(PCAN_USBBUS1, &message, &timestamp);
    if (status != PCAN_ERROR_OK) {
        qDebug() << "Ошибка при получении данных через CAN: " << status;
        return false;
    }

    data = QByteArray((const char*)message.DATA, message.LEN);
    return true;
}
