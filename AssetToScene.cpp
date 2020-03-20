#include "AssetToScene.h"

AssetToScene::AssetToScene(QWidget* parent) : QDockWidget(parent)
{
	setAcceptDrops(true);
}

void AssetToScene::dragEnterEvent(QDragEnterEvent* ev)
{
	bool allEGG = true;
	if (ev->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
		QByteArray data = ev->mimeData()->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&data, QIODevice::ReadOnly);
		QStringList name;
		while (!stream.atEnd()){
			int row, col;
			QMap<int, QVariant> roleDataMap;
			stream >> row >> col >> roleDataMap;
			name << roleDataMap[Qt::DisplayRole].toString();
		}
		QString filename = name.join("");
		QFileInfo file(filename);
		fileName = file.baseName();
		if (file.suffix() == "egg" || file.suffix() == "pz") {
			ev->acceptProposedAction();
		}
	}
}

void AssetToScene::dragMoveEvent(QDragMoveEvent* ev)
{
	ev->acceptProposedAction();
}

void AssetToScene::dragLeaveEvent(QDragLeaveEvent* ev)
{
	ev->accept();
}

void AssetToScene::dropEvent(QDropEvent* ev)
{
	pandaEngine.createObject(fileName.toStdString());
}
