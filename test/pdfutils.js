describe('pdfutils', function() {
	it("can be initialized", function() {
		pdfutils();
		expect(pdfutils.PdfDocument).to.be.an(Function);
		new pdfutils.PdfDocument()
	});
	it("can load PDF File synchronously", function() {
		var pdf = pdfutils(__dirname + "/pdfs/testfile1.pdf");
		expect(pdf).to.be.an(Object);
	});

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

