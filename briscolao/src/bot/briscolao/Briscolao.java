package bot.briscolao;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.io.UnsupportedEncodingException;
import java.io.IOException;
import java.text.MessageFormat;
import java.util.Enumeration;
import java.util.MissingResourceException;
import java.util.ResourceBundle;
import java.util.TimerTask;
import java.util.Timer;

import org.jibble.pircbot.Colors;
import org.jibble.pircbot.IrcException;
import org.jibble.pircbot.NickAlreadyInUseException;
import org.jibble.pircbot.PircBot;
import org.jibble.pircbot.User;


/**
 * TODO frasi random ad ogni giocata da peschare da un database o file .properties
 * TODO gioco a 4
 * TODO mostrare il punteggio all'ultima mano
 * TODO controllo su fine partita per non far finire la chat!
 * @author skypjack, Droscy
 */
public class Briscolao extends PircBot
{
	private static final String PROGRAM_NAME = "briscolao";
	private static final String VERSION = "v1.5.3 rc2";
	private static final String YEAR = "2008";
	private static final String BOT_COMMAND = "!briscola";
	
	private static String server = null;
	private static String[] channels = null;
	private static int port = 6667;
	private static String password = null;
	
	private Game games;
	private Timer watchdog;
	
	private static final String BUNDLE_NAME = "bot.briscolao.frasi";
	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle(BUNDLE_NAME);
	private static final int FRASI;
	static
	{
		Enumeration<String> enumeration = RESOURCE_BUNDLE.getKeys();
		int frasi = 0;
		while(enumeration.hasMoreElements())
		{
			enumeration.nextElement();
			frasi++;
		}
		FRASI = frasi;
	}

	/**
	 * Gets a random message form <code>frasi.properties</code> file
	 * @param player the player that played the card
	 * @param card the played card
	 * @return the correctly formatted message
	 */
	private static String getRandomMessage(String player, String card)
	{
		int random = (int)Math.floor(Math.random()*FRASI);
		String message;
		
		try
		{
			message = RESOURCE_BUNDLE.getString("PLAY" + String.valueOf(random));
		}
		catch(MissingResourceException e)
		{
			message= "{0} ha giocato la carta {1}";
		}
		
		return MessageFormat.format(message, player, Colors.BOLD + card + Colors.NORMAL);
	}

	private class Game
	{
		private static final String MSG_START_MATCH = Colors.RED + "------------[INIZIA LA PARTITA]------------" + Colors.NORMAL;
		private static final String MSG_END_GAME = Colors.RED + "------------[FINE PARTITA]------------" + Colors.NORMAL;
		private static final String MSG_NEW_TURN = Colors.RED + "------------[NUOVA MANO]------------" + Colors.NORMAL;
		private static final String MSG_HELP = Colors.LIGHT_GRAY + "digita il numero corrispondente alla carta scelta e premi invio per giocarla" + Colors.BLACK;
		
		public static final int NULL_INT = -1;
		
		private int ticks;

		private Game prev, next;
		private String[] players;
		
		/**
		 * serve per sapere quando ambe due i giocatori hanno giocato la carta.<br>
		 * TODO da sistemare per il gioco in 4
		 */
		private boolean status;
		private int turn;

		private int[] cards;
		private int pos;

		private int[][] playersHand;
		private int[] playedCard;
		private int[] points;
		private int hc;
		private int hcm;

		public Game(String[] players)
		{
			this.players = players;
			prev = null;
			next = null;
			turn = ((int)Math.floor(Math.random()*2));
			status = false;
			cards = new int[40];
			for(int iter = 0; iter < 40; iter++)
				cards[iter] = iter;
			pos = 40;
			playersHand = new int[this.players.length][3];
			playedCard = new int[this.players.length];
			points = new int[this.players.length];
			for(int playerId=0; playerId<this.players.length; playerId++)
			{
				for(int card = 0; card < 3; card++)
					playersHand[playerId][card] = NULL_INT;

				playedCard[playerId] = NULL_INT;
				points[playerId] = 0;
			}
			
			hc = -1;
			hcm = hc;
			ticks = 99;
		}

