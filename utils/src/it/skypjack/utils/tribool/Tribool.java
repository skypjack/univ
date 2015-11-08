package it.skypjack.utils.tribool;


/**
 * @author skypjack
 * 
 * Tribool: three-state safe bool value
 */
public final class Tribool {
	
	public static abstract class Bool { }
	public static final class True extends Bool { }
	public static final class False extends Bool { }
	public static final class Indeterminate extends Bool { }
	
	private Bool bool;
	
	public Tribool() {
		bool = new Indeterminate();
	}
	
	public Tribool(Bool bool) {
		if(bool != null) this.bool = bool;
		else this.bool = new Indeterminate();
	}
	
	public boolean isTrue() {
		return bool.getClass() == True.class;
	}
	
	public boolean isFalse() {
		return bool.getClass() == False.class;
	}
	
	public boolean isTrueOrFalse() {
		return bool.getClass() != Indeterminate.class;
	}
	
	public boolean isIndeterminate() {
		return bool.getClass() == Indeterminate.class;
	}

	public Bool getBool() {
		return bool;
	}

	public void setBool(Object obj) {
		bool = new Indeterminate();
	}
	
	public void setBool(Tribool bool) {
		this.bool = bool.getBool();
	}

	public void setBool(Bool bool) {
		this.bool = bool;
	}
	
	public void setBool(Boolean bool) {
		setBool(bool.booleanValue());
	}
	
	public void setBool(boolean bool) {
		if(bool) this.bool = new True();
		else this.bool = new False();
	}
	
	public void unset() {
		bool = new Indeterminate();
	}
	
	public boolean equals(Object obj) {
		return false;
	}
	
	public boolean equals(Tribool tribool) {
		return tribool.getBool().getClass() == bool.getClass();
	}
	
	public boolean equals(Bool bool) {
		return bool.getClass() == this.bool.getClass();
	}
	
	public boolean equals(Boolean bool) {
		return equals(bool.booleanValue());
	}
	
	public boolean equals(boolean bool) {
		return this.bool.getClass() == (bool ? True.class : False.class);
	}
	
}
