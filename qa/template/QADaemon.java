// Open Ephyra
import info.ephyra.*;

// Thrift java libraries 
import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TServer.Args;
import org.apache.thrift.server.TSimpleServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.transport.TSSLTransportFactory;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TServerTransport;
import org.apache.thrift.transport.TSSLTransportFactory.TSSLTransportParameters;

// Generated code
import qastubs.QAService;

// Java libraries
import java.util.HashMap;

// Start the thrift qa server
public class QADaemon {
	public static QAServiceHandler handler;
	public static QAService.Processor<QAServiceHandler> processor;
	public static void main(String [] args) {
		try {
			int tmp_port = 9091;
			if (args.length == 1)
			{
				tmp_port = Integer.parseInt(args[0].trim());
				System.out.println("Using port: " + tmp_port);
			}
			else
			{
				System.out.println("Using default port for question answer service: " + tmp_port);
			}
			// inner classes receive copies of local variables to work with.
			// Local vars must not change to ensure that inner classes are synced.
			final int port = tmp_port;
			
			// the handler implements the generated java interface
			// that was originally specified in the thrift file.
			// When it's called, an Open Ephyra object is created.
			handler = new QAServiceHandler();
			processor = new QAService.Processor<QAServiceHandler>(handler);
			Runnable simple = new Runnable() {
				// This is the code that the thread will run
				public void run() {
					simple(processor, port);
				}
			};
			// Let system schedule the thread
			new Thread(simple).start();
		} catch (Exception x) {
			x.printStackTrace();
		}
	}

	//public static void simple(QAService.Processor processor, int port, int cmdcenterport) {
	public static void simple(QAService.Processor processor, int port) {
		try {
			// Start the question-answer server
			TServerTransport serverTransport = new TServerSocket(port);
			//TServer server = new TSimpleServer(new Args(serverTransport).processor(processor));
			TServer server = new TThreadPoolServer(new TThreadPoolServer.Args(serverTransport).processor(processor));
			
			Thread t1 = new Thread(new Runnable() {
			    public void run() {
		         	System.out.println("Starting the question-answer server at port " + port + "...");
					server.serve();
			    }
			});  
			t1.start();
			t1.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
