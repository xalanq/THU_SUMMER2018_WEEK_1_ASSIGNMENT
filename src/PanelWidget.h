#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <vector>

#include <QWidget>

#include "MainWidget.h"
#include "PipeWidget.h"

using std::vector;

/**
 * @brief 信息栏部件
 *
 * 对经过、选中的管道的信息进行显示
 * 每个管道通过一个PipeWidget进行显示
 */
class PanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = nullptr);
    void setMainWidget(MainWidget *mainWidget);

signals:

public slots:

    /**
     * @brief 当鼠标经过管道后，部件相应的更新信息
     */
    void slotUpdateCurrentInfo();

    /**
     * @brief 当选中管道后，部件相应的更新信息
     */
    void slotUpdateSelectedInfo();

    /**
     * @brief 按下Insert键后，插入管道
     */
    void slotKeyIns();

    /**
     * @brief 按下Delete键后，删除管道
     */
    void slotKeyDel();

private:

    /**
     * @brief 根据所提供的管道id数组，更新信息到小部件PipeWidget中
     * @param ids 管道id数组
     */
    void addWidgets(vector<int> &ids);

    MainWidget *mw;
    vector<PipeWidget *> pips; ///< 存储PipeWidget，目的在于减少new的次数

    int cnt; ///< 当前用了多少个pips中的PipeWidget
};

#endif // PANELWIDGET_H
