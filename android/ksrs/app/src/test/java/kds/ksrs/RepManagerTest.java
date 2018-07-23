package kds.ksrs;

import org.junit.Test;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Iterator;

import kds.ksrs.model.Card;
import kds.ksrs.model.Deck;
import kds.ksrs.model.RepManager;

import static org.junit.Assert.assertEquals;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class RepManagerTest {
    @Test
    public void generateReps_Generates1Rep() {
        Deck deck = new Deck("test0");
        Card card = new Card("FRONT", "BACK");
        deck.addCard(card);
        RepManager rm = new RepManager();
        rm.generateReps(deck);
        Card r0 = rm.nextRep();
        assertEquals(card, r0);
        assertEquals(null, rm.nextRep());
        assertEquals(null, rm.nextRep());
    }

    @Test
    public void generateReps_GeneratesManyReps() {
        Deck deck = new Deck("test0");
        ArrayList<Card> cards = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            Card card = new Card("FRONT" + i, "BACK" + i);
            deck.addCard(card);
            cards.add(card);
        }
        RepManager rm = new RepManager();
        rm.generateReps(deck);
        for (int i = 0; i < 100; i++) {
            Card card = rm.nextRep();
            deck.addCard(card);
            assertEquals(cards.get(i), card);
        }
        assertEquals(null, rm.nextRep());
    }

    @Test
    public void addCardIfDue_AddsOnlyDueCards() {
        Deck deck = new Deck("test0");
        LocalDateTime now = LocalDateTime.now().withMinute(0).withSecond(0).withHour(0);
        Card cardBef = new Card("FRONT", "BACK", 5, now.minusDays(1));
        Card cardNow = new Card("FRONT1", "BACK", 5, now);
        Card cardAft = new Card("FRONT2", "BACK", 5, now.plusDays(1));
        deck.addCard(cardBef);
        deck.addCard(cardNow);
        deck.addCard(cardAft);
        RepManager rm = new RepManager();
        rm.generateReps(deck);
        assertEquals(cardBef, rm.nextRep());
        assertEquals(cardNow, rm.nextRep());
        assertEquals(null, rm.nextRep());
    }

    @Test
    public void answerRep_IncreasesDueDateProperly() {
        Deck deck = new Deck("test0");
        LocalDateTime now = LocalDateTime.now().withMinute(0).withSecond(0).withHour(0).minusDays(15);
        int[] days = {1, 1, 1, 1, 2, 3, 4, 6, 8, 11, 15, 20, 27, 36, 47, 62};
        int[] sums = {0, 1, 2, 3, 4, 6, 9, 13, 19, 27, 38, 53, 73, 100, 136, 183};
        for (int i = 0; i < 15; i++) {
            Card card = new Card("FRONT", "BACK", sums[i], now);
            deck.addCard(card);
        }
        RepManager rm = new RepManager();
        rm.generateReps(deck);
        Iterator<Card> it = deck.getIterator();
        for (int i = 0; i < 15; i++) {
            Card card = it.next();
            rm.answerRep(RepManager.Answer.RIGHT, card);
            LocalDateTime expectedTime = LocalDateTime.now().plusDays(days[i]).withMinute(0).withSecond(0).withHour(0);
            assertEquals(sums[i+1], card.getIntervalSum());
            assertEquals(expectedTime.getDayOfMonth(), card.getDueDate().getDayOfMonth());
            assertEquals(expectedTime.getMonth(), card.getDueDate().getMonth());
            assertEquals(expectedTime.getYear(), card.getDueDate().getYear());
        }
    }

    @Test
    public void answerRep_DecreasesDueDateProperly() {
        Deck deck = new Deck("test0");
        LocalDateTime now = LocalDateTime.now().withMinute(0).withSecond(0).withHour(0).minusDays(15);
        int[] sums = {0, 1, 2, 3, 4, 6, 9, 13, 19, 27, 38, 53, 73, 100, 136, 183};
        for (int i = 0; i < 15; i++) {
            Card card = new Card("FRONT", "BACK", sums[i], now);
            deck.addCard(card);
        }
        RepManager rm = new RepManager();
        rm.generateReps(deck);
        Iterator<Card> it = deck.getIterator();
        for (int i = 0; i < 15; i++) {
            Card card = it.next();
            rm.answerRep(RepManager.Answer.WRONG, card);
            LocalDateTime expectedTime = LocalDateTime.now().withMinute(0).withSecond(0).withHour(0);
            assertEquals(0, card.getIntervalSum());
            assertEquals(expectedTime.getDayOfMonth(), card.getDueDate().getDayOfMonth());
            assertEquals(expectedTime.getMonth(), card.getDueDate().getMonth());
            assertEquals(expectedTime.getYear(), card.getDueDate().getYear());
        }
    }
}