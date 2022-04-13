#include "mainwidget.h"
#include "./ui_mainwidget.h"

#include <opencv2/opencv.hpp>

#include "QImage"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), 
      m_ui(new Ui::MainWidget)
{
    m_ui->setupUi(this);
    setWindowTitle("ImgDetect");

    QPixmap pixmap("/home/andrew/MY/PWR/Projekt zespolowy/ProjektZespolowy/ImgDetect/resources/bears.jpg");
    cv::Mat m = cv::Mat();

    m_ui->imageLabel->setPixmap(pixmap);
    m_ui->imageLabel->setMask(pixmap.mask());
}

MainWidget::~MainWidget()
{
    delete m_ui;
}

