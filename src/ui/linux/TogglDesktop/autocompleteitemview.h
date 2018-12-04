#ifndef AUTOCOMPLETEITEMVIEW_H
#define AUTOCOMPLETEITEMVIEW_H

#include <QListView>
#include <QListWidget>
#include <QKeyEvent>
#include <QMutex>
#include <QItemDelegate>

#include "./autocompleteview.h"

class AutocompleteDropdownList : public QListView
{
    Q_OBJECT
public:
    explicit AutocompleteDropdownList(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e) override;

signals:
    void keyPress(QKeyEvent *e);
    void returnPressed();

public slots:
    void onItemClicked(const QModelIndex &index);

protected:
    void keyPressEvent(QKeyEvent *e);

};

class AutoCompleteItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AutoCompleteItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString format(const AutocompleteView *view) const;
};

#endif // AUTOCOMPLETEITEMVIEW_H
