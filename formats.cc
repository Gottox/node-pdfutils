#ifndef FORMATS_H
#define FORMATS_H
enum Format {
	FORMAT_SVG,
	FORMAT_PNG
};

extern const char *formatFunctions[] = {
	[FORMAT_SVG] = "toSVG",
	[FORMAT_PNG] = "toPNG",
	NULL,
};
#endif
