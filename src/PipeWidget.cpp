#include <cmath>

#include <QIntValidator>
#include <QKeyEvent>
#include <QMessageBox>

#include "PipeWidget.h"
#include "ui_PipeWidget.h"

PipeWidget::PipeWidget(MainWidget *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeWidget) {

    this->mw = mw;

    ui->setupUi(this);
    ui->editWidth->setValidator(new QIntValidator(20, ~0u >> 1, this));
    layout()->setContentsMargins(0, 0, 0, 0);

    connect(ui->editWidth, &QLineEdit::editingFinished, [this] {
        int w = ui->editWidth->text().toInt();
        if (w != this->w) {
            auto &p = this->mw->pipeById(id);
            if (!p.hide && !this->mw->checkWidth(id, w)) {
                ui->editWidth->setText(QString::number(this->w));
                ui->editWidth->setFocus();
                QMessageBox::warning(this,
                                     tr("Error"),
                                     tr("The distance between two pipelines should be more than 200um"));
            } else {
                p.w = w;
                this->mw->cal();
                this->mw->repaint();

                this->w = w;
                ui->editSpeed->setText(QString::number(abs(p.speed)));
                ui->editConcen->setText(QString::number(p.concen));
            }
        }
    });
    connect(ui->cboxChecked, &QCheckBox::clicked, [this](bool checked) {
        if (checked != this->checked) {
            auto &p = this->mw->pipeById(id);
            if (checked && !this->mw->checkWidth(id, this->w)) {
                ui->cboxChecked->setChecked(this->checked);
                ui->cboxChecked->setFocus();
                QMessageBox::warning(this,
                                     tr("Error"),
                                     tr("The distance between two pipelines should be more than 200um"));
            } else {
                p.hide = !checked;
                this->mw->cal();
                this->mw->repaint();

                this->checked = checked;
                ui->editSpeed->setText(QString::number(abs(p.speed)));
                ui->editConcen->setText(QString::number(p.concen));
            }
        }
    });
}

void PipeWidget::setData(int id, int w, int l, bool checked, double speed, double concen) {
    this->id = id;
    this->w = w;
    this->checked = checked;
    ui->lblID->setText(tr("ID: ") + QString::number(id));
    ui->cboxChecked->setChecked(checked);
    ui->editSpeed->setText(QString::number(abs(speed)));
    ui->editConcen->setText(QString::number(concen));
    ui->editLength->setText(QString::number(l));
    ui->editWidth->setText(QString::number(w));
}

void PipeWidget::ins() {
    if (!ui->cboxChecked->isChecked())
        ui->cboxChecked->click();
}

void PipeWidget::del() {
    if (ui->cboxChecked->isChecked())
        ui->cboxChecked->click();
}

void PipeWidget::setMainWidget(MainWidget *mw) {
    this->mw = mw;
}

PipeWidget::~PipeWidget() {
    delete ui;
}
