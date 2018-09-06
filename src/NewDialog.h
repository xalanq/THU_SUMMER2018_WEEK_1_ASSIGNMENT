#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

namespace Ui {
class NewDialog;
}

/**
 * @brief 新建一个管道图的引导窗口
 */
class NewDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0);
    ~NewDialog();

    /**
     * @brief 从部件中获取到数值，管道列数排序后存储进参数中
     * @param n 网格边长
     * @param i1 1号输入口所在列
     * @param i2 2号输入口所在列
     * @param o1 1号输出口所在列
     * @param o2 2号输出口所在列
     * @param o3 3号输出口所在列
     * @param length 管道初始长度
     * @param width 管道初始宽度
     * @param speed 管道初始速度
     * @param concen1 1号输入口初始浓度
     * @param concen2 2号输入口初始浓度
     */
    void setData(int &n, int &i1, int &i2, int &o1, int &o2, int &o3, int &length, int &width, double &speed, double &concen1, double &concen2);

public slots:

    /**
     * @brief 当管道边长改变后，输入、输出口的可选列表也要改变
     * @param s 管道边长
     */
    void slotUpdate(QString s);

protected:

    /**
     * @brief 只有当合法的时候才能接受
     *
     * 判断输入管道、输出管道是否有重复的，没有重复的才能接受
     */
    void accept() override;

private:
    Ui::NewDialog *ui;
};

#endif // NEWDIALOG_H
