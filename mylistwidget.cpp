#include "mylistwidget.h"
#include <QDebug>
#include <QPainter>

MyListWidget::MyListWidget(QWidget *w) : QListWidget(w) {
    this->sel_item_pos = QPoint(0, 0);
    this->move_item_pos = QPoint(0, 0);
    this->sel_item = nullptr;
    this->installEventFilter(this);
}

MyListWidget::~MyListWidget() {}

void MyListWidget::mousePressEvent(QMouseEvent *e) {
    this->sel_item_pos = e->pos();
}

void MyListWidget::mouseReleaseEvent(QMouseEvent *e) {
    this->move_item_pos = e->pos();
    QListWidgetItem *sel_item = this->itemAt(this->sel_item_pos);
    QListWidgetItem *move_item = this->itemAt(this->move_item_pos);
    if (sel_item != nullptr && move_item != nullptr) {
        QString str1 = sel_item->text();
        QString str2 = move_item->text();
        QIcon icon1 = sel_item->icon();
        QIcon icon2 = move_item->icon();
        this->itemAt(this->sel_item_pos)->setText(str2);
        this->itemAt(this->move_item_pos)->setText(str1);
        this->itemAt(this->sel_item_pos)->setIcon(icon2);
        this->itemAt(this->move_item_pos)->setIcon(icon1);
        this->itemAt(this->move_item_pos)->setSelected(true);
        this->sel_item = this->itemAt(this->move_item_pos);
    }
    this->sel_item_pos = QPoint(0, 0);
    this->move_item_pos = QPoint(0, 0);
}

QListWidgetItem* MyListWidget::currentItem() {
    return this->sel_item;
}

bool MyListWidget::eventFilter(QObject *object, QEvent *event) {
    QPainter p(this);
    p.fillRect(100, 0, 100, 100, QColor(0, 0, 0));
    return QListWidget::eventFilter(object, event);
}
