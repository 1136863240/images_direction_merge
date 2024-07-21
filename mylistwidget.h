#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QMouseEvent>
#include <QListWidget>
#include <QPoint>
#include <QIcon>
#include <QListWidgetItem>
#include <QEvent>

class MyListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MyListWidget(QWidget *w = nullptr);
    ~MyListWidget();
    QListWidgetItem* currentItem();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
//    bool event(QEvent *event) override;
//    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QPoint sel_item_pos;
    QPoint move_item_pos;
    QListWidgetItem *sel_item;
    void paint();
};

#endif // MYLISTWIDGET_H
