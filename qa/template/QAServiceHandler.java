// This handler implements the services provided to the client.

// Open Ephyra packages
import info.ephyra.OpenEphyra;
import info.ephyra.search.Result;
import info.ephyra.io.MsgPrinter;

// Java packages
import java.util.*;

// Interface definition
import qastubs.QAService;

public class QAServiceHandler implements QAService.Iface {
	private OpenEphyra oe;

	public QAServiceHandler()
	{
		String dir = "";

		MsgPrinter.enableStatusMsgs(true);
		MsgPrinter.enableErrorMsgs(true);

		oe = new OpenEphyra(dir);
	}

	public String askFactoidThrift(String question)
	{
		MsgPrinter.printStatusMsg("askFactoidThrift(): Arg = " + question);
		
		Result result = oe.askFactoid(question);
		String answer = result.getAnswer();

		return answer;
	}

	public List<String> askListThrift(String question)
	{
		float relThresh = 0.5f; //user may change this value
		
		MsgPrinter.printStatusMsg("askListThrift(): Arg = " + question);

		Result[] results = oe.askList(question, relThresh);
		List<String> answersList = new ArrayList<String>();
		// add all answers to answersList
		for (Result r : results)
		{
			answersList.add(r.getAnswer());
		}
		return answersList;
	}
}

