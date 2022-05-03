#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <videocapture.h>

#include <QPainter>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void onPixmapChanged(QPixmap pixmap);
    void on_resetButton_clicked();
    void on_trackFeaturePointButton_clicked();

private:
    bool m_b;
    cv::Mat m_img;
    Ui::MainWidget *m_ui;
    VideoCapture m_videoCapture;
};
#endif // MAINWIDGET_H
