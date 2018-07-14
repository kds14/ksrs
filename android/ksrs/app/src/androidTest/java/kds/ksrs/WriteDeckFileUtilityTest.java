package kds.ksrs;

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import org.json.JSONException;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;
import java.text.ParseException;

import kds.ksrs.model.Card;
import kds.ksrs.model.Deck;
import kds.ksrs.model.DeckFileUtility;

import static org.junit.Assert.assertEquals;

@RunWith(AndroidJUnit4.class)
public class WriteDeckFileUtilityTest {

    private Context context;
    @Before
    public void setup() {
        context = InstrumentationRegistry.getTargetContext();
    }

    @Test
    public void WritesAndReadsEmptyDeck() throws IOException, JSONException, ParseException {
        final String fileName = "testfile0";
        final String deckName = "testDeck";
        Deck deck = new Deck(deckName);
        DeckFileUtility.WriteDeckFile(fileName, context, deck);
        Deck deckOut = DeckFileUtility.ReadDeckFile(fileName, context);
        assertEquals(deck, deckOut);
    }

    @Test
    public void WritesAndReadsSize1Deck() throws IOException, JSONException, ParseException {
        final String fileName = "testfile1";
        final String deckName = "testDeck";
        Deck deck = new Deck(deckName);
        Card card = new Card("FRONT", "BACK");
        deck.addCard(card);
        DeckFileUtility.WriteDeckFile(fileName, context, deck);
        Deck deckOut = DeckFileUtility.ReadDeckFile(fileName, context);
        assertEquals(deck, deckOut);
    }

    @Test
    public void WritesAndReadsSize3Deck() throws IOException, JSONException, ParseException {
        final String fileName = "testfile1";
        final String deckName = "testDeck";
        Deck deck = new Deck(deckName);
        Card card = new Card("FRONT", "BACK");
        Card card1 = new Card("1", "11");
        Card card2 = new Card("2", "22");
        deck.addCard(card);
        deck.addCard(card1);
        deck.addCard(card2);
        DeckFileUtility.WriteDeckFile(fileName, context, deck);
        Deck deckOut = DeckFileUtility.ReadDeckFile(fileName, context);
        assertEquals(deck, deckOut);
    }
}