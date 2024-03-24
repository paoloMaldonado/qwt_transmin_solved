#include "extra/recordlist.h"

int RecordList::counter = 0;
 
RecordList::RecordList(QWidget* parent) :
	QListWidget(parent)
{
	this->setStyleSheet(
        "color: black;"
        "background-color: gray;"
        "selection-background-color: rgb(222,222,222);"
        "selection-color: black;");
}

QSize RecordList::sizeHint() const
{
    QSize s(0, 0);
    int h = QListWidget::sizeHint().height();
    int w = this->sizeHintForColumn(0);
    s.setHeight(h);
    s.setWidth(w);
    return s;
}
