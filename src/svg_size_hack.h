#ifndef SVG_SIZE_HACK_H
#define SVG_SIZE_HACK_H

#include "util.h"

// A hack to let cairo output px instead of pt sized SVGs.

enum State {
	WHITESPACE,
	TAG_START,
	TAG_NAME,
	TAG_WHITESPACE,
	ATTRIBUTE_NAME,
	ATTRIBUTE_EQUALS,
	ATTRIBUTE_OPEN_QUOTES,
	ATTRIBUTE_VALUE,
	ATTRIBUTE_CLOSE_QUOTES
};

enum Attribute {
	ATTR_UNKNOWN,
	ATTR_WIDTH,
	ATTR_HEIGHT
};

class SvgSizeHack {
	public:
		SvgSizeHack();
		~SvgSizeHack();
		void parse(char *chunk, int length);
		bool finished;
	private:
		inline void addBuffer(char c) {
			if(this->bufpos-1 >= (int)LENGTH(this->buffer))
				return;
			this->buffer[this->bufpos++] = c;
			this->buffer[this->bufpos] = 0;
		}
		enum State state;
		char quote;
		char buffer[8];
		int bufpos;
		bool isSVGTag;
		bool widthDone;
		bool heightDone;
		enum Attribute attribute;
};

#endif
