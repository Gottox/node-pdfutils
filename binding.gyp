{
	"targets": [{
		"target_name": "pdfutils",
		"sources": [
			"src/init.cpp",
			"src/PdfEngine.cpp",
		],
	}, {
		"target_name": "popplerEngine",
		"sources": [
			"src/poppler/init.cpp",
			"src/PdfEngineFactory.cpp",
			"src/PdfEngine.cpp",
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
