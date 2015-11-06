#include "object.h"

Object::Object( Object* parent ) : parent( parent ) {
	if( parent )
		parent->children.insert( this );
}

Object::~Object() {
	Objects cpy =children;
	for( Objects::iterator it =cpy.begin(); it != cpy.end(); it++ )
		delete (*it);
		
	if( parent )
		parent->children.erase( this );
}
	
Object*
Object::getParent( ) const { return parent; }

Object::Objects
Object::getChildren( ) const {
	return children;
}

void 
Object::update( float deltatime ) {
	for( Objects::iterator it =children.begin(); it != children.end(); it++ )
		(*it)->update( deltatime );
}
