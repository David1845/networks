from tornado.ioloop import IOLoop
from tornado.options import define, options
from tornado import gen
from tornado.iostream import StreamClosedError
from tornado.tcpclient import TCPClient
from tornado.tcpserver import TCPServer
import tornado.web

define('port', default=8888, help="TCP port to use")
define('server', default=False, help="Run as the echo server")
define('encoding', default='utf-8', help="String encoding")

class MainHandler(tornado.web.RequestHandler):
    @gen.coroutine
    def get(self):
        "create TCP connection"
        stream = yield TCPClient().connect('127.0.0.1', options.port)
        try:
            "init connection to back end"
            stream.write(b"Poker\n");
            
            "Read reply from Back-end"
            data = yield stream.read_until('\n'.encode(options.encoding))
            
            "Send reply to browser"
            self.write(data);
            
            "print the data sent"
            print('Echoing data: ' + repr(data))
        except KeyboardInterrupt:
            stream.close()

if __name__ == "__main__":
    application = tornado.web.Application([
        (r"/", MainHandler),
    ])

    application.listen(8889);
    tornado.ioloop.IOLoop.current().start();
