#include "mainwidget.h"
#include "./ui_mainwidget.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>

#include <QElapsedTimer>
#include <QVideoWidget>
#include <QImage>
#include <QDebug>

#include <vector>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::MainWidget)
{
    m_ui->setupUi(this);
    m_ui->videoOutput->setScaledContents(true);
    setWindowTitle("ImgDetect");

    connect(&m_videoCapture, &VideoCapture::newPixmapCaptured, this, &MainWidget::onPixmapChanged);

    m_ui->algorythmWorkTime->setText("0 ms");
}

MainWidget::~MainWidget()
{
    delete m_ui;
}

void MainWidget::onPixmapChanged(QPixmap pixmap)
{
    m_ui->videoOutput->setPixmap(pixmap.scaled(1280, 720, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWidget::on_trackFeaturePointButton_clicked()
{
    m_videoCapture.setImageFilter([this](cv::Mat &currentFrame, cv::Mat &previousFrame)
    {
        QElapsedTimer timer;
        timer.start();

        currentFrame = m_imgDetector.detect(currentFrame, previousFrame).clone();
        m_ui->algorythmWorkTime->setText(QString::number(timer.elapsed()) + " ms");
    });
}

void MainWidget::on_resetButton_clicked()
{
    m_videoCapture.setImageFilter([&](cv::Mat &currentFrame, cv::Mat &previousFrame)
    {
        QElapsedTimer timer;
        timer.start();

        m_imgDetector.clearMask();

        m_ui->algorythmWorkTime->setText(QString::number(timer.elapsed()) + " ms");
    });
}
