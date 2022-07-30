from http.server import SimpleHTTPRequestHandler, HTTPServer
import sys, json

SERVER = ("0.0.0.0", 80)

class HTTPCustomHandler(SimpleHTTPRequestHandler):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, directory=".", **kwargs)

	def do_GET(self):
		if self.path == "/ip-api.com/json":
			body = b""
			code = sys.argv[1] if len(sys.argv) == 2 else "default"
			with open(f"ip-api.com/json-{code}.json") as f: body = f.read().encode("utf-8")
			self.send_response(200)
			self.send_header("Content-Type", "application/json; charset=utf-8")
			self.send_header("Content-Length", str(len(body)))
			self.send_header("Access-Control-Allow-Origin", "*")
			self.end_headers()
			self.wfile.write(body)
		else: super().do_GET()

if __name__ == "__main__":
	server = HTTPServer(SERVER, HTTPCustomHandler)
	print(f"Serving HTTP on {SERVER} ...")
	server.serve_forever()
