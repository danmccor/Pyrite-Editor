#include "DragAndDrop.h"

DragAndDrop::DragAndDrop(QWidget* parent) : QListWidget(parent)
{
	setDragDropMode(QAbstractItemView::DropOnly);
	setAcceptDrops(true);

	LoadFiles();
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
	//For each file dragged in
	foreach(const QUrl & url, ev->mimeData()->urls()) {
		//Break up file into url and file name
		QUrl fileDir = url.toLocalFile();
		QString fileDirStr = fileDir.toString();
		QString filename = fileDir.fileName();
		QFile file = fileDir.toLocalFile();
		//If assets folder does not exist
		if (!QDir(QString::fromStdString(ProjectDirectory + "Assets")).exists()) {
			//Create assets folder
			QDir().mkdir(QString::fromStdString(ProjectDirectory + "Assets"));
		}
		//If this file already exists
		QString dir = QString::fromStdString(ProjectDirectory + "Assets/") + filename;
		if (QFile::exists(dir)) {
			//Delete file
			QFile::remove(dir);
		}
		//Copy new file into assets 
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
	//Set assets
	QString dir = "Assets/" + filename;
	//Get icon of the file
	QFileIconProvider* provider = new QFileIconProvider;
	QListWidgetItem* item = new QListWidgetItem(filename);
	//Set the icon of list widget to the file type icon
	item->setIcon(provider->icon(dir));
	addItem(item);
}

void DragAndDrop::ChangeDirectory(std::string dir)
{

	ProjectDirectory = dir;
	this->clear();
	LoadFiles();
}

void DragAndDrop::LoadFiles()
{
	//Set directory
	QDir dir(QString::fromStdString(ProjectDirectory + "Assets/"));
	QStringList files = dir.entryList(QStringList(), QDir::Files);
	//Load all files from that directory.
	foreach(QString filename, files) {
		AddItem(filename);
	}
}
