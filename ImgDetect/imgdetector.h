#ifndef IMGDETECTOR_H
#define IMGDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#include <QVector>

class ImgDetector
{
public:
    using TrackPoints_t = QVector<cv::Point2d>;

    ImgDetector();

    cv::Mat detect(const cv::Mat &frame, const cv::Mat &previousFrame);
    void addTrackPoint(cv::Point2f point);
    void clearMask();

private:
    cv::Mat zeroFrame;
    cv::Mat oldGrayFrame;
    std::vector<cv::Point2f> trackPoints;
    cv::Mat drawMask;
};

#endif // IMGDETECTOR_H
