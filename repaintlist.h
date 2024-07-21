#ifndef REPAINTLIST_H
#define REPAINTLIST_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QSize>
#include <QList>
#include <QImage>
#include <QMouseEvent>
#include <QPoint>

class RepaintList : public QWidget
{
    Q_OBJECT
public:
    explicit RepaintList(QWidget *parent = nullptr);
    ~RepaintList();

    void setIconSize(int width, int height);
    void setFont(QFont font);
    void addItem(QString image_file_name, QString text = QString());
    void setItemHSpace(int space);
    void setItemVSpace(int space);
    void itemAt(QPoint p);
    QList<QImage> imageList();
    void removeAt(int i);
    void removeBatch(QList<int> index_list);
    QList<int> currentIndexList();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    QFont font;
    QSize icon_size;
    QList<QImage> image_list;  // 全体列表
    QList<QString> str_list;  // 全体列表
    QList<int> selected_list;  // 选中的列表
    bool is_ctrl_down;
    int line_index;
    int item_hspace;
    int item_vspace;
    QImage opacity_image;
    bool is_opacity_image_show;
    QPoint opacity_image_point;
    int selectAt(QPoint p);
    QList<QImage> removeImageBatch(QList<QImage> img_list, QList<int> remove_list);
    QList<QString> removeStringBatch(QList<QString> str_list, QList<int> remove_list);
};

#endif // REPAINTLIST_H
