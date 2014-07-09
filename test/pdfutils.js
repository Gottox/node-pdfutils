var fs = require("fs");

describe('pdfutils', function() {
	it("can be initialized", function() {
		pdfutils();
		expect(pdfutils.PdfDocument).to.be.an(Function);
		new pdfutils.PdfDocument()
	});
	it("can load PDF File synchronously", function() {
		var pdf = pdfutils(__dirname + "/pdfs/testfile1.pdf");
		expect(pdf).to.be.an(Object);
		expect(pdf._engine.name).to.be('poppler');
		expect(pdf.length).to.be.greaterThan(0);
		expect(pdf).to.have.property('author');
		expect(pdf).to.have.property('creation_date');
		expect(pdf).to.have.property('keywords');
		expect(pdf).to.have.property('linearized');
		expect(pdf).to.have.property('metadata');
		expect(pdf).to.have.property('modification_date');
		expect(pdf).to.have.property('pageLayout');
		expect(pdf).to.have.property('pageMode');
		expect(pdf).to.have.property('producer');
		expect(pdf).to.have.property('subject');
		expect(pdf).to.have.property('title');
		expect(pdf.permissions).to.only.have.keys(
				'print',
				'modify',
				'copy',
				'addNotes',
				'fillForm',
				'extractContents',
				'assemble',
				'printHighResolution'
		);
	});

	it("can load PDF File synchronously from data", function() {
		var pdf = pdfutils(fs.readFileSync(__dirname + "/pdfs/testfile1.pdf"));
		expect(pdf).to.be.an(Object);
	})

	/*it("can load PDF File asynchronously", function(done) {
		var pdf = pdfutils(__dirname + "/pdfs/testfile1.pdf", function(err, pdf) {
			expect(pdf).to.be(null)
			expect(pdf).to.be.an(Object)
			done();
		});
		expect(pdf).to.be(undefined);
	});*/

	it("should throw on nonexistent file", function() {
		try {
			var pdf = pdfutils(__dirname + "/pdfs/UNEXISTENT.pdf");
			expect().fail("does not throw")
		} catch(e) {
		}
	})
});

