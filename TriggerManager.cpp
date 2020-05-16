#include "TriggerManager.h"

TriggerManager::TriggerManager(QWidget* parent) : QGroupBox(parent)
{
}

void TriggerManager::UpdateSelectedObject(GameObject* selectedObject)
{
	this->selectedObject = selectedObject;
}

void TriggerManager::UpdateDirectory(std::string directory)
{
	ProjectDirectory = directory;
}

void TriggerManager::AddTriggerAction(int id, int enterID, int selectedObjectID, Action action, Direction direction, bool newAction)
{
	GameObject* connectedObject = pandaEngine.GetObjectByID(selectedObjectID);
	GameObject* EnteringObject = pandaEngine.GetObjectByID(EnteringObjectBox->currentData().toInt());

	QWidget* widget = new QWidget(this);
	widget->setObjectName("TriggerChangeBox");
	QHBoxLayout* layout = new QHBoxLayout(widget);
	QSize maximumsize = widget->size();

	QLabel* changeLabel = new QLabel();
	changeLabel->setText("Change:");
	layout->addWidget(changeLabel);

	QComboBox* changeBox = new QComboBox();
	changeBox->setObjectName("ChangeBox");
	layout->addWidget(changeBox);

	QLabel* toLabel = new QLabel();
	toLabel->setText("to:");
	layout->addWidget(toLabel);

	QComboBox* directionBox = new QComboBox();
	directionBox->setObjectName("DirectionBox");
	directionBox->addItem("Forward", QVariant::fromValue(Direction::Forward));
	directionBox->addItem("Backward", QVariant::fromValue(Direction::Backward));
	directionBox->addItem("Left", QVariant::fromValue(Direction::Left));
	directionBox->addItem("Right", QVariant::fromValue(Direction::Right));
	directionBox->addItem("Up", QVariant::fromValue(Direction::Up));
	directionBox->addItem("Down", QVariant::fromValue(Direction::Down));
	layout->addWidget(directionBox);
	if (connectedObject->HasTransform()) {
		qDebug() << "Are WE in here?";
		for (int i = 0; i < connectedObject->GetNumberOfActions(); i++) {
			std::ostringstream tag;
			tag << i + 1;
			std::string name;
			if (connectedObject->GetTransformAction(i).type == TransformType::Add) {
				if (connectedObject->GetTransformAction(i).action == Action::Move) {
					name = tag.str() + ". Move";
					changeBox->addItem(QString::fromStdString(name), QVariant::fromValue(Action::Move));
				}
				if (connectedObject->GetTransformAction(i).action == Action::Rotate) {
					name = tag.str() + ". Rotate";
					changeBox->addItem(QString::fromStdString(name), QVariant::fromValue(Action::Rotate));
				}
			}
		}
	}

	directionBox->setCurrentIndex(QVariant::fromValue(direction).toInt());
	directionBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	changeBox->setCurrentIndex(id);
	changeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	this->setLayout(layout);
	this->layout()->addWidget(widget);
	if (newAction) {
		selectedObject->StoreTriggerActions(EnteringObject->id, connectedObject->id, directionBox->currentData().toInt(), changeBox->currentData().toInt(), changeBox->currentIndex());
	}
	else {
		ConnectedObjectBox->setCurrentIndex(selectedObjectID);
		EnteringObjectBox->setCurrentIndex(enterID);
	}
	qDebug() << "Crash here?";
}

