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
    m_b = true;
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

        if(m_b)
        {
            m_b = false;
            m_img = previousFrame.clone();
        }

        cv::Mat img_1 = m_img.clone();
        cv::Mat img_2 = currentFrame.clone();

        auto sift = cv::SIFT::create();
        std::vector<cv::KeyPoint> keypoint1, keypoint2;

        cv::Mat descriptor1, descriptor2;
        auto detector = cv::ORB::create();
        auto descriptor = cv::ORB::create();

        auto matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

        detector->detect(img_1, keypoint1);
        detector->detect(img_2, keypoint2);

        descriptor->compute(img_1, keypoint1, descriptor1);
        descriptor->compute(img_2, keypoint2, descriptor2);

        cv::drawKeypoints(previousFrame, keypoint1, currentFrame, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

        std::vector<cv::DMatch> matches;

        matcher->match(descriptor1, descriptor2, matches);

        double min_dist = 10000, max_dist = 0;

        for ( int i = 0; i < descriptor1.rows; i++ )
        {
            double dist = matches[i].distance;
            if ( dist < min_dist ) min_dist = dist;
            if ( dist > max_dist ) max_dist = dist;
        }

        std::vector< cv::DMatch > good_matches;

        for ( int i = 0; i < descriptor1.rows; i++ )
        {
            if ( matches[i].distance <= std::max ( 2*min_dist, 20.0 ) )
            {
                good_matches.push_back ( matches[i] );
            }
        }

        cv::Mat img_match;
        cv::Mat img_goodmatch;
        //drawMatches ( img_1, keypoint1, img_2, keypoint2, matches, currentFrame );
        drawMatches ( img_1, keypoint1, img_2, keypoint2, good_matches, currentFrame );


        m_ui->algorythmWorkTime->setText(QString::number(timer.elapsed()) + " ms");
    });
}

void MainWidget::on_resetButton_clicked()
{
    m_videoCapture.setImageFilter([&](cv::Mat &currentFrame, cv::Mat &previousFrame)
    {
        QElapsedTimer timer;
        timer.start();
        m_b = true;
        m_ui->algorythmWorkTime->setText(QString::number(timer.elapsed()) + " ms");
    });
}
