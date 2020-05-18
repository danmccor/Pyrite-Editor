#include "AssetToScene.h"
#include "PyriteEditor.h"

AssetToScene::AssetToScene(QWidget* parent) : QDockWidget(parent)
{
	setAcceptDrops(true);
}

void AssetToScene::dragEnterEvent(QDragEnterEvent* ev)
{
	//Get Format for models
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
		//Check if file is .egg, pz, mp3 or wav
		if (file.suffix() == "egg" || file.suffix() == "pz" || file.suffix() == "mp3" || file.suffix() == "wav") {
			suffix = file.suffix();
			if (suffix == "pz") {
				suffix = "egg.pz";
			}
			//Accept drop if it is
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
	//Reconstruct file name
	std::string file = ProjectDirectory + "Assets/" + fileName.toStdString() + "." + suffix.toStdString();
	Filename pandafile = Filename::from_os_specific(file);

	//If the file ended in egg or egg.pz
	if (suffix == "egg" || suffix == "egg.pz") {
		//Add Object to list of objects
		((PyriteEditor*)parentWidget())->AddObjectToObjectList(pandaEngine.CreateObject(pandafile, fileName.toStdString()));
	}
	//if mp3 or wav
	else if (suffix == "mp3" || suffix == "wav") {
		//Set the music of the level
		pandaEngine.SetMusic(pandafile);
	}
}

void AssetToScene::ChangeDirectory(std::string directory)
{
	ProjectDirectory = directory;
}

