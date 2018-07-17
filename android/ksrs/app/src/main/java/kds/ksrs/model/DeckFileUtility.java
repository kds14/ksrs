package kds.ksrs.model;

import android.content.Context;
import android.util.Log;

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

public final class DeckFileUtility {

    private final static String NAME_STR = "Name";
    private final static String FRONT_STR = "Front";
    private final static String BACK_STR = "Back";
    private final static String INTSUM_STR = "IntSum";
    private final static String DUEDATE_STR = "DueDate";
    private final static String CARDS_STR = "Card";
    private final static String DECK_STR = "Deck";

    private DeckFileUtility() {}

    public static Deck ReadDeckFile(String fileName, Context context) throws IOException, JSONException, ParseException {
        FileInputStream inStream = context.openFileInput(fileName);
        return ReadDeckFile(fileName, inStream);
    }

    public static Deck ReadDeckFile(String fileName, InputStream inStream) throws IOException, JSONException, ParseException {
        InputStreamReader inputStreamReader = new InputStreamReader(inStream, "UTF-8");
        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
        StringBuilder stringBuilder = new StringBuilder();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            stringBuilder.append(line + "\n");
        }
        //Log.d("JSONOBJ READ", stringBuilder.toString());
        JSONObject jsonObj = new JSONObject(stringBuilder.toString());
        String name = jsonObj.getString(NAME_STR);
        Deck deck = new Deck(name);
        JSONArray jsonArr = jsonObj.getJSONObject(DECK_STR).getJSONArray(CARDS_STR);
        for (int i = 0; i < jsonArr.length(); i++) {
            JSONObject cardObj = jsonArr.getJSONObject(i);
            String front = cardObj.getString(FRONT_STR);
            String back = cardObj.getString(BACK_STR);
            int intsum = cardObj.getInt(INTSUM_STR);
            String dueDate = cardObj.getString(DUEDATE_STR);
            Card card = new Card(front, back, intsum, dueDate);
            deck.addCard(card);
        }
        return deck;
    }

    public static void WriteDeckFile(String fileName, Context context, Deck deck) throws IOException, JSONException {
        JSONObject jsonObj = CreateDeckJSON(deck);
        FileOutputStream outStream = context.openFileOutput(fileName, context.MODE_PRIVATE);
        Log.d("JSONOBJ WRITE", jsonObj.toString());
        outStream.write(jsonObj.toString().getBytes());
        outStream.close();
    }

    private static JSONObject CreateDeckJSON(Deck deck) throws JSONException {
        JSONObject jsonObj = new JSONObject();
        Iterator<Card> deckIterator = deck.getIterator();
        jsonObj.put(NAME_STR, deck.getName());
        JSONObject deckObj = new JSONObject();
        JSONArray cardArr = new JSONArray() ;
        while (deckIterator.hasNext()) {
            Card card = deckIterator.next();
            JSONObject cardObj = new JSONObject();
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
