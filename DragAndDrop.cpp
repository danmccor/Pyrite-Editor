#include "DragAndDrop.h"

DragAndDrop::DragAndDrop(QWidget* parent) : QListWidget(parent)
{
	setDragDropMode(QAbstractItemView::DropOnly);
	setAcceptDrops(true);

	QDir dir("Assets");
	QStringList files = dir.entryList(QStringList(), QDir::Files);
	foreach(QString filename, files) {
		AddItem(filename);
	}
}

void DragAndDrop::dragEnterEvent(QDragEnterEvent* ev)
{
	if (ev->mimeData()->hasUrls()) {
		ev->acceptProposedAction();
	}
}

void DragAndDrop::dragMoveEvent(QDragMoveEvent* ev)
{
	ev->acceptProposedAction();
}

void DragAndDrop::dragLeaveEvent(QDragLeaveEvent* ev)
{
	ev->accept();
}

void DragAndDrop::dropEvent(QDropEvent* ev)
{
	foreach(const QUrl & url, ev->mimeData()->urls()) {
		QUrl fileDir = url.toLocalFile();
		QString fileDirStr = fileDir.toString();
		QString filename = fileDir.fileName();
		QFile file = fileDir.toLocalFile();

		if (!QDir("Assets").exists()) {
			QDir().mkdir("Assets");
		}
		QString dir = "Assets/" + filename;
		if (QFile::exists(dir)) {
			QFile::remove(dir);
		}
		file.copy(fileDir.toString(), dir);

		QList<QListWidgetItem*> items = findItems(filename, Qt::MatchExactly);
		if (items.isEmpty()) {
			AddItem(filename);
		}
		else {
			items.takeFirst();
		}

	}
}

void DragAndDrop::AddItem(QString filename)
{
	QString dir = "Assets/" + filename;
	QFileIconProvider* provider = new QFileIconProvider;
	QListWidgetItem* item = new QListWidgetItem(filename);
	item->setIcon(provider->icon(dir));
	addItem(item);
}
