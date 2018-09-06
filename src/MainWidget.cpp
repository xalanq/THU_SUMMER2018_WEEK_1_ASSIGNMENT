#include <QBrush>
#include <QImage>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "Core.h"
#include "MainWidget.h"

// const int MainWidget::PIPE_WIDTH = 200;
// const int MainWidget::PIPE_LENGTH = 1600;
const int MainWidget::PIPE_HEIGHT = 200;
const int MainWidget::PIPE_LENGTH_PIXEL = 100;

MainWidget::MainWidget(int n, int i1, int i2, int o1, int o2, int o3, int length, int width, double speed, double concen1, double concen2, QWidget *parent) :
    QWidget(parent) {

    this->PIPE_WIDTH = width;
    this->PIPE_LENGTH = length;
    this->speed = speed;
    this->concen1 = concen1;
    this->concen2 = concen2;

    int w = toPixel(PIPE_LENGTH) * (n + 1) + toPixel(PIPE_WIDTH) * n + 40;

    setMinimumSize(w - PIPE_LENGTH_PIXEL / 2, w);
    setMouseTracking(true);

    initData(n, i1, i2, o1, o2, o3);
}

double MainWidget::toPixel(double x) {
    return x * PIPE_LENGTH_PIXEL / PIPE_LENGTH;
}

QRectF MainWidget::toPixel(const QRectF &r) {
    auto tl = r.topLeft();
    auto br = r.bottomRight();
    tl.setX(toPixel(tl.x()));
    tl.setY(toPixel(tl.y()) + 20);
    br.setX(toPixel(br.x()));
    br.setY(toPixel(br.y()) + 20);
    return QRectF(tl, br);
}

