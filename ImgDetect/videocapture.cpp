#include "videocapture.h"

#include <QDebug>
#include <cvutils.h>

VideoCapture::VideoCapture(QObject *parent, int cameraId)
    : QObject(parent),
      m_imageFilter(nullptr),
      m_sink(new QVideoSink(this))
{
    m_camera = new QCamera(QMediaDevices::defaultVideoInput());
    m_captureSession.setCamera(m_camera.data());
    m_captureSession.setVideoSink(m_sink);
    m_captureSession.

    connect(m_sink, &QVideoSink::videoFrameChanged, this, &VideoCapture::handleVideoFrames);
    m_camera->start();
}

VideoCapture::~VideoCapture()
{
    m_camera->stop();
}

void VideoCapture::setImageFilter(std::function<void(cv::Mat&)> lambda)
{
    m_imageFilter = lambda;
}

void VideoCapture::handleVideoFrames(const QVideoFrame &frame)
{
    QImage image = frame.toImage();
    cv::Mat cvFrame = cvutils::QImageToCvMat(image);

    if(m_imageFilter != nullptr)
    {
      m_imageFilter(cvFrame);
    }

    QPixmap pixmap = cvutils::cvMatToQPixmap(cvFrame);
    emit newPixmapCaptured(pixmap);
}
