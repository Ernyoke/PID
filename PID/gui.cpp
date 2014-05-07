#include "gui.h"
#include "ui_gui.h"

GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignLeft);
    scene->setSceneRect(0,0, ui->graphicsView->width(), ui->graphicsView->height());
    blackpen = new QPen(Qt::black);
    redpen = new QPen(Qt::red);
    blackpen->setWidth(2);
    redpen->setWidth(4);

    QSize size = ui->graphicsView->size();
    //qDebug() << size.width() << " " << size.height();
    //qDebug() << ui->graphicsView->height() / 2;
    running = false;
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startInitiated()));
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(modeChangeInitiated()));
}

GUI::~GUI()
{
    delete ui;
}

void GUI::plot(float prew, float curr, int it, float min, float max) {
    int b = (ui->graphicsView->height() * 0.8) / 2;
    int a = -b;
    int y1 = a + ((prew - min) * (b - a)) / (max - min);
    int y2 = a + ((curr - min) * (b - a)) / (max - min);
    //qDebug() << prew << " " << curr << " " <<  y1 << " " << y2;
    int dt = (it - 1) * 30;
    scene->addLine(dt - 30, -y1, dt, -y2, *blackpen);
    scene->addLine(dt - 30, -y1, dt - 30, -y1, *redpen);
    scene->addLine(dt, -y2, dt, -y2, *redpen);

    QString outputString = QString::number(it) + ". y_out: " + QString::number(curr) + " err: ";
    QListWidgetItem *item = new QListWidgetItem(outputString);
    ui->listWidget->addItem(item);
}

void GUI::startInitiated() {
    if(!running) {
        QString input = ui->gainInput->text();
        QErrorMessage msg(this);
        if(input.isEmpty()) {
            msg.showMessage("GAIN input is not filled!");
            msg.exec();
            return;
        }

        input = ui->tiInput->text();
        if(input.isEmpty()) {
            msg.showMessage("Ti input is not filled!");
            msg.exec();
            return;
        }

        input = ui->tdInput->text();
        if(input.isEmpty()) {
            msg.showMessage("Td input is not filled!");
            msg.exec();
            return;
        }

        input = ui->tlagInput->text();
        if(input.isEmpty()) {
            msg.showMessage("T_LAG input is not filled!");
            msg.exec();
            return;
        }

        input = ui->xminInput->text();
        if(input.isEmpty()) {
            msg.showMessage("XMIN input is not filled!");
            msg.exec();
            return;
        }

        input = ui->xmaxInput->text();
        if(input.isEmpty()) {
            msg.showMessage("XMAX input is not filled!");
            msg.exec();
            return;
        }

        input = ui->feedffdInput->text();
        if(input.isEmpty()) {
            msg.showMessage("FEED_FFD input is not filled!");
            msg.exec();
            return;
        }
        if(!ui->p_checkBox->isChecked() && !ui->i_checkBox->isChecked() && !ui->d_checkBox->isChecked()) {
            msg.showMessage("Invalid controller settings!");
            msg.exec();
            return;
        }
        emit startButtonPushed();
        ui->startButton->setText("Stop");
        scene->clear();
        running = true;
    }
    else {
        emit stopButtonPushed();
        ui->startButton->setText("Start");
        running = false;
    }
}

void GUI::modeChangeInitiated() {
    emit modeChanged();
}

float GUI::getGain() {
    QString input = ui->gainInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getTi() {
    QString input = ui->tiInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getTd() {
    QString input = ui->tdInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getT_lag() {
    QString input = ui->tlagInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getXmin() {
    QString input = ui->xminInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getXmax() {
    QString input = ui->xmaxInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getYman() {
    QString input = ui->ymanInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

float GUI::getFeed_ffd() {
    QString input = ui->feedffdInput->text();
    float val;
    if(input.isEmpty()) {
        val = 0;
    }
    else {
       val = input.toFloat();
    }
    return val;
}

bool GUI::en_p_isChecked() {
    if(ui->p_checkBox->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::en_d_isChecked() {
    if(ui->d_checkBox->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::en_i_isChecked() {
    if(ui->i_checkBox->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::en_d_on_pv_isChecked() {
    if(ui->d_on_pv_checkBox->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::autoIsChecked() {
    if(ui->autoRadioButton->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::manualIsChecked() {
    if(ui->manualRadioButton->isChecked()) {
        return true;
    }
    return false;
}

bool GUI::haltIsChecked() {
    if(ui->haltRadioButton->isChecked()) {
        return true;
    }
    return false;
}



