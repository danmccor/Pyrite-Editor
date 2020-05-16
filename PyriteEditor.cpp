#include "PyriteEditor.h"
#include "AddComponent.h"
#include "qstylefactory.h"
extern SaveManager* saveManager;

PyriteEditor::PyriteEditor(QWidget* parent)
	: QMainWindow(parent)
{
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	//Set up the ui 
	ui.setupUi(this);

	findChild<TriggerManager*>("TriggerBox")->show();
	triggerManager = findChild<TriggerManager*>("TriggerBox");
	triggerManager->Initialise(ProjectDirectory);
	collisionManager = findChild<CollisionManager*>("CollisionBox");
	collisionManager->Initialise();
	transformManager = findChild<TransformManager*>("TransformBox");
	transformManager->Initialise();

	ConnectUI();

	saveManager = new SaveManager(this);

}

//Connect all the UI buttons to functions
void PyriteEditor::ConnectUI()
{
	QListWidget* selectedObjectFromList = findChild<QListWidget*>("ObjectList");
 	connect(selectedObjectFromList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(SelectObjectFromList(QListWidgetItem*)));

	QAction* newProjectAction = findChild<QAction*>("actionNewProject");
	connect(newProjectAction, SIGNAL(triggered()), this, SLOT(NewProject()));

	QAction* loadProjectAction = findChild<QAction*>("actionLoadProject");
	connect(loadProjectAction, SIGNAL(triggered()), this, SLOT(LoadProject()));

	//Connect File->New
	QAction* newAction = findChild<QAction*>("actionNew");
	connect(newAction, SIGNAL(triggered()), this, SLOT(New()));

	//Connect File->Save As
	QAction* saveAsAction = findChild<QAction*>("actionSaveAs");
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(SaveAs()));

	//Connect File->Save
	QAction* saveAction = findChild<QAction*>("actionSave");
	connect(saveAction, SIGNAL(triggered()), this, SLOT(Save()));

	//Connect File->Load
	QAction* loadAction = findChild<QAction*>("actionLoad");
	connect(loadAction, SIGNAL(triggered()), this, SLOT(Load()));

	//Connect File->Build
	QAction* buildAction = findChild<QAction*>("actionBuild");
	connect(buildAction, SIGNAL(triggered()), this, SLOT(Build()));

	//Connect Scene->Add Trigger Object
	QAction* addWorldTrigger = findChild<QAction*>("actionSquare");
	connect(addWorldTrigger, SIGNAL(triggered()), this, SLOT(AddWorldTrigger()));

	//Connect Scene->Add Camera
	QAction* cameraAction = findChild<QAction*>("actionCamera");
	connect(cameraAction, SIGNAL(triggered()), this, SLOT(AddCamera()));

	//Connect Debug->Run Program
	QAction* action = findChild<QAction*>("actionRun_Program");
	connect(action, SIGNAL(triggered()), this, SLOT(RunGame()));

	QAction* buildWindow = findChild<QAction*>("actionBuildWindow");
	connect(buildWindow, SIGNAL(triggered()), this, SLOT(LaunchBuildWindow()));

	//Connect Add Trigger Action 
	/*QGroupBox* TriggerBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<QGroupBox*>("TriggerBox");*/
	QPushButton* AddTriggerButton = this->findChild<QPushButton*>("AddTriggerAction");

	if (AddTriggerButton != nullptr) {
		AddComponent* triggerMenu = new AddComponent(AddTriggerButton, triggerManager);

		QAction* addChangeMovement = triggerMenu->addAction("Change Transform");
		connect(addChangeMovement, SIGNAL(triggered()), triggerManager, SLOT(AddTriggerAction()));

		QAction* addMoveToMovement = triggerMenu->addAction("Move Position");
		connect(addMoveToMovement, SIGNAL(triggered()), triggerManager, SLOT(AddTriggerMoveAction()));

		QAction* addSceneChange = triggerMenu->addAction("Change Scene");
		connect(addSceneChange, SIGNAL(triggered()), triggerManager, SLOT(AddTriggerChangeScene()));

		qDebug() << "These Have Connected>!>!";

		AddTriggerButton->setMenu(triggerMenu);
	}
	//Connect Add Transform Action
		QPushButton* Button = this->findChild<QPushButton*>("AddMovement");
		if (Button != nullptr) {
			AddComponent* transformMenu = new AddComponent(Button, transformManager);

			QAction* addMovement = transformMenu->addAction("Add Movement");
			connect(addMovement, SIGNAL(triggered()), transformManager, SLOT(AddActionBox()));
			
			QAction* followObject = transformMenu->addAction("Follow Object");
			connect(followObject, SIGNAL(triggered()), transformManager, SLOT(AddFollowObject()));


			Button->setMenu(transformMenu);
		}
		transformManager->hide();

	//Create menu button
	AddComponent* menu = new AddComponent(ui.pushButton, this);

	//Connect add Transform
	QAction* addTransform = menu->addAction("Add Transform");
	connect(addTransform, SIGNAL(triggered()), transformManager, SLOT(AddTransform()));

	//Connect add Collision
	QAction* addCollision = menu->addAction("Add Collision");
	connect(addCollision, SIGNAL(triggered()), collisionManager, SLOT(AddCollision()));

	//Connect add Trigger
	QAction* addTrigger = menu->addAction("Add Trigger");
	connect(addTrigger, SIGNAL(triggered()), triggerManager, SLOT(AddTrigger()));

	QPushButton* deleteObject = this->findChild<QGroupBox*>("ObjectProps")->findChild<QPushButton*>("DeleteObject");
	connect(deleteObject, SIGNAL(clicked()), this, SLOT(DeleteObject()));

	//Set Button
	ui.pushButton->setMenu(menu);
	qDebug() << "Finished Connecting";
}

