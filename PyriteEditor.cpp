#include "PyriteEditor.h"
#include "AddComponent.h"

PyriteEditor::PyriteEditor(QWidget* parent)
	: QMainWindow(parent)
{
	//Set up the ui 
	ui.setupUi(this);

	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	if (TransformBox != nullptr) {
		QPushButton* Button = TransformBox->findChild<QPushButton*>("AddMovement");
		if (Button != nullptr) {
			connect(Button, SIGNAL(clicked()), this, SLOT(AddActionBox()));
		}
		TransformBox->hide();
	}
	QAction* action = findChild<QAction*>("actionRun_Program");
	connect(action, SIGNAL(triggered()), this, SLOT(RunGame()));

	QAction* addWorldTrigger = findChild<QAction*>("actionSquare");
	connect(addWorldTrigger, SIGNAL(triggered()), this, SLOT(AddWorldTrigger()));

	QAction* saveAction = findChild<QAction*>("actionSave");
	connect(saveAction, SIGNAL(triggered()), this, SLOT(Save()));

	////Create menu for button
	AddComponent* menu = new AddComponent(ui.pushButton, this);

	//Add actions for menu
	QAction* addTransform = menu->addAction("Add Transform");
	connect(addTransform, SIGNAL(triggered()), this, SLOT(AddTransform()));

	QAction* addCollision = menu->addAction("Add Collision");
	connect(addCollision, SIGNAL(triggered()), this, SLOT(AddCollision()));

	QAction* addTrigger = menu->addAction("Add Trigger");
	connect(addTrigger, SIGNAL(triggered()), this, SLOT(AddTrigger()));

	QGroupBox* CollisionBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("CollisionBox");
	QComboBox* CollisionType = CollisionBox->findChild<QComboBox*>("CollisionType");
	CollisionType->addItem("Box", QVariant::fromValue(CollisionType::Box));
	CollisionType->addItem("Sphere", QVariant::fromValue(CollisionType::Sphere));
	CollisionType->addItem("Polygon", QVariant::fromValue(CollisionType::Polygon));

	QGroupBox* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("TriggerBox");
	QPushButton* AddTriggerButton = TriggerBox->findChild<QPushButton*>("AddTriggerAction");
	connect(AddTriggerButton, SIGNAL(clicked()), this, SLOT(AddTriggerAction()));


	//attach menu to button
	ui.pushButton->setMenu(menu);
}


//Add transform box to Component Window
void PyriteEditor::AddTransform() {
	if (selectedObject != nullptr && !selectedObject->HasTransform()) {
		AddAction* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<AddAction*>("TransformBox");
		if (TransformBox != nullptr) {
			TransformBox->show();
		}
		selectedObject->AddTransform();
	}
}

//Add collision box to Component Window
void PyriteEditor::AddCollision()
{
	if (selectedObject != nullptr && !selectedObject->HasCollision()) {
		QGroupBox* CollisionBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("CollisionBox");
		if (CollisionBox != nullptr) {
			CollisionBox->show();
		}
		selectedObject->AddCollision();
		selectedObject->SetCollisionType(CollisionType::Box);
	}
}


//Add trigger box to Component Window
void PyriteEditor::AddTrigger()
{
	if (selectedObject != nullptr && !selectedObject->HasTrigger()) {
		QGroupBox* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("TriggerBox");
		if (TriggerBox != nullptr) {
			TriggerBox->show();
		}
		selectedObject->AddTrigger();
	}
}

void PyriteEditor::RunGame()
{
	isEditing = false;
}

void PyriteEditor::LoadSelectedObject()
{
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QWidget* CollisionBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("CollisionBox");
	QWidget* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TriggerBox");
	if (selectedObject == nullptr && oneTimeSelect) {
		if (TransformBox != nullptr) {
			RemoveActionBoxes();
			TransformBox->hide();
		}
		if (CollisionBox != nullptr) {
			CollisionBox->hide();
		}
		if (TriggerBox != nullptr) {
			TriggerBox->hide();
			RemoveTriggerBoxes();
		}
		oneTimeSelect = false;
	}
	else if (selectedObject != nullptr) {
		if (selectedObject->HasTransform()) {
			if (TransformBox != nullptr) {
				TransformBox->show();
				LoadActionBoxes();
			}
		}
		else {
			TransformBox->hide();
		}
		//IF OBJECT HAS COLLISION
		if (selectedObject->HasCollision()) {
			CollisionBox->show();
			CollisionType col = selectedObject->GetCollisionType();
			CollisionBox->findChild<QComboBox*>("CollisionType")->setCurrentIndex(QVariant::fromValue(col).toInt());
		}
		else {
			CollisionBox->hide();
		}
		if (selectedObject->HasTrigger()) {
			TriggerBox->show();
			LoadTriggerBoxes();

		}
		else {
			TriggerBox->hide();
		}
	}
}

