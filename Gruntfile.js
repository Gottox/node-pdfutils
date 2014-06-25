/*
 * Gruntfile.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */
var REPORTER="dot";
var SRC = [ "lib/**/*.js", "index.js" ];

module.exports = function(grunt) {
	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),
		gyp: {
			debug: {
				options: {
					debug: true
				}
			},
			release: {
				options: {
					debug: false
				}
			},
			clean: {
				command: "clean"
			}
		},
		jshint: {
			all: SRC
		},
		jsdoc : {
			all: {
				src: [].concat.apply([ 'README.md' ], SRC),
				options: {
					destination: "doc",
					private: false
				}
			}
		},
		mochaTest: {
			all: {
				src: [ "test/*.js" ],
				options: {
					reporter: REPORTER,
					require: ["test/common.js"]
				}
			}
		},
		exec: {
			doxygen: "mkdir -p doc/cpp; doxygen doxygen.conf"
		},
		clean: [ "build", "doc" ]
	});

	grunt.loadNpmTasks('grunt-node-gyp');
	grunt.loadNpmTasks("grunt-contrib-jshint")
	grunt.loadNpmTasks("grunt-jsdoc")
	grunt.loadNpmTasks("grunt-mocha-test")
	grunt.loadNpmTasks('grunt-contrib-clean');
	grunt.loadNpmTasks('grunt-exec');

	grunt.registerTask('default', [
			'gyp:release'
	]);

	grunt.registerTask("test", [
			"gyp:debug",
			"jshint",
			"mochaTest"
	]);

	grunt.registerTask("doc", [
			"jsdoc",
			"exec:doxygen"
	]);
};
