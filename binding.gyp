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
		"cflags": [
			"-fPIC"
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
		"xcode_settings": {
			"OTHER_CFLAGS": [
				"<!@(pkg-config --cflags poppler-glib) -std=c++0x"
			],
		},
		"cflags": [
			"<!@(pkg-config --cflags poppler-glib) -fPIC -std=c++0x"
		],
		"ldflags": [
			"<!@(pkg-config --libs poppler-glib)"
		],
		"libraries": [
			"<!@(pkg-config --libs poppler-glib)"
		]
	}]
}
