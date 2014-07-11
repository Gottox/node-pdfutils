{
	"targets": [{
		"target_name": "pdfutils",
		"sources": [
			"src/PdfController.cpp",
			"src/PdfDocument.cpp",
			"src/PdfEngine.cpp",
			"src/PdfEngineFactory.cpp",
			"src/PdfPage.cpp",
			"src/PdfPageController.cpp",
			"src/init.cpp",
			"src/v8utils.cpp",
		],
		"include_dirs" : [
			"<!(node -e \"require('nan')\")"
		],
	}, {
		"target_name": "popplerEngine",
		"sources": [
			"src/PdfDocument.cpp",
			"src/PdfEngine.cpp",
			"src/PdfEngineFactory.cpp",
			"src/PdfPage.cpp",
			"src/poppler/PopplerEngine.cpp",
			"src/v8utils.cpp",
		],
		"cflags": [
			"<!@(pkg-config --cflags poppler-glib)"
		],
		"libraries": [
			"<!@(pkg-config --libs poppler-glib)"
		]
	}]
}
