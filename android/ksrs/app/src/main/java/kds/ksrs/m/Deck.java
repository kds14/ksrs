package kds.ksrs.m;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Objects;

public class Deck {
    private final ArrayList<Card> cards = new ArrayList<Card>();
    private String name;

    public Deck(String name) {
        this.name = name;
    }

    public int getSize() {
        return cards.size();
    }

    public void addCard(Card card) {
        cards.add(card);
    }

    public void removeCard(Card card) {
        cards.remove(card);
    }

    public Iterator<Card> getIterator() {
        return cards.iterator();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public boolean equals(final Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        final Deck deck = (Deck) o;
        return Objects.equals(new HashSet<>(cards), new HashSet<>(deck.cards)) &&
                Objects.equals(name, deck.name);
    }

    @Override
    public String toString() {
        return "Deck{" +
                "cards=" + cards +
                ", name='" + name + '\'' +
                '}';
    }

    @Override
    public int hashCode() {
        return Objects.hash(cards, name);
    }
}
