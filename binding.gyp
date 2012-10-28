{
	'targets': [{
		'target_name': 'pdfutils',
		"sources": [ "src/document.cc", "src/page.cc", "src/page_job.cc", "src/init.cc", "src/formats.cc" ],
		'default_configuration': 'Debug',
		'xcode_settings': {
			'OTHER_CFLAGS': [
				'<!@(pkg-config --cflags poppler-glib cairo)'
			],
		},
		'cflags': [
			'<!@(pkg-config --cflags poppler-glib cairo)'
		],
		'ldflags': [
			'<!@(pkg-config --libs poppler-glib cairo)'
		],
		'libraries': [
			'<!@(pkg-config --libs poppler-glib cairo)'
		]
	}]
}
