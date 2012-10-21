{
	'targets': [{
		'target_name': 'pdfutils',
		"sources": [ "src/document.cc", "src/page.cc", "src/page_job.cc", "src/init.cc", "src/util.cc", "src/formats.cc" ],
		'cflags': [
			'<!@(pkg-config --cflags poppler-glib cairo)'
		],
		'ldflags': [
			'<!@(pkg-config --libs-only-L --libs-only-other poppler-glib cairo)'
		],
		'libraries': [
			'<!@(pkg-config --libs-only-l poppler-glib cairo)'
		]
	}]
}
