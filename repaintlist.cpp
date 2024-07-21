#include "repaintlist.h"
#include <QFontMetrics>
#include <QVariant>
#include <QFileInfo>
#include <QDebug>
#include <math.h>

RepaintList::RepaintList(QWidget *parent) : QWidget(parent) {
    this->icon_size.setWidth(50);
    this->icon_size.setHeight(50);
    this->font = QFont("宋体", 10);
    this->item_hspace = 0;
    this->item_vspace = 0;
    this->line_index = -1;
    this->is_opacity_image_show = false;
    this->opacity_image_point = QPoint(0, 0);
    this->is_ctrl_down = false;
    this->setFocusPolicy(Qt::ClickFocus);
}

RepaintList::~RepaintList() {}

void RepaintList::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setFont(this->font);
    QFontMetrics fm(p.font());  // 获取字体尺寸
    int image_x = 0;  // x像素
    int image_y = -1;  // 图片数量
    int fm_height = fm.height();
    int icon_width = this->icon_size.width();
    int icon_height = this->icon_size.height();
    int list_len = this->image_list.length();
    int ctrl_width = this->width();
    int max_img_line = ctrl_width / (icon_width + this->item_hspace);
    int i = 0;
    for (; i < list_len; ) {
        QImage img = this->image_list.at(i);
        QString str = this->str_list.at(i);
        if (i % max_img_line == 0) {
            image_x = this->item_hspace;
            ++image_y;
        }
        p.setClipRect(image_x - this->item_hspace / 2,
                      image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                      icon_width + this->item_hspace / 2,
                      icon_height + fm_height + 5);
        if (i == this->line_index) {
            p.setBrush(QColor(0, 0, 0));
            p.drawRect(image_x - this->item_hspace / 2,
                       image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                       2,
                       icon_height + fm_height + 5);
        }
        for (int j : this->selected_list) {
            if (i == j) {
                p.setClipRect(image_x,
                              image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                              icon_width,
                              icon_height + fm_height + 5);
                p.setBrush(QColor(0, 0, 255));
                p.drawRect(image_x,
                           image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                           icon_width,
                           icon_height + fm_height + 5);
            }
        }
        p.drawImage(image_x,
                    image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                    img.scaled(icon_width, icon_height));
        p.setPen(QColor(0, 0, 0));
        p.drawText(image_x,  // 文字是从下往上画的
                   image_y * (icon_height + fm_height) + fm_height + icon_height + image_y * this->item_vspace,
                   str);
        image_x += icon_width + this->item_hspace;
        ++i;
        if (i == this->line_index) {
            p.setClipRect(image_x + this->item_hspace / 2,
                          image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                          2,
                          icon_height + fm_height + 5);
            p.setBrush(QColor(0, 0, 0));
            p.drawRect(image_x + this->item_hspace / 2,
                       image_y * (icon_height + fm_height) + image_y * this->item_vspace,
                       2,
                       icon_height + fm_height + 5);
        }
    }
    if (this->is_opacity_image_show) {
        p.setClipRect(0, 0, this->width(), this->height());
        p.setOpacity(0.5);
        QPoint point = this->opacity_image_point;
        point.setX(point.x() - icon_width / 2 - this->item_hspace);
        point.setY(point.y() - icon_height / 2);
        p.drawImage(point, this->opacity_image.scaled(icon_width, icon_height));
    }

    int max_img_column = ceil((double)(list_len) / (double)(max_img_line));
    this->setMinimumHeight(max_img_column * (icon_height + this->item_vspace + fm_height));  // 滚动条必须
}

void RepaintList::mousePressEvent(QMouseEvent *e) {
    this->itemAt(e->pos());
}

void RepaintList::mouseMoveEvent(QMouseEvent *e) {
    if (this->selected_list.length() > 0) {
        QPoint p = e->pos();
        p.setX(p.x() + this->icon_size.width() / 2);
        this->line_index = this->selectAt(p);
        this->opacity_image_point = e->pos();
        this->update();
    }
}

