{
  'targets': [
    {
      'target_name': 'pdfconv',
      "sources": [ "src/document.cc", "src/page.cc", "src/page_job.cc", "src/init.cc", "src/util.cc", "src/formats.cc" ],
      'libraries': [
         '-lpoppler',
         '-lcairo',
         '-lpoppler-glib'
      ],
      'include_dirs': [
         '/usr/include/poppler/glib',
         '/usr/include/glib-2.0',
         '/usr/lib/i386-linux-gnu/glib-2.0/include',
         '/usr/include/cairo/'
      ],
    }
  ]
}
