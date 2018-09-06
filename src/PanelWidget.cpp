#include <QVBoxLayout>

#include "PanelWidget.h"

PanelWidget::PanelWidget(QWidget *parent) :
    QWidget(parent) {

    mw = nullptr;
    setMinimumWidth(300);
}

void PanelWidget::setMainWidget(MainWidget *mainWidget) {
    mw = mainWidget;
    connect(mw, SIGNAL(signalCurrentChanged()), this, SLOT(slotUpdateCurrentInfo()));
    connect(mw, SIGNAL(signalSelectedChanged()), this, SLOT(slotUpdateSelectedInfo()));
}

void PanelWidget::slotUpdateSelectedInfo() {
    addWidgets(mw->selected);
}

void PanelWidget::slotUpdateCurrentInfo() {
    addWidgets(mw->current);
}

void PanelWidget::slotKeyIns() {
    if (cnt)
        pips[0]->ins();
}

void PanelWidget::slotKeyDel() {
    if (cnt)
        pips[0]->del();
}

void PanelWidget::addWidgets(vector<int> &ids) {
    for (int i = 0; i < int(pips.size()); ++i)
        pips[i]->hide();
    cnt = 0;
    if (layout() != nullptr)
        delete layout();
    auto *myLayout = new QVBoxLayout;
    for (auto id : ids) {
        MainWidget::Pipe &p = mw->pipeById(id);
        PipeWidget *w = nullptr;
        if (cnt == pips.size())
            pips.push_back(w = new PipeWidget(mw, this));
        else {
            w = pips[cnt];
            w->setMainWidget(mw);
        }
        w->setData(id, p.w, p.l, !p.hide, p.speed, p.concen);
        w->show();
        myLayout->addWidget(w);
        ++cnt;
    }
    myLayout->addStretch();
    setLayout(myLayout);
}
