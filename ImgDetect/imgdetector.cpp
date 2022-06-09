#include "imgdetector.h"

bool reinit = true;

ImgDetector::ImgDetector()
{

}

cv::Mat ImgDetector::detect(const cv::Mat &frame, const cv::Mat &previousFrame)
{
    cv::Mat result;
    cv::Mat oldFrame;
    std::vector<cv::Point2f> p1;
    cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);

    std::vector<cv::Scalar> colors;
    cv::RNG rng;
    for(int i = 0; i < 100; i++)
    {
      int r = rng.uniform(0, 256);
      int g = rng.uniform(0, 256);
      int b = rng.uniform(0, 256);
      colors.push_back(cv::Scalar(r,g,b));
    }

    oldFrame = previousFrame.clone();

    cv::cvtColor(oldFrame, oldGrayFrame, cv::COLOR_BGR2GRAY);

    // create a mask image for drawing
    if (reinit)
    {
        reinit = false;
        cv::goodFeaturesToTrack(oldGrayFrame, trackPoints, 100, 0.3, 7, cv::Mat(), 7, false, 0.04);
        cv::cornerSubPix(oldGrayFrame, trackPoints, cv::Size(10, 10), cv::Size(-1, -1), criteria);
        drawMask = cv::Mat::zeros(oldFrame.size(), oldFrame.type());
        zeroFrame = oldFrame.clone();
    }

    cv::Mat frame1, frameGray;
    frame1 = frame.clone();

    cv::cvtColor(frame1, frameGray, cv::COLOR_RGB2GRAY);

    //calculate optical flow
    std::vector<uchar> status;
    std::vector<float> err;

    cv::calcOpticalFlowPyrLK(oldGrayFrame, frameGray, trackPoints, p1, status, err, cv::Size(31, 31), 8, criteria);

    std::vector<cv::Point2f> goodNew;
    for(int i = 0; i < trackPoints.size(); i++)
    {
        // Select good points
        if(status[i] == 1)
        {
            goodNew.push_back(p1[i]);
            //draw the tracks
            cv::line(drawMask, p1[i], trackPoints[i], colors[i], 2);
            cv::circle(frame1, p1[i], 4, colors[i], 1);
        }
    }
    cv::add(frame1, drawMask, result);

    oldGrayFrame = frameGray.clone();
    trackPoints = goodNew;

    return result;
}

void ImgDetector::addTrackPoint(cv::Point2f point)
{
    trackPoints.push_back(point);
    reinit = true;
}

void ImgDetector::clearMask()
{
    drawMask = cv::Mat::zeros(zeroFrame.size(), zeroFrame.type());
    reinit = true;
}
