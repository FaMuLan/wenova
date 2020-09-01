#include "State.h"
#include "JoystickButton.h"
#include "InputManager.h"
#include "Collision.h"

#define ABXY_CENTER_X 1145
#define ABXY_CENTER_Y 610
#define DPAD_CENTER_X 135
#define DPAD_CENTER_Y 610
#define TRIGGER_CENTER_X 640
#define TRIGGER_CENTER_Y 120
#define START_CENTER_X 640
#define START_CENTER_Y 670
#define BUTTON_DISTANCE 50
#define DPAD_DISTANCE 35
#define TRIGGER_DISTANCE_X 450
#define TRIGGER_DISTANCE_Y 40
#define START_DISTANCE_X 60

State::State(){
	m_pop_requested = m_quit_requested = false;
	add_object(new JoystickButton(DPAD_CENTER_X, DPAD_CENTER_Y - DPAD_DISTANCE, "UP", InputManager::UP, 0, "up"));
	add_object(new JoystickButton(DPAD_CENTER_X + DPAD_DISTANCE, DPAD_CENTER_Y, "RIGHT", InputManager::RIGHT, 0,"right"));
	add_object(new JoystickButton(DPAD_CENTER_X, DPAD_CENTER_Y + DPAD_DISTANCE, "DOWN", InputManager::DOWN, 0, "down"));
	add_object(new JoystickButton(DPAD_CENTER_X - DPAD_DISTANCE, DPAD_CENTER_Y, "LEFT", InputManager::LEFT, 0, "left"));

	add_object(new JoystickButton(ABXY_CENTER_X, ABXY_CENTER_Y - BUTTON_DISTANCE, "Y", InputManager::Y, 0, "button"));
	add_object(new JoystickButton(ABXY_CENTER_X + BUTTON_DISTANCE, ABXY_CENTER_Y, "B", InputManager::B, 0, "button"));
	add_object(new JoystickButton(ABXY_CENTER_X, ABXY_CENTER_Y + BUTTON_DISTANCE, "A", InputManager::A, 0, "button"));
	add_object(new JoystickButton(ABXY_CENTER_X - BUTTON_DISTANCE, ABXY_CENTER_Y, "X", InputManager::X, 0, "button"));
	
	add_object(new JoystickButton(TRIGGER_CENTER_X - TRIGGER_DISTANCE_X, TRIGGER_CENTER_Y + TRIGGER_DISTANCE_Y, "LB", InputManager::LB, 0, "trigger"));
	add_object(new JoystickButton(TRIGGER_CENTER_X + TRIGGER_DISTANCE_X, TRIGGER_CENTER_Y + TRIGGER_DISTANCE_Y, "RB", InputManager::RB, 0, "trigger"));
	add_object(new JoystickButton(TRIGGER_CENTER_X - TRIGGER_DISTANCE_X, TRIGGER_CENTER_Y - TRIGGER_DISTANCE_Y, "LT", InputManager::LT, 0, "back_trigger"));
	add_object(new JoystickButton(TRIGGER_CENTER_X + TRIGGER_DISTANCE_X, TRIGGER_CENTER_Y - TRIGGER_DISTANCE_Y, "RT", InputManager::RT, 0, "back_trigger"));

	add_object(new JoystickButton(START_CENTER_X - START_DISTANCE_X, START_CENTER_Y, "select", InputManager::SELECT, 0, "select_start"));
	add_object(new JoystickButton(START_CENTER_X + START_DISTANCE_X, START_CENTER_Y, "start", InputManager::START, 0, "select_start"));

}

State::~State(){
}

void State::add_object(GameObject * object){
	object_array.emplace_back(object);
}

bool State::pop_requested(){
	return m_pop_requested;
}

bool State::quit_requested(){
	return m_quit_requested;
}

void State::update_array(float delta){
	//collision tests
	for(unsigned i = 0; i < object_array.size(); ++i){
		for(unsigned j = i + 1; j < object_array.size(); ++j){
			auto a = object_array[i].get();
			auto b = object_array[j].get();
			if(Collision::is_colliding(a->box, b->box, a->rotation, b->rotation)){
				a->notify_collision(*b);
				b->notify_collision(*a);
			}
		}
	}

	//update
	for(unsigned it = 0; it < object_array.size(); ++it){
		object_array[it]->update(delta);
	}

	//death check
	for(unsigned it = 0; it < object_array.size(); ++it){
		if(object_array[it]->is_dead()){
			object_array.erase(object_array.begin() + it);
			break;
		}
	}
}

void State::render_array(){
	for(auto & go : object_array){
		go->render();
	}
}

void State::load_assets(){
	//NOTHING TO DO
}
