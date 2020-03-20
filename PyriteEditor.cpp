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
	
	////Create menu for button
	AddComponent* menu = new AddComponent(ui.pushButton, this);

	//Add actions for menu
	QAction* addTransform = menu->addAction("Add Transform");
	connect(addTransform, SIGNAL(triggered()), this, SLOT(AddTransform()));

	QAction* addCollision = menu->addAction("Add Collision");
	connect(addCollision, SIGNAL(triggered()), this, SLOT(AddCollision()));

	QAction* addTrigger = menu->addAction("Add Trigger");
	connect(addTrigger, SIGNAL(triggered()), this, SLOT(AddTrigger()));

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
	QGroupBox* newBox = new QGroupBox(findChild<QWidget*>("ComponentWidget"));
	newBox->setTitle("Collision");
	newBox->setObjectName("CollisionBox");
	QLayoutItem* Spacer = findChild<QWidget*>("ComponentWidget")->layout()->takeAt(findChild<QWidget*>("ComponentWidget")->layout()->count() - 2);
	QLayoutItem* Button = findChild<QWidget*>("ComponentWidget")->layout()->takeAt(findChild<QWidget*>("ComponentWidget")->layout()->count() - 1);
	findChild<QWidget*>("ComponentWidget")->layout()->addWidget(newBox);
	findChild<QWidget*>("ComponentWidget")->layout()->addItem(Spacer);
	findChild<QWidget*>("ComponentWidget")->layout()->addItem(Button);
}


//Add trigger box to Component Window
void PyriteEditor::AddTrigger()
{
	QGroupBox* newBox = new QGroupBox(findChild<QWidget*>("ComponentWidget"));
	newBox->setTitle("Trigger");
	QLayoutItem* Spacer = findChild<QWidget*>("ComponentWidget")->layout()->takeAt(findChild<QWidget*>("ComponentWidget")->layout()->count() - 2);
	QLayoutItem* Button = findChild<QWidget*>("ComponentWidget")->layout()->takeAt(findChild<QWidget*>("ComponentWidget")->layout()->count() - 1);
	findChild<QWidget*>("ComponentWidget")->layout()->addWidget(newBox);
	findChild<QWidget*>("ComponentWidget")->layout()->addItem(Spacer);
	findChild<QWidget*>("ComponentWidget")->layout()->addItem(Button);
}

void PyriteEditor::LoadSelectedObject()
{
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	if (selectedObject == nullptr && oneTimeSelect) {
		if (TransformBox != nullptr) {
			RemoveActionBoxes();
			TransformBox->hide();
		}

		QWidget* CollisionBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("CollisionBox");
		if (CollisionBox != nullptr) {
			CollisionBox->hide();
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
	}
}

void PyriteEditor::AddActionBox(Action action, std::string key, float speed, Direction direction, bool newAction)
{
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QLayoutItem* Button = TransformBox->layout()->takeAt(TransformBox->layout()->count()-1);

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
		qDebug() << "If new Action statement, lineEditBox->Text: " << lineEditBox->text();
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
			oneTimeSelect = true;
			LoadSelectedObject();
		}
		else {
			selectedObject->SetPosition(findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->value(), 
										findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->value(),
										findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->value());

			selectedObject->SetRotation(findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->value(),
										findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->value(),
										findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->value());

			selectedObject->SetScale(findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("Size")->value());
		}
	}
	else {
		selectedObject = nullptr;
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
	QWidget* TransformBox = findChild<QWidget*>("ComponentWidget")->findChild<QWidget*>("TransformBox");
	QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
	for (int i = 0; i < widgets.count(); i++) {
		selectedObject->ChangeTransformAction(i,
			Action(widgets[i]->findChild<QComboBox*>("ActionDropDown")->currentData().toInt()),
			widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
			widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value(),
			Direction(widgets[i]->findChild<QComboBox*>("DirectionDropDown")->currentData().toInt())
		);
		qDebug() << "Update Components for loop KeyBox->Text: " << widgets[i]->findChild<QLineEdit*>("KeyBox")->text();
	}
}





//Create a new vertical box layout 
	//QVBoxLayout* layout = new QVBoxLayout(ui.Components);
	//
	////Create widget to overlay dock widget
	//QWidget* widget = new QWidget(ui.Components);
	////apply layout to widget


	//widget->setLayout(layout);

	////Set widget to dock
	//ui.Components->setWidget(widget);
	//ui.Components->widget()->layout()->addWidget(this->findChild<QWidget*>("ObjectProps"));
	////Add button to widget
	//ui.Components->widget()->layout()->addWidget(ui.pushButton);