		/**
		 * Decrement the <code>ticks</code> attribute.<br>
		 * Is used by the watchdog to control the timeout
		 * @return the value of <code>ticks</code> decremented by 1
		 */
		public int decTicks()
		{
			return --ticks;
		}

		public synchronized void setPrev(Game prev)
		{
			this.prev = prev;
		}

		public synchronized Game getPrev()
		{
			return prev;
		}

		public synchronized void setNext(Game next)
		{
			this.next = next;
		}

		public synchronized Game getNext()
		{
			return next;
		}

		public synchronized boolean checkPlayer(String player)
		{
			boolean check = false;
			int i = 0;
			while(check==false)
			{
				check = players[i].equals(player);
				i++;
			}
			
			if(check)
				return true;
			else if(next != null)
				return next.checkPlayer(player);
			else
				return false;
		}

		
		/**
		 * Starts a new game.<br>
		 * Shuffles the cards, initializes the players and sends the first messages
		 * @param channel the channel in wich the game starts
		 */
		public synchronized void startGame(String channel)
		{
			if(players.length == 2)
				System.out.println("Nuova partita: " + players[0] + " VS " + players[1] + " (" + channel + ")");
			else
				System.out.println("Nuova partita: (" + players[0] + "," + players[1] + ") VS (" + players[2] + "," + players[3] + ")");
			
			int mrk, bs, swp;
			
			/* mescola le carte */
			for(int iter = 0; iter < 100; iter++)
			{
				bs = (int) (40 * Math.random());
				mrk = (int) (40 * Math.random());
				swp = cards[mrk];
				cards[mrk] = cards[bs];
				cards[bs] = swp;
			}
			
			
			/* distribuisce le carte */
			for(int i=0; i<players.length; i++)
			{
				for(int card = 0; card < 3; card++)
				{
					playersHand[i][card] = cards[--pos];
				}
			}
			
			hcm = hc = cards[--pos];
			
			sendMessage(players[0], "partita a briscola con " + Colors.BOLD + players[1] + Colors.NORMAL);
			sendMessage(players[0], MSG_START_MATCH);
			
			sendFastMessageToOthers(0, new String[]{"sei stato invitato ad una partita a briscola da " + Colors.BOLD + players[0] + Colors.NORMAL,
					"se non vuoi giocare, non rispondere a questo messaggio e la partita sar\u00E0  interrotta automaticamente",
					MSG_START_MATCH});
			
			splashTurn();
		}

		
		/**
		 * Close a game (both for timeout or really and)
		 */
		public synchronized void endGame()
		{
			if(ticks<=0)
				sendMessageToAllPlayers(new String[]{Colors.RED + Colors.BOLD + "watchdog timeout!" + Colors.NORMAL + Colors.BLACK, MSG_END_GAME});
			else
				sendMessageToAllPlayers(MSG_END_GAME);
			
			String[] results = new String[players.length];
			for(int i=0; i<players.length; i++)
				results[i] = Colors.RED + players[i] + ": " + points[i] + " punti" + Colors.BLACK;
			sendMessageToAllPlayers(results);
				
			
			if(players.length==2)
			{
				System.out.print("La partita tra " + players[0] + " e " + players[1] + " \u00E8 terminata");
				if(ticks > 0)
				{
					if(points[0] != points[1])
						System.out.println(", ha vinto " + (points[0]>points[1]?players[0]:players[1]));
					else
						System.out.println(" in parit\u00E0");
				}
				else
					System.out.println(" per timeout!");
			}
			else
			{
				System.out.print("La partita tra " + players[0] + ", " + players[1] + ", " + players[2] + " e " + players[3] + " \u00E8 terminata");
				// TODO stampa risultati per 4 giocatori!
			}
			
			
			if(prev != null)
				prev.setNext(next);
			else
				games = null;
			if(next != null) next.setPrev(prev);
		}
		
		
		//TODO sistemarlo per 4 giocatori
		private synchronized void splashTurn()
		{
			if(!status)
			{
				/*for(int t=turn; t<(turn+players.length); t++)
				{
					int p = t%players.length;
					sendMessage(players[p], "briscola: " + Colors.BOLD + Briscolao.cardToString(hcm) + Colors.NORMAL + ", carte nel mazzo: " + (pos>0?(pos+1):0));
					//sendMessage(players[p], "carte nel mazzo: " + (pos>0?(pos+1):0));
				}*/
				
				sendMessageToOthers(turn, "attendi, sto distribuendo le carte...");
				sendMessage(players[turn], "briscola: " + Colors.BOLD + Briscolao.cardToString(hcm) + Colors.NORMAL + ", carte nel mazzo: " + (pos>0?(pos+1):0));
				sendMessage(players[turn], "carte in mano:");
				
				String hand = "";
				for(int iter = 0; iter < 3; iter++)
					if(playersHand[turn][iter] != NULL_INT)
						hand += Colors.BOLD + "[" + (iter+1) + "]" + Colors.NORMAL + " " + Briscolao.cardToString(playersHand[turn][iter]) + "  ";
				
				sendMessage(players[turn], hand.substring(0, (hand.length()-2)) + Colors.NORMAL);
				sendMessage(players[turn], "\u00E8 il tuo turno, " + MSG_HELP);
				
				
				sendMessageToOthers(turn,
						"briscola: " + Colors.BOLD + Briscolao.cardToString(hcm) + Colors.NORMAL
						+ ", carte nel mazzo: " + (pos>0?(pos+1):0));

				
				for(int t=(turn+1); t<(turn+players.length); t++)
				{
					int p = t%players.length;
					sendMessage(players[p], "carte in mano:");
					
					hand = "";
					for(int iter = 0; iter < 3; iter++)
						if(playersHand[p][iter] != NULL_INT)
							hand += Colors.BOLD + "[" + (iter+1) + "]" + Colors.NORMAL + " " + Briscolao.cardToString(playersHand[p][iter]) + "  ";
					
					sendMessage(players[p], hand.substring(0, (hand.length()-2)) + Colors.NORMAL);
				}
				
				sendMessageToOthers(turn, "\u00E8 il turno di " + players[turn] + ", attendi...");
				
				/*for(int t=(turn+1); t<(turn+1+players.length); t++)
				{
					int p = t%players.length;
					if(p != turn)
						sendMessage(players[p], "\u00E8 il turno di " + players[turn] + ", attendi...");
					else
					{
						sendMessage(players[p], "\u00E8 il tuo turno, " + MSG_HELP);
						//sendMessage(players[p], MSG_HELP);
					}
				}*/
				
				
				
				/*for(int t=turn; t<(turn+players.length); t++)
				{
					int p = t%players.length;
					sendMessage(players[p], "briscola: " + Colors.BOLD + Briscolao.cardToString(hcm));
					sendMessage(players[p], "carte nel mazzo: " + (pos>0?(pos+1):0));
					sendMessage(players[p], "carte in mano:");
					
					String hand = "";
					for(int iter = 0; iter < 3; iter++)
						if(playersHand[p][iter] != NULL_INT)
							hand += Colors.BOLD + "[" + iter + "]" + Colors.NORMAL + " " + Briscolao.cardToString(playersHand[p][iter]) + "  ";
					
					sendMessage(players[p], hand.substring(0, (hand.length()-2)) + Colors.NORMAL);
					
					if(p == turn)
					{
						sendMessage(players[p], "\u00E8 il tuo turno");
						sendMessage(players[p], MSG_HELP);
					}
					else
						sendMessage(players[p], "\u00E8 il turno di " + players[turn] + ", attendi...");
				}*/
			}
			else
			{
				sendMessageToOthers(turn, "\u00E8 il turno di " + players[turn] + ", attendi...");
				sendMessage(players[turn], "\u00E8 il tuo turno, " + MSG_HELP);
				//sendFastMessage(players[turn], MSG_HELP);
			}
			
			ticks = 10;
		}
		
		
		private int getPlayerId(String player)
		{
			int playerId = NULL_INT;
			int i=0;
			while(playerId == NULL_INT)
			{
				if(player.equals(players[i]))
					playerId = i;
				i++;
			}
			
			return playerId;
		}
		

