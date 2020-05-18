#include "TransformManager.h"

TransformManager::TransformManager(QWidget* parent) : QGroupBox(parent) {

}

void TransformManager::Initialise()
{
}

void TransformManager::AddTransform()
{
	if (selectedObject != nullptr && !selectedObject->HasTransform()) {
		this->show();
		selectedObject->transform = new Transform();
	}
}

void TransformManager::RemoveActionBoxes()
{
	QWidgetList widgets = findChildren<QWidget*>("ActionBox");
	//Loop through actionBoxes
	for (int i = 0; i < widgets.size(); i++) {
		//Remove the action boxes
		layout()->removeWidget(widgets[i]);
		widgets[i]->hide();
		widgets[i]->setDisabled(true);
		widgets[i]->deleteLater();
		delete widgets[i];
	}
}

void TransformManager::LoadActionBoxes()
{
	for (int i = 0; i < selectedObject->GetNumberOfActions(); i++) {
		//Add the box and set values
		if (selectedObject->GetTransformAction(i).type == TransformType::Add) {
			AddActionBox(selectedObject->GetTransformAction(i).action, selectedObject->GetTransformAction(i).Key, selectedObject->GetTransformAction(i).Speed, selectedObject->GetTransformAction(i).direction, false);
		}
		if (selectedObject->GetTransformAction(i).type == TransformType::Follow) {
			qDebug() << "Inside Load ActionBoxes: " + QString::fromStdString(std::to_string(selectedObject->GetTransformAction(i).ConnectedObject));
			AddFollowObject(selectedObject->GetTransformAction(i).axis, selectedObject->GetTransformAction(i).ConnectedObject, selectedObject->GetTransformAction(i).Key, selectedObject->GetTransformAction(i).Speed, false);
		}
	}
}

void TransformManager::AddActionBox(Action action, std::string key, float speed, Direction direction, bool newAction)
{
	QLayoutItem* Button = layout()->takeAt(layout()->count() - 1);

	//Add a new Box 
	QWidget* widget = new QWidget(this);
	//Set box name to ActionBox
	widget->setObjectName("ActionBox");
	widget->setMaximumWidth(450);
	//Create a new layout for the Actionbox
	QHBoxLayout* layout = new QHBoxLayout(widget);
	//Set the maximum size of the box
	QSize maximumsize = widget->size();

	//Add a label for the action
	QLabel* actionLabel = new QLabel();
	actionLabel->setText("Action:");
	layout->addWidget(actionLabel);

	//Add a combo box for different actions
	QComboBox* actionBox = new QComboBox();
	actionBox->setObjectName("ActionDropDown");
	actionBox->addItem("Move", QVariant::fromValue(Action::Move));
	actionBox->addItem("Rotate", QVariant::fromValue(Action::Rotate));
	actionBox->setCurrentIndex(QVariant::fromValue(action).toInt());
	actionBox->adjustSize();
	//Add to the layout
	layout->addWidget(actionBox);

	//Create a label for the direction
	QLabel* directionLabel = new QLabel();
	directionLabel->setText("Direction:");
	layout->addWidget(directionLabel);

	//Add a combobox for different directions
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
	//Add to the layout
	layout->addWidget(directionBox);

	//Create a label for the speed 
	QLabel* speedLabel = new QLabel();
	speedLabel->setText("Speed:");
	speedLabel->adjustSize();
	layout->addWidget(speedLabel);

	//Add a double spin box (box for doubles)
	QDoubleSpinBox* speedBox = new QDoubleSpinBox();
	speedBox->setObjectName("SpeedBox");
	speedBox->setSingleStep(0.1);
	speedBox->setMaximumSize(80, 20);
	speedBox->setMinimumSize(80, 20);
	speedBox->setValue(speed);
	//Add to layout
	layout->addWidget(speedBox);

	//Add a label for the key
	QLabel* keyLabel = new QLabel();
	keyLabel->setText("Key:");
	keyLabel->adjustSize();
	layout->addWidget(keyLabel);

	//Add a line edit box
	QLineEdit* lineEditBox = new QLineEdit();
	lineEditBox->setObjectName("KeyBox");
	lineEditBox->setMaximumSize(80, 20);
	//Only allow one character to be entered into the box
	lineEditBox->setMaxLength(1);
	lineEditBox->setText(QString::fromStdString(key));
	//Add it to the layour
	layout->addWidget(lineEditBox);

	//Set the layout to the new Action box
	widget->setLayout(layout);
	//Add the action box to the transform box
	this->layout()->addWidget(widget);
	//Readd the button
	this->layout()->addItem(Button);
	//If this is a new action (not just readding the boxes)
	if (newAction) {
		//Create new transform action for component
		selectedObject->AddTransformAction(Action(actionBox->currentData().toInt()), lineEditBox->text().toStdString(), speedBox->value(), Direction(directionBox->currentData().toInt()));
	}
}

