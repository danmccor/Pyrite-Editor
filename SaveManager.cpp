#include "SaveManager.h"

SaveManager::SaveManager(QWidget* main)
{
	mainWindow = main;
}

void SaveManager::New()
{
	QFile saveFile(QFileDialog::getSaveFileName(mainWindow, "Create New Project", "", "Pyrite Project (*.pyr)"));
	QUrl projectDirectory = saveFile.fileName();
	ProjectName = projectDirectory.fileName().toStdString();
	ProjectDirectory = projectDirectory.url(QUrl::RemoveFilename).toStdString();
	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
	}
}

void SaveManager::Save()
{
	if (ProjectDirectory != "") {
		QFile saveFile(QString::fromStdString(ProjectDirectory + ProjectName));
		saveFile.open(QIODevice::WriteOnly);
		QJsonArray GameObjectArray;
		for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
			QJsonObject gameObject;
			gameObject["ObjectID"] = pandaEngine.GetVectorOfGameObjects()[i].id;
			gameObject["ObjectName"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetObjectName());
			gameObject["FileLocation"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetFileLocation());
			gameObject["PositionX"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionX();
			gameObject["PositionY"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionY();
			gameObject["PositionZ"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionZ();
			gameObject["RotationX"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationX();
			gameObject["RotationY"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationY();
			gameObject["RotationZ"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationZ();
			gameObject["Scale"] = pandaEngine.GetVectorOfGameObjects()[i].GetScale();


			gameObject["HasTransform"] = pandaEngine.GetVectorOfGameObjects()[i].HasTransform();
			if (pandaEngine.GetVectorOfGameObjects()[i].HasTransform()) {
				QJsonArray ObjectTranforms;
				for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i].GetNumberOfActions(); j++) {
					QJsonObject transformObjects;
					transformObjects["Action"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).action).toInt();
					transformObjects["Direction"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).direction).toInt();
					transformObjects["Speed"] = pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).Speed;
					transformObjects["Key"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).Key);
					ObjectTranforms.append(transformObjects);
				}
				gameObject["Transforms"] = ObjectTranforms;
			}

			gameObject["HasCollision"] = pandaEngine.GetVectorOfGameObjects()[i].HasCollision();
			if (pandaEngine.GetVectorOfGameObjects()[i].HasCollision()) {
				gameObject["CollisionType"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetCollisionType()).toInt();
			}

			gameObject["HasTrigger"] = pandaEngine.GetVectorOfGameObjects()[i].HasTrigger();
			if (pandaEngine.GetVectorOfGameObjects()[i].HasTrigger()) {
				QJsonArray ObjectTriggers;
				for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i].GetNumberOfTriggerActions(); j++) {
					QJsonObject triggerAction;
					triggerAction["ActionID"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).actionID;
					triggerAction["ConnectedObjectID"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).connectedObjectID;
					triggerAction["newAction"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).newAction;
					triggerAction["newDirection"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).newDirection;
					ObjectTriggers.append(triggerAction);
				}
				gameObject["Triggers"] = ObjectTriggers;
			}
			GameObjectArray.append(gameObject);
		}
		ProjectSave["GameObjects"] = GameObjectArray;
		QJsonDocument saveDoc(ProjectSave);
		saveFile.write(saveDoc.toJson());
	}
	else {
		SaveAs();
	}
}

