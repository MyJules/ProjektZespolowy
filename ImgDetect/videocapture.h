#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <opencv2/opencv.hpp>

#include <QPixmap>
#include <QCamera>
#include <QImageCapture>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoSink>

#include <functional>

class VideoCapture : public QObject
{
    Q_OBJECT

public:
    VideoCapture(QObject *parent = nullptr, int cameraId = 0);
    ~VideoCapture();

    void setImageFilter(std::function<void(cv::Mat&, cv::Mat&)> lambda);
    void setVideoOutput(QObject *object);

signals:
    void newPixmapCaptured(QPixmap m_pixmap);

private slots:
    void handleVideoFrames(const QVideoFrame &frame);

private:
    QImage m_previousFrame;
    QCamera *m_camera;
    QVideoSink *m_sink;
    QMediaCaptureSession m_captureSession;
    std::function<void(cv::Mat&, cv::Mat&)> m_imageFilter;
};

#endif // VIDEOCAPTURE_H