#pragma region Object Manipulation

//Load the selected Object
void PyriteEditor::LoadSelectedObject()
{
	//If there is no object, and this is the first loop
	if (selectedObject == nullptr && oneTimeSelect) {
		//If transform box exists
		if (transformManager != nullptr) {
			//Remove all transform action boxes
			transformManager->RemoveActionBoxes();
			//hide the transform box
			transformManager->hide();
		}
		//If collision box exists
		if (collisionManager != nullptr) {
			//Hide the collision box
			//CollisionBox->findChild<QCheckBox*>("CollisionCheckBox")->setChecked(false);
			collisionManager->hide();
		}
		if (triggerManager != nullptr) {
			triggerManager->RemoveTriggerBoxes();
			triggerManager->hide();
		}
		//Set first loop to false
		oneTimeSelect = false;
	}


	//If there is an object Selected
	else if (selectedObject != nullptr) {

		//if the selected object has a transform component
		if (selectedObject->HasTransform()) {
			//If the transform box exists
			if (transformManager != nullptr) {
				//Show the box
				transformManager->show();
				//Load all the actions
				transformManager->LoadActionBoxes();
			}
		}
		//Else make sure the transform box is hidden
		else {
			transformManager->hide();
		}

		//IF OBJECT HAS COLLISION
		if (selectedObject->HasCollision()) {
			collisionManager->show();
			//Show the collision box
			//Get the collision type of the object
			CollisionType col = selectedObject->GetCollisionType();
			collisionManager->SetCollisionType(col);
		}
		//Else make sure the collision box is hidden
		else {
			collisionManager->hide();
		}

		//If the object has a trigger component
		if (selectedObject->HasTrigger()) {
			triggerManager->show();
			triggerManager->LoadTriggerBoxes();
		}
		else {
			triggerManager->RemoveTriggerBoxes();
			triggerManager->hide();
		}
	}
}

//Set the property box for a selected object with floats
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

