#include "SaveManager.h"

SaveManager::SaveManager(QWidget* main)
{
	mainWindow = main;
}

void SaveManager::New()
{
	//Create a new save file
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
	//if the project directory is not null
	if (ProjectDirectory != "") {
		//Create the save file
		QFile saveFile(QString::fromStdString(ProjectDirectory + ProjectName));
		//Open the save file as write only
		saveFile.open(QIODevice::WriteOnly);
		//Create a JSON array
		QJsonArray GameObjectArray;
		//Loop through all game objects
		for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
			//Create a game object
			QJsonObject gameObject;
			//Assign all attributes
			gameObject["ObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->id;
			gameObject["ObjectName"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetObjectName());
			gameObject["FileLocation"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetFileLocation());
			gameObject["PositionX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionX();
			gameObject["PositionY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionY();
			gameObject["PositionZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionZ();
			gameObject["RotationX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationX();
			gameObject["RotationY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationY();
			gameObject["RotationZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationZ();
			gameObject["ScaleX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleX();
			gameObject["ScaleY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleY();
			gameObject["ScaleZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleZ();
			gameObject["IsCamera"] = pandaEngine.GetVectorOfGameObjects()[i]->IsCamera();
			gameObject["HasTransform"] = pandaEngine.GetVectorOfGameObjects()[i]->HasTransform();
			//If the object has a transform component
			if (pandaEngine.GetVectorOfGameObjects()[i]->HasTransform()) {
				//Create a new array
				QJsonArray ObjectTranforms;
				//Loop through all transform actions
				for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i]->GetNumberOfActions(); j++) {
					//Create an object
					QJsonObject transformObjects;
					//Get the transform variables
					transformObjects["Type"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).type).toInt();
					transformObjects["Action"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).action).toInt();
					transformObjects["Direction"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).direction).toInt();
					transformObjects["Axis"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).axis).toInt();
					transformObjects["ConnectedID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).ConnectedObject;
					transformObjects["Speed"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).Speed;
					transformObjects["Key"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).Key);
					//Append the transform object to the transform array
					ObjectTranforms.append(transformObjects);
				}
				//Add the transform array to the game object
				gameObject["Transforms"] = ObjectTranforms;
			}

			//Repeat for collision
			gameObject["HasCollision"] = pandaEngine.GetVectorOfGameObjects()[i]->HasCollision();
			if (pandaEngine.GetVectorOfGameObjects()[i]->HasCollision()) {
				gameObject["CollisionType"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetCollisionType()).toInt();
				gameObject["CantPushObjects"] = pandaEngine.GetVectorOfGameObjects()[i]->GetCollisionPushSetting();
			}
			//Repeat for Trigger
			gameObject["HasTrigger"] = pandaEngine.GetVectorOfGameObjects()[i]->HasTrigger();
			if (pandaEngine.GetVectorOfGameObjects()[i]->HasTrigger()) {
				QJsonArray ObjectTriggers;
				for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i]->GetNumberOfTriggerActions(); j++) {
					QJsonObject triggerAction;
					triggerAction["ActionID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).actionID;
					triggerAction["EnteringObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).enteringObjectID;
					triggerAction["ConnectedObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).connectedObjectID;
					triggerAction["newAction"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).newAction;
					triggerAction["newDirection"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).newDirection;
					ObjectTriggers.append(triggerAction);
				}
				gameObject["Triggers"] = ObjectTriggers;
			}
			//Add the game object to the game object array
			GameObjectArray.append(gameObject);
		}
		//Set the project save
		ProjectSave["GameObjects"] = GameObjectArray;
		//Save document to a file
		QJsonDocument saveDoc(ProjectSave);
		//Write to the file
		saveFile.write(saveDoc.toJson());
	}
	//If the project directory is empty
	else {
		//Save the project as rather than just saving
		SaveAs();
	}
}

void SaveManager::SaveAs()
{
	//Repeat save with added windows controls to choose a new save location
	QJsonArray GameObjectArray;
	for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
		QJsonObject gameObject;
		gameObject["ObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->id;
		gameObject["ObjectName"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetObjectName());
		gameObject["FileLocation"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetFileLocation());
		gameObject["PositionX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionX();
		gameObject["PositionY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionY();
		gameObject["PositionZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetPositionZ();
		gameObject["RotationX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationX();
		gameObject["RotationY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationY();
		gameObject["RotationZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetRotationZ();
		gameObject["ScaleX"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleX();
		gameObject["ScaleY"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleY();
		gameObject["ScaleZ"] = pandaEngine.GetVectorOfGameObjects()[i]->GetScaleZ();
		gameObject["IsCamera"] = pandaEngine.GetVectorOfGameObjects()[i]->IsCamera();

		gameObject["HasTransform"] = pandaEngine.GetVectorOfGameObjects()[i]->HasTransform();
		if (pandaEngine.GetVectorOfGameObjects()[i]->HasTransform()) {
			QJsonArray ObjectTranforms;
			for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i]->GetNumberOfActions(); j++) {
				QJsonObject transformObjects;
				transformObjects["Type"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).type).toInt();
				transformObjects["Action"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).action).toInt();
				transformObjects["Direction"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).direction).toInt();
				transformObjects["Axis"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).axis).toInt();
				transformObjects["ConnectedID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).ConnectedObject;
				transformObjects["Speed"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).Speed;
				transformObjects["Key"] = QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetTransformAction(j).Key);
				ObjectTranforms.append(transformObjects);
			}
			gameObject["Transforms"] = ObjectTranforms;
		}

		gameObject["HasCollision"] = pandaEngine.GetVectorOfGameObjects()[i]->HasCollision();
		if (pandaEngine.GetVectorOfGameObjects()[i]->HasCollision()) {
			gameObject["CollisionType"] = QVariant::fromValue(pandaEngine.GetVectorOfGameObjects()[i]->GetCollisionType()).toInt();
			gameObject["CantPushObjects"] = pandaEngine.GetVectorOfGameObjects()[i]->GetCollisionPushSetting();
		}

		gameObject["HasTrigger"] = pandaEngine.GetVectorOfGameObjects()[i]->HasTrigger();
		if (pandaEngine.GetVectorOfGameObjects()[i]->HasTrigger()) {
			QJsonArray ObjectTriggers;
			for (int j = 0; j < pandaEngine.GetVectorOfGameObjects()[i]->GetNumberOfTriggerActions(); j++) {
				QJsonObject triggerAction;
				triggerAction["ActionID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).actionID;
				triggerAction["EnteringObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).enteringObjectID;
				triggerAction["ConnectedObjectID"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).connectedObjectID;
				triggerAction["newAction"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).newAction;
				triggerAction["newDirection"] = pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerAction(j).newDirection;
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
	//Reverse engineer the save function
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
		GameObject* object;
		if (gameObject["FileLocation"].toString().toStdString() != "") {
			if (!gameObject["IsCamera"].toBool()) {
				object = new GameObject(pandaEngine.GetWindow(), gameObject["FileLocation"].toString().toStdString());
				object->LoadModel(&pandaEngine.framework);
			}
			else {
				object = new GameCamera(pandaEngine.GetWindow());
				object->LoadModel(&pandaEngine.framework);
			}
		}
		else {
			object = new GameObject(pandaEngine.GetWindow(), TriggerShape(0));
		}
		object->id = gameObject["ObjectID"].toInt();
		object->SetObjectName(gameObject["ObjectName"].toString().toStdString());
		object->SetPosition(gameObject["PositionX"].toDouble(), gameObject["PositionY"].toDouble(), gameObject["PositionZ"].toDouble());
		object->SetRotation(gameObject["RotationX"].toDouble(), gameObject["RotationY"].toDouble(), gameObject["RotationZ"].toDouble());
		object->SetScale(gameObject["ScaleX"].toDouble(), gameObject["ScaleY"].toDouble(), gameObject["ScaleZ"].toDouble());

		if (gameObject["HasTransform"].toBool()) {
			object->AddTransform();
			QJsonArray transformActions = gameObject["Transforms"].toArray();
			for (int j = 0; j < transformActions.size(); j++) {
				QJsonObject action = transformActions[j].toObject();
				if (action["Type"] == 0) {
					object->AddTransformAction(Action(action["Action"].toInt()), action["Key"].toString().toStdString(), action["Speed"].toDouble(), Direction(action["Direction"].toInt()));
				}
				if (action["Type"] == 1) {
					object->AddTranformFollowAction(TransformAxis(action["Axis"].toInt()), action["ConnectedID"].toInt(), action["Key"].toString().toStdString(), action["Speed"].toDouble());
				}
			}
		}
		if (gameObject["HasCollision"].toBool()) {
			object->AddCollision();
			object->SetCollisionType(CollisionType(gameObject["CollisionType"].toInt()), gameObject["CantPushObjects"].toBool());
		}
		if (gameObject["HasTrigger"].toBool()) {
			object->AddTrigger();
			QJsonArray TriggerActions = gameObject["Triggers"].toArray();
			for (int j = 0; j < TriggerActions.size(); j++) {
				QJsonObject action = TriggerActions[j].toObject();
				object->StoreTriggerActions(action["EnteringObjectID"].toInt(), action["ConnectedObjectID"].toInt(), action["newDirection"].toInt(), action["newAction"].toInt(), action["ActionID"].toInt());
			}
		}
		pandaEngine.AddGameObject(object);
	}
}

void SaveManager::CreateBuild()
{
	QDir* dir = new QDir(QCoreApplication::applicationDirPath() + "/compiler/");

	QStringList fileList = dir->entryList();
	std::string stringOutput = "Files found: " + std::to_string(fileList.size());
	QMessageBox* filePathAlert = new QMessageBox{ QMessageBox::Warning, QCoreApplication::applicationDirPath() + "/compiler/", QString::fromStdString(stringOutput), QMessageBox::Ok };
	filePathAlert->show();
	for (int i = 0; i < fileList.size(); i++) {
		QFile* file = new QFile(fileList[i]);
		QMessageBox* Failed;
		file->setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther);
		if (!file->copy(dir->path() + QDir::separator() + file->fileName(), QString::fromStdString(ProjectDirectory) + file->fileName())) {
			Failed = new QMessageBox(QMessageBox::Warning, dir->path() + QDir::separator() + file->fileName(), file->errorString() + QString::fromStdString(ProjectDirectory) + file->fileName(), QMessageBox::Ok);
			Failed->show();
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
		GameObject* object;
		if (gameObject["FileLocation"].toString().toStdString() != "") {
			if (!gameObject["IsCamera"].toBool()) {
				object = new GameObject(pandaEngine.GetWindow(), gameObject["FileLocation"].toString().toStdString());
				object->LoadModel(&pandaEngine.framework);
			}
			else {
				object = new GameCamera(pandaEngine.GetWindow());
				object->LoadModel(&pandaEngine.framework);
				object->ActivateCamera();
			}
		}
		else {
			object = new GameObject(pandaEngine.GetWindow(), TriggerShape(0));
		}
		object->id = gameObject["ObjectID"].toInt();
		object->SetObjectName(gameObject["ObjectName"].toString().toStdString());
		object->SetPosition(gameObject["PositionX"].toDouble(), gameObject["PositionY"].toDouble(), gameObject["PositionZ"].toDouble());
		object->SetRotation(gameObject["RotationX"].toDouble(), gameObject["RotationY"].toDouble(), gameObject["RotationZ"].toDouble());
		object->SetScale(gameObject["ScaleX"].toDouble(), gameObject["ScaleY"].toDouble(), gameObject["ScaleZ"].toDouble());

		if (gameObject["HasTransform"].toBool()) {
			object->AddTransform();
			QJsonArray transformActions = gameObject["Transforms"].toArray();
			for (int j = 0; j < transformActions.size(); j++) {
				QJsonObject action = transformActions[j].toObject();
				if (action["Type"] == 0) {
					object->AddTransformAction(Action(action["Action"].toInt()), action["Key"].toString().toStdString(), action["Speed"].toDouble(), Direction(action["Direction"].toInt()));
				}
				if (action["Type"] == 1) {
					object->AddTranformFollowAction(TransformAxis(action["Axis"].toInt()), action["ConnectedID"].toInt(), action["Key"].toString().toStdString(), action["Speed"].toDouble());
				}
			}
		}
		if (gameObject["HasCollision"].toBool()) {
			object->AddCollision();
			object->SetCollisionType(CollisionType(gameObject["CollisionType"].toInt()), gameObject["CantPushObjects"].toBool());
		}
		if (gameObject["HasTrigger"].toBool()) {
			object->AddTrigger();
			QJsonArray TriggerActions = gameObject["Triggers"].toArray();
			for (int j = 0; j < TriggerActions.size(); j++) {
				QJsonObject action = TriggerActions[j].toObject();
				object->StoreTriggerActions(action["EnteringObjectID"].toInt(), action["ConnectedObjectID"].toInt(), action["newDirection"].toInt(), action["newAction"].toInt(), action["ActionID"].toInt());
			}
		}
		pandaEngine.AddGameObject(object);
		pandaEngine.GetVectorOfGameObjects()[i]->GetTriggerNodePath().hide();
		pandaEngine.GetVectorOfGameObjects()[i]->GetColNodePath().hide();
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
