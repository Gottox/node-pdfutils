var http = require('http');
var pdfutils = require("../../index").pdfutils;

var port = process.argv[2];
var file = process.argv[3];
if(port === undefined || file === undefined) {
	console.log("Usage: " + process[0] + " " + process[1] + " <port> <pdf file>")
	return;
}

function dashboard(doc, res) {
		res.writeHead(200, {'Content-Type': 'text/html'});
		for(var i = 0; i < doc.length; i++) {
			res.write('<a href="/'+i+'.html"><img src="/'+i+'.png" width="200" /></a>');
		}
		res.end();
}

function pageHtml(doc, match, res) {
	res.writeHead(200, {'Content-Type': 'text/html'});
	if(match[1] != 0)
		res.write('<a href="'+(match[1]-1)+'.html">&lt;- Previous Page</a>\n');
	res.write('<a href="/">^ Dashboard ^</a>\n');
	if(match[1] <= doc.length-1)
		res.write('<a href="'+(parseInt(match[1])+1)+'.html">Next -&gt;</a>\n');
	res.end("<br><embed width='600' src='"+match[1]+".svg' type='image/svg+xml' />");
}

function page(doc, match, res) {
	var stream;
	match[2] == 'png';
	if(match[2] == 'svg') {
		res.writeHead(200, {'Content-Type': 'image/svg+xml'});
		doc[match[1]].asSVG({minWidth: 600}).pipe(res, {end: true});
	}
	else {
		res.writeHead(200, {'Content-Type': 'image/png'});
		doc[match[1]].asPNG({width: 200}).pipe(res, {end: true});
	}
}

pdfutils(file, function(err, doc) {
	http.createServer(function (req, res) {
		var match;
		if(req.url == "/")
			 dashboard(doc, res);
		else if((match = req.url.match(/([0-9]+)\.html$/)) && match[1] < doc.length)
			pageHtml(doc, match, res);
		else if((match = req.url.match(/([0-9]+)\.(png|svg)$/)) && match[1] < doc.length)
			page(doc, match, res);
		else {
			res.writeHead(404, {'Content-Type': 'text/html'});
			res.end('not found\n');
		}
	}).listen(port, '0.0.0.0');
});