void PyriteEditor::RemoveObjectFromList(GameObject* object)
{
	QListWidget* list = findChild<QListWidget*>("ObjectList");
	list->takeItem(object->id);
	list->clear();
}

void PyriteEditor::AddAllObjectsToList()
{
	QListWidget* objectList = findChild<QListWidget*>("ObjectList");

	for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
		QListWidgetItem* item = new QListWidgetItem();
		item->setData(Qt::UserRole, pandaEngine.GetVectorOfGameObjects()[i]->id);
		item->setText(QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetObjectName()));
		objectList->addItem(item);
	}
}

void PyriteEditor::SelectObjectFromList(QListWidgetItem* item)
{
	if (pandaEngine.GetSelectedObject() != nullptr) {
		pandaEngine.GetSelectedObject()->SetHighlight(false);
	}
	pandaEngine.SetSelectedObject(pandaEngine.GetVectorOfGameObjects()[item->data(Qt::UserRole).toInt()]);
	pandaEngine.GetVectorOfGameObjects()[item->data(Qt::UserRole).toInt()]->SetHighlight(true);
}

void PyriteEditor::AddObjectToObjectList(GameObject* gameObject)
{
	QListWidget* objectList = findChild<QListWidget*>("ObjectList");

	QListWidgetItem* item = new QListWidgetItem();
	item->setData(Qt::UserRole, gameObject->id);
	item->setText(QString::fromStdString(gameObject->GetObjectName()));
	objectList->addItem(item);
}

void PyriteEditor::ReloadGameObjectDropDowns(QComboBox* comboBox)
{
	for (int i = 0; i < pandaEngine.GetVectorOfGameObjects().size(); i++) {
		comboBox->addItem(QString::fromStdString(pandaEngine.GetVectorOfGameObjects()[i]->GetObjectName()), i);
	}
}