void PyriteEditor::AddActionBox(Action action, std::string key, float speed, Direction direction, bool newAction)
{
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QLayoutItem* Button = TransformBox->layout()->takeAt(TransformBox->layout()->count() - 1);

	QWidget* widget = new QWidget(TransformBox);
	widget->setObjectName("ActionBox");
	QHBoxLayout* layout = new QHBoxLayout(widget);
	QSize maximumsize = widget->size();

	QLabel* actionLabel = new QLabel();
	actionLabel->setText("Action:");
	layout->addWidget(actionLabel);


	QComboBox* actionBox = new QComboBox();
	actionBox->setObjectName("ActionDropDown");
	actionBox->addItem("Move", QVariant::fromValue(Action::Move));
	actionBox->addItem("Rotate", QVariant::fromValue(Action::Rotate));
	actionBox->setCurrentIndex(QVariant::fromValue(action).toInt());
	actionBox->adjustSize();
	layout->addWidget(actionBox);


	QLabel* directionLabel = new QLabel();
	directionLabel->setText("Direction:");
	layout->addWidget(directionLabel);


	QComboBox* directionBox = new QComboBox();
	directionBox->setObjectName("DirectionDropDown");
	directionBox->addItem("Forward", QVariant::fromValue(Direction::Forward));
	directionBox->addItem("Backward", QVariant::fromValue(Direction::Backward));
	directionBox->addItem("Left", QVariant::fromValue(Direction::Left));
	directionBox->addItem("Right", QVariant::fromValue(Direction::Right));
	directionBox->addItem("Up", QVariant::fromValue(Direction::Up));
	directionBox->addItem("Down", QVariant::fromValue(Direction::Down));
	directionBox->setCurrentIndex(QVariant::fromValue(direction).toInt());
	directionBox->adjustSize();
	layout->addWidget(directionBox);


	QLabel* speedLabel = new QLabel();
	speedLabel->setText("Speed:");
	speedLabel->adjustSize();
	layout->addWidget(speedLabel);


	QDoubleSpinBox* speedBox = new QDoubleSpinBox();
	speedBox->setObjectName("SpeedBox");
	speedBox->setSingleStep(0.1);
	speedBox->setMaximumSize(80, 20);
	speedBox->setMinimumSize(80, 20);
	speedBox->setValue(speed);
	layout->addWidget(speedBox);


	QLabel* keyLabel = new QLabel();
	keyLabel->setText("Key:");
	keyLabel->adjustSize();
	layout->addWidget(keyLabel);


	QLineEdit* lineEditBox = new QLineEdit();
	lineEditBox->setObjectName("KeyBox");
	lineEditBox->setMaximumSize(80, 20);
	lineEditBox->setMaxLength(1);
	lineEditBox->setText(QString::fromStdString(key));
	layout->addWidget(lineEditBox);


	widget->setLayout(layout);
	TransformBox->layout()->addWidget(widget);
	TransformBox->layout()->addItem(Button);
	if (newAction) {
		selectedObject->AddTransformAction(Action(actionBox->currentData().toInt()), lineEditBox->text().toStdString(), speedBox->value(), Direction(directionBox->currentData().toInt()));
	}
}

void PyriteEditor::RemoveActionBoxes()
{
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
	for (int i = 0; i < widgets.size(); i++) {
		TransformBox->layout()->removeWidget(widgets[i]);
		widgets[i]->hide();
		widgets[i]->setDisabled(true);
		widgets[i]->deleteLater();
		delete widgets[i];
	}
}

void PyriteEditor::LoadActionBoxes()
{
	for (int i = 0; i < selectedObject->GetNumberOfActions(); i++) {
		AddActionBox(selectedObject->GetTransformAction(i).action, selectedObject->GetTransformAction(i).Key, selectedObject->GetTransformAction(i).Speed, selectedObject->GetTransformAction(i).direction, false);
	}
}

void PyriteEditor::SetObjectProperties(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scale)
{
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->setValue(posX);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->setValue(posY);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->setValue(posZ);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->setValue(rotX);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->setValue(rotY);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->setValue(rotZ);
	this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->setValue(scale);
}

