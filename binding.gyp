{
	"targets": [{
		"target_name": "pdfutils",
		"sources": [
			"src/PdfController.cpp",
			"src/PdfDocument.cpp",
			"src/PdfEngine.cpp",
			"src/PdfEngineFactory.cpp",
			"src/PdfPage.cpp",
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
				"<!@(pkg-config --cflags poppler-glib)"
			],
		},
		"cflags": [
			"<!@(pkg-config --cflags poppler-glib) -fPIC"
		],
		"ldflags": [
			"<!@(pkg-config --libs poppler-glib)"
		],
		"libraries": [
			"<!@(pkg-config --libs poppler-glib)"
		]
	}]
}
