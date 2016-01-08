#include "application.h"
#include <algorithm>

void 
Application::attachEventHandler( EventHandler* handler ) {

	EventHandlerList::iterator it =std::find( 
		handler_list.begin(), 
		handler_list.end(),
		handler );
	if( it == handler_list.end() )
		handler_list.push_back( handler );
}

void
Application::detachEventHandler( EventHandler* handler ) {
	EventHandlerList::iterator it =std::find( 
		handler_list.begin(), 
		handler_list.end(),
		handler );
	if( it != handler_list.end() )
		handler_list.erase( it );
}
		
void
Application::emit( Event* e ) {
	for( int i =0; i < handler_list.size(); i++ ) {
		handler_list[i]->event( e );
		if( e->isAccepted() )
			break;
	}
}