void PyriteEditor::SetObjectProperties(GameObject* gameObject)
{
	if (gameObject != nullptr) {
		if (gameObject != selectedObject) {
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->setText(QString::fromStdString(gameObject->GetObjectName()));
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->setValue(gameObject->GetPositionX());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->setValue(gameObject->GetPositionY());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->setValue(gameObject->GetPositionZ());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->setValue(gameObject->GetRotationX());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->setValue(gameObject->GetRotationY());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->setValue(gameObject->GetRotationZ());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->setValue(gameObject->GetScale());
			selectedObject = gameObject;
			RemoveActionBoxes();
			RemoveTriggerBoxes();
			oneTimeSelect = true;
			LoadSelectedObject();
		}
		else {
			selectedObject->SetObjectName(findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->text().toStdString());
			selectedObject->SetPosition((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->value());

			selectedObject->SetRotation((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->value());

			selectedObject->SetScale((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->value());
		}
	}
	else {
		selectedObject = nullptr;
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->setText("");
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->setReadOnly(true);
		LoadSelectedObject();
	}
}

void PyriteEditor::UpdateComponents()
{
	QGroupBox* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("TriggerBox");
	QComboBox* ConnectedObjectBox = TriggerBox->findChild<QComboBox*>("ConnectedObject");
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
	for (int i = 0; i < widgets.count(); i++) {
		selectedObject->ChangeTransformAction(i,
			Action(widgets[i]->findChild<QComboBox*>("ActionDropDown")->currentData().toInt()),
			widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
			widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value(),
			Direction(widgets[i]->findChild<QComboBox*>("DirectionDropDown")->currentData().toInt())
		);
	}
	QWidgetList trigWidgets = TriggerBox->findChildren<QWidget*>("TriggerChangeBox");
	for (int i = 0; i < trigWidgets.count(); i++) {
		selectedObject->ChangeTriggerAction(
			i,
			ConnectedObjectBox->currentIndex(),
			trigWidgets[i]->findChild<QComboBox*>("DirectionBox")->currentData().toInt(),
			trigWidgets[i]->findChild<QComboBox*>("ChangeBox")->currentData().toInt(),
			trigWidgets[i]->findChild<QComboBox*>("ChangeBox")->currentIndex()
		);
	}

	if (selectedObject != nullptr) {
		QWidget* CollisionBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("CollisionBox");
		QComboBox* CollisionTypeBox = CollisionBox->findChild<QComboBox*>("CollisionType");
		if (selectedObject->HasCollision()) {
			if (lastCollisionType != CollisionTypeBox->currentData().toInt()) {
				selectedObject->ChangeCollisionType(CollisionType(CollisionTypeBox->currentData().toInt()));
				lastCollisionType = CollisionTypeBox->currentData().toInt();
			}
		}
	}
	else {
		ConnectedObjectBox->clear();
		for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
			ConnectedObjectBox->addItem(QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i].GetObjectName()), i);
		}
	}
}

void PyriteEditor::AddTriggerAction(int id, int selectedObjectID, Action action, Direction direction, bool newAction)
{
	QGroupBox* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QGroupBox*>("TriggerBox");
	QComboBox* ConnectedObjectBox = TriggerBox->findChild<QComboBox*>("ConnectedObject");
	GameObject connectedObject = pandaEngine.GetVectorOfGameObjects()[ConnectedObjectBox->currentData().toInt()];

	QWidget* widget = new QWidget(TriggerBox);
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

	if (connectedObject.HasTransform()) {
		for (int i = 0; i < connectedObject.GetNumberOfActions(); i++) {
			std::ostringstream tag;
			tag << i + 1;
			std::string name;
			if (connectedObject.GetTransformAction(i).action == Action::Move) {
				name = tag.str() + ". Move";
				changeBox->addItem(QString::fromStdString(name), QVariant::fromValue(Action::Move));
			}
			if (connectedObject.GetTransformAction(i).action == Action::Rotate) {
				name = tag.str() + ". Rotate";
				changeBox->addItem(QString::fromStdString(name), QVariant::fromValue(Action::Rotate));
			}
		}
	}

	directionBox->setCurrentIndex(QVariant::fromValue(direction).toInt());
	changeBox->setCurrentIndex(id);
	widget->setLayout(layout);
	TriggerBox->layout()->addWidget(widget);
	if (newAction) {
		selectedObject->StoreTriggerActions(connectedObject.id, directionBox->currentData().toInt(), changeBox->currentData().toInt(), changeBox->currentIndex());
	}
	else {
		ConnectedObjectBox->setCurrentIndex(selectedObjectID);
	}
}

void PyriteEditor::LoadTriggerBoxes()
{
	for (int i = 0; i < selectedObject->GetNumberOfTriggerActions(); i++) {
		AddTriggerAction(selectedObject->GetTriggerAction(i).actionID, selectedObject->GetTriggerAction(i).connectedObjectID, Action(selectedObject->GetTriggerAction(i).newAction), Direction(selectedObject->GetTriggerAction(i).newDirection), false);
	}
}

void PyriteEditor::RemoveTriggerBoxes()
{
	QWidget* TriggerBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TriggerBox");
	QWidgetList widgets = TriggerBox->findChildren<QWidget*>("TriggerChangeBox");
	for (int i = 0; i < widgets.size(); i++) {
		TriggerBox->layout()->removeWidget(widgets[i]);
		widgets[i]->hide();
		widgets[i]->setDisabled(true);
		widgets[i]->deleteLater();
		delete widgets[i];
	}
}

void PyriteEditor::AddWorldTrigger()
{
	pandaEngine.AddWorldTrigger();
}

void PyriteEditor::Save()
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

	QFile saveFile(QStringLiteral("Project.pyr"));
	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
	}

	QJsonDocument saveDoc(ProjectSave);
	saveFile.write(saveDoc.toJson());
}

void PyriteEditor::Load()
{
}