void TransformManager::AddFollowObject(TransformAxis axis, int selectedObjectID, std::string key, float speed, bool newAction)
{

	//Remove the button and store it (Second to last item, ALWAYS)
	QLayoutItem* Button = this->layout()->takeAt(this->layout()->count() - 1);

	//Add a new Box 
	QWidget* widget = new QWidget(this);
	//Set box name to ActionBox
	widget->setObjectName("ActionBox");
	//Create a new layout for the Actionbox
	QHBoxLayout* layout = new QHBoxLayout(widget);
	//Set the maximum size of the box
	QSize maximumsize = widget->size();

	//Add a label for the action
	QLabel* axisLabel = new QLabel();
	axisLabel->setText("Axis:");
	layout->addWidget(axisLabel);

	QComboBox* axisBox = new QComboBox();
	axisBox->setObjectName("AxisDropDown");
	axisBox->addItem("X", QVariant::fromValue(TransformAxis::X));
	axisBox->addItem("Y", QVariant::fromValue(TransformAxis::Y));
	axisBox->addItem("Z", QVariant::fromValue(TransformAxis::Z));
	axisBox->setCurrentIndex(QVariant::fromValue(axis).toInt());
	axisBox->adjustSize();
	//Add to the layout
	layout->addWidget(axisBox);

	//Create a label for the object 
	QLabel* objectLabel = new QLabel();
	objectLabel->setText("Object:");
	objectLabel->adjustSize();
	layout->addWidget(objectLabel);

	QComboBox* objectBox = new QComboBox();
	objectBox->setObjectName("objectBox");
	layout->addWidget(objectBox);

	QLabel* speedLabel = new QLabel();
	speedLabel->setText("Speed:");
	speedLabel->adjustSize();
	layout->addWidget(speedLabel);

	//Add a double spin box (box for doubles)
	QDoubleSpinBox* speedBox = new QDoubleSpinBox();
	speedBox->setObjectName("SpeedBox");
	speedBox->setSingleStep(0.1);
	speedBox->setMaximumSize(80, 20);
	speedBox->setMinimumSize(80, 20);
	speedBox->setValue(speed);
	//Add to layout
	layout->addWidget(speedBox);

	QLabel* keyLabel = new QLabel();
	keyLabel->setText("Key:");
	keyLabel->adjustSize();
	layout->addWidget(keyLabel);

	//Add a line edit box
	QLineEdit* lineEditBox = new QLineEdit();
	lineEditBox->setObjectName("KeyBox");
	lineEditBox->setMaximumSize(80, 20);
	//Only allow one character to be entered into the box
	lineEditBox->setMaxLength(1);
	lineEditBox->setText(QString::fromStdString(key));
	//Add it to the layour
	layout->addWidget(lineEditBox);

	widget->setLayout(layout);
	//Add the action box to the transform box
	this->layout()->addWidget(widget);
	//Readd the button
	this->layout()->addItem(Button);

	/*ReloadGameObjectDropDowns(objectBox);*/
	for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
		objectBox->addItem(QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetObjectName()), i);
	}

	if (newAction) {
		//Create new transform action for component
		selectedObject->AddTranformFollowAction(TransformAxis(axisBox->currentData().toInt()), objectBox->currentData().toInt(), lineEditBox->text().toStdString(), speedBox->value());
	}
	else {
		objectBox->setCurrentIndex(selectedObjectID);
	}
}

void TransformManager::UpdateSelectedObject(GameObject* object)
{
	selectedObject = object;
}

void TransformManager::UpdateActions()
{
	QWidgetList widgets = this->findChildren<QWidget*>("ActionBox");
	for (int i = 0; i < widgets.count(); i++) {
		if (selectedObject->GetTransformAction(i).type == TransformType::Add) {
			//Change the add action of the transform to user input
			selectedObject->ChangeTransformAction(i,
				Action(widgets[i]->findChild<QComboBox*>("ActionDropDown")->currentData().toInt()),
				widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
				widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value(),
				Direction(widgets[i]->findChild<QComboBox*>("DirectionDropDown")->currentData().toInt())
			);
		}
		if (selectedObject->GetTransformAction(i).type == TransformType::Follow) {
			int currentObjectID = widgets[i]->findChild<QComboBox*>("objectBox")->currentIndex();
			if (currentObjectID < 0) currentObjectID = 0;

			selectedObject->ChangeTransformFollowAction(i,
				TransformAxis(widgets[i]->findChild<QComboBox*>("AxisDropDown")->currentData().toInt()),
				currentObjectID,
				widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
				widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value()
			);
		}
	}
}
