package edu.umd.cs.mind;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.geronimo.javamail.transport.smtp.SMTPSTransport;
import java.security.Security;
import java.util.Date;
import java.util.Properties;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

/**
 * Servlet implementation class authID
 */
public class authID extends HttpServlet {
	private static final long serialVersionUID = 1L;
	public String strFile = "/home/prbharga/dotsserver/checkid";
	public static int count = 0;
	public Map<String, String> idName = new HashMap<String, String>();
	public Map<String, String> idEmail = new HashMap<String, String>();

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public authID() {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @return
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	public void init() {

		BufferedReader br;
		try {
			br = new BufferedReader(new FileReader(strFile));
			String strLine = "";
			StringTokenizer st = null;
			while ((strLine = br.readLine()) != null) {
				String[] arr = strLine.split(",");
				// System.out.println("id" + arr[0]);
				// System.out.println("name" + arr[1]);
				if (!idName.containsKey(arr[0])) {
					idName.put(arr[0], arr[1]);
					idEmail.put(arr[0], arr[2]);
				}
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doPost(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		PrintWriter out = response.getWriter();
		// String destination = "";
		String id = null;
		InputStream is = null;
		is = request.getInputStream();
		id = request.getParameter("checkID").trim();
		// MessageConnection con = (MessageConnection)Connector.open("sms://+" +
		// msisdn);
		System.out.println("ID:"+id);
		if (idName.containsKey(id)) {
			count++;
			System.out.println("Hello " + idName.get(id));
			out.println("<Authorized>");
			Security.addProvider(new com.sun.net.ssl.internal.ssl.Provider());
			final String SSL_FACTORY = "javax.net.ssl.SSLSocketFactory";

			// Get a Properties object
			Properties props = System.getProperties();
			props.setProperty("mail.smtps.host", "smtp.gmail.com");
			props.setProperty("mail.smtp.socketFactory.class", SSL_FACTORY);
			props.setProperty("mail.smtp.socketFactory.fallback", "false");
			props.setProperty("mail.smtp.port", "465");
			props.setProperty("mail.smtp.socketFactory.port", "465");
			props.setProperty("mail.smtps.auth", "true");

			/*
			 * If set to false, the QUIT command is sent and the connection is
			 * immediately closed. If set to true (the default), causes the
			 * transport to wait for the response to the QUIT command.
			 * 
			 * ref :
			 * http://java.sun.com/products/javamail/javadocs/com/sun/mail/
			 * smtp/package-summary.html
			 * http://forum.java.sun.com/thread.jspa?threadID=5205249
			 * smtpsend.java - demo program from javamail
			 */
			props.put("mail.smtps.quitwait", "false");

			Session session = Session.getInstance(props, null);

			// -- Create a new message --
			final MimeMessage msg = new MimeMessage(session);

			// -- Set the FROM and TO fields --
			try {
				msg.setFrom(new InternetAddress("838fassgnmnt3@gmail.com"));
				msg.setRecipients(Message.RecipientType.TO,
						InternetAddress.parse(idEmail.get(id), false));

				msg.setSubject("Hi");
				msg.setText("You just boarded Shuttle #1", "utf-8");
				msg.setSentDate(new Date());

				SMTPSTransport t = (SMTPSTransport) session.getTransport("smtps");

				t.connect("smtp.gmail.com", "838fassgnmnt3@gmail.com",
						"shuttlecheck");
				t.sendMessage(msg, msg.getAllRecipients());
				t.close();
			} catch (AddressException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (MessagingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		} else {
			System.out.println("Unauthorized");
			out.println("<Unauthorized>");
		}
		out.close();
	}
}