		protected synchronized void checkStep(String player, String message)
		{
			int playerId = getPlayerId(player);
			
			if(playerId != NULL_INT)
			{
				try
				{
					int card = Integer.parseInt(message) - 1; // c'è il -1 perché nel mostrarlo ho messo +1
					
					if(turn == playerId)
					{
						if((card < 0) || (card > 3) || (playersHand[turn][card] == NULL_INT))
							sendFastMessage(players[turn], "carta non valida");
						else
						{
							playedCard[turn] = playersHand[turn][card];
							sendFastMessage(players[turn], "hai giocato la carta " + Colors.BOLD + Briscolao.cardToString(playedCard[turn]) + Colors.NORMAL);
							//sendMessageToOthers(turn, players[turn] + " ha giocato la carta " + Colors.BOLD + Briscolao.cardToString(playedCard[turn]) + Colors.NORMAL);
							
							sendMessageToOthers(turn, getRandomMessage(players[turn], Briscolao.cardToString(playedCard[turn])));
							
							playersHand[turn][card] = NULL_INT;
							checkStatus();
						}
					}
					else
					{
						sendFastMessage(players[playerId], "aspetta, non \u00E8 ancora il tuo turno");
					}

				}
				catch(NumberFormatException e)
				{
					sendFastMessageToOthers(playerId, Colors.BOLD + Colors.DARK_GREEN + player + ": " + Colors.NORMAL + Colors.DARK_GREEN + message); // TODO colore messaggio di chat
				}
			}
			else if(next != null)
				next.checkStep(player, message);
			else
			{
				String[] channels = getChannels();
				if(channels.length == 1)
					sendMessage(player, "nessuna partita in corso, prima devi iniziare un match dal canale " + channels[0]); // TODO stampa caratteri strani!
				else
				{
					sendMessage(player, "nessuna partita in corso, prima devi iniziare un match da uno dei seguenti canali:");
					for(String channel : channels)
						sendMessage(player, channel);
				}
				//botUsage(player);
			}
		}
		
		
		private void sendMessageToAllPlayers(String message)
		{
			sendMessageToAllPlayers(new String[]{message},0);
		}
		
