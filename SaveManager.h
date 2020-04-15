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

	void NewProject();
	void LoadProject();
	void SaveProject();

	void New();
	void Save();
	void SaveAs();
	void Load(std::string firstScene = "");

	void CreateBuild();

	void LoadBuild();
	void LoadBuildScene(std::string sceneName);
	
	std::string GetProjectDirectory();
	std::string GetProjectName();
	QJsonObject GetProjectSave();

private:
	QWidget* mainWindow;
	QJsonObject ProjectSave;
	std::string ProjectDirectory = "";
	std::string ProjectName = "";

	std::string OpenScene = "";

	Q_ENUM(Direction);
	Q_ENUM(Action);
	Q_ENUM(CollisionType);
	Q_ENUM(TransformType);
	Q_ENUM(TransformAxis);
	Q_ENUM(TriggerType);
};

