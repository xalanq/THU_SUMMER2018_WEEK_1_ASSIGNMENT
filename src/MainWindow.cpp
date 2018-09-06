#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QRect>
#include <QTimer>

#include "NewDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    mainWidget = nullptr;

    panelWidget = new PanelWidget(this);
    panelDock = new QDockWidget(tr("Pipeline Info"), this);
    panelDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    panelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    panelDock->setWidget(panelWidget);

    addDockWidget(Qt::RightDockWidgetArea, panelDock);

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout()));
    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(actionNew()));
    connect(ui->actionInsert, SIGNAL(triggered(bool)), panelWidget, SLOT(slotKeyIns()));
    connect(ui->actionDelete, SIGNAL(triggered(bool)), panelWidget, SLOT(slotKeyDel()));
    ui->statusBar->hide();

    centralWidget()->setMinimumSize(600, 600);
    adjustSize();
    makeCenter(width(), height());

    QTimer::singleShot(0, this, SLOT(actionNew()));

    setWindowIcon(QIcon(":/icon/icon.ico"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::makeCenter(int w, int h) {
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - w) / 2;
    int y = (screenGeometry.height() - h) / 2;
    move(x, y);
}

void MainWindow::actionAbout() {
    QMessageBox::about(this,
                       tr("About"),
                       tr("<center><b>Week1 Assignment</b></center>" \
                          "<p>CopyRight &copy; 2018 by <b>xalanq</b>.</p>" \
                          "<p>Email: xalanq@qq.com</p>" \
                          "<p>Lisence: LGPL v3.0</p>" \
                          "<p>Blog: <a href='https://blog.xalanq.com'>blog.xalanq.com</a></p>"));
}

void MainWindow::actionNew() {
    NewDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        if (mainWidget != nullptr)
            delete mainWidget;
        int n, i1, i2, o1, o2, o3, length, width;
        double speed, con1, con2;
        dialog.setData(n, i1, i2, o1, o2, o3, length, width, speed, con1, con2);

        mainWidget = new MainWidget(n, i1, i2, o1, o2, o3, length, width, speed, con1, con2, this);
        connect(mainWidget, &MainWidget::signalCurrentChanged, [this]{
            if (mainWidget->current.size()) {
                ui->actionInsert->setEnabled(true);
                ui->actionDelete->setEnabled(true);
            } else {
                ui->actionInsert->setEnabled(false);
                ui->actionDelete->setEnabled(false);
            }
        });
        connect(mainWidget, &MainWidget::signalSelectedChanged, [this]{
            if (mainWidget->selected.size()) {
                ui->actionInsert->setEnabled(true);
                ui->actionDelete->setEnabled(true);
            } else {
                ui->actionInsert->setEnabled(false);
                ui->actionDelete->setEnabled(false);
            }
        });

        panelWidget->setMainWidget(mainWidget);
        setCentralWidget(mainWidget);

        adjustSize();
        makeCenter(mainWidget->width() + panelWidget->width(), mainWidget->height() + 100);
    }
}
