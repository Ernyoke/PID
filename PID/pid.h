#ifndef PID_H
#define PID_H

#include <QObject>
#include <QErrorMessage>
#include <QFile>
#include <QTimer>
#include "gui.h"

class PID : public QObject
{
    Q_OBJECT
public:
    explicit PID(GUI *gui, QObject *parent = 0);

private:
    struct mode {
        bool en_p;
        bool en_i;
        bool en_d;
        bool en_p_on_d;
        bool halt;
        bool automatic;
        bool manual;
    };

    struct param {
        float gain;
        float ti;
        float td;
        float td_lag;
        float ymax;
        float ymin;
    };

    struct status {
        bool qmax;
        bool qmin;
    };

    struct mode c_type;
    struct param parameters;

    float feed_ffd;
    float yman;

    float sp;
    float pw, pw_old;

    float y;
    float y_old;
    float err;
    float err_old;
    float dt;
    float yp, yi, yd;
    float yd_old;
    struct status st;

    int it;

    GUI *gui;
    QFile *input;
    QTextStream *stream;
    QStringList *temp;
    QFile *output;
    QTimer *timer;



signals:

public slots:
    void setParameters();
    void calculateOutput();
    void updateMode();

};

#endif // PID_H
