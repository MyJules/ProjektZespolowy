#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <opencv2/opencv.hpp>

#include <QThread>
#include <QPixmap>

#include <functional>

class VideoCapture : public QThread
{
    Q_OBJECT

public:
    VideoCapture(QObject *parent = nullptr, int cameraId = 0);

    QPixmap getPixmap() const;
    QImage cvMatToQImage(const cv::Mat &inMat) const;
    QPixmap cvMatToQPixmap(const cv::Mat &inMat) const;
    void setImageFilter(std::function<void(cv::Mat&)> lambda);

protected:
    void run() override;

signals:
    void newPixmapCaptured();

private:
    QPixmap m_pixmap;
    cv::Mat m_frame;
    cv::VideoCapture m_videoCapture;
    std::function<void(cv::Mat&)> m_imageFilter;
};

#endif // VIDEOCAPTURE_H
