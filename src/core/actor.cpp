#include "actor.h"
#include "../utils/glm/glm.hpp"

Actor::Actor( Object *parent )  : Object( parent ), m_visible( true ) {

}

Actor::~Actor() {}