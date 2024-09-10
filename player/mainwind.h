#ifndef MAINWIND_H
#define MAINWIND_H

#include <QMainWindow>
#include "ijkmediaplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWind; }
QT_END_NAMESPACE

class MainWind : public QMainWindow
{
    Q_OBJECT
public:
    MainWind(QWidget *parent = nullptr);
    ~MainWind();
private:
    void InitSignalsAndSlots();
    int message_loop(void* arg);
private slots:
    void OnPlayOrPause();
private:
    Ui::MainWind *ui;
    IjkMediaPlayer* _mp = NULL;

};
#endif // MAINWIND_H
