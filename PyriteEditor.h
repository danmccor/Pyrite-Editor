#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QLineEdit.h>
#include <QtWidgets/qsizepolicy.h>
#include <QtWidgets/qcombobox.h>

#include <QtCore/qcoreapplication.h>
#include "ui_PyriteEditor.h"
#include "Panda3D.h"
#include "GameObject.h"
#include "AddAction.h"

extern Panda3D pandaEngine;

class PyriteEditor : public QMainWindow
{
	Q_OBJECT

public:
	PyriteEditor(QWidget* parent = Q_NULLPTR);
public slots:
	void AddTransform();
	void AddCollision();
	void AddTrigger();
	void RunGame();

	void LoadSelectedObject();


	void AddActionBox(Action action = Action::Move, std::string key = "", float speed = NULL, Direction direction = Direction::Forward, bool newAction = true);
	void RemoveActionBoxes();
	void LoadActionBoxes();

	bool IsEditing() { return isEditing; };

	void SetObjectProperties(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0, float rotZ = 0, float scale = 0);
	void SetObjectProperties(GameObject* gameObject);
	void UpdateComponents();

	void AddTriggerAction();

private:

	int lastCollisionType = 1;
	int lastAmountOfObjects = 0;

	Ui::PyriteEngineClass ui;
	bool isEditing = true;
	QVBoxLayout* boxLayout;
	QWidget* widget;
	GameObject* selectedObject;
	bool oneTimeSelect = true;



	Q_ENUM(Direction);
	Q_ENUM(Action);
	Q_ENUM(CollisionType);
};