void TriggerManager::AddTriggerMoveAction(int enterID, int selectedObjectID, LPoint3 position, bool newAction)
{
	GameObject* connectedObject = pandaEngine.GetObjectByID(selectedObjectID);
	GameObject* enteringObject = pandaEngine.GetObjectByID(enterID);

	QWidget* widget = new QWidget(this);
	widget->setObjectName("TriggerMoveBox");
	QHBoxLayout* layout = new QHBoxLayout(widget);
	QSize maximumsize = widget->size();

	QLabel* moveToLabel = new QLabel();
	moveToLabel->setText("Move To");
	layout->addWidget(moveToLabel);

	QLabel* posX = new QLabel();
	posX->setText("X:");
	posX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(posX);

	QDoubleSpinBox* posXBox = new QDoubleSpinBox();
	posXBox->setMinimum(-1000000);
	posXBox->setMaximum(1000000);
	posXBox->setObjectName("PosXBox");
	posXBox->setMaximumSize(110, 20);
	posXBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	posXBox->setValue(position.get_x());
	layout->addWidget(posXBox);

	QLabel* posY = new QLabel();
	posY->setText("Y:");
	posY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(posY);

	QDoubleSpinBox* posYBox = new QDoubleSpinBox();
	posYBox->setMinimum(-1000000);
	posYBox->setMaximum(1000000);
	posYBox->setObjectName("PosYBox");
	posYBox->setMaximumSize(110, 20);
	posYBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	posYBox->setValue(position.get_y());
	layout->addWidget(posYBox);

	QLabel* posZ = new QLabel();
	posZ->setText("Z:");
	posZ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(posZ);

	QDoubleSpinBox* posZBox = new QDoubleSpinBox();
	posZBox->setMinimum(-1000000);
	posZBox->setMaximum(1000000);
	posZBox->setObjectName("PosZBox");
	posZBox->setMaximumSize(110, 20);
	posZBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	posZBox->setValue(position.get_z());
	layout->addWidget(posZBox);

	widget->setLayout(layout);
	this->layout()->addWidget(widget);

	if (newAction) {
		selectedObject->StoreTriggerMoveTo(enteringObject->id, connectedObject->id, position);
	}
	else {
		ConnectedObjectBox->setCurrentIndex(selectedObjectID);
		EnteringObjectBox->setCurrentIndex(enterID);
	}
}

void TriggerManager::AddTriggerChangeScene(int enterID, int selectedObjectID, std::string newScene, bool newAction)
{
	GameObject* connectedObject = pandaEngine.GetObjectByID(selectedObjectID);
	GameObject* enteringObject = pandaEngine.GetObjectByID(enterID);

	QWidget* widget = new QWidget(this);
	widget->setObjectName("TriggerSceneBox");
	QHBoxLayout* layout = new QHBoxLayout(widget);
	QSize maximumsize = widget->size();

	QLabel* moveToLabel = new QLabel();
	moveToLabel->setText("Move To");
	layout->addWidget(moveToLabel);

	QComboBox* sceneBox = new QComboBox();
	QStringList filter("*.pyr");
	sceneBox->setObjectName("SceneComboBox");
	QDir directory(QString::fromStdString(ProjectDirectory));
	QStringList filelist = directory.entryList(filter);
	for (int i = 0; i < filelist.size(); i++) {
		sceneBox->addItem(filelist[i]);
	}
	layout->addWidget(sceneBox);

	widget->setLayout(layout);
	this->layout()->addWidget(widget);

	if (newAction) {
		selectedObject->StoreTriggerScene(enteringObject->id, connectedObject->id, ProjectDirectory + sceneBox->currentText().toStdString());
	}
	else {
		ConnectedObjectBox->setCurrentIndex(selectedObjectID);
		EnteringObjectBox->setCurrentIndex(enterID);
		int sceneIndex = sceneBox->findText(QString::fromStdString(newScene));
		if (sceneIndex != -1) {
			sceneBox->setCurrentIndex(sceneIndex);
		}
	}
}

void TriggerManager::Initialise(std::string directory)
{
	qDebug() << "Crash Here?"; 
	ConnectedObjectBox = findChild<QComboBox*>("ConnectedObject");
	EnteringObjectBox = findChild<QComboBox*>("EnteringObject");
	ProjectDirectory = directory;
	qDebug() << "Nope";
}

