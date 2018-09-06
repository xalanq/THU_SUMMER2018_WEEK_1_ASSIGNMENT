#include <algorithm>

#include <QDoubleValidator>
#include <QMessageBox>
#include <QStringList>

#include "NewDialog.h"
#include "ui_NewDialog.h"

using std::swap;

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog) {

    ui->setupUi(this);
    ui->editWidth->setValidator(new QIntValidator(20, ~0u >> 1, this));
    ui->editLength->setValidator(new QIntValidator(1, ~0u >> 1, this));
    ui->editSpeed->setValidator(new QDoubleValidator(1.0, 1e300, 10, this));
    ui->editConcen_1->setValidator(new QIntValidator(0, ~0u >> 1, this));
    ui->editConcen_2->setValidator(new QIntValidator(0, ~0u >> 1, this));

    connect(ui->cbboxSize, SIGNAL(currentTextChanged(QString)), this, SLOT(slotUpdate(QString)));
    slotUpdate(ui->cbboxSize->currentText());
}

NewDialog::~NewDialog() {
    delete ui;
}

void NewDialog::setData(int &n, int &i1, int &i2, int &o1, int &o2, int &o3, int &length, int &width, double &speed, double &concen1, double &concen2) {
    n = ui->cbboxSize->currentText().toInt();
    i1 = ui->cbboxIn_1->currentText().toInt();
    i2 = ui->cbboxIn_2->currentText().toInt();
    if (i1 > i2)
        swap(i1, i2);
    o1 = ui->cbboxOut_1->currentText().toInt();
    o2 = ui->cbboxOut_2->currentText().toInt();
    if (o1 > o2)
        swap(o1, o2);
    o3 = ui->cbboxOut_3->currentText().toInt();
    if (o2 > o3)
        swap(o2, o3);
    if (o1 > o2)
        swap(o1, o2);
    length = ui->editLength->text().toInt();
    width = ui->editWidth->text().toInt();
    speed = ui->editSpeed->text().toDouble();
    concen1 = ui->editConcen_1->text().toDouble();
    concen2 = ui->editConcen_2->text().toDouble();
}

void NewDialog::slotUpdate(QString s) {
    int n = s.toInt();
    QStringList l;
    for (int i = 0; i < n; ++i)
        l.append(QString::number(i));
    ui->cbboxIn_1->addItems(l);
    ui->cbboxIn_2->addItems(l);
    ui->cbboxOut_1->addItems(l);
    ui->cbboxOut_2->addItems(l);
    ui->cbboxOut_3->addItems(l);
    ui->cbboxIn_1->setCurrentIndex(0);
    ui->cbboxIn_2->setCurrentIndex(1);
    ui->cbboxOut_1->setCurrentIndex(0);
    ui->cbboxOut_2->setCurrentIndex(1);
    ui->cbboxOut_3->setCurrentIndex(2);
}

void NewDialog::accept() {
    int n, i1, i2, o1, o2, o3, length, width;
    double speed, con1, con2;
    setData(n, i1, i2, o1, o2, o3, length, width, speed, con1, con2);
    if (i1 == i2) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Inputs should be different!"));
        ui->cbboxIn_1->setFocus();
    } else if (o1 == o2 || o1 == o3 || o2 == o3) {
        QMessageBox::warning(this,
                     tr("Error"),
                     tr("Ouputs should be different!"));
        ui->cbboxOut_1->setFocus();
    } else if (length < width) {
        QMessageBox::warning(this,
                     tr("Error"),
                     tr("Length can not be less than width!"));
        ui->editLength->setFocus();
    } else {
        QDialog::accept();
    }
}
