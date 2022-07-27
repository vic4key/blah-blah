from http.server import BaseHTTPRequestHandler, HTTPServer
import sys, json

SERVER = ("0.0.0.0", 80)

class RequestHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		body = b""
		endpoint = "ip-api.com/json"
		code = sys.argv[1] if len(sys.argv) == 2 else "default"
		if self.path == f"/{endpoint}":
			with open(f"{endpoint}-{code}.json") as f: body = f.read().encode("utf-8")
		self.send_response(200)
		self.send_header("Content-Type", "application/json; charset=utf-8")
		self.send_header("Content-Length", str(len(body)))
		self.send_header("Access-Control-Allow-Origin", "*")
		self.end_headers()
		self.wfile.write(body)
		return

if __name__ == "__main__":
	server = HTTPServer(SERVER, RequestHandler)
	print(f"Serving HTTP on {SERVER} ...")
	server.serve_forever()