void SaveManager::SaveAs()
{
	QJsonArray GameObjectArray;
	for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
		QJsonObject gameObject;
		gameObject["ObjectID"] = pandaEngine.GetVectorOfGameObjects()[i].id;
		gameObject["ObjectName"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetObjectName());
		gameObject["FileLocation"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetFileLocation());
		gameObject["PositionX"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionX();
		gameObject["PositionY"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionY();
		gameObject["PositionZ"] = pandaEngine.GetVectorOfGameObjects()[i].GetPositionZ();
		gameObject["RotationX"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationX();
		gameObject["RotationY"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationY();
		gameObject["RotationZ"] = pandaEngine.GetVectorOfGameObjects()[i].GetRotationZ();
		gameObject["Scale"] = pandaEngine.GetVectorOfGameObjects()[i].GetScale();


		gameObject["HasTransform"] = pandaEngine.GetVectorOfGameObjects()[i].HasTransform();
		if (pandaEngine.GetVectorOfGameObjects()[i].HasTransform()) {
			QJsonArray ObjectTranforms;
			for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i].GetNumberOfActions(); j++) {
				QJsonObject transformObjects;
				transformObjects["Action"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).action).toInt();
				transformObjects["Direction"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).direction).toInt();
				transformObjects["Speed"] = pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).Speed;
				transformObjects["Key"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetTransformAction(j).Key);
				ObjectTranforms.append(transformObjects);
			}
			gameObject["Transforms"] = ObjectTranforms;
		}

		gameObject["HasCollision"] = pandaEngine.GetVectorOfGameObjects()[i].HasCollision();
		if (pandaEngine.GetVectorOfGameObjects()[i].HasCollision()) {
			gameObject["CollisionType"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i].GetCollisionType()).toInt();
		}

		gameObject["HasTrigger"] = pandaEngine.GetVectorOfGameObjects()[i].HasTrigger();
		if (pandaEngine.GetVectorOfGameObjects()[i].HasTrigger()) {
			QJsonArray ObjectTriggers;
			for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i].GetNumberOfTriggerActions(); j++) {
				QJsonObject triggerAction;
				triggerAction["ActionID"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).actionID;
				triggerAction["ConnectedObjectID"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).connectedObjectID;
				triggerAction["newAction"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).newAction;
				triggerAction["newDirection"] = pandaEngine.GetVectorOfGameObjects()[i].GetTriggerAction(j).newDirection;
				ObjectTriggers.append(triggerAction);
			}
			gameObject["Triggers"] = ObjectTriggers;
		}
		GameObjectArray.append(gameObject);
	}
	ProjectSave["GameObjects"] = GameObjectArray;
	QFile saveFile(QFileDialog::getSaveFileName(mainWindow, "Save Project", "", "Pyrite Project (*.pyr)"));
	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
	}

	QJsonDocument saveDoc(ProjectSave);
	saveFile.write(saveDoc.toJson());
}

void SaveManager::Load()
{
	QString fileName = QFileDialog::getOpenFileName(mainWindow, "Open Project", "", "Pyrite Project (*.pyr)");
	QFile loadFile(fileName);


	QUrl projectDirectory = loadFile.fileName();
	ProjectDirectory = projectDirectory.url(QUrl::RemoveFilename).toStdString();
	ProjectName = projectDirectory.fileName().toStdString();


	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't Open File");
	}

	QByteArray data = loadFile.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(data));
	QJsonArray GameObjects = loadDoc["GameObjects"].toArray();
	for (int i = 0; i < GameObjects.size(); i++) {
		QJsonObject gameObject = GameObjects[i].toObject();
		GameObject object;
		if (gameObject["FileLocation"].toString().toStdString() != "") {
			object = GameObject(pandaEngine.GetWindow(), gameObject["FileLocation"].toString().toStdString());
			object.LoadModel(&pandaEngine.framework);
		}
		else {
			object = GameObject(pandaEngine.GetWindow(), TriggerType(0));
		}
		object.id = gameObject["ObjectID"].toInt();
		object.SetObjectName(gameObject["ObjectName"].toString().toStdString());
		object.SetPosition(gameObject["PositionX"].toDouble(), gameObject["PositionY"].toDouble(), gameObject["PositionZ"].toDouble());
		object.SetRotation(gameObject["RotationX"].toDouble(), gameObject["RotationY"].toDouble(), gameObject["RotationZ"].toDouble());
		object.SetScale(gameObject["Scale"].toDouble());

		if (gameObject["HasTransform"].toBool()) {
			object.AddTransform();
			QJsonArray transformActions = gameObject["Transforms"].toArray();
			for (int j = 0; j < transformActions.size(); j++) {
				QJsonObject action = transformActions[j].toObject();
				object.AddTransformAction(Action(action["Action"].toInt()), action["Key"].toString().toStdString(), action["Speed"].toDouble(), Direction(action["Direction"].toInt()));
			}
		}
		if (gameObject["HasCollision"].toBool()) {
			object.AddCollision();
			object.SetCollisionType(CollisionType(gameObject["CollisionType"].toInt()));
		}
		if (gameObject["HasTrigger"].toBool()) {
			object.AddTrigger();
			QJsonArray TriggerActions = gameObject["Triggers"].toArray();
			for (int j = 0; j < TriggerActions.size(); j++) {
				QJsonObject action = TriggerActions[j].toObject();
				object.StoreTriggerActions(action["ConnectedObjectID"].toInt(), action["newDirection"].toInt(), action["newAction"].toInt(), action["ActionID"].toInt());
			}
		}
		pandaEngine.addGameObject(object);
	}
}

