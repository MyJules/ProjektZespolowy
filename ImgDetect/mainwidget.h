#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <videocapture.h>

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
    void on_filterButton_clicked();
    void on_filterButton1_clicked();
    void on_filterButton2_clicked();
    void on_resetButton_clicked();

private:
    Ui::MainWidget *m_ui;
    VideoCapture *m_videoCapture;
};
#endif // MAINWIDGET_H
