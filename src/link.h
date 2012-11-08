#ifndef LINK_H
#define LINK_H

#include <node.h>
#include <poppler.h>
#include "page.h"

class Document;

class Link : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		Page *page;

		Link(Page *page, PopplerLinkMapping *link);


	private:
		~Link();

		static v8::Persistent<v8::Function> constructor;

		void createObject();

		PopplerAction *action;
		double x;
		double y;
		double w;
		double h;
		char *title;
		void fillGoto();
		void fillRemote();
		void fillLaunch();
		void fillUri();
		void fillNamed();
		void fillMovie();
		void fillRendition();
		void fillOCGState();
		void fillJavascript();

		void fillDest(PopplerDestType dest);
};

#endif
