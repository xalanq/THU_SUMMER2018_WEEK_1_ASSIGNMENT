#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <vector>

#include <QPainter>
#include <QRectF>
#include <QWidget>

using std::vector;

/**
 * @brief 绘制管道的部件
 */
class MainWidget : public QWidget {
    Q_OBJECT
public:

    /**
     * @brief 管道类
     */
    struct Pipe {
        int w, ///< 宽度
            h, ///< 高度
            l; ///< 长度
        int x, ///< 中心点的x坐标
            y; ///< 中心点的y坐标
        int prev, ///< 前一个相邻管道的id
            next; ///< 后一个相邻管道的id
        double speed; ///< 流速
        double concen; ///< 浓度
        bool hide; ///< true表示隐藏
        bool vertical; ///< true表示竖直
        bool selected; ///< true表示选中
        bool current; ///< true表示光标在上边

        Pipe() : w(0), h(0), l(0), x(0), y(0), prev(-1), next(-1), speed(0), concen(0), hide(false), vertical(false), selected(false), current(false) {}

        /**
         * @brief 根据坐标、长、宽计算在二维平面里的管道矩形
         * @return 管道矩形
         */
        QRectF rect() const {
            if (vertical)
                return QRectF(x - w / 2.0, y - l / 2.0, w, l);
            else
                return QRectF(x - l / 2.0, y - w / 2.0, l, w);
        }
    };

    /**
     * @brief 管道相交的节点类
     */
    struct Node {
        int w; ///< 宽度
        int x, ///< 中心点的x坐标
            y; ///< 中心点的y坐标
        vector<int> adj; /// 相邻的管道id

        Node() : w(0), x(0), y(0) {}

        /**
         * @brief 根据坐标、长、宽计算在二维平面里的矩形
         */
        QRectF rect() const {
            return QRectF(x - w / 2.0, y - w / 2.0, w, w);
        }
    };

    /**
     * @brief 构造函数
     * @param n 网格边长
     * @param i1 1号输入口所在列
     * @param i2 2号输入口所在列
     * @param o1 1号输出口所在列
     * @param o2 2号输出口所在列
     * @param o3 3号输出口所在列
     * @param length 管道初始长度
     * @param width 管道初始宽度
     * @param speed 管道初始速度
     * @param concen1 1号输入口的浓度
     * @param concen2 2号输入口的浓度
     * @param parent 父部件
     *
     * 部件的宽度根据length、width、n来计算得出
     */
    explicit MainWidget(int n, int i1, int i2, int o1, int o2, int o3, int length, int width, double speed, double concen1, double concen2, QWidget *parent = nullptr);
    ~MainWidget();

    /**
     * @brief 将平面上的坐标转换为屏幕上的坐标
     * @param x 平面上的坐标数值
     * @return 转换后的坐标数值
     */
    double toPixel(double x);

    /**
     * @brief 将平面上的矩形转换成屏幕上的矩形
     * @param r 平面上的矩形
     * @return 转换后的矩形
     */
    QRectF toPixel(const QRectF &r);

    /**
     * @brief 初始化一切管道、节点数据
     * @param n 网格边长
     * @param i1 1号输入口所在列
     * @param i2 2号输入口所在列
     * @param o1 1号输出口所在列
     * @param o2 2号输出口所在列
     * @param o3 3号输出口所在列
     */
    void initData(int n, int i1, int i2, int o1, int o2, int o3);

    /**
     * @brief 检查id管道宽度变化为w后，是否合法
     * @param id 管道id
     * @param w 变化后的宽度
     * @return 合法返回true；否则返回false
     *
     * 分别遍历prev和next，找到第一个hide为false的管道
     */
    bool checkWidth(int id, int w);

    /**
     * @brief 返回id管道的引用
     * @param id 管道id
     * @return 管道引用
     */
    Pipe& pipeById(int id);

    /**
     * @brief 计算所有管道的流速
     *
     * 通过调用core.h里的方法来计算 \n
     * 宽度变化对应到长度的变化上去
     */
    void cal();

signals:
    /**
     * @brief 当选中管道变化时，发出信号
     */
    void signalSelectedChanged();

    /**
     * @brief 当鼠标经过管道变化时，发出信号
     */
    void signalCurrentChanged();

protected:
    /**
     * @brief 绘制整个widget
     * @param event 事件
     *
     * 按照顺序来绘制 \n
     * 层叠优先级：灰色边框 < 绿色填充 < 蓝色边框 < 红色边框
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 重载鼠标经过的事件，定位相应的管道id，且存入current数组中
     * @param event 事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 重载鼠标按下的事件，定位相应的管道id，且存入selected数组中（会清空掉current数组）
     * @param event 事件
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 获取屏幕(x, y)位置上的管道id序列
     * @param x x坐标
     * @param y y坐标
     * @return 管道id序列
     *
     * 通过遍历每个管道所在平面上的矩形来判断
     */
    vector<int> getID(int x, int y);

    /**
     * @brief 计算位于x列y行的节点相邻可见管道的个数
     * @param x x列
     * @param y y行
     * @return 可见管道个数
     */
    int calSize(int x, int y);

    /**
     * @brief drawArrow 绘制箭头
     * @param p painter
     * @param id 管道id
     */
    void drawArrow(QPainter &p, int id);

    int PIPE_LENGTH; ///< 管道初始长度
    int PIPE_WIDTH; ///< 管道初始宽度
    static const int PIPE_HEIGHT; ///< 管道初始高度
    static const int PIPE_LENGTH_PIXEL; ///< 一根管道长度所占屏幕像素点个数
    double speed; ///< 输入口初始流速
    double concen1; ///< 输入口1的浓度
    double concen2; ///< 输入口2的浓度

    Pipe* pips; ///< 管道数组
    Node* nodes; ///< 节点数组
    int n, ///< 网格边长
        i1, ///< 1号输入口所在列
        i2, ///< 2号输入口所在列
        o1, ///< 1号输出口所在列
        o2, ///< 2号输出口所在列
        o3, ///< 3号输出口所在列
        tot; ///< tot表示管道总数

public:
    vector<int> current; ///< 当前光标所在位置上的管道id数组
    vector<int> selected; ///< 鼠标按下选中后的管道id数组

};

#endif // MAINWIDGET_H
