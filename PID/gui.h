#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QErrorMessage>
#include <QListWidgetItem>

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();

    float getGain();
    float getTi();
    float getTd();
    float getT_lag();
    float getXmin();
    float getXmax();
    float getFeed_ffd();
    float getYman();

    bool autoIsChecked();
    bool manualIsChecked();
    bool haltIsChecked();

    bool en_p_isChecked();
    bool en_i_isChecked();
    bool en_d_isChecked();
    bool en_d_on_pv_isChecked();

    void plot(float, float, int, float min, float max);

private:
    Ui::GUI *ui;
    QGraphicsScene *scene;
    QPen *blackpen;
    QPen *redpen;

    bool running;
    bool checkInputValues();




private slots:
    void startInitiated();
    void modeChangeInitiated();

signals:
    void startButtonPushed();
    void stopButtonPushed();
    void modeChanged();

};

#endif // GUI_H
