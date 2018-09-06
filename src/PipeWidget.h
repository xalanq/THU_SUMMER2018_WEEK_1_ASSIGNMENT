#ifndef PIPEWIDGET_H
#define PIPEWIDGET_H

#include <QWidget>

#include "MainWidget.h"

namespace Ui {
class PipeWidget;
}

/**
 * @brief 显示管道信息的部件，并可以编辑管道信息
 *
 * 可以对管道宽度进行修改，也能显示/隐藏相应管道
 */
class PipeWidget : public QWidget {
    Q_OBJECT

public:
    explicit PipeWidget(MainWidget *mw, QWidget *parent = 0);
    ~PipeWidget();

    /**
     * @brief 设置信息
     * @param id 管道id
     * @param w 管道宽度
     * @param l 管道长度
     * @param checked 管道是否已经显示出来了，true表示已经显示
     * @param speed 管道流速
     * @param concen 管道浓度
     */
    void setData(int id, int w, int l, bool checked, double speed, double concen);

    /**
     * @brief 若当前管道未插入（还是隐藏的），则模拟按下checked选项框
     */
    void ins();

    /**
     * @brief 若当前管道已插入（未隐藏的），则模拟按下checked选项框
     */
    void del();

    /**
     * @brief mw的setter
     * @param mw
     */
    void setMainWidget(MainWidget *mw);

private:
    Ui::PipeWidget *ui;
    MainWidget *mw;

    int id, ///< 管道id
        w; ///< 管道宽度
    bool checked; ///< 管道是否显示，true表示已经显示
};

#endif // PIPEWIDGET_H
