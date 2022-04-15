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

    QPixmap getPixmap() const;
    void setImageFilter(std::function<void(cv::Mat&)> lambda);
    static bool checkCameraAvailability();

signals:
    void newPixmapCaptured();

private slots:
    void handleVideoFrames(const QVideoFrame &frame);

private:
    QPixmap m_pixmap;
    QCamera *m_camera;
    QVideoSink *m_sink;
    QMediaCaptureSession m_captureSession;
    std::function<void(cv::Mat&)> m_imageFilter;
};

#endif // VIDEOCAPTURE_H
