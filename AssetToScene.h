#pragma once
#include <qdockwidget.h>
#include <QtGui>
#include <qlist.h>
#include <qfileiconprovider.h>
#include "Panda3D.h"
//#include "ui_PyriteEditor.h"

extern Panda3D pandaEngine;

class AssetToScene : public QDockWidget
{
public:
	AssetToScene(QWidget* parent = 0);
	void dragEnterEvent(QDragEnterEvent* ev);
	void dragMoveEvent(QDragMoveEvent* ev);
	void dragLeaveEvent(QDragLeaveEvent* ev);
	void dropEvent(QDropEvent* ev);
	void ChangeDirectory(std::string directory);

private:
	QDockWidget* dockWidget;
	QString fileName;
	QString suffix;


	std::string ProjectDirectory = "";
};

