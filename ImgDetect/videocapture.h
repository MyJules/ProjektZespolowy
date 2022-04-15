#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <opencv2/opencv.hpp>

#include <QThread>
#include <QPixmap>
#include <QCamera>
#include <QImageCapture>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QPointer>

#include <functional>

class VideoCapture : public QObject
{
    Q_OBJECT

public:
    VideoCapture(QObject *parent = nullptr, int cameraId = 0);
    ~VideoCapture();

    void setImageFilter(std::function<void(cv::Mat&)> lambda);

signals:
    void newPixmapCaptured(const QPixmap &pixmap);

private slots:
    void handleVideoFrames(const QVideoFrame &frame);

private:
    QPixmap m_pixmap;
    QVideoSink *m_sink;
    QPointer<QCamera> m_camera;
    QMediaCaptureSession m_captureSession;
    std::function<void(cv::Mat&)> m_imageFilter;
};

#endif // VIDEOCAPTURE_H