		private void sendMessageToAllPlayers(String[] messages)
		{
			sendMessageToAllPlayers(messages, 0);
		}
		
		/*private void sendMessageToAllPlayers(String message, int first)
		{
			sendMessageToAllPlayers(new String[]{message},first);
		}*/
		
		private void sendMessageToAllPlayers(String[] messages, int first)
		{
			for(String msg : messages)
			{
				for(int i=first; i<(first + players.length); i++)
				{
					int p = i%players.length;
					sendMessage(players[p], msg);
				}
			}
		}
		
		
		
		/*private void sendFastMessageToAllPlayers(String message)
		{
			sendFastMessageToAllPlayers(new String[]{message});
		}
		
		private void sendFastMessageToAllPlayers(String[] messages)
		{
			for(String msg : messages)
			{
				for(int i=0; i<players.length; i++)
				{
					sendFastMessage(players[i], msg);
				}
			}
		}*/
		
		
		
		private void sendFastMessageToOthers(int sender, String message)
		{
			sendFastMessageToOthers(sender, new String[]{message});
		}
		
		private void sendFastMessageToOthers(int sender, String[] messages)
		{
			for(String msg : messages)
			{
				for(int i=(sender+1); i<(sender + players.length); i++)
				{
					int p = i%players.length;
					//if(i != sender)
						sendFastMessage(players[p], msg);
				}
			}
		}
		
