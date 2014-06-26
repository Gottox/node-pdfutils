{
	"targets": [{
		"target_name": "pdfutils",
		"sources": [
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
				"<!@(pkg-config --cflags poppler-glib gio-unix-2.0)"
			],
		},
		"cflags": [
			"<!@(pkg-config --cflags poppler-glib gio-unix-2.0) -fPIC"
		],
		"ldflags": [
			"<!@(pkg-config --libs poppler-glib gio-unix-2.0)"
		],
		"libraries": [
			"<!@(pkg-config --libs poppler-glib gio-unix-2.0)"
		]
	}]
}
