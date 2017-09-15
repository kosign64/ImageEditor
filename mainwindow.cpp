#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mylabel.h"
#include <QFileDialog>
#include <QPainter>
#include <QClipboard>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    anim(false),
    paint(true),
    resize(true),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("SAU", "Graphical display");
    path = settings->value("/Settings/Path", "").toString();
    myLabel = new MyLabel(this);
    myLabel->setGeometry(10, 30, 512, 256);
    myLabel->setScaledContents(true);
    monoImg = QImage(128, 64, QImage::Format_Mono);
    QPainter paint;
    paint.begin(&monoImg);
    paint.setBrush(QBrush(Qt::white));
        paint.setPen(QPen(Qt::white));
        paint.drawRect(QRect(0, 0, 127, 63));
    paint.end();
    myLabel->setPixmap(QPixmap::fromImage(monoImg));

    ui->codeEdit->setGeometry(10, 325, 512, 300);
    ui->getCodeButton->setGeometry(185, 295, 165, 23);

    QObject::connect(myLabel, SIGNAL(sendCoordinates(int, int)),
                     this, SLOT(getCoordinates(int, int)));

    // Set window properties
    setFixedSize(532, 655);
    setWindowTitle("Графический дисплей");
    setWindowIcon(QIcon(":/icon.ico"));
}

MainWindow::~MainWindow()
{
    settings->setValue("/Settings/Path", path);
    delete ui;
}

//=========================================================================================================

// Load image button
void MainWindow::on_action_2_triggered()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), path,
                                            tr("Изображения (*.png *.jpg *.bmp *.jpeg);; Все файлы (*.*)"));
    if(!filename.isEmpty())
    {
        anim = false;
        paint = false;
        monoImg = QImage(filename);
        //monoImg = monoImg.scaled(128, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        monoImg = monoImg.convertToFormat(QImage::Format_Mono);
        myLabel->setPixmap(QPixmap::fromImage(monoImg));
        QDir dir = QFileInfo(filename).absoluteDir();
        path = dir.absolutePath();
    }
}

// Load animation button
void MainWindow::on_action_3_triggered()
{
    animation.stop();
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Открыть анимацию"), path,
                                            tr("Анимация (*.gif);; Все файлы (*.*)"));
    if(!filename.isEmpty())
    {
        anim = true;
        animation.setFileName(filename);
        myLabel->setMovie(&animation);
        animation.start();
        QDir dir = QFileInfo(filename).absoluteDir();
        path = dir.absolutePath();
    }
}

// Directory
void MainWindow::on_action_6_triggered()
{
    QString newPath;
    newPath = QFileDialog::getExistingDirectory(this, "Выберите папку", path);
    if(!newPath.isEmpty())
    {
        path = newPath;
    }
}

//=========================================================================================================

// Exit button
void MainWindow::on_action_triggered()
{
    close();
}

// About programm
void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, "О программе",
                       "Программа для удобного использования \n графического дисплея");
}

// About Qt
void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "This program uses Qt");
}

//=========================================================================================================

// Get code button
void MainWindow::on_getCodeButton_clicked()
{
    code.clear();
    unsigned char currentByte;
    unsigned int count = 0;
    if(anim)
    {
        code += "const unsigned char animationArray[][1024] PROGMEM = {";
        animation.stop();
        animation.jumpToFrame(0);
        for(int a = 0; a < animation.frameCount(); ++a)
        {
            code += "{";
            monoImg = animation.currentImage();
            monoImg = monoImg.scaled(128, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            monoImg = monoImg.convertToFormat(QImage::Format_Mono);
            for(int i = 0; i < monoImg.width(); ++i)
            {
                for(int j = 0; j < monoImg.height() - 7; j += 8)
                {
                    currentByte = 0;
                    for(int k = 0; k < 8; ++k)
                    {
                        if(QColor::fromRgb(monoImg.pixel(i, j + k)) == Qt::black)
                            currentByte |= (1 << k);
                    }
                    code += "0x";
                    code += QString::number(currentByte, 16);
                    if(i >= monoImg.width() -1 && j >= monoImg.height() - 8)
                    {
                        if(a < animation.frameCount() - 1)
                        {
                            code += "}, \n \n";
                        }
                        else
                        {
                            code += "}};";
                        }
                    }
                    else
                    {
                        code += ", ";
                        if(++count > 17)
                        {
                            count = 0;
                            code += "\n";
                        }
                    }
                }
            }
            animation.jumpToNextFrame();
        }
        code += "\nconst unsigned char frameCount = " + QString::number(animation.frameCount()) +";";
        animation.start();
    }
    else
    {
        code += "const unsigned char imageArray[] PROGMEM = {";
        for(int i = 0; i < monoImg.width(); ++i)
        {
            for(int j = 0; j < monoImg.height() - 7; j += 8)
            {
                currentByte = 0;
                for(int k = 0; k < 8; ++k)
                {
                    if(QColor::fromRgb(monoImg.pixel(i, j + k)) == Qt::black)
                        currentByte |= (1 << k);
                }
                code += "0x";
                code += QString::number(currentByte, 16);
                if(i >= monoImg.width() -1 && j >= monoImg.height() - 8)
                {
                    code += "};";
                }
                else
                {
                    code += ", ";
                    if(++count > 17)
                    {
                        count = 0;
                        code += "\n";
                    }
                }
            }
        }
    }
    qApp->clipboard()->setText(code);
    ui->codeEdit->setText(code);
}

// Get coordinates slot
void MainWindow::getCoordinates(int x, int y)
{
    if(!anim)
    {
        if(paint)
        {
            QColor c = QColor::fromRgb(monoImg.pixel(x / 4, y / 4));
            if(c == Qt::white)
            {
                monoImg.setPixel(x / 4, y / 4, 0);
            }
            else
            {
                monoImg.setPixel(x / 4, y / 4, 1);
            }
        }
        else
        {
            QColor c = QColor::fromRgb(monoImg.pixel(x / 4, y / 4));
            if(c == Qt::white)
            {
                monoImg.setPixel(x / 4, y / 4, 1);
            }
            else
            {
                monoImg.setPixel(x / 4, y / 4, 0);
            }
        }
        myLabel->setPixmap(QPixmap::fromImage(monoImg));
    }
}