		/**
		 * Sends a message to other players except the <code>sender</code>
		 * @param sender
		 * @param message
		 */
		private void sendMessageToOthers(int sender, String message)
		{
			sendMessageToOthers(sender, new String[]{message});
		}
		
		/**
		 * Sends messages to other players except the <code>sender</code>
		 * @param sender
		 * @param messages
		 */
		private void sendMessageToOthers(int sender, String[] messages)
		{
			for(String msg : messages)
			{
				for(int i=0; i<players.length; i++)
				{
					if(i != sender)
						sendMessage(players[i], msg);
				}
			}
		}

		
		private synchronized void checkStatus()
		{
			if(players.length==2)
			{
				if(status)
				{
					status = !status;
					int point = Briscolao.cardToPoints(playedCard[0]) + Briscolao.cardToPoints(playedCard[1]);
					
					/*
					 * poiché è i turno del secondo giocatore, la prima carta giocata è stata quella del primo!
					 */
					if(cardVsCard(playedCard[(turn+1)%players.length], playedCard[turn], Briscolao.cardType(hcm)) == playedCard[(turn+1)%players.length])
						turn = (turn+1)%players.length;
					
					points[turn] += point;
					
					sendMessage(players[turn], "hai vinto la mano, punti conquistati: " + point);
					sendMessage(players[(turn+1)%players.length], "mano vinta da " + players[turn]);
					
					
					/* pesca -1 quando il mazzo è finito e in mano solo solo <3 carte */
					if(hc < 0) hc--;
					
					if(hc < -3)
						endGame();
					else
					{
						if(hc >= 0)
						{
							int card;
							if(pos > 1)
							{
								for(card = 0; card < 3 && playersHand[turn][card] != NULL_INT; card++);
								playersHand[turn][card] = cards[--pos];
								
								for(card = 0; card < 3 && playersHand[(turn+1)%players.length][card] != NULL_INT; card++);
								playersHand[(turn+1)%players.length][card] = cards[--pos];
							}
							else if(pos == 1)
							{
								for(card = 0; card < 3 && playersHand[turn][card] != NULL_INT; card++);
								playersHand[turn][card] = cards[--pos];
								
								for(card = 0; card < 3 && playersHand[(turn+1)%players.length][card] != NULL_INT; card++);
								playersHand[(turn+1)%players.length][card] = hc;
								
								hc = -1;
							}

						}
						
						//sendMessage(players[turn], MSG_NEW_TURN);
						//sendMessage(players[(turn+1)%players.length], MSG_NEW_TURN);
						sendMessageToAllPlayers(MSG_NEW_TURN);
						splashTurn();
					}
					
				}
				else
				{
					turn = (turn+1)%players.length;
					status = !status;
					splashTurn();
				}
					
			}
			else
			{
				// TODO controllare per il gioco in 4!
			}

		}
	}

	
	/**
	 * Initializes this new bot
	 */
	public Briscolao()
	{
		setName(PROGRAM_NAME);
		games = null;
		
		watchdog = new Timer();
		watchdog.schedule(new TimerTask()
		{
			public void run()
			{
				Game game = games;
				while(game != null)
				{
					if(game.decTicks() == 0)
						game.endGame();
					game = game.getNext();
				}
			}
		}, 0, 30000);
		
		Runtime.getRuntime().addShutdownHook(new KillingBriscolao());
	}

	/**
	 * When the connection is established, Birscolao tries to join the specified channel
	 */
	protected void onConnect()
	{
		System.out.println("Connesso al server " + server);
		//joinChannel(channel);
		for(String channel : channels)
			joinChannel(channel);
	}
	
	/**
	 * After disconnection a message is printed to the std output
	 */
	protected void onDisconnect()
	{
		System.out.println("Disconnesso da " + server);
	}

