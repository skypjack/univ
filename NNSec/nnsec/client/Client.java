/**
 * Client class represents the client GUI.
 *
 * @author Michele Caini
 */

package nnsec.client;

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JOptionPane;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextArea;
import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;
import javax.swing.event.TableModelListener;
import javax.swing.event.TableModelEvent;
import javax.swing.JFileChooser;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import javax.swing.Action;
import javax.swing.border.TitledBorder;
import javax.swing.border.BevelBorder;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.FocusListener;
import java.awt.event.FocusEvent;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.Dimension;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;
import java.security.AccessControlException;
import java.util.Properties;
import nnsec.cryptosystem.Cryptosystem;

/**
 * Client side GUI.
 */
public class Client extends JFrame {

	/** Java Swing related useful components. */
	private JTabbedPane jtb;
	private JPanel jpconn;
	private JPanel jpnets;
	private JPanel jplogs;

	/** Status tab components. */
	private JLabel staddr;
	private JTextField staddrf;
	private JLabel stport;
	private JTextField stportf;
	private JLabel stname;
	private JTextField stnamef;
	private JLabel ststat;
	private JComboBox jcbbits;
	private JComboBox jcbcert;
	private JTextArea jtapuk;

	/** Nets tab components. */
	private JTable netstable;

	/** Logs tab components. */
	private JTextArea ltlogs;

	/** Server informations. */
	private String id;
	private String addr;
	private int port;

	/** Communication module. */
	private CommModule cmif;

	/**
	 * Exception thrown when trying to exploit the protocol, and then some
	 * errors occur.
	 */
	static public class ClientException extends Exception {
		
		/**
		 * Constructs a ClientException.
		 */
		public ClientException() { super(); }

		/**
		 * Constructs a ClientException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public ClientException(String msg) { super(msg); }
	}

	/**
	 * Table model useful for the nets table.
	 */
	private class NetsTableModel extends AbstractTableModel {
		
		/** The columns name. */
		private String[] headers = { "ID", "Name", "Description", "Inputs", "Outputs" };

		/** Rows pool. */
		private Vector<Object[]> pool;

		/**
		 * Default constructor.
		 */
		public NetsTableModel() {
			pool = new Vector<Object[]>();
		}

		/**
		 * Method that permits to add a new row to the table.
		 * @param row the new row.
		 */
		public void insertRow(Object[] value) {
			pool.addElement(value);
			int pos = pool.size() - 1;
			fireTableRowsInserted(pos, pos);
		}

		/**
		 * Method that permits to delete all the rows from the table.
		 */
		public void deleteRows() {
			if(pool.size() != 0) {
				int last = pool.size();
				for(int iter = last - 1; iter >= 0; iter--)
					pool.remove(iter);
				fireTableRowsDeleted(0, last - 1);
			}
		}

		/**
		 * Method that permits to retrieve a specific row.
		 * @param row row identifier.
		 * @return array of Object that are contained in that row.
		 */
		public Object[] getRow(int row) {
			Object[] values = null;
			if(row < pool.size())
				values = pool.elementAt(row);
			return values;
		}

		/**
		 * Method that returns the columns count.
		 * @return columns count.
		 */
		public int getColumnCount() {
			return headers.length;
		}

		/**
		 * Method that returns the rows count.
		 * @return rows count.
		 */
		public int getRowCount() {
			return pool.size();
		}

		/**
		 * Method that returns the name of the specified column.
		 * @param col column identifier.
		 * @return column name.
		 */
		public String getColumnName(int col) {
			if(col < headers.length)
				return headers[col];
			else return new String("");
		}

		/**
		 * Method that returns the object stored to the specified
		 * position in the table.
		 * @param row row identifier.
		 * @param col column identifier.
		 * @return stored object if exists, null otherwise.
		 */
		public Object getValueAt(int row, int col) {
			if((row < pool.size()) && (col < headers.length)) {
				Object[] rowr = pool.elementAt(row);
				return rowr[col];
			} else return null;
		}

		/**
		 * Method that JTable will use to determine the default
		 * renderer/editor for each cell.
		 * @param col column identifier.
		 * @return class of the column if exists, Object class
		 * 	otherwise.
		 */
		public Class getColumnClass(int col) {
			if(col < headers.length)
				return getValueAt(0, col).getClass();
			else return (new Object()).getClass();
		}

