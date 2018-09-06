#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>

#include <MainWidget.h>
#include <PanelWidget.h>

namespace Ui {
class MainWindow;
}

/**
 * @brief 主要窗口
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief 根据窗口大小居中
     * @param w 窗口宽度
     * @param h 窗口高度
     */
    void makeCenter(int w, int h);

public slots:

    /**
     * @brief “关于”的槽，弹出一个对话框
     */
    void actionAbout();

    /**
     * @brief 新建一个管道图
     */
    void actionNew();

private:
    Ui::MainWindow *ui;

    MainWidget *mainWidget; ///< 中心部件
    PanelWidget *panelWidget; ///< dock部件
    QDockWidget *panelDock; ///< dock部件
};

#endif // MAINWINDOW_H