	/**
	 * After joining the choosed channel, Briscolao send a message to the whole
	 * channel to inform the users how to use this bot
	 */
	protected void onJoin(String channel, String sender, String login, String hostname)
	{
		if(sender.equals(getNick()))
		{
			System.out.println("Entrato nel canale " + channel);
			sendMessage(channel, PROGRAM_NAME + " " + VERSION + ": la briscola di skypjack (mod by Droscy) su " + channel);
			botUsage(channel);
		}
	}
	
	/**
	 * 
	 */
	protected void onKick(String channel, String kickerNick,
			String kickerLogin, String kickerHostname, String recipientNick,
			String reason)
	{
		if(recipientNick.equals(getNick()))
		{
			System.out.println(PROGRAM_NAME + ": il bot è stato espulso dal canale " + channel + " da " + kickerNick);
			String[] channels = getChannels();
			if(channels.length==0)
			{
				System.out.println("Non sono presente in nessun canale, mi chiudo");
				disconnect();
				System.exit(0);
			}
		}
	}

	protected void onMessage(String channel, String sender, String login, String hostname, String message)
	{
		if(!sender.equals(getNick()))
		{
			if(message.startsWith(BOT_COMMAND))
				startMatch(channel, sender, message);
			else
			{
				UserBotUsage user = new UserBotUsage(sender);
				//if(message.contains(getNick()) && usersBotUsageHash.add(user))
				if(message.contains(getNick()) && user.addToHash())
				{
					watchdog.schedule(user, 300000);
					botUsage(channel);
				}
			}
		}
	}

	protected void onPrivateMessage(String sender, String login, String hostname, String message)
	{
		if(games != null)
			games.checkStep(sender, message);
		else
		{
			sendMessage(sender, "nessuna partita in corso per il tuo utente, prima devi iniziare un match dal canale " + channels);
			botUsage(sender);
		}
	}
	
	private void sendFastMessage(String target, String message)
	{
		sendRawLine("PRIVMSG " + target + " :" + message);
	}
	

	private synchronized void startMatch(String channel, String sender, String message)
	{
		// TODO sistemare per 4 giocatori!
		String[] tokens = message.split("\\s");
		if(tokens.length != 2)
		{
			sendMessage(channel, "comando di inizio partita non valido");
			botUsage(channel);
		}
		else if(tokens[1].equals(getNick()))
			sendMessage(channel, "non puoi giocare contro il computer, ma solo contro un altro utente");
		else if(tokens[1].equals(sender))
			sendMessage(channel, "non puoi giocare contro te stesso :-)");
		else
		{
			String playerOne = sender;
			String playerTwo = null;
			User[] users = getUsers(channel);
			
			int iter = 0;
			while((iter < users.length) && (playerTwo == null))
			{
				if(users[iter].getNick().equals(tokens[1]))
					playerTwo = tokens[1];
				
				iter++;
			}
			
			if(playerTwo == null)
				sendMessage(channel, tokens[1] + " non \u00E8 un utente presente su questo canale");
			else if((games != null) && (games.checkPlayer(playerOne)))
				sendMessage(channel, playerOne + " sta gi\u00E0  giocando una partita a briscola");
			else if((games != null) && (games.checkPlayer(playerTwo)))
				sendMessage(channel, playerTwo + " sta gi\u00E0  giocando una partita a briscola");
			else
			{
				Game game = new Game(new String[]{playerOne,playerTwo});
				game.setNext(games);
				games = game;
				sendMessage(channel, "partita tra " + playerOne + " e " + playerTwo + " iniziata");
				game.startGame(channel);
			}
		}
	}

