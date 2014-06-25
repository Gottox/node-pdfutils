{
	"targets": [{
		"target_name": "pdfutils",
		"sources": [
			"src/PdfDocument.cpp",
			"src/PdfEngine.cpp",
			"src/PdfPage.cpp",
			"src/init.cpp",
		],
	}, {
		"target_name": "popplerEngine",
		"sources": [
			"src/PdfDocument.cpp",
			"src/PdfEngine.cpp",
			"src/PdfEngineFactory.cpp",
			"src/PdfPage.cpp",
			"src/poppler/init.cpp",
		],
		"xcode_settings": {
			"OTHER_CFLAGS": [
				"<!@(pkg-config --cflags poppler-cairo)"
			],
		},
		"cflags": [
			"<!@(pkg-config --cflags poppler-cairo)"
		],
		"ldflags": [
			"<!@(pkg-config --libs poppler-cairo)"
		],
		"libraries": [
			"<!@(pkg-config --libs poppler-cairo)"
		]
	}]
}
