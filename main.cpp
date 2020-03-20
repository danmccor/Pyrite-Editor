#include "PyriteEditor.h"
#include <QtWidgets/QApplication>
#include "Panda3D.h"
#include "GameObject.h"

ClockObject globalClock;
Panda3D pandaEngine;

int main(int argc, char* argv[])
{
	//Start application
	QApplication a(argc, argv);
	//Create window
	PyriteEditor pyriteEditor;
	//show window
	pyriteEditor.show();

	//Find scene widget
	QWidget* qw = pyriteEditor.findChild<QWidget*>("RenderZone");

	//Initialise Panda3D and show scene
	pandaEngine.init((size_t)qw->winId(), argc, argv, qw->size().width(), qw->size().height(), qw->pos().x(), qw->pos().y());
	//Display panda3D scene
	pandaEngine.displayScene();

	//Create thread
	Thread* mainloop = Thread::get_current_thread();
	//Loop of the engine 
	while (pandaEngine.framework.do_frame(mainloop)) {
		//Process application events
		a.processEvents();
		//Check for keyboard and mouse inputs
		pandaEngine.MouseCollider();
		pandaEngine.runLoop();
		pyriteEditor.SetObjectProperties(pandaEngine.GetSelectedObject());
		pyriteEditor.UpdateComponents();
		if (!pyriteEditor.IsEditing()) {
			pandaEngine.runLoop();
		}


		//check if window is still active
		if (!pyriteEditor.isVisible()) {
			//close panda3d
			pandaEngine.closePanda3D();
			//quit application
			a.quit();
		}
		globalClock.tick();
	}
	return 0;
}
