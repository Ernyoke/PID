#include "pid.h"

PID::PID(GUI *gui, QObject *parent) :
    QObject(parent)
{
    this->gui = gui;
    connect(gui, SIGNAL(startButtonPushed()), this, SLOT(setParameters()));
    connect(gui, SIGNAL(modeChanged()), this, SLOT(updateMode()));

    timer = new QTimer(this);
    y = 0;
    y_old = 0;
    err = 0;
    err_old = 0;
    yp = 0;
    yi = 0;
    yd = 0;
    yd_old = 0;

    gui->show();
}

void PID::setParameters() {
    parameters.gain = gui->getGain();
    parameters.td = gui->getTd();
    parameters.ti = gui->getTi();
    parameters.td_lag = gui->getT_lag();
    parameters.ymin = gui->getXmin();
    parameters.ymax = gui->getXmax();
    yman = gui->getYman();
    //qDebug() <<"minmax " << parameters.ymin << " " << parameters.ymax;

    feed_ffd = gui->getFeed_ffd();

    c_type.automatic = gui->autoIsChecked();
    c_type.manual = gui->manualIsChecked();
    c_type.halt = gui->haltIsChecked();

    c_type.en_p = gui->en_p_isChecked();
    c_type.en_d = gui->en_d_isChecked();
    c_type.en_i = gui->en_i_isChecked();
    c_type.en_p_on_d = gui->en_d_on_pv_isChecked();

    input =  new QFile("in.txt");
    if(!input->open(QIODevice::ReadOnly)) {
        QErrorMessage msg;
        msg.showMessage("Error opening input file!");
        msg.exec();
    }
    else {
        stream = new QTextStream(input);
        connect(timer, SIGNAL(timeout()), this, SLOT(calculateOutput()));
        it = 1;
        dt = 1;
        timer->start(1000);
        //calculateOutput();
    }
}

void PID::calculateOutput() {
    if(!stream->atEnd()) {
        QString line = stream->readLine();
        if(!line.isNull()) {
            it++;
            QStringList fields = line.split(" ");
            sp = fields[0].toFloat();
            pw_old = pw;
            pw = fields[1].toFloat();
            err_old = err;
            y_old = y;
            err = sp - pw;
            if(c_type.en_p == true) {
                yp = err * parameters.gain;
            }
            else {
                yp = 0;
            }

            if(c_type.en_i == true) {
                if(c_type.automatic ==  true) {
                    yi = yi + parameters.gain * (dt/parameters.ti) * ((err_old + err) / 2);
                }
                else {
                    yi = y - (yp - feed_ffd);
                }

            }
            else {
                yi = 0;
            }

            if(c_type.en_d == true) {
                if(c_type.en_p_on_d == false) {
                    yd = (yd_old * parameters.td_lag + parameters.td * parameters.gain * (err - err_old)) / (dt + parameters.td_lag);
                }
                else {
                    yd = (yd_old * parameters.td_lag + parameters.td * parameters.gain * (pw - pw_old)) / (dt + parameters.td_lag);
                }

            }
            else {
                yd = 0;
            }
            y = yp + yi + yd + feed_ffd;

            if(c_type.manual == true) {
                y = yman;
            }
            if(c_type.halt == true) {
                y = y_old;
            }

            if(y > parameters.ymax) {
                st.qmax = true;
                y = parameters.ymax;
            }
            else {
                st.qmax = false;
            }

            if(y < parameters.ymin) {
                st.qmin = true;
                y = parameters.ymin;
            }
            else {
                st.qmin = false;
            }

            //antiwindup
            if(c_type.en_i == true && st.qmin == true) {
                if(parameters.ymin - feed_ffd - yp >= yi) {
                    yi = parameters.ymin - feed_ffd - yp;
                }
            }

            if(c_type.en_i ==  true && st.qmax == true) {
                if(parameters.ymax - feed_ffd - yp <= yi) {
                    yi = parameters.ymax - feed_ffd - yp;
                }
            }
            qDebug() << "yd=" << yd << " yi=" << yi << " yp=" << yd;
            if(it >= 2) {
                gui->plot(y_old, y, it, parameters.ymin, parameters.ymax);
            }
        }
    }
    else {
        timer->stop();
    }

}

void PID::updateMode() {
    c_type.automatic = gui->autoIsChecked();
    c_type.manual = gui->manualIsChecked();
    c_type.halt = gui->haltIsChecked();
    yman = gui->getYman();
}
