#include "videocapture.h"

#include <QDebug>
#include <cvutils.h>

VideoCapture::VideoCapture(QObject *parent, int cameraId)
    : QObject(parent),
      m_imageFilter(nullptr),
      m_camera(new QCamera(this)),
      m_sink(new QVideoSink(this))
{
    m_captureSession.setCamera(m_camera);
    m_captureSession.setVideoSink(m_sink);

    connect(m_sink, &QVideoSink::videoFrameChanged, this, &VideoCapture::handleVideoFrames);
    m_camera->start();
}

VideoCapture::~VideoCapture()
{
    m_camera->stop();
}

QPixmap VideoCapture::getPixmap() const
{
    return m_pixmap;
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
    m_pixmap = cvutils::cvMatToQPixmap(cvFrame);
    emit newPixmapCaptured();
}

bool VideoCapture::checkCameraAvailability()
{
    if(QMediaDevices::videoInputs().count() > 0)
    {
        return true;
    }else
    {
        return false;
    }
}
