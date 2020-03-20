#pragma once
#include <QtWidgets/qlistwidget.h>
#include <QtGui>
#include <qlist.h>
#include <qfileiconprovider.h>

class DragAndDrop : public QListWidget
{
public:
	DragAndDrop(QWidget* parent = 0);
	void dragEnterEvent(QDragEnterEvent* ev);
	void dragMoveEvent(QDragMoveEvent* ev);
	void dragLeaveEvent(QDragLeaveEvent* ev);
	void dropEvent(QDropEvent* ev);

	void AddItem(QString filename);

private:
	QListWidget* listWidget;
};