		/**
		 * Method that permits to set as not editable a group of cells.
		 * @param row row identifier.
		 * @param col column identifier.
		 * @return false in any case.
		 */
		public boolean isCellEditable(int row, int col) {
			return false;
		}
	}

	/**
	 * Constructs a stand-alone Client.
	 */
	public Client() {
		id = new String("no name");
		addr = new String("127.0.0.1");
		port = 1099;
		cmif = null;
		jtb = new JTabbedPane(JTabbedPane.TOP);
		/* Status tab. */
		makeUpStatusTab();
		jtb.addTab("Status", jpconn);
		/* Nets tab. */
		makeUpNetsTab();
		jtb.addTab("Nets", jpnets);
		/* Logs tab. */
		makeUpLogsTab();
		jtb.addTab("Log", jplogs);
		/* Finish him! :-) */
		jtb.addChangeListener( new ChangeListener() {
			public void stateChanged(ChangeEvent ce) {
				setTitle("Client - " + jtb.getTitleAt(jtb.getSelectedIndex()));
			}
		} );
		setTitle("Client - " + jtb.getTitleAt(jtb.getSelectedIndex()));
		getContentPane().add(jtb);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		pack();
		setVisible(true);
	}

	/**
	 * Method that makes up the status tab panel.
	 */
	private void makeUpStatusTab() {
		JPanel jptmp, jpspool;
		JButton jbtmp;
		jpconn = new JPanel(new BorderLayout(4, 4));
		jpconn.add(new JLabel("CONNECTION STATUS", JLabel.CENTER), BorderLayout.NORTH);
		jptmp = new JPanel(new GridLayout(2, 2, 8, 8));
		jpspool = new JPanel(new GridLayout(4, 2, 4, 4));
		jpspool.setBorder(new TitledBorder("Connection Info"));
		jpspool.add(new JLabel("Address: "));
		staddr = new JLabel(addr);
		jpspool.add(staddr);
		jpspool.add(new JLabel("Port: "));
		stport = new JLabel(new String(""+port));
		jpspool.add(stport);
		jpspool.add(new JLabel("Name: "));
		stname = new JLabel(id);
		jpspool.add(stname);
		jpspool.add(new JLabel("Status: "));
		ststat = new JLabel("not connected");
		jpspool.add(ststat);
		jptmp.add(jpspool);
		jpspool = new JPanel(new GridLayout(3, 2, 4, 4));
		jpspool.setBorder(new TitledBorder("Connection Control"));
		jpspool.add(new JLabel("Address: "));
		staddrf = new JTextField(addr, 16);
		staddrf.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) { checkip(); }
		} );
		staddrf.addFocusListener(new FocusListener() {
			public void focusGained(FocusEvent fe) {}
			public void focusLost(FocusEvent fe) { checkip(); }
		} );
		jpspool.add(staddrf);
		jpspool.add(new JLabel("Port: "));
		stportf = new JTextField(""+port, 16);
		stportf.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) { checkport(); }
		} );
		stportf.addFocusListener(new FocusListener() {
			public void focusGained(FocusEvent fe) {}
			public void focusLost(FocusEvent fe) { checkport(); }
		} );
		jpspool.add(stportf);
		jpspool.add(new JLabel("Name: "));
		stnamef = new JTextField(id, 16);
		stnamef.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) { checkname(); }
		} );
		stnamef.addFocusListener(new FocusListener() {
			public void focusGained(FocusEvent fe) {}
			public void focusLost(FocusEvent fe) { checkname(); }
		} );
		jpspool.add(stnamef);
		jptmp.add(jpspool);
		jpspool = new JPanel(new GridLayout(2, 2, 4, 4));
		jpspool.setBorder(new TitledBorder("Cryptosystem Control"));
		jpspool.add(new JLabel("# of bits of the modulus "));
		String[] bits = { "512", "1024", "2048", "4096" };
		String[] cert = { "1000", "10000", "100000", "1000000" };
		jcbbits = new JComboBox(bits);
		jcbbits.setSelectedItem(bits[0]);
		jcbbits.setEditable(false);
		jpspool.add(jcbbits);
		jpspool.add(new JLabel("Certainty "));
		jcbcert = new JComboBox(cert);
		jcbcert.setSelectedItem(cert[0]);
		jcbcert.setEditable(false);
		jpspool.add(jcbcert);
		jptmp.add(jpspool);
		jpspool = new JPanel(new GridLayout(1, 1, 4, 4));
		jpspool.setBorder(new TitledBorder("Public Key"));
		jtapuk = new JTextArea(Cryptosystem.getInstance().getPublicKey().getModulus().toString(), 0, 0);
		jtapuk.setLineWrap(true);
		jtapuk.setEditable(false);
		jpspool.add(new JScrollPane(jtapuk));
		jptmp.add(jpspool);
		jptmp.setBorder(new BevelBorder(BevelBorder.RAISED));
		jpconn.add(jptmp, BorderLayout.CENTER);
		jpspool = new JPanel(new FlowLayout());
		jbtmp = new JButton(" Connect ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try { startConn(); }
				catch (ClientException se) {
					msgbox("Connection Exception", new JLabel("Error trying to connect to the server."));
					log(se.getMessage());
					int pos = jtb.indexOfTab("Log");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				}
			}
		} );
		jpspool.add(jbtmp);
		jbtmp = new JButton(" Disconnect ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				stopConn();
			}
		} );
		jpspool.add(jbtmp);
		jbtmp = new JButton(" Make Keys ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				Cryptosystem cs = Cryptosystem.getInstance();
				cs.generateKeys(Integer.parseInt((String) jcbbits.getSelectedItem()),
						Integer.parseInt((String) jcbcert.getSelectedItem()));
				jtapuk.setText(Cryptosystem.getInstance().getPublicKey().getModulus().toString());
			}
		} );
		jpspool.add(jbtmp);
		jpconn.add(jpspool, BorderLayout.SOUTH);
	}

	/**
	 * Method that makes up the nets tab panel.
	 */
	private void makeUpNetsTab() {
		JPanel jptmp, jpspool;
		JButton jbtmp;
		jpnets = new JPanel(new BorderLayout(4, 4));
		jpnets.add(new JLabel("AVAILABLE NETS", JLabel.CENTER), BorderLayout.NORTH);
		jptmp = new JPanel(new FlowLayout());
		netstable = new JTable(new NetsTableModel());
		netstable.setPreferredScrollableViewportSize(new Dimension(758, 160));
		jptmp.add(new JScrollPane(netstable));
		jpspool = new JPanel(new FlowLayout());
		jbtmp = new JButton(" Retrieve Nets List ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					startConn();
					((NetsTableModel) netstable.getModel()).deleteRows();
					Object[][] list =  cmif.getNetsList();
					for(int iter = 0; iter < list.length; iter++)
						((NetsTableModel) netstable.getModel()).insertRow(list[iter]);
				} catch (ClientException ce) {
					msgbox("Connection Exception", new JLabel("Error trying to connect to the server."));
					log(ce.getMessage());
					int pos = jtb.indexOfTab("Log");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				} catch (CommModule.ConnectionException se) {
					msgbox("Connection Exception", new JLabel("Error trying to retrieve the nets list."));
					log(se.getMessage());
					int pos = jtb.indexOfTab("Log");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				}
			}
		} );
		jpspool.add(jbtmp);
		jbtmp = new JButton(" Use ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					int row = netstable.getSelectedRow();
					if(row != -1) {
						Object[] values = ((NetsTableModel) netstable.getModel()).getRow(row);
						int id, in, out;
						id = ((Integer) values[0]).intValue();
						in = ((Integer) values[3]).intValue();
						out = ((Integer) values[4]).intValue();
						String[] inputs = getInputs(in);
						String[] outputs = cmif.getOutputs(id, inputs);
						splashOutputs(outputs);
						log("total time: " + cmif.getTotalGap() + " ms (" + (cmif.getTotalGap() / 1000) + " s)");
						log("local time: " + cmif.getLocalGap() + " ms (" + (cmif.getLocalGap() / 1000) + " s)");
						log("worker time: " + cmif.getWorkerGap() + " ms (" + (cmif.getWorkerGap() / 1000) + " s)");
					} else { msgbox("Nets Exception", new JLabel("First, please select a net to be used.")); }
				} catch (IOException ioe) {
					msgbox("Input/Output Exception", new JLabel("Error trying to obtain input values"));
				} catch (Exception excp) {
					msgbox("Protocol Exception", new JLabel("Error trying to compute the result."));
					log(excp.getMessage());
					int pos = jtb.indexOfTab("Log");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				}
			}
		} );
		jpspool.add(jbtmp);
		jbtmp = new JButton(" Use (input from file) ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					int row = netstable.getSelectedRow();
					if(row != -1) {
						JFileChooser jfc = new JFileChooser();
						jfc.setMultiSelectionEnabled(false);
						jfc.showOpenDialog(netstable);
						File file = jfc.getSelectedFile();
						if(file != null) {
							Object[] values = ((NetsTableModel) netstable.getModel()).getRow(row);
							int id, in, out;
							id = ((Integer) values[0]).intValue();
							in = ((Integer) values[3]).intValue();
							out = ((Integer) values[4]).intValue();
							String[] inputs = getInputsFromFile(in, file);
							String[] outputs = cmif.getOutputs(id, inputs);
							splashOutputs(outputs);
							log("total time: " + cmif.getTotalGap() + " ms (" + (cmif.getTotalGap() / 1000) + " s)");
							log("local time: " + cmif.getLocalGap() + " ms (" + (cmif.getLocalGap() / 1000) + " s)");
							log("worker time: " + cmif.getWorkerGap() + " ms (" + (cmif.getWorkerGap() / 1000) + " s)");
						}
					} else { msgbox("Nets Exception", new JLabel("First, please select a net to be used.")); }
				} catch (AccessControlException ace) {
					msgbox("Access Control Exception", new JLabel("Error trying to access to the filesystem"));
				} catch (IOException ioe) {
					msgbox("Input/Output Exception", new JLabel("Error trying to read input values from file"));
				} catch (Exception excp) {
					msgbox("Protocol Exception", new JLabel("Error trying to compute the result."));
					log(excp.getMessage());
					int pos = jtb.indexOfTab("Log");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				}
			}
		} );
		jpspool.add(jbtmp);
		jpnets.add(jptmp, BorderLayout.CENTER);
		jpnets.add(jpspool, BorderLayout.SOUTH);
	}

	/**
	 * Method that makes up the logs tab panel.
	 */
	private void makeUpLogsTab() {
		JPanel jptmp;
		JButton jbtmp;
		jplogs = new JPanel(new BorderLayout(4, 4));
		jplogs.add(new JLabel("LOG", JLabel.CENTER), BorderLayout.NORTH);
		ltlogs = new JTextArea();
		ltlogs.setLineWrap(false);
		ltlogs.setEditable(false);
		jplogs.add(new JScrollPane(ltlogs), BorderLayout.CENTER);
		jbtmp = new JButton(" Clean ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				ltlogs.setText("");
			}
		} );
		jptmp = new JPanel(new FlowLayout());
		jptmp.add(jbtmp);
		jplogs.add(jptmp, BorderLayout.SOUTH);
	}

	/**
	 * Method that permits to check ip format.
	 */
	public void checkip() {
		stopConn();
		try {
			String[] oct = staddrf.getText().split("\\.",4);
			if(oct.length < 4) {
				msgbox("IP Format Exception", new JLabel("IP Format: xxx.xxx.xxx.xxx" +
					" ( xxx is a number between 0 and 255 )"));
				staddrf.setText(addr);
			} else {
				int oct0 = Integer.parseInt(oct[0]);
				int oct1 = Integer.parseInt(oct[1]);
				int oct2 = Integer.parseInt(oct[2]);
				int oct3 = Integer.parseInt(oct[3]);
				if((oct0 >= 0) && (oct0 <= 255) && (oct1 >= 0) && (oct1 <= 255)
					& (oct2 >= 0) && (oct2 <= 255) && (oct3 >= 0) && (oct2 <= 255)) {
						addr = staddrf.getText();
						staddr.setText(addr);
				} else {
					msgbox("IP Format Exception", new JLabel("IP Format: xxx.xxx.xxx.xxx" +
						" ( xxx is a number between 0 and 255 )"));
					staddrf.setText(addr);
				}
			}
		} catch (NumberFormatException nfe) {
			msgbox("IP Format Exception", new JLabel("IP Format: xxx.xxx.xxx.xxx" +
				" ( xxx is a number between 0 and 255 )"));
			staddrf.setText(addr);
		}
	}

	/**
	 * Method that permits to check port format.
	 */
	public void checkport() {
		stopConn();
		if(!stportf.getText().equals("")) {
			try { port = Integer.parseInt(stportf.getText()); }
			catch (NumberFormatException nfe) {
				msgbox("Port Format Exception", new JLabel("Port has to be a number."));
				stportf.setText(""+port);
			}
			stport.setText(""+port);
		} else {
			msgbox("Port Format Exception", new JLabel("Port has to be a number."));
			stportf.setText(""+port);
		}
	}

	/**
	 * Method that permits to check name format.
	 */
	public void checkname() {
		stopConn();
		if(!stnamef.getText().equals("")) {
			id = stnamef.getText();
			stname.setText(id);
		} else {
			msgbox("Name Format Exception", new JLabel("Empty name is not allowed."));
			stnamef.setText(id);
		}
	}

	/**
	 * Method that permits to ask to the user for the inputs.
	 * @param num number of inputs required.
	 * @return inputs in form of array.
	 * @throws IOException thrown when some errors occur trying to read
	 * 	input values from file.
	 */
	private String[] getInputs(int num) throws IOException {
		String[] inputs = new String[num];
		for(int iter = 0; iter < num; iter++) {
			String stmp = null;
			boolean valid = false;
			while(!valid) {
				stmp = JOptionPane.showInputDialog("Please input a value.");
				if(stmp == null)
					throw new IOException("wrong input value format");
				valid = true;
				try { Double.parseDouble(stmp); }
				catch (NumberFormatException nfe) {
					valid = false;
					msgbox("Number Format Exception", new JLabel("Input has to be a float value."));
				}
			}
			inputs[iter] = stmp;
		}
		return inputs;
	}

	/**
	 * Method that permits to retrieve input values from file.
	 * @param num number of inputs required.
	 * @param file file reference.
	 * @return inputs in form of array.
	 * @throws IOException thrown when some errors occur trying to read
	 * 	input values from file.
	 */
	private String[] getInputsFromFile(int num, File file) throws IOException {
		String[] inputs = new String[num];
		BufferedReader br = new BufferedReader(new FileReader(file));
		for(int iter = 0; iter < num; iter++) {
			String stmp = null;
			boolean valid = false;
			while(!valid) {
				stmp = br.readLine();
				if(stmp == null)
					throw new IOException("wrong input file format");
				valid = true;
				try { Double.parseDouble(stmp); }
				catch (NumberFormatException nfe) {
					valid = false;
					log("Number Format Exception: input has to be a float value.");
				}
			}
			inputs[iter] = stmp;
		}
		return inputs;
	}

	/**
	 * This method permits to "splash" an array of strings, that means it
	 * help to show to the user the provided strings.
	 * @param outputs array of string to be shown.
	 */
	private void splashOutputs(String[] outputs) {
		JPanel jptmp, jpspool;
		jptmp = new JPanel(new BorderLayout(0, 8));
		jpspool = new JPanel(new GridLayout(outputs.length, 1, 4, 4));
		jptmp.add(new JLabel("Results:"), BorderLayout.NORTH);
		jptmp.add(jpspool, BorderLayout.CENTER);
		for(int iter = 0; iter < outputs.length; iter++)
			jpspool.add(new JLabel("Output #" + (iter+1) +": " + outputs[iter]));
		JOptionPane.showMessageDialog(this, jptmp, "Results", JOptionPane.INFORMATION_MESSAGE);
	}

        /**
	 * Method that creates an error message box to warn users
	 * about something.
	 * @param title the title of the message box.
	 * @param msg message to be delivered.
	 */
	public void msgbox(String title, Object msg) {
		JOptionPane.showMessageDialog(this, msg, title, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Method that writes a log message into the logs area (simply, a JTextArea).
	 * @param msg message to be logged.
	 */
	public void log(String msg) {
		if(ltlogs != null)
			ltlogs.append(msg + "\n");
	}

	/**
	 * This method starts the connection with the server.
	 * @throws ClientException thrown when some errors occur trying to start
	 * 	the connection.
	 */
	public void startConn() throws ClientException {
		try {
			if(cmif == null)
				cmif = new CommModule();
			cmif.createConnection(addr, port, id);
			ststat.setText("connected");
		} catch (Exception excp) {
			stopConn();
			throw new ClientException(excp.getMessage());
		}
	}

	/**
	 * This method stops the connection.
	 */
	public void stopConn() {
		ststat.setText("not connected");
		if(cmif != null)
			cmif.destroyConnection();
	}

	/**
	 * Main function (for test only).
	 * @param args command line arguments.
	 */
	public static void main (String[] args) {
		Client ps = new Client();
	}
}