void TriggerManager::LoadTriggerBoxes()
{
	for (int i = 0; i < selectedObject->GetNumberOfTriggerActions(); i++) {
		if (selectedObject->GetTriggerAction(i).type == TriggerType::Change) {
			AddTriggerAction(selectedObject->GetTriggerAction(i).actionID, selectedObject->GetTriggerAction(i).enteringObjectID, selectedObject->GetTriggerAction(i).connectedObjectID, Action(selectedObject->GetTriggerAction(i).newAction), Direction(selectedObject->GetTriggerAction(i).newDirection), false);
		}
		if (selectedObject->GetTriggerAction(i).type == TriggerType::MoveTo) {
			AddTriggerMoveAction(selectedObject->GetTriggerAction(i).enteringObjectID, selectedObject->GetTriggerAction(i).connectedObjectID, selectedObject->GetTriggerAction(i).toPos, false);
		}
		if (selectedObject->GetTriggerAction(i).type == TriggerType::Scene) {
			AddTriggerChangeScene(selectedObject->GetTriggerAction(i).enteringObjectID, selectedObject->GetTriggerAction(i).connectedObjectID, selectedObject->GetTriggerAction(i).newScene, false);
		}
	}
}

void TriggerManager::RemoveTriggerBoxes()
{
	//Find the trigger box
	//Get a list of the widgets attached to the trigger box
	QWidgetList widgets = this->findChildren<QWidget*>("TriggerChangeBox");
	widgets.append(this->findChildren<QWidget*>("TriggerMoveBox"));
	widgets.append(this->findChildren<QWidget*>("TriggerSceneBox"));

	//Remove them all
	for (int i = 0; i < widgets.size(); i++) {
		this->layout()->removeWidget(widgets[i]);
		widgets[i]->hide();
		widgets[i]->setDisabled(true);
		widgets[i]->deleteLater();
		delete widgets[i];
	}
}

void TriggerManager::UpdateActions()
{
	QWidgetList trigWidgets = this->findChildren<QWidget*>("TriggerChangeBox");
	trigWidgets.append(this->findChildren<QWidget*>("TriggerMoveBox"));
	trigWidgets.append(this->findChildren<QWidget*>("TriggerSceneBox"));
	for (int i = 0; i < trigWidgets.count(); i++) {
		//Update the trigger action to user input
		if (selectedObject->GetTriggerAction(i).type == TriggerType::Change) {
			selectedObject->ChangeTriggerAction(
				i,
				EnteringObjectBox->currentIndex(),
				ConnectedObjectBox->currentIndex(),
				trigWidgets[i]->findChild<QComboBox*>("DirectionBox")->currentData().toInt(),
				trigWidgets[i]->findChild<QComboBox*>("ChangeBox")->currentData().toInt(),
				trigWidgets[i]->findChild<QComboBox*>("ChangeBox")->currentIndex()
			);
		}
		if (selectedObject->GetTriggerAction(i).type == TriggerType::MoveTo) {
			selectedObject->ChangeTriggerMoveAction(i,
				EnteringObjectBox->currentIndex(),
				ConnectedObjectBox->currentIndex(),
				LPoint3(trigWidgets[i]->findChild<QDoubleSpinBox*>("PosXBox")->value(),
					trigWidgets[i]->findChild<QDoubleSpinBox*>("PosYBox")->value(),
					trigWidgets[i]->findChild<QDoubleSpinBox*>("PosZBox")->value())
				);
		}
		if (selectedObject->GetTriggerAction(i).type == TriggerType::Scene) {
			QComboBox* SceneComboBox = this->findChild<QComboBox*>("SceneComboBox");
			selectedObject->ChangeTriggerScene(i,
				EnteringObjectBox->currentIndex(),
				ConnectedObjectBox->currentIndex(),
				SceneComboBox->currentText().toStdString());
		}
	}
}


void TriggerManager::AddTrigger() {
	if (selectedObject != nullptr) {
		selectedObject->AddTrigger();
		show();
		qDebug() << "Trigger Added";
	}
}
