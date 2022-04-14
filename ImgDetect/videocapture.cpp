#include "videocapture.h"

#include <QDebug>

VideoCapture::VideoCapture(QObject *parent, int cameraId)
    : QThread(parent),
      m_videoCapture(cameraId),
      m_imageFilter(nullptr)
{
}

QPixmap VideoCapture::getPixmap() const
{
    return m_pixmap;
}

inline QPixmap VideoCapture::cvMatToQPixmap(const cv::Mat &inMat) const
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

void VideoCapture::setImageFilter(std::function<void(cv::Mat&)> lambda)
{
    m_imageFilter = lambda;
}

void VideoCapture::run()
{
    if(!m_videoCapture.isOpened()) return;

    while(true)
    {
        m_videoCapture >> m_frame;
        if(!m_frame.empty())
        {
            if(m_imageFilter != nullptr)
            {
                m_imageFilter(m_frame);
            }
            m_pixmap = cvMatToQPixmap(m_frame);
            emit newPixmapCaptured();
        }
    }
}

QImage VideoCapture::cvMatToQImage(const cv::Mat &inMat) const
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}
