package kds.ksrs.model;

import java.util.ArrayList;
import java.util.Iterator;

public class Deck {
    private final ArrayList<Card> cards = new ArrayList<Card>();

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
}