void MainWidget::initData(int n, int i1, int i2, int o1, int o2, int o3) {
    this->n = n;
    this->i1 = i1;
    this->i2 = i2;
    this->o1 = o1;
    this->o2 = o2;
    this->o3 = o3;

    int num = 2 * n * n - 2 * n;
    tot = num + 5;

    pips = new Pipe[tot];
    nodes = new Node[n * n];

    // node
    for (int i = 0; i < n * n; ++i)
        nodes[i].w = PIPE_WIDTH;

    for (int i = 0; i < n * n - n; i += n - 1) {
        for (int j = 0; j < n - 1; ++j) {
            int id = i + j;
            int x = i / (n - 1);
            int y1 = j, y2 = j + 1;
            nodes[x * n + y1].adj.push_back(id);
            nodes[x * n + y2].adj.push_back(id);
        }
    }

    for (int i = n * n - n; i < 2 * (n * n - n); i += n) {
        for (int j = 0; j < n; ++j) {
            int id = i + j;
            int x1 = (i - n * n + n) / n, x2 = x1 + 1;
            int y = j;
            nodes[x1 * n + y].adj.push_back(id);
            nodes[x2 * n + y].adj.push_back(id);
        }
    }

    nodes[i1 * n].adj.push_back(num);
    nodes[i2 * n].adj.push_back(num + 1);
    nodes[o1 * n + n - 1].adj.push_back(num + 2);
    nodes[o2 * n + n - 1].adj.push_back(num + 3);
    nodes[o3 * n + n - 1].adj.push_back(num + 4);

    // hide
    for (int i = 0; i < num; ++i)
        pips[i].hide = true;

    // vertical
    for (int i = 0; i < n * n - n; ++i)
        pips[i].vertical = true;
    for (int i = 0; i < 5; ++i)
        pips[num + i].vertical = true;

    // adj
    for (int i = 0; i < n * n - n - (n - 1); i += n - 1) {
        for (int j = 0; j < n - 1; ++j) {
            int a = i + j;
            int b = a + (n - 1);
            pips[a].next = b;
            pips[b].prev = a;
        }
    }
    for (int i = n * n - n; i < 2 * n * n - 2 * n; i += n) {
        for (int j = 0; j < n - 1; ++j) {
            int a = i + j;
            int b = a + 1;
            pips[a].next = b;
            pips[b].prev = a;
        }
    }
    pips[num].next = num + 1;

    pips[num + 1].prev = num;

    pips[num + 2].next = num + 3;

    pips[num + 3].prev = num + 2;
    pips[num + 3].next = num + 4;

    pips[num + 4].prev = num + 3;

    // prop
    for (int i = 0; i < tot; ++i) {
        Pipe &p = pips[i];
        p.w = PIPE_WIDTH;
        p.h = PIPE_HEIGHT;
        p.l = PIPE_LENGTH;
    }

    // x, y
    auto calxy1 = [this](int x) -> int {
        return PIPE_LENGTH * (x + 1) + PIPE_WIDTH * x + PIPE_WIDTH / 2;
    };

    auto calxy2 = [this](int x) -> int {
        return (PIPE_LENGTH + PIPE_WIDTH) * (x + 1) + PIPE_LENGTH / 2;
    };

    pips[num].x = calxy1(i1);
    pips[num + 1].x = calxy1(i2);
    pips[num + 2].x = calxy1(o1);
    pips[num + 3].x = calxy1(o2);
    pips[num + 4].x = calxy1(o3);

    pips[num].y = pips[num + 1].y = calxy2(-1);
    pips[num + 2].y = pips[num + 3].y = pips[num + 4].y = calxy2(n - 1);

    for (int i = 0; i < n * n - n; ++i) {
        pips[i].x = calxy1(i / (n - 1));
        pips[i].y = calxy2(i % (n - 1));
    }

    for (int b = n * n - n, i = b; i < 2 * n * n - 2 * n; ++i) {
        pips[i].x = calxy2((i - b) / n);
        pips[i].y = calxy1((i - b) % n);
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            nodes[i * n + j].x = calxy1(i),
            nodes[i * n + j].y = calxy1(j);

    // debug
    /*
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            qDebug("id: %d, i: %d, j: %d, (%d, %d)", i * n + j, i, j, nodes[i * n + j].x, nodes[i * n + j].y);
            for (auto k : nodes[i * n + j].adj)
                qDebug("adj: %d", k);
        }
    for (int i = 0; i < tot; ++i) {
        Pipe &p = pips[i];
        qDebug("id: %d, x: %d, y: %d, prev: %d, next: %d", i, p.x, p.y, p.prev, p.next);
    }
    */
}

bool MainWidget::checkWidth(int id, int w) {
    int i;
    for (i = pips[id].prev; i != -1 && pips[i].hide; i = pips[i].prev);
    bool ans = (i == -1 || !((w + pips[i].w) / 2 >= abs(pips[id].x - pips[i].x) + abs(pips[id].y - pips[i].y) + PIPE_WIDTH));
    for (i = pips[id].next; i != -1 && pips[i].hide; i = pips[i].next);
    ans = ans && (i == -1 || !((w + pips[i].w) / 2 >= abs(pips[id].x - pips[i].x) + abs(pips[id].y - pips[i].y) + PIPE_WIDTH));
    return ans;
}

MainWidget::~MainWidget() {
    delete[] pips;
    delete[] nodes;
}

MainWidget::Pipe& MainWidget::pipeById(int id) {
    if (id >= tot) {
        qDebug("error, id:%d, tot:%d", id, tot);
        exit(1);
    }
    return pips[id];
}

void MainWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QPen pen(Qt::NoPen);
    QPen pen1(Qt::gray, 2);
    QPen pen2(Qt::blue, 2);
    QPen pen3(Qt::red, 2);

    p.setPen(pen1);
    p.setBrush(Qt::NoBrush);
    for (int i = 0; i < tot; ++i)
        if (!pips[i].current && !pips[i].selected && pips[i].hide)
            p.drawRect(toPixel(pips[i].rect()));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (calSize(i, j) < 2)
                p.drawRect(toPixel(nodes[i * n + j].rect()));

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::green);
    for (int i = 0; i < tot; ++i)
        if (!pips[i].current && !pips[i].selected && !pips[i].hide) {
            p.drawRect(toPixel(pips[i].rect()));
            drawArrow(p, i);
        }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (calSize(i, j) >= 2)
                p.drawRect(toPixel(nodes[i * n + j].rect()));

    p.setPen(pen2);
    p.setBrush(Qt::NoBrush);
    for (int i = 0; i < tot; ++i)
        if (pips[i].current && pips[i].hide)
            p.drawRect(toPixel(pips[i].rect()));

    p.setPen(pen2);
    p.setBrush(Qt::green);
    for (int i = 0; i < tot; ++i)
        if (pips[i].current && !pips[i].hide) {
            p.drawRect(toPixel(pips[i].rect()));
            drawArrow(p, i);
        }

    p.setPen(pen3);
    p.setBrush(Qt::NoBrush);
    for (int i = 0; i < tot; ++i)
        if (pips[i].selected && pips[i].hide)
            p.drawRect(toPixel(pips[i].rect()));

    p.setPen(pen3);
    p.setBrush(Qt::green);
    for (int i = 0; i < tot; ++i)
        if (pips[i].selected && !pips[i].hide) {
            p.drawRect(toPixel(pips[i].rect()));
            drawArrow(p, i);
        }

    /*
    qDebug("(%d, %d)", size().width(), size().height());
    for (int i = 0; i < tot; ++i)
        qDebug("id: %d, (%d, %d) - (%d, %d)", i - (tot - 5), toPixel(pips[i].rect()).topLeft().x(), toPixel(pips[i].rect()).topLeft().y(), toPixel(pips[i].rect()).bottomRight().x(), toPixel(pips[i].rect()).bottomRight().y());
    */
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
    if (selected.size()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    auto t = getID(event->x(), event->y());
    bool equal = t.size() == current.size();
    for (int i = 0; equal && i < int(t.size()); ++i)
        if (t[i] != current[i])
            equal = false;
    if (!equal) {
        for (auto i : current)
            pips[i].current = false;
        current = t;
        for (auto i : current)
            pips[i].current = true;
        repaint();
        emit signalCurrentChanged();
    }
}

