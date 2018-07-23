package kds.ksrs.m;

import java.time.LocalDate;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;

public class RepManager {

    public enum Answer { RIGHT, WRONG }
    private Queue<Card> reps = null;
    private boolean isRandomized = false;

    public RepManager(final boolean isRandomized) {
        this.isRandomized = isRandomized;
    }

    public RepManager() { }

    public int getRepsDue() {
        return reps.size();
    }

    public void generateReps(final Deck deck) {
        reps = new LinkedList<>();
        final Iterator<Card> it = deck.getIterator();
        while (it.hasNext()) {
            addCardIfDue(it.next());
        }
    }

    private void addCardIfDue(final Card card) {
        final LocalDate now = LocalDate.now();
        if (card.isDue()) {
            reps.add(card);
        }
    }

    public Card nextRep() {
        return reps.isEmpty() ? null : reps.remove();
    }

    public void answerRep(final Answer ans, final Card card) {
        if (ans == Answer.RIGHT) {
            setNextDueDate(card);
        } else {
            card.setIntervalSum(0);
            card.setDueDate(LocalDate.now());
            reps.add(card);
        }
    }

    private int nextInterval(final int intsum) {
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

    private void setNextDueDate(final Card card) {
        final int interval = nextInterval(card.getIntervalSum());
        final LocalDate next = LocalDate.now().plusDays(interval);
        card.setDueDate(next);
        card.setIntervalSum(card.getIntervalSum() + interval);
    }
}