package bot.briscolao;

import java.util.HashSet;
import java.util.TimerTask;


public class UserBotUsage extends TimerTask
{
	//public static final int START_TTL = 18;
	
	private static HashSet<UserBotUsage> hashSet = new HashSet<UserBotUsage>();
	
	private String userNick;
	//private int ttl;
	
	public UserBotUsage(String userNick)
	{
		this.userNick = userNick;
		//this.ttl = START_TTL;
	}
	
	public synchronized boolean addToHash()
	{
		return hashSet.add(this);
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((userNick == null)?0:userNick.hashCode());
		return result;
	}


	@Override
	public boolean equals(Object obj)
	{
		if(this == obj) return true;
		if(obj == null) return false;
		if(getClass() != obj.getClass()) return false;
		final UserBotUsage other = (UserBotUsage) obj;
		if(userNick == null)
		{
			if(other.userNick != null) return false;
		}
		else if(!userNick.equals(other.userNick)) return false;
		return true;
	}

	
	/*public synchronized int decrementTtl()
	{
		return --ttl;
	}*/

	@Override
	public void run()
	{
		hashSet.remove(this);
		cancel();
	}
}
