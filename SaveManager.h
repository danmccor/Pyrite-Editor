#pragma once
#include <qjsonobject.h>
#include <qcoreapplication.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qvariant.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qthread.h>

#include "Panda3D.h"

extern Panda3D pandaEngine;

class SaveManager : QObject
{
	Q_OBJECT

public:
	SaveManager(QWidget* main);

	void New();
	void Save();
	void SaveAs();
	void Load();

	void CreateBuild();

	void LoadBuild();

	
	std::string GetProjectDirectory();
	std::string GetProjectName();
	QJsonObject GetProjectSave();

private:
	QWidget* mainWindow;
	QJsonObject ProjectSave;
	std::string ProjectDirectory = "";
	std::string ProjectName = "";

	Q_ENUM(Direction);
	Q_ENUM(Action);
	Q_ENUM(CollisionType);
	Q_ENUM(TransformType);
	Q_ENUM(TransformAxis);
	Q_ENUM(TriggerType);
};

