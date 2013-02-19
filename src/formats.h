#ifndef FORMATS_H
#define FORMATS_H
enum Format {
	FORMAT_SVG,
	FORMAT_PNG,
	FORMAT_PDF,
	FORMAT_TEXT,
	FORMAT_UNKOWN
};

inline const char *formatName(int id) {
	switch(id) {
	case FORMAT_SVG: return "asSVG";
	case FORMAT_PNG: return "asPNG";
	case FORMAT_PDF: return "asPDF";
	case FORMAT_TEXT: return "asText";
	default: return NULL;
	}
}
#endif
