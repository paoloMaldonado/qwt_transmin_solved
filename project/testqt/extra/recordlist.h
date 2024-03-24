#include <qlistwidget.h>

class RecordList : public QListWidget
{
	Q_OBJECT

public:
	RecordList(QWidget* = NULL);
	virtual QSize sizeHint() const override;

private:
	static int counter;
};
