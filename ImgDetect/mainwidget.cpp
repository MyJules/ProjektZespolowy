#include "mainwidget.h"
#include "./ui_mainwidget.h"

#include "QImage"
#include <opencv2/opencv.hpp>
#include <vector>


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::MainWidget),
      m_videoCapture(new VideoCapture(this))
{
    m_ui->setupUi(this);
    setWindowTitle("ImgDetect");

    connect(m_videoCapture, &VideoCapture::newPixmapCaptured, this, [&](const QPixmap &pixmap){
        m_ui->imageLabel->setPixmap(pixmap.scaled(m_ui->imageLabel->width(), m_ui->imageLabel->height()));
    });
}

MainWidget::~MainWidget()
{
    delete m_ui;
    delete m_videoCapture;
}

void MainWidget::on_filterButton_clicked()
{
    m_videoCapture->setImageFilter([](cv::Mat& frame){
        cv::Mat img_gray;
        cvtColor(frame, img_gray, cv::COLOR_BGR2GRAY);
        cv::Mat thresh;
        threshold(img_gray, thresh, 150, 255, cv::THRESH_BINARY);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        cv::Mat image_copy = frame.clone();
        drawContours(frame, contours, -1, cv::Scalar(0, 255, 0), 2);
    });
}


void MainWidget::on_filterButton1_clicked()
{
    m_videoCapture->setImageFilter([](cv::Mat& frame){
        cv::blur(frame, frame, cv::Size(5,5));
        cv::cvtColor( frame, frame, cv::COLOR_BGR2GRAY);
    });
}


void MainWidget::on_filterButton2_clicked()
{
    m_videoCapture->setImageFilter([](cv::Mat& frame){
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(frame, frame, cv::Size(5, 5), 1.5);
        cv::Canny(frame, frame, 100, 200);
    });
}


void MainWidget::on_resetButton_clicked()
{
    m_videoCapture->setImageFilter([](cv::Mat& frame){
    });
}

