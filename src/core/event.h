/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef EVENT_H
#define EVENT_H

enum EventType {
	EVENT_NONE =0x0,
	EVENT_KEYPRESS =0x1
};

class Event {
	public:
		Event() : m_accepted( false ) {}
	
		void accept() { m_accepted =true; }
		bool isAccepted() const { return m_accepted; }
		
		virtual EventType type() const =0;
	
	protected:
		bool m_accepted;
};

class KeyPressEvent : public Event {
	public:
		enum KeyType {
			KEY_CHARACTER
		};
		KeyPressEvent( KeyType t, char c=0 ) : m_type( t ), m_char( c ) {}	
		
		char getChar() const { return m_char; }
		EventType type() const { return EVENT_KEYPRESS; }
		
	private:
		KeyType m_type;
		char m_char;
};

class EventHandler {
	public:
		EventHandler() : m_filters(0) {}
		
		void setEventFilters( int pass ) { m_filters =pass; }
		int eventFilters() const { return m_filters; }
		
		virtual void event( Event* e ) =0;
		
	private:
		int m_filters;
};

#endif
