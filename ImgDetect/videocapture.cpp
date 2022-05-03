#include "videocapture.h"

#include <QDebug>
#include <QElapsedTimer>
#include <cvutils.h>

VideoCapture::VideoCapture(QObject *parent, int cameraId)
    : QObject(parent),
      m_imageFilter(nullptr),
      m_sink(new QVideoSink(this))
{
    m_camera = new QCamera(QMediaDevices::defaultVideoInput());
    m_captureSession.setCamera(m_camera);
    m_captureSession.setVideoOutput(m_sink);

    connect(m_sink, &QVideoSink::videoFrameChanged, this, &VideoCapture::handleVideoFrames);
    m_camera->start();
}

VideoCapture::~VideoCapture()
{
    m_camera->stop();
    delete m_camera;
}

void VideoCapture::setImageFilter(std::function<void(cv::Mat&, cv::Mat&)> lambda)
{
    m_imageFilter = lambda;
}

void VideoCapture::setVideoOutput(QObject *object)
{
    m_captureSession.setVideoOutput(object);
}

void VideoCapture::handleVideoFrames(const QVideoFrame &frame)
{
    if(!frame.isValid()) return;

    QImage currentFrame = frame.toImage();

    cv::Mat cvCurrentFrame = cvutils::QImageToCvMat(currentFrame);
    cv::Mat cvPreviousFrame = cvutils::QImageToCvMat(m_previousFrame);

    if(m_imageFilter != nullptr &&
            !cvPreviousFrame.empty())
    {
      m_imageFilter(cvCurrentFrame, cvPreviousFrame);
    }

    m_previousFrame = currentFrame;

    QPixmap pixmap = cvutils::cvMatToQPixmap(cvCurrentFrame);
    emit newPixmapCaptured(pixmap);
}