void MainWidget::mousePressEvent(QMouseEvent *event) {
    auto t = getID(event->x(), event->y());
    bool equal = t.size() == selected.size();
    for (int i = 0; equal && i < int(t.size()); ++i)
        if (t[i] != selected[i])
            equal = false;
    if (!equal) {
        for (auto i : selected)
            pips[i].selected = false;
        selected = t;
        for (auto i : selected)
            pips[i].selected = true;
        for (auto i : current)
            pips[i].current = false;
        current.clear();
        repaint();
        emit signalSelectedChanged();
    }
}

vector<int> MainWidget::getID(int x, int y) {
    vector<int> ans;
    for (int i = 0; i < tot; ++i) {
        auto tl = toPixel(pips[i].rect()).topLeft();
        auto br = toPixel(pips[i].rect()).bottomRight();
        if (tl.x() <= x && x <= br.x() && tl.y() <= y && y <= br.y())
            ans.push_back(i);
    }
    return ans;
}

int MainWidget::calSize(int x, int y) {
    int id = x * n + y;
    int cnt = 0;
    for (auto i : nodes[id].adj)
        if (!pips[i].hide)
            ++cnt;
    return cnt;
}

void MainWidget::drawArrow(QPainter &painter, int id) {
    static QImage l(":/img/arrow_left.png");
    static QImage r(":/img/arrow_right.png");
    static QImage u(":/img/arrow_up.png");
    static QImage d(":/img/arrow_down.png");
    if (pips[id].speed == 0)
        return;
    double x = toPixel(pips[id].x), y = toPixel(pips[id].y);
    QImage *p = nullptr;
    if (pips[id].vertical) {
        if (pips[id].speed > 0)
            p = &d;
        else
            p = &u;
    } else {
        if (pips[id].speed > 0)
            p = &r;
        else
            p = &l;
    }
    painter.drawImage(QRectF(x - p->width() / 2.0, y - p->height() / 2.0 + 20, p->width(), p->height()), *p);
}

void MainWidget::cal() {
    vector<double> length;
    for (int i = 0; i < tot; ++i) {
        Pipe &p = pips[i];
        if (p.hide)
            length.push_back(0);
        else
            length.push_back((double)p.l * p.w / PIPE_WIDTH);
    }
    vector<double> s, c, a;
    for (int i = 0; i < tot; ++i)
        a.push_back(pips[i].h * pips[i].w);

    Core::work(n, length, a, i1, i2, o1, o2, o3, speed, concen1, concen2, s, c);

    for (int i = 0; i < tot; ++i)
        pips[i].speed = s[i];
    for (int i = 0; i < tot; ++i)
        pips[i].concen = c[i];
}