	/**
	 * TODO
	 * Controlla quale delle due carte vince
	 */
	private static int cardVsCard(int first, int second, int bType)
	{
		int value = -1;
		
		int fvalue = cardValue(first);
		int svalue = cardValue(second);
		
		int ftype = cardType(first);
		int stype = cardType(second);
		
		if((stype != ftype) && (stype != bType))
			value = first;
		else if((stype != ftype) && (stype == bType))
			value = second;
		else if(stype == ftype)
		{
			if((svalue == 0) || (fvalue == 0))
				value = (svalue == 0)?second:first;
			else if((svalue == 2) || (fvalue == 2))
				value = (svalue == 2)?second:first;
			else
				value = (svalue > fvalue)?second:first;
		}
		return value;
	}

	// TODO perché fai questa conversione?
	private static int cardValue(int card)
	{
		return card % 10;
	}

	// TODO anche questo non capisco a cosa serva?
	private static int cardType(int card)
	{
		return (card - (card % 10)) / 10;
	}

	
	/**
	 * Converts the number typed by a player in the corrispective card
	 * @param card the int typed by the user
	 * @return the name of the choosen card (returns ?? if something goes wrong)
	 */
	private static String cardToString(int card)
	{
		int type = cardType(card);
		int value = cardValue(card);
		String rep = "";
		
		switch(value)
		{
			case Cards.ASSO:
				rep += "Asso di ";
				break;
			case  Cards.DUE:
				rep += "Due di ";
				break;
			case Cards.TRE:
				rep += "Tre di ";
				break;
			case Cards.QUATTRO:
				rep += "Quattro di ";
				break;
			case Cards.CINQUE:
				rep += "Cinque di ";
				break;
			case Cards.SEI:
				rep += "Sei di ";
				break;
			case Cards.SETTE:
				rep += "Sette di ";
				break;
			case Cards.GOBBO:
				rep += "Gobbo di ";
				break;
			case Cards.DONNA:
				rep += "Donna di ";
				break;
			case Cards.REGIO:
				rep += "Regio di ";
				break;
			default:
				rep += "?? di ";
		}
		
		switch(type)
		{
			case Cards.CUORI:
				rep += "Cuori";
				break;
			case Cards.MATTONI:
				rep += "Mattoni";
				break;
			case Cards.PICCHE:
				rep += "Picche";
				break;
			case Cards.FIORI:
				rep += "Fiori";
				break;
			default:
				rep += "??";
		}
		
		return rep;
	}

	
	/**
	 * For each played card returns its own points
	 * @param card the int that represent a played card
	 * @return the points associated with that card
	 */
	private static int cardToPoints(int card)
	{
		int value = cardValue(card);
		
		switch(value)
		{
			case Cards.ASSO:
				return 11;
			case Cards.TRE:
				return 10;
			case Cards.GOBBO:
				return 2;
			case Cards.DONNA:
				return 3;
			case Cards.REGIO:
				return 4;
			default:
				return 0;
		}
	}

	
	/**
	 * Sends a message to dest with the howto instructions
	 * @param dest the recipient of this message (can be a user or a channel)
	 */
	private void botUsage(String dest)
	{
		sendMessage(dest, "Per giocare: " + BOT_COMMAND + " avversario");
	}
	

