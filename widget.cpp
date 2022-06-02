#include "widget.h"
#include "ui_widget.h"

#include "pcmcaptureandplay.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    PcmCaptureAndPlay *PCAP = new PcmCaptureAndPlay;
    PCAP->start();
}

Widget::~Widget()
{
    delete ui;
}
