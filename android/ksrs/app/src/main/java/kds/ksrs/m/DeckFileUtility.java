package kds.ksrs.m;

import android.content.Context;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.util.Iterator;

public class DeckFileUtility {

    private final static String NAME_STR = "Name";
    private final static String FRONT_STR = "Front";
    private final static String BACK_STR = "Back";
    private final static String INTSUM_STR = "IntSum";
    private final static String DUEDATE_STR = "DueDate";
    private final static String CARDS_STR = "Card";
    private final static String DECK_STR = "Deck";

    private DeckFileUtility() {}

    public static Deck ReadDeckFile(final String fileName, final Context context) throws IOException, JSONException, ParseException {
        final FileInputStream inStream = context.openFileInput(fileName);
        return ReadDeckFile(fileName, inStream);
    }

    public static Deck ReadDeckFile(final String fileName, final InputStream inStream) throws IOException, JSONException, ParseException {
        final InputStreamReader inputStreamReader = new InputStreamReader(inStream, "UTF-8");
        final BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
        final StringBuilder stringBuilder = new StringBuilder();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            stringBuilder.append(line + "\n");
        }
        final JSONObject jsonObj = new JSONObject(stringBuilder.toString());
        final String name = jsonObj.getString(NAME_STR);
        final Deck deck = new Deck(name);
        final JSONArray jsonArr = jsonObj.getJSONObject(DECK_STR).getJSONArray(CARDS_STR);
        for (int i = 0; i < jsonArr.length(); i++) {
            final JSONObject cardObj = jsonArr.getJSONObject(i);
            final String front = cardObj.getString(FRONT_STR);
            final String back = cardObj.getString(BACK_STR);
            final int intsum = cardObj.getInt(INTSUM_STR);
            final String dueDate = cardObj.getString(DUEDATE_STR);
            final Card card = new Card(front, back, intsum, dueDate);
            deck.addCard(card);
        }
        return deck;
    }

    public static void WriteDeckFile(final String fileName, final Context context, final Deck deck) throws IOException, JSONException {
        final JSONObject jsonObj = CreateDeckJSON(deck);
        final FileOutputStream outStream = context.openFileOutput(fileName, context.MODE_PRIVATE);
        outStream.write(jsonObj.toString().getBytes());
        outStream.close();
    }

    private static JSONObject CreateDeckJSON(final Deck deck) throws JSONException {
        final JSONObject jsonObj = new JSONObject();
        final Iterator<Card> deckIterator = deck.getIterator();
        jsonObj.put(NAME_STR, deck.getName());
        final JSONObject deckObj = new JSONObject();
        final JSONArray cardArr = new JSONArray() ;
        while (deckIterator.hasNext()) {
            final Card card = deckIterator.next();
            final JSONObject cardObj = new JSONObject();
            cardObj.put(FRONT_STR, card.getFront());
            cardObj.put(BACK_STR, card.getBack());
            cardObj.put(INTSUM_STR, card.getIntervalSum());
            cardObj.put(DUEDATE_STR, card.getDueDateString());
            cardArr.put(cardObj);
        }
        deckObj.put(CARDS_STR, cardArr);
        jsonObj.put(DECK_STR, deckObj);
        return jsonObj;
    }
}