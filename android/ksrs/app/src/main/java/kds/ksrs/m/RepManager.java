package kds.ksrs.m;

import java.time.LocalDate;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;

public final class RepManager {

    public enum Answer { RIGHT, WRONG }
    private Queue<Card> reps = null;
    private boolean isRandomized = false;

    public RepManager(boolean isRandomized) {
        this.isRandomized = isRandomized;
    }

    public RepManager() { }

    public void generateReps(Deck deck) {
        reps = new LinkedList<>();
        Iterator<Card> it = deck.getIterator();
        while (it.hasNext()) {
            addCardIfDue(it.next());
        }
    }

    private void addCardIfDue(Card card) {
        LocalDate now = LocalDate.now();
        if (card.isDue()) {
            reps.add(card);
        }
    }

    public Card nextRep() {
        return reps.isEmpty() ? null : reps.remove();
    }

    public void answerRep(Answer ans, Card card) {
        if (ans == Answer.RIGHT) {
            setNextDueDate(card);
        } else {
            card.setIntervalSum(0);
            card.setDueDate(LocalDate.now());
            reps.add(card);
        }
    }

    private int nextInterval(int intsum) {
        if (intsum < 3) {
            return 1;
        }
        else {
            int i = 0;
            int sum = intsum - 3;
            while (sum > 0) {
                i = (int)(1.3f * i + 1);
                sum -= i;
            }
            return (int)(1.3f * i + 1);
        }
    }

    private void setNextDueDate(Card card) {
        int interval = nextInterval(card.getIntervalSum());
        LocalDate next = LocalDate.now().plusDays(interval);
        card.setDueDate(next);
        card.setIntervalSum(card.getIntervalSum() + interval);
    }
}