void RepaintList::mouseReleaseEvent(QMouseEvent *) {
    if (this->selected_list.length() > 0 && this->line_index != -1 &&
            this->image_list.length() >= this->selected_list.length()) {
        QList<QImage> sel_img_list;
        QList<QString> sel_str_list;
        int left_len = 0;
        for (int i : this->selected_list) {
            if (i < this->line_index) {
                ++left_len;
            }
            sel_img_list.append(this->image_list.at(i));
            sel_str_list.append(this->str_list.at(i));
        }
        this->line_index -= left_len;
        this->image_list = this->removeImageBatch(this->image_list, this->selected_list);
        this->str_list = this->removeStringBatch(this->str_list, this->selected_list);
        int sel_len = this->selected_list.length();
        for (int i = 0; i < sel_len; ++i) {
            this->image_list.insert(this->line_index + i, sel_img_list.at(i));
            this->str_list.insert(this->line_index + i, sel_str_list.at(i));
        }
        this->selected_list.clear();
        for (int i = 0; i < sel_len; ++i) {
            this->selected_list.append(this->line_index + i);
        }
    }
    this->is_opacity_image_show = false;
    this->opacity_image_point = QPoint(0, 0);
    this->line_index = -1;
    this->update();
}

void RepaintList::setFont(QFont font) {
    this->font = font;
}

void RepaintList::setIconSize(int width, int height) {
    this->icon_size.setWidth(width);
    this->icon_size.setHeight(height);
    this->update();
}

void RepaintList::addItem(QString image_file_name, QString text) {
    this->image_list.append(QImage(image_file_name));
    if (text.length() == 0) {
        QString file_name = image_file_name.split("/").last();
        this->str_list.append(file_name);
    } else {
        this->str_list.append(text);
    }
}

void RepaintList::setItemHSpace(int space) {
    this->item_hspace = space;
}

void RepaintList::setItemVSpace(int space) {
    this->item_vspace = space;
}

void RepaintList::itemAt(QPoint p) {
    int sel_at = this->selectAt(p);
    if (sel_at >= this->image_list.length()) {
        this->selected_list.clear();
        this->update();
        return;
    }
    if (!this->selected_list.contains(sel_at) &&
            sel_at < this->image_list.length()) {
        if (this->is_ctrl_down) {
            this->selected_list.append(sel_at);
        } else {
            this->selected_list.clear();
            this->selected_list.append(sel_at);
        }
        this->update();
    }
}

int RepaintList::selectAt(QPoint p) {
    int x = p.x();
    int y = p.y();
    QFontMetrics fm(this->font);
    int fm_height = fm.height();
    int icon_width = this->icon_size.width();
    int icon_height = this->icon_size.height();
    int index_height = icon_height + fm_height + 5;
    int item_x = x / (icon_width + this->item_hspace);
    int item_y = y / index_height;
    int line_max = this->width() / (icon_width + this->item_hspace);
    int at = item_y * line_max + item_x;
    if (at > this->image_list.length()) {
        at = this->image_list.length();
    }
    return at;
}

QList<QImage> RepaintList::imageList() {
    return this->image_list;
}

void RepaintList::removeAt(int i) {
    if (i >= 0 && i < this->image_list.length()) {
        this->image_list.removeAt(i);
        this->str_list.removeAt(i);
    }
    this->selected_list.clear();
    this->update();
}

QList<int> RepaintList::currentIndexList() {
    return this->selected_list;
}

void RepaintList::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Control) {
        this->is_ctrl_down = true;
    }
}

void RepaintList::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Control) {
        this->is_ctrl_down = false;
    }
}

QList<QImage> RepaintList::removeImageBatch(QList<QImage> img_list, QList<int> remove_list) {
    QList<QImage> ret;
    int list_len = img_list.length();
    for (int i = 0; i < list_len; ++i) {
        if (!remove_list.contains(i)) {
            ret.append(img_list.at(i));
        }
    }
    return ret;
}

QList<QString> RepaintList::removeStringBatch(QList<QString> str_list, QList<int> remove_list) {
    QList<QString> ret;
    int list_len = str_list.length();
    for (int i = 0; i < list_len; ++i) {
        if (!remove_list.contains(i)) {
            ret.append(str_list.at(i));
        }
    }
    return ret;
}

void RepaintList::removeBatch(QList<int> index_list) {
    QList<QImage> img_list;
    QList<QString> str_list;
    int list_len = this->image_list.length();
    for (int i = 0; i < list_len; ++i) {
        if (!index_list.contains(i)) {
            img_list.append(this->image_list.at(i));
            str_list.append(this->str_list.at(i));
        }
    }
    this->image_list = img_list;
    this->str_list = str_list;
    this->update();
}