//Set the property box for a selected object by passing in the gameobject
void PyriteEditor::SetObjectProperties(GameObject* gameObject)
{
	//if the game object is not null
	if (gameObject != nullptr) {
		//and the game object is not the same as the currently selected one
		if (gameObject != selectedObject) {
			//Fill property box with variables. Allow user to edit box.
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QPushButton*>("DeleteObject")->setEnabled(true);
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
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->setValue(gameObject->GetScaleX());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->setValue(gameObject->GetScaleY());
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->setReadOnly(false);
			this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->setValue(gameObject->GetScaleZ());
			//Set selected object to new game object
			selectedObject = gameObject;
			//remove the boxes from the old game object
			transformManager->RemoveActionBoxes();
			/*RemoveTriggerBoxes();*/
			triggerManager->RemoveTriggerBoxes();
			transformManager->UpdateSelectedObject(selectedObject);
			triggerManager->UpdateSelectedObject(selectedObject);
			collisionManager->UpdateSelectedObject(selectedObject);
			qDebug() << "In this Section";
			updateListOnce = false;
			oneTimeSelect = true;
			LoadSelectedObject();
			qDebug() << "Selected Object Loaded";
			
		}
		else {
			//Use variables to update the game objects
			selectedObject->SetObjectName(findChild<QGroupBox*>("ObjectProps")->findChild<QLineEdit*>("ObjectName")->text().toStdString());
			selectedObject->SetPosition((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosX")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosY")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("PosZ")->value());

			selectedObject->SetRotation((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotX")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotY")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("RotZ")->value());

			//Prevent scale from being set to 0. 
			if((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->value() == 0)
				this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->setValue(0.01f);
			if((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->value() == 0)
				this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->setValue(0.01f);
			if((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->value() == 0)
				this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->setValue(0.01f);

			//Update Scale
			selectedObject->SetScale((float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->value(),
				(float)findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->value());
		}
	}
	else {
		//If no object is selected then reset the property box and dont allow user to edit.
		selectedObject = nullptr;
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QPushButton*>("DeleteObject")->setEnabled(false);
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
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleX")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleY")->setReadOnly(true);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->setValue(0);
		this->findChild<QGroupBox*>("ObjectProps")->findChild<QDoubleSpinBox*>("ScaleZ")->setReadOnly(true);
		LoadSelectedObject();
	}
}

//Update the selected objects components
void PyriteEditor::UpdateComponents()
{
	//Find all component boxes
	QWidget* TransformBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<QWidget*>("TransformBox");
	QComboBox* TransformObject = TransformBox->findChild<QWidget*>("ActionBox")->findChild<QComboBox*>("objectBox");
	
	
	if (newWindow != nullptr) {
		if (newWindow->isVisible()) {
			QGroupBox* buildSettings = newWindow->findChild<QGroupBox*>("BuildGroup");
			if (buildSettings != nullptr) {
				foreach(QRadioButton * button, buildSettings->findChildren<QRadioButton*>()) {
					if (button->isChecked()) {
						pandaEngine.SetFirstScene(button->text().toStdString());
					}
				}
			}
		}
	}


	//Get the widgets attached to the transform box
	//QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
	//	for (int i = 0; i < widgets.count(); i++) {
	//		if (selectedObject->GetTransformAction(i).type == TransformType::Add) {
	//			//Change the add action of the transform to user input
	//			selectedObject->ChangeTransformAction(i,
	//				Action(widgets[i]->findChild<QComboBox*>("ActionDropDown")->currentData().toInt()),
	//				widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
	//				widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value(),
	//				Direction(widgets[i]->findChild<QComboBox*>("DirectionDropDown")->currentData().toInt())
	//			);
	//		}
	//		if (selectedObject->GetTransformAction(i).type == TransformType::Follow) {
	//			int currentObjectID = widgets[i]->findChild<QComboBox*>("objectBox")->currentIndex();
	//			if (currentObjectID < 0) currentObjectID = 0;

	//			selectedObject->ChangeTransformFollowAction(i,
	//				TransformAxis(widgets[i]->findChild<QComboBox*>("AxisDropDown")->currentData().toInt()),
	//				currentObjectID,
	//				widgets[i]->findChild<QLineEdit*>("KeyBox")->text().toStdString(),
	//				widgets[i]->findChild<QDoubleSpinBox*>("SpeedBox")->value()
	//			);
	//		}
	//	}


	transformManager->UpdateActions();
	triggerManager->UpdateActions();
	//If there is a selected object
	if (selectedObject != nullptr) {
		if (selectedObject->HasCollision()) {
			//Update the collision type to user input
			collisionManager->UpdateCollision();
		}
		if (!updateListOnce && selectedObject->HasTransform()) {
			QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
			QComboBox* objectDropDown = TransformBox->findChild<QComboBox*>("objectBox");
			if (objectDropDown != nullptr) {
				objectDropDown->clear();

				for (int i = 0; i < widgets.size(); i++) {
					if (selectedObject->GetTransformAction(i).type == TransformType::Follow) {
						ReloadGameObjectDropDowns(widgets[i]->findChild<QComboBox*>("objectBox"));
						widgets[i]->findChild<QComboBox*>("objectBox")->setCurrentIndex(selectedObject->GetTransformAction(i).ConnectedObject);
					}
				}
			}
			updateListOnce = true;
		}
		
	}
	else {
	//	//Clear the connected object box
		findChild<QComboBox*>("ConnectedObject")->clear();
		findChild<QComboBox*>("EnteringObject")->clear();
	//	//Add all objects to connected object drop down box
		ReloadGameObjectDropDowns(findChild<QComboBox*>("EnteringObject"));
		ReloadGameObjectDropDowns(findChild<QComboBox*>("ConnectedObject"));
		updateListOnce = false;
	}
}
#pragma endregion

#pragma region Transform Component
//Add transform box to Component Window
//void PyriteEditor::AddTransform() {
//	if (selectedObject != nullptr && !selectedObject->HasTransform()) {
//		AddAction* TransformBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<AddAction*>("TransformBox");
//		if (TransformBox != nullptr) {
//			TransformBox->show();
//		}
//		selectedObject->AddTransform();
//	}
//}

//Add action boxes to transform
//void PyriteEditor::AddActionBox(Action action, std::string key, float speed, Direction direction, bool newAction)
//{
//	Find the transform box
//	QWidget* TransformBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<QWidget*>("TransformBox");
//	Remove the button and store it (Second to last item, ALWAYS)
//	QLayoutItem* Button = TransformBox->layout()->takeAt(TransformBox->layout()->count() - 1);
//
//	//Add a new Box 
//	QWidget* widget = new QWidget(TransformBox);
//	//Set box name to ActionBox
//	widget->setObjectName("ActionBox");
//	widget->setMaximumWidth(450);
//	//widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//	//Create a new layout for the Actionbox
//	QHBoxLayout* layout = new QHBoxLayout(widget);
//	//Set the maximum size of the box
//	QSize maximumsize = widget->size();
//
//	//Add a label for the action
//	QLabel* actionLabel = new QLabel();
//	actionLabel->setText("Action:");
//	layout->addWidget(actionLabel);
//
//	//Add a combo box for different actions
//	QComboBox* actionBox = new QComboBox();
//	actionBox->setObjectName("ActionDropDown");
//	actionBox->addItem("Move", QVariant::fromValue(Action::Move));
//	actionBox->addItem("Rotate", QVariant::fromValue(Action::Rotate));
//	actionBox->setCurrentIndex(QVariant::fromValue(action).toInt());
//	actionBox->adjustSize();
//	//Add to the layout
//	layout->addWidget(actionBox);
//
//	//Create a label for the direction
//	QLabel* directionLabel = new QLabel();
//	directionLabel->setText("Direction:");
//	layout->addWidget(directionLabel);
//
//	//Add a combobox for different directions
//	QComboBox* directionBox = new QComboBox();
//	directionBox->setObjectName("DirectionDropDown");
//	directionBox->addItem("Forward", QVariant::fromValue(Direction::Forward));
//	directionBox->addItem("Backward", QVariant::fromValue(Direction::Backward));
//	directionBox->addItem("Left", QVariant::fromValue(Direction::Left));
//	directionBox->addItem("Right", QVariant::fromValue(Direction::Right));
//	directionBox->addItem("Up", QVariant::fromValue(Direction::Up));
//	directionBox->addItem("Down", QVariant::fromValue(Direction::Down));
//	directionBox->setCurrentIndex(QVariant::fromValue(direction).toInt());
//	directionBox->adjustSize();
//	//Add to the layout
//	layout->addWidget(directionBox);
//
//	//Create a label for the speed 
//	QLabel* speedLabel = new QLabel();
//	speedLabel->setText("Speed:");
//	speedLabel->adjustSize();
//	layout->addWidget(speedLabel);
//
//	//Add a double spin box (box for doubles)
//	QDoubleSpinBox* speedBox = new QDoubleSpinBox();
//	speedBox->setObjectName("SpeedBox");
//	speedBox->setSingleStep(0.1);
//	speedBox->setMaximumSize(80, 20);
//	speedBox->setMinimumSize(80, 20);
//	speedBox->setValue(speed);
//	//Add to layout
//	layout->addWidget(speedBox);
//
//	//Add a label for the key
//	QLabel* keyLabel = new QLabel();
//	keyLabel->setText("Key:");
//	keyLabel->adjustSize();
//	layout->addWidget(keyLabel);
//
//	//Add a line edit box
//	QLineEdit* lineEditBox = new QLineEdit();
//	lineEditBox->setObjectName("KeyBox");
//	lineEditBox->setMaximumSize(80, 20);
//	//Only allow one character to be entered into the box
//	lineEditBox->setMaxLength(1);
//	lineEditBox->setText(QString::fromStdString(key));
//	//Add it to the layour
//	layout->addWidget(lineEditBox);
//
//	//Set the layout to the new Action box
//	widget->setLayout(layout);
//	//Add the action box to the transform box
//	TransformBox->layout()->addWidget(widget);
//	//Readd the button
//	TransformBox->layout()->addItem(Button);
//	//If this is a new action (not just readding the boxes)
//	if (newAction) {
//		//Create new transform action for component
//		selectedObject->AddTransformAction(Action(actionBox->currentData().toInt()), lineEditBox->text().toStdString(), speedBox->value(), Direction(directionBox->currentData().toInt()));
//	}
//}

//void PyriteEditor::AddFollowObject(TransformAxis axis, int selectedObjectID, std::string key, float speed, bool newAction)
//{
//	qDebug() << "SelectedObjectID in AddFollowObject: " + QString::fromStdString(std::to_string(selectedObjectID));
//	QWidget* TransformBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<QWidget*>("TransformBox");
//	//Remove the button and store it (Second to last item, ALWAYS)
//	QLayoutItem* Button = TransformBox->layout()->takeAt(TransformBox->layout()->count() - 1);
//
//	//Add a new Box 
//	QWidget* widget = new QWidget(TransformBox);
//	//Set box name to ActionBox
//	widget->setObjectName("ActionBox");
//	//Create a new layout for the Actionbox
//	QHBoxLayout* layout = new QHBoxLayout(widget);
//	//Set the maximum size of the box
//	QSize maximumsize = widget->size();
//
//	//Add a label for the action
//	QLabel* axisLabel = new QLabel();
//	axisLabel->setText("Axis:");
//	layout->addWidget(axisLabel);
//
//	QComboBox* axisBox = new QComboBox();
//	axisBox->setObjectName("AxisDropDown");
//	axisBox->addItem("X", QVariant::fromValue(TransformAxis::X));
//	axisBox->addItem("Y", QVariant::fromValue(TransformAxis::Y));
//	axisBox->addItem("Z", QVariant::fromValue(TransformAxis::Z));
//	axisBox->setCurrentIndex(QVariant::fromValue(axis).toInt());
//	axisBox->adjustSize();
//	//Add to the layout
//	layout->addWidget(axisBox);
//
//	//Create a label for the object 
//	QLabel* objectLabel = new QLabel();
//	objectLabel->setText("Object:");
//	objectLabel->adjustSize();
//	layout->addWidget(objectLabel);
//
//	QComboBox* objectBox = new QComboBox();
//	objectBox->setObjectName("objectBox");
//	layout->addWidget(objectBox);
//
//	QLabel* speedLabel = new QLabel();
//	speedLabel->setText("Speed:");
//	speedLabel->adjustSize();
//	layout->addWidget(speedLabel);
//
//	//Add a double spin box (box for doubles)
//	QDoubleSpinBox* speedBox = new QDoubleSpinBox();
//	speedBox->setObjectName("SpeedBox");
//	speedBox->setSingleStep(0.1);
//	speedBox->setMaximumSize(80, 20);
//	speedBox->setMinimumSize(80, 20);
//	speedBox->setValue(speed);
//	//Add to layout
//	layout->addWidget(speedBox);
//
//	QLabel* keyLabel = new QLabel();
//	keyLabel->setText("Key:");
//	keyLabel->adjustSize();
//	layout->addWidget(keyLabel);
//
//	//Add a line edit box
//	QLineEdit* lineEditBox = new QLineEdit();
//	lineEditBox->setObjectName("KeyBox");
//	lineEditBox->setMaximumSize(80, 20);
//	//Only allow one character to be entered into the box
//	lineEditBox->setMaxLength(1);
//	lineEditBox->setText(QString::fromStdString(key));
//	//Add it to the layour
//	layout->addWidget(lineEditBox);
//
//	widget->setLayout(layout);
//	//Add the action box to the transform box
//	TransformBox->layout()->addWidget(widget);
//	//Readd the button
//	TransformBox->layout()->addItem(Button);
//
//	ReloadGameObjectDropDowns(objectBox);
//	
//
//	if (newAction) {
//		//Create new transform action for component
//		selectedObject->AddTranformFollowAction(TransformAxis(axisBox->currentData().toInt()), objectBox->currentData().toInt(), lineEditBox->text().toStdString(), speedBox->value());
//		qDebug() << "New Transform Follow Action: ";
//		qDebug() << axisBox->currentData().toInt();
//		qDebug() << objectBox->currentData().toInt();
//		qDebug() << lineEditBox->text();
//		qDebug() << speedBox->value();
//	}
//	else {
//		qDebug() << "Not new Follow Action: ";
//		objectBox->setCurrentIndex(selectedObjectID);
//	}
//	qDebug() << "Add Follow Object End";
//}
//
////Reload the action boxes for selected object
//void PyriteEditor::LoadActionBoxes()
//{
//	//loop through the number of actions attached to an object
//	for (int i = 0; i < selectedObject->GetNumberOfActions(); i++) {
//		//Add the box and set values
//		if (selectedObject->GetTransformAction(i).type == TransformType::Add) {
//			AddActionBox(selectedObject->GetTransformAction(i).action, selectedObject->GetTransformAction(i).Key, selectedObject->GetTransformAction(i).Speed, selectedObject->GetTransformAction(i).direction, false);
//		}
//		if (selectedObject->GetTransformAction(i).type == TransformType::Follow) {
//			qDebug() << "Inside Load ActionBoxes: " + QString::fromStdString(std::to_string(selectedObject->GetTransformAction(i).ConnectedObject));
//			AddFollowObject(selectedObject->GetTransformAction(i).axis, selectedObject->GetTransformAction(i).ConnectedObject, selectedObject->GetTransformAction(i).Key, selectedObject->GetTransformAction(i).Speed, false);
//		}
//	}
//}
//Remove all the Action boxes
//void PyriteEditor::RemoveActionBoxes()
//{
//	//Find the transform box
//	QWidget* TransformBox = findChild<QScrollArea*>("ComponentScrollWindow")->findChild<QWidget*>("TransformBox");
//	//Get a list of attached ActionBoxes
//	QWidgetList widgets = TransformBox->findChildren<QWidget*>("ActionBox");
//	//Loop through actionBoxes
//	for (int i = 0; i < widgets.size(); i++) {
//		//Remove the action boxes
//		TransformBox->layout()->removeWidget(widgets[i]);
//		widgets[i]->hide();
//		widgets[i]->setDisabled(true);
//		widgets[i]->deleteLater();
//		delete widgets[i];
//	}
//}
#pragma endregion


#pragma region Add to Scene
//Add a trigger to the world
void PyriteEditor::AddWorldTrigger()
{
	pandaEngine.AddWorldTrigger();
}

//Add a camera to the world
void PyriteEditor::AddCamera()
{
	pandaEngine.AddGameCamera();
}
void PyriteEditor::DeleteObject()
{
	RemoveObjectFromList(selectedObject);
	pandaEngine.DeleteGameObject(selectedObject->id);
	AddAllObjectsToList();
}
void PyriteEditor::LaunchBuildWindow()
{
	newWindow = new QWidget();
	newWindow->setWindowTitle("Build Settings");
	newWindow->setObjectName("BuildSettings");
	QVBoxLayout* widgetLayout = new QVBoxLayout(newWindow);
	newWindow->setLayout(widgetLayout);

	QGroupBox* buildSettings = new QGroupBox(newWindow);
	buildSettings->setObjectName("BuildGroup");
	buildSettings->setTitle("Pyrite Scene");
	QVBoxLayout* layout = new QVBoxLayout(buildSettings);

	QStringList filter("*.pyr");
	QDir directory(QString::fromStdString(ProjectDirectory));
	QStringList filelist = directory.entryList(filter);

	for (int i = 0; i < filelist.size(); i++) {
		QRadioButton* newbutton = new QRadioButton(filelist[i], buildSettings);
		newbutton->setObjectName("radioButton");
		layout->addWidget(newbutton);
	}
	
	buildSettings->setLayout(layout);
	newWindow->layout()->addWidget(buildSettings);
	newWindow->show();
}
#pragma endregion

#pragma region Project Management
//Create a new project

void PyriteEditor::NewProject()
{
	saveManager->NewProject();
	ProjectDirectory = saveManager->GetProjectDirectory();
	//Get the project name from the same manager
	ProjectName = saveManager->GetProjectName();
	triggerManager->UpdateDirectory(ProjectDirectory);
	if (ProjectName != "") {
		//Set the Asset folder creation to the new directory
		DragAndDrop* AssetDrop = findChild<QWidget*>("Assets")->findChild<DragAndDrop*>("listWidget");
		AssetDrop->ChangeDirectory(ProjectDirectory);
		AssetToScene* scene = findChild<AssetToScene*>("RenderZone");
		scene->ChangeDirectory(ProjectDirectory);
		QMenu* menuFile = findChild<QMenu*>("menuFile");
		foreach(QAction* action, menuFile->actions()) {
			action->setEnabled(true);
		}
	}
}

void PyriteEditor::LoadProject()
{
	saveManager->LoadProject();
	ProjectDirectory = saveManager->GetProjectDirectory();
	ProjectName = saveManager->GetProjectName();
	ProjectSave = saveManager->GetProjectSave();
	triggerManager->UpdateDirectory(ProjectDirectory);
	if (ProjectName != "") {
		DragAndDrop* AssetDrop = findChild<QWidget*>("Assets")->findChild<DragAndDrop*>("listWidget");
		AssetDrop->ChangeDirectory(ProjectDirectory);
		AssetToScene* scene = findChild<AssetToScene*>("RenderZone");
		scene->ChangeDirectory(ProjectDirectory);
		QMenu* menuFile = findChild<QMenu*>("menuFile");
		foreach(QAction * action, menuFile->actions()) {
			action->setEnabled(true);
		}
	}

}

void PyriteEditor::New()
{
	//Run save manager->new()
	saveManager->New();
	//Get the project directory from the save manager
	triggerManager->UpdateDirectory(ProjectDirectory);
}

//Save the project as..
void PyriteEditor::SaveAs()
{
	//Save the project as
	saveManager->SaveAs();
	//Get the project directory
	ProjectDirectory = saveManager->GetProjectDirectory();
	//Get the project name
	ProjectName = saveManager->GetProjectName();
	//Get the project save object
	ProjectSave = saveManager->GetProjectSave();
	triggerManager->UpdateDirectory(ProjectDirectory);
}

//Save the project
void PyriteEditor::Save()
{
	//Functionally the same as 'Save As'
	saveManager->Save();
	ProjectDirectory = saveManager->GetProjectDirectory();
	ProjectName = saveManager->GetProjectName();
	ProjectSave = saveManager->GetProjectSave();
	triggerManager->UpdateDirectory(ProjectDirectory);
}

//Load the project
void PyriteEditor::Load()
{
	//Remove all game objects from the scene
	pandaEngine.RemoveAllGameObjects();
	//Load the project
	saveManager->Load("");
	//Update project details
	triggerManager->UpdateDirectory(ProjectDirectory);
}

//Build the project
void PyriteEditor::Build()
{
	//Run the build function
	saveManager->CreateBuild();
}

//Run the Game
void PyriteEditor::RunGame()
{
	isEditing = false;
}

#pragma endregion
