var fs = require("fs");

function testDocumentStructure(pdf) {
	expect(pdf).to.be.an(Object);
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
}

describe('pdfutils', function() {
	it("can be initialized", function() {
		pdfutils();
		expect(pdfutils.PdfDocument).to.be.an(Function);
		new pdfutils.PdfDocument()
	});

	it("can load PDF File synchronously", function() {
		var pdf = pdfutils(__dirname + "/pdfs/testfile1.pdf");
		testDocumentStructure(pdf);
	});

	it("can load PDF Data synchronously", function() {
		var pdf = pdfutils(fs.readFileSync(__dirname + "/pdfs/testfile1.pdf"));
		testDocumentStructure(pdf);
	})

	it("can load PDF File asynchronously", function(done) {
		var pdf = pdfutils(__dirname + "/pdfs/testfile1.pdf", function(err, pdf) {
			testDocumentStructure(pdf);
			done();
		});
		expect(pdf).to.be(undefined);
	});

	it("should throw on nonexistent file on synchronous load", function() {
		try {
			var pdf = pdfutils(__dirname + "/pdfs/NOFILE.pdf");
			expect().fail("does not throw")
		} catch(err) {
			expect(err).to.be.an(Error);
		}
	});

	it("should callback error on nonexistent file on asynchronous load", function(done) {
		var pdf = pdfutils(__dirname + "/pdfs/NOFILE.pdf", function(err, pdf) {
			expect(err).to.be.an(Error);
			expect(pdf).to.be(null);
			done();
		});
	});

	/*it("should save PDFs", function() {
		var pdf = pdfutils(__dirname + "/pdfs/NOFILE.pdf");
		var file = "/tmp/pdfutils" + Math.random() + ".pdf";

		pdf.asPdf().toFile(file);

		var pdf2 = pdfutils(file);
		testDocumentStructure(pdf2);

		fs.unlinkSync(file);
	});
	*/
});

