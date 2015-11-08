/**
 * PocketServer class represents the server GUI. it's funny name is dued to it's
 * limited dimension. Is it, PocketServer GUI is really a pocket GUI! :-)
 *
 * @author Michele Caini
 */

package nnsec.server;

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
import java.security.AccessControlException;
import java.io.File;
import java.util.Vector;
import nnsec.net.NetManager;

/**
 * Server side GUI.
 */
public class PocketServer extends JFrame {

	/** Java Swing related useful components. */
	private JTabbedPane jtb;
	private JPanel jpstat;
	private JPanel jpnets;
	private JPanel jplogs;

	/** Status tab components. */
	private JLabel stport;
	private JTextField stportf;
	private JLabel stname;
	private JTextField stnamef;
	private JLabel ststat;

	/** Nets tab components. */
	private JTable netstable;
	private JTextField ntsfile;

	/** Logs tab component. */
	private JTextArea ltlogs;

	/** Server informations. */
	private String id;
	private int port;

	/** WorkerFactory reference. */
	private WorkerFactory wf;

	/** NetManager reference. */
	private NetManager nm;

	/**
	 * Exception thrown when trying to start the server, and then some
	 * errors occur.
	 */
	static public class ServerException extends Exception {
		
		/**
		 * Constructs a ServerException.
		 */
		public ServerException() { super(); }

		/**
		 * Constructs a ServerException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public ServerException(String msg) { super(msg); }
	}

	/**
	 * Table model useful for the nets table.
	 */
	private class NetsTableModel extends AbstractTableModel {
		
		/** The columns name. */
		private String[] headers = { "ID", "Name", "Description", "Inputs", "Outputs",
			"Hiddens", "Filename", "Wrapper",
			"Scale", "Available" };

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
		 * Method that permits to delete a row from the table.
		 * @param row row identifier.
		 * @return row values.
		 */
		public Object[] deleteRow(int row) {
			Object[] values = null;
			if(row < pool.size()) {
				values = pool.remove(row);
				fireTableRowsDeleted(row, row);
			}
			return values;
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
		 * @return true if the specified cell is editable, false
		 * 	otherwise.
		 */
		public boolean isCellEditable(int row, int col) {
			if(col > 6) return true;
			else return false;
		}

		/**
		 * Method that permits to set a new value for editable cells.
		 * @param value the new value.
		 * @param row row identifier.
		 * @param col column identifier.
		 */
		public void setValueAt(Object value, int row, int col) {
			if((row < pool.size()) && (col < headers.length)) {
				Object[] rowr = pool.elementAt(row);
				rowr[col] = value;
				fireTableCellUpdated(row, col);
			}
		}
	}

