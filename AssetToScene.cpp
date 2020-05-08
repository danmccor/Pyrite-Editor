#include "AssetToScene.h"
#include "PyriteEditor.h"

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
		if (file.suffix() == "egg" || file.suffix() == "pz" || file.suffix() == "mp3" || file.suffix() == "wav") {
			suffix = file.suffix();
			if (suffix == "pz") {
				suffix = "egg.pz";
			}
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
	std::string file = ProjectDirectory + "Assets/" + fileName.toStdString() + "." + suffix.toStdString();
	Filename pandafile = Filename::from_os_specific(file);

	if (suffix == "egg" || suffix == "egg.pz") {
		((PyriteEditor*)parentWidget())->AddObjectToObjectList(pandaEngine.CreateObject(pandafile, fileName.toStdString()));
		OutputDebugStringA(pandafile.c_str());
	}
	else if (suffix == "mp3" || suffix == "wav") {
		pandaEngine.SetMusic(pandafile);
		OutputDebugStringA(pandafile.c_str());
	}
}

void AssetToScene::ChangeDirectory(std::string directory)
{
	ProjectDirectory = directory;
}