void SaveManager::CreateBuild()
{
	QDir dir("Resource");
	QStringList fileList = dir.entryList();
	for (int i = 0; i < fileList.size(); i++) {
		QFile file = fileList[i];
		if (file.fileName() != "PyriteEditor.ui" && file.fileName() != "AddAction.h" && file.fileName() != "AddComponent.h" && file.fileName() != "AssetToScene.h" && file.fileName() != "DragAndDrop.h" && file.fileName() != "PyriteEditor.h" &&
			file.fileName() != "PyriteEditor.qrc" && file.fileName() != "AddAction.cpp" && file.fileName() != "AddComponent.cpp" && file.fileName() != "AssetToScene.cpp" && file.fileName() != "DragAndDrop.cpp" && file.fileName() != "PyriteEditor.cpp") {
			qDebug() << file.fileName();
			file.copy(file.fileName(), QString::fromStdString(ProjectDirectory) + file.fileName());
		}
	}
}

void SaveManager::LoadBuild()
{
	QStringList filter("*.pyr");
	QDir directory("");
	QStringList filelist = directory.entryList(filter);

	QFile loadFile(filelist.first());
	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't Open File");
	}

	QByteArray data = loadFile.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(data));
	QJsonArray GameObjects = loadDoc["GameObjects"].toArray();
	for (int i = 0; i < GameObjects.size(); i++) {
		QJsonObject gameObject = GameObjects[i].toObject();
		GameObject object;
		if (gameObject["FileLocation"].toString().toStdString() != "") {
			object = GameObject(pandaEngine.GetWindow(), gameObject["FileLocation"].toString().toStdString());
			object.LoadModel(&pandaEngine.framework);
		}
		else {
			object = GameObject(pandaEngine.GetWindow(), TriggerType(0));
		}
		object.id = gameObject["ObjectID"].toInt();
		object.SetObjectName(gameObject["ObjectName"].toString().toStdString());
		object.SetPosition(gameObject["PositionX"].toDouble(), gameObject["PositionY"].toDouble(), gameObject["PositionZ"].toDouble());
		object.SetRotation(gameObject["RotationX"].toDouble(), gameObject["RotationY"].toDouble(), gameObject["RotationZ"].toDouble());
		object.SetScale(gameObject["Scale"].toDouble());

		if (gameObject["HasTransform"].toBool()) {
			object.AddTransform();
			QJsonArray transformActions = gameObject["Transforms"].toArray();
			for (int j = 0; j < transformActions.size(); j++) {
				QJsonObject action = transformActions[j].toObject();
				object.AddTransformAction(Action(action["Action"].toInt()), action["Key"].toString().toStdString(), action["Speed"].toDouble(), Direction(action["Direction"].toInt()));
			}
		}
		if (gameObject["HasCollision"].toBool()) {
			object.AddCollision();
			object.SetCollisionType(CollisionType(gameObject["CollisionType"].toInt()));
		}
		if (gameObject["HasTrigger"].toBool()) {
			object.AddTrigger();
			QJsonArray TriggerActions = gameObject["Triggers"].toArray();
			for (int j = 0; j < TriggerActions.size(); j++) {
				QJsonObject action = TriggerActions[j].toObject();
				object.StoreTriggerActions(action["ConnectedObjectID"].toInt(), action["newDirection"].toInt(), action["newAction"].toInt(), action["ActionID"].toInt());
			}
		}
		pandaEngine.addGameObject(object);
		pandaEngine.GetVectorOfGameObjects()[i].GetTriggerNodePath().hide();
		pandaEngine.GetVectorOfGameObjects()[i].GetColNodePath().hide();
	}

}

std::string SaveManager::GetProjectDirectory()
{
	return ProjectDirectory;
}

std::string SaveManager::GetProjectName()
{
	return ProjectName;
}

QJsonObject SaveManager::GetProjectSave()
{
	return ProjectSave;
}