	public static void main(String args[])
	{
		System.out.println(PROGRAM_NAME + " " + VERSION);
		System.out.println("Copyright \u00A9 " + YEAR + " skypjack, Droscy");
		System.out.println("This program is released under GNU General Public License v2 " +
				"and it is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY!\n");
		
		if(args.length == 0)
			usageANDexit();
		
		LongOpt[] longOpts = new LongOpt[]
		                                 {
				new LongOpt("server",LongOpt.REQUIRED_ARGUMENT,null,'s'),
				new LongOpt("chan",LongOpt.REQUIRED_ARGUMENT,null,'c'),
				new LongOpt("pwd",LongOpt.REQUIRED_ARGUMENT,null,'p'),
				new LongOpt("help",LongOpt.NO_ARGUMENT,null,'h')
		                                 };
		
		Getopt getOpt = new Getopt(PROGRAM_NAME,args,"h?s:c:p:",longOpts);
		int opt;
		while((opt = getOpt.getopt()) != -1)
		{
			switch(opt)
			{
				case 's':
					String[] access = getOpt.getOptarg().split(":");
					server = access[0];
					try
					{
						port = Integer.parseInt(access[1]);
					}
					catch(Exception e)
					{
						port = 6667;
					}
					break;
				case 'c':
					String[] tokens = getOpt.getOptarg().split(",");
					channels = new String[tokens.length];
					for(int i=0; i<tokens.length; i++)
					{
						channels[i] = "#" + tokens[i].replaceFirst("#", "");
					};
					break;
				case 'p':
					password = getOpt.getOptarg();
					break;
		        case ':':
		            printError("l'opzione " + (char)getOpt.getOptopt() + " richiede un parametro!");
		            usageANDexit();
		            break;
		        case '?':
				case 'h':
					longUsageANDexit();
					break;
			}
		}
		
		if(server == null || channels == null)
		{
			printError("parametri non validi!");
			usageANDexit();
		}
		
		
		Briscolao bot = new Briscolao();
		bot.setVersion(VERSION);
		bot.setVerbose(false);
		//bot.setVerbose(true);
		
		try
		{	
			try
			{
				bot.setEncoding("UTF8");
				//bot.setEncoding("ISO-8859-15");
			}
			catch(UnsupportedEncodingException e)
			{
				bot.setEncoding(null);
			}
			
			System.out.print("In connessione...\n  Server: " + server + ":" + port + (channels.length==1?"\n  Canale: ":"\n  Canali: "));
			for(String channel : channels)
				System.out.print(channel + " ");
			System.out.println();
			
			bot.connect(server,port,password);
		}
		catch(IOException ioe)
		{
			printError("impossibile collegarsi al server!");
			System.exit(1);
		}
		catch(NickAlreadyInUseException ne)
		{
			printError("il nick " + PROGRAM_NAME + " \u00E8 gi\u00E0 presente nel canale");
			System.exit(1);
		}
		catch(IrcException ie)
		{
			printError("il server nega l'accesso di questo bot!");
			System.exit(1);
		}
	}
	
	
	/**
	 * Prints the usage string in the standard output and exit the program
	 */
	private static void usageANDexit()
	{
		System.out.println("Usage: java -jar briscolao.jar -s <server>[:port] -c <channel> [-p <password>]");
		System.out.println();
		System.exit(1);
	}
	
	/**
	 * Prints the long usage in the standard output and exit the program
	 */
	private static void longUsageANDexit()
	{
		System.out.println("Usage: java -jar briscolao.jar <options>");
		System.out.println("\nOPTIONS:");
		System.out.println("   -s, --server <server> the url of the server to connect to (with optional trailing [:port] number)");
		System.out.println("   -c, --chan <channel>\tthe channel to join after connection (without leading #)");
		System.out.println("   -p, --pwd <password>\tthe password of the server (optional)");
		System.out.println("   -h, --help\tthis help");
		System.out.println("\nCHANNELS:");
		System.out.println("   multiple channels are permitted");
		System.out.println("   <channel> ::= <channel> | <channel> , [{,<channel>}]");
		System.out.println();
		System.exit(1);
	}
	
	/**
	 * Prints a msg error to the standard output
	 * @param msg the message to print
	 */
	public static void printError(String msg)
	{
		System.err.println(PROGRAM_NAME + ": " + msg);
	}
	
	
	/**
	 * This class is used on Ctrl+C pressing for disconnetting briscolao
	 * from che server.
	 * @author Droscy
	 */
	private class KillingBriscolao extends Thread
	{
		public void run()
		{
			for(String channel : channels)
			{
				sendFastMessage(channel, "mi sto chiudendo... ciao");
			}
			quitServer("il bot \u00E8 stato chiuso");
			System.out.println("Disconnesso da " + server);
		}
	}
}