	/**
	 * Constructs a stand-alone PocketServer.
	 */
	public PocketServer() {
		nm = new NetManager();
		id = new String("no name");
		port = 1099;
		wf = null;
		jtb = new JTabbedPane(JTabbedPane.TOP);
		/* Status tab. */
		makeUpStatusTab();
		jtb.addTab("Status", jpstat);
		/* Nets tab. */
		makeUpNetsTab();
		jtb.addTab("Nets", jpnets);
		/* Logs tab.*/
		makeUpLogsTab();
		jtb.addTab("Logs", jplogs);
		/* Finish him! :-) */
		jtb.addChangeListener( new ChangeListener() {
			public void stateChanged(ChangeEvent ce) {
				setTitle("Pocket Server - " + jtb.getTitleAt(jtb.getSelectedIndex()));
			}
		} );
		setTitle("Pocket Server - " + jtb.getTitleAt(jtb.getSelectedIndex()));
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
		jpstat = new JPanel(new BorderLayout(4, 4));
		jpstat.add(new JLabel("SERVER STATUS", JLabel.CENTER), BorderLayout.NORTH);
		jptmp = new JPanel(new GridLayout(1, 2, 8, 8));
		jpspool = new JPanel(new GridLayout(4, 2, 4, 4));
		jpspool.setBorder(new TitledBorder("Server Info"));
		jpspool.add(new JLabel("Address: "));
		jpspool.add(new JLabel("127.0.0.1"));
		jpspool.add(new JLabel("Port: "));
		stport = new JLabel(new String(""+port));
		jpspool.add(stport);
		jpspool.add(new JLabel("Name: "));
		stname = new JLabel(id);
		jpspool.add(stname);
		jpspool.add(new JLabel("Status: "));
		ststat = new JLabel("off-line");
		jpspool.add(ststat);
		jptmp.add(jpspool);
		jpspool = new JPanel(new GridLayout(3, 2, 4, 4));
		jpspool.setBorder(new TitledBorder("Server Control"));
		jpspool.add(new JLabel("Address: "));
		JTextField jtftmp = new JTextField("127.0.0.1", 16);
		jtftmp.setEditable(false);
		jpspool.add(jtftmp);
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
		jptmp.setBorder(new BevelBorder(BevelBorder.RAISED));
		jpstat.add(jptmp, BorderLayout.CENTER);
		jptmp = new JPanel(new FlowLayout());
		jbtmp = new JButton(" Start ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					startServer();
					stportf.setEditable(false);
					stnamef.setEditable(false);
				}
				catch (ServerException se) {
					msgbox("Server Exception", new JLabel("Error trying to start the server."));
					log(se.getMessage());
					int pos = jtb.indexOfTab("Logs");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				}
			}
		} );
		jptmp.add(jbtmp);
		jbtmp = new JButton(" Stop ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				stopServer();
			}
		} );
		jptmp.add(jbtmp);
		jpstat.add(jptmp, BorderLayout.SOUTH);
	}

	/**
	 * Method that makes up the nets tab panel.
	 */
	private void makeUpNetsTab() {
		JPanel jptmp, jpspool;
		JButton jbtmp;
		jpnets = new JPanel(new BorderLayout(4, 4));
		jpnets.add(new JLabel("NETS POOL", JLabel.CENTER), BorderLayout.NORTH);
		jptmp = new JPanel(new FlowLayout());
		jptmp.add(new JLabel("File: "));
		ntsfile = new JTextField("", 32);
		jptmp.add(ntsfile);
		jbtmp = new JButton(" Find ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					JFileChooser jfc = new JFileChooser();
					jfc.setMultiSelectionEnabled(false);
					jfc.showOpenDialog(ntsfile);
					File file = jfc.getSelectedFile();
					if(file != null)
						ntsfile.setText(file.toString());
				} catch (AccessControlException ace) {
					msgbox("Access Control Exception", new JLabel("Error trying to access to the filesystem."));
				}
			}
		} );
		jptmp.add(jbtmp);
		jbtmp = new JButton(" Load ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				try {
					File file = new File(ntsfile.getText());
					int netid = -1;
					if((file != null) && (file.exists()) && (file.isFile())) {
						netid = nm.loadNet(file.toString());
						Object[] info = nm.getNetInformations(netid);
						NetsTableModel ntm = (NetsTableModel) netstable.getModel();
						ntm.insertRow(info);
					} else msgbox("File Format Exception", new JLabel("File " + file.toString() +
						"is not a valid file."));
				} catch (NetManager.NetLoadingException nle) {
					msgbox("Net Loading Exception", new JLabel("Error trying to load the net from file " +
							 ntsfile.getText() + "."));
					log(nle.getMessage());
					int pos = jtb.indexOfTab("Logs");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				} catch (Error err) {
					msgbox("File Parsing Exception", new JLabel("Error trying to parse the file " +
							 ntsfile.getText() + "."));
					log(err.getMessage());
					int pos = jtb.indexOfTab("Logs");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				} catch (Exception excp) {
					msgbox("Filesystem Access Exception", new JLabel("Error trying to access to the filesystem."));
					log(excp.getMessage());
					int pos = jtb.indexOfTab("Logs");
					if(pos != -1)
						jtb.setSelectedIndex(pos);
				} finally { ntsfile.setText(""); }
			}
		} );
		jptmp.add(jbtmp);
		jbtmp = new JButton(" Delete ");
		jbtmp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				NetsTableModel ntm = (NetsTableModel) netstable.getModel();
				int row = netstable.getSelectedRow();
				if(row != -1) {
					Object[] values = ntm.deleteRow(row);
					if(values[0] instanceof Integer)
						nm.unloadNet(((Integer) values[0]).intValue());
				}
			}
		} );
		jptmp.add(jbtmp);
		jpspool = jptmp;
		jptmp = new JPanel(new FlowLayout());
		netstable = new JTable(new NetsTableModel());
		netstable.getModel().addTableModelListener(new TableModelListener() {
			public void tableChanged(TableModelEvent tme) {
				boolean res;
				int first = tme.getFirstRow();
				int last = tme.getLastRow();
				for(int iter = first; iter <= last; iter++)
					nm.updateNet(((NetsTableModel) netstable.getModel()).getRow(iter));
			}
		} );
		netstable.setPreferredScrollableViewportSize(new Dimension(758, 80));
		jptmp.add(new JScrollPane(netstable), BorderLayout.CENTER);
		jpnets.add(jptmp, BorderLayout.CENTER);
		jpnets.add(jpspool, BorderLayout.SOUTH);
	}

	/**
	 * Method that makes up the logs tab panel.
	 */
	private void makeUpLogsTab() {
		JPanel jpspool;
		JButton jbtmp;
		jplogs = new JPanel(new BorderLayout(4, 4));
		jplogs.add(new JLabel("LOGS", JLabel.CENTER), BorderLayout.NORTH);
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
		jpspool = new JPanel(new FlowLayout());
		jpspool.add(jbtmp);
		jplogs.add(jpspool, BorderLayout.SOUTH);

	}

	/**
	 * Method that permits to check port format.
	 */
	private void checkport() {
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
	private void checkname() {
		if(!stnamef.getText().equals("")) {
			id = stnamef.getText();
			stname.setText(id);
		} else {
			msgbox("Name Format Exception", new JLabel("Empty name is not allowed."));
			stnamef.setText(id);
		}
	}

	/**
	 * Method that creates an error message box to warn users about something.
	 * @param title the title of the message box.
	 * @param msg message to be delivered.
	 */
	private void msgbox(String title, Object msg) {
		JOptionPane.showMessageDialog(this, msg, title, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Method that writes a log message into the logs area (simply, a JTextArea).
	 * @param msg message to be logged.
	 */
	private void log(String msg) {
		if(ltlogs != null)
			ltlogs.append(msg + "\n");
	}

	/**
	 * This method starts the server.
	 * @throws ServerException thrown when some errors occur trying to start
	 * 	the server.
	 */
	private void startServer() throws ServerException {
		try {
			if(wf == null) wf = new WorkerFactory(nm);
			if(!wf.isConnectionAvailable())
				wf.createConnection(port, id);
			else wf.setActive(true);
			ststat.setText("on-line");
		} catch (Exception excp) {
			stopServer();
			throw new ServerException(excp.getMessage());
		}

	}

	/**
	 * This method stops the server.
	 */
	private void stopServer() {
		if(wf != null)
			wf.setActive(false);
		ststat.setText("off-line");
	}

	/**
	 * Main function (for test only).
	 * @param args command line arguments.
	 */
	public static void main (String[] args) {
		PocketServer ps = new PocketServer();
	}
}
