#include "Panda3D.h"

bool Panda3D::init(size_t hwnd, int argc, char* argv[], int width, int height, int originX, int originY)
{
    //Open the panda3D framework
    framework.open_framework(argc, argv);
    //Create a windows properties variable
    WindowProperties properties;
    //Retrieve the default window properties
    framework.get_default_window_props(properties);
    //Set the parent window to the handle
    properties.set_parent_window(hwnd);
    //Set the size of window to the width and height of the handle window
    properties.set_size(width, height);
    //Set the origin to the handles origin
    properties.set_origin(originX, originY);
    //Create the panda3D window using properties
    window = framework.open_window(properties, 0);
    //4window->setup_trackball();
    window->enable_keyboard();
    camera = window->get_camera_group();
    camera.set_pos(0, -10, 0);
    mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());

    yupAxis = window->load_model(framework.get_models(), "models/yup-axis");
    yupAxis.reparent_to(window->get_render());
    yupAxis.hide();

    cHandler = new CollisionHandlerQueue();
    cTravHandler = new CollisionHandlerQueue();
    cPusher = new CollisionHandlerPusher();

    collisionRay_Node = new CollisionNode("mouseRay");
    collisionRay_NodePath = camera.attach_new_node(collisionRay_Node);
    collisionRay_Node->set_collide_mask(BitMask32::bit(1));
    collisionRay = new CollisionRay();
    collisionRay_Node->add_solid(collisionRay);
    collisionRay_NodePath.show();
    cTrav.add_collider(collisionRay_NodePath, cHandler);
    NodePath render = window->get_render();
    cTrav.show_collisions(render);

    

    return true;
}


void Panda3D::closePanda3D()
{
    framework.close_all_windows();
    framework.close_framework();
}

void Panda3D::runLoop()
{
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i].Run();
        CheckObjectCollisions();
        CheckObjectTriggers();
    }   
    //window->loop_animations(0);
}

void Panda3D::CameraMovement()
{
    if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('W'))) {
        OutputDebugStringW(L"W pressed!");
        camera.set_pos(camera.get_x() + 10);
    }

}

void Panda3D::MouseCollider()
{
    static float mouseX = mouseWatcher->get_mouse_x();
    static float mouseY = mouseWatcher->get_mouse_y();
    //this works
    if (selectedObject != nullptr) {
        yupAxis.set_pos(selectedObject->GetModelNodePath().get_pos());
    }

    if (!mouseWatcher->has_mouse()) {
        return;
    }

    LPoint2 mpos = mouseWatcher->get_mouse();

    collisionRay->set_from_lens(window->get_camera(0), mpos);
    cTrav.traverse(window->get_render());
    if (cHandler->get_num_entries() > 0) {
        cHandler->sort_entries();
        int i = atoi(cHandler->get_entry(0)->get_into_node()->get_tag("Object").c_str());
        if (mouseWatcher->is_button_down(MouseButton::one())) {
            if (selectedObject != nullptr) {
                selectedObject->ToggleHighlight();
            }
            selectedObject = &gameObjects[i];
            selectedObject->ToggleHighlight();
            yupAxis.set_pos(selectedObject->GetModelNodePath().get_pos());
            yupAxis.show();
        }
    }
    else {
        if (mouseWatcher->is_button_down(MouseButton::one())) {
            if (selectedObject != nullptr) {
                selectedObject->ToggleHighlight();
            }
            yupAxis.hide();
            selectedObject = nullptr;
        }
    }

    static bool InitialRightClick = true;
    if (mouseWatcher->is_button_down(MouseButton::three())) {
        if (!InitialRightClick) {
            static float heading = 0.0f;
            static float pitch = 0.0f;

            double mouseX = window->get_graphics_window()->get_pointer(0).get_x();
            double mouseY = window->get_graphics_window()->get_pointer(0).get_y();

            mouseX -= window->get_graphics_window()->get_x_size() / 2;
            mouseY -= window->get_graphics_window()->get_y_size() / 2;

            heading -= mouseX;
            pitch -= mouseY;

            camera.set_hpr(heading, pitch, 0);

            if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('w'))) {
                LVecBase3f forward = camera.get_net_transform()->get_mat().get_row3(1);
                forward.normalize();
                camera.set_pos(camera.get_pos() + (forward / 100));

            }
            if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('s'))) {
                LVecBase3f forward = camera.get_net_transform()->get_mat().get_row3(1);
                forward.normalize();
                camera.set_pos(camera.get_pos() - (forward / 100));
            }
            if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('d'))) {
                camera.set_x(camera.get_x() + 0.01f);
            }
            if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('a'))) {
                camera.set_x(camera.get_x() - 0.01f);
            }
        }
        window->get_graphics_window()->move_pointer(0, window->get_graphics_window()->get_x_size() / 2, window->get_graphics_window()->get_y_size() / 2);
        InitialRightClick = false;
    }
    else {
        InitialRightClick = true;
    }
  
}

void Panda3D::AddCollider(NodePath collider)
{
    cTrav.add_collider(collider, cHandler);
}


void Panda3D::createObject(std::string modelLocation)
{
    GameObject gameObject(window, modelLocation);
    gameObject.LoadModel(&framework);
    gameObjects.push_back(gameObject);
}

GameObject* Panda3D::GetSelectedObject()
{
    return selectedObject;
}

void Panda3D::CheckObjectCollisions()
{
    static bool firstLoop = true;
    if (firstLoop)
    {
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i].HasCollision()) {
                cPusher->add_collider(gameObjects[i].GetColNodePath(), gameObjects[i].GetModelNodePath());
                cTrav.add_collider(gameObjects[i].GetColNodePath(), cPusher);
            }
        }
        firstLoop = false;
    }
}

void Panda3D::CheckObjectTriggers()
{
    static bool triggersAdded = false;
    if (!triggersAdded) {
        for (int i = 0; i < gameObjects.size(); i++) {
            if (gameObjects[i].HasTrigger()) {
                cTriggerTrav.add_collider(gameObjects[i].GetTriggerNodePath(), cTravHandler);
            }
        }
        triggersAdded = true;
    }
    cTriggerTrav.traverse(window->get_render());
    if (cTravHandler->get_num_entries() > 0) {
        int i = atoi(cTravHandler->get_entry(0)->get_from_node()->get_tag("TriggerBox").c_str());
        for (int j = 0; j < gameObjects[i].GetNumberOfTriggerActions(); j++) {
            std::string Output = "GameObject objectCount: " + std::to_string(gameObjects[i].objectCount) + " GameObject ID: " + std::to_string(gameObjects[i].id) + "\n";
            OutputDebugStringA(Output.c_str());
            TriggerActions triggerActions = gameObjects[i].GetTriggerAction(j);
            
            gameObjects[triggerActions.connectedObjectID].ChangeTransformAction(
                triggerActions.actionID,
                Action(triggerActions.newAction),
                gameObjects[triggerActions.connectedObjectID].GetTransformAction(triggerActions.actionID).Key,
                gameObjects[triggerActions.connectedObjectID].GetTransformAction(triggerActions.actionID).Speed,
                Direction(triggerActions.newDirection)
            );


        }
    }
}

void Panda3D::AddWorldTrigger()
{
    GameObject triggerBox(window, TriggerType::Box);
    gameObjects.push_back(triggerBox);
}

std::vector<GameObject> Panda3D::GetVectorOfGameObjects()
{
    return gameObjects;
}

