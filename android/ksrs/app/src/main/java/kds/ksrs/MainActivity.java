package kds.ksrs;

import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.OpenableColumns;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.json.JSONException;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;

import kds.ksrs.model.Deck;
import kds.ksrs.model.DeckFileUtility;

public class MainActivity extends AppCompatActivity {

    private static final int JSON_REQ_CODE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("ONCREATE","ONCREATE");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onLoadClick(View v) {
        searchForJSONFiles();
    }

    private void searchForJSONFiles() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("application/octet-stream");
        startActivityForResult(intent, JSON_REQ_CODE);
    }

    private String getFileNameFromUri(Uri uri) {
        String fileName = null;
        Cursor cursor = getContentResolver().query(uri, null, null, null, null, null);
        if (cursor != null && cursor.moveToFirst()) {
            fileName = cursor.getString(cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME));
        }
        cursor.close();
        if (fileName == null) {
            String[] tokens = uri.getLastPathSegment().split("\\/");
            fileName = tokens[tokens.length - 1];
        }
        String[] extTokens = fileName.split("\\.");
        String ext = extTokens[extTokens.length - 1];
        if (!ext.toLowerCase().equals("json")) {
            fileName = null;
        }
        return fileName;
    }

    private void changeTitleText(String newText) {
        TextView textView = (TextView)findViewById(R.id.textView);
        textView.setText(newText);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == JSON_REQ_CODE) {
            if (resultCode == RESULT_OK) {
                boolean succ = true;
                try {
                    String fileName = getFileNameFromUri(data.getData());
                    if (fileName == null) {
                        // TODO: Handle null file
                        Log.d("ERROR", "FILE DID NOT LOAD");
                        return;
                    }
                    InputStream inStream = getContentResolver().openInputStream(data.getData());
                    Deck deck = DeckFileUtility.ReadDeckFile(fileName, inStream);
                    changeTitleText(getString(R.string.loaded_deck_msg) + " " + deck.getName());
                    Log.d("DECK_RESULT", deck.toString());
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                    succ = false;
                } catch (ParseException e) {
                    e.printStackTrace();
                    succ = false;
                } catch (IOException e) {
                    e.printStackTrace();
                    succ = false;
                } catch (JSONException e) {
                    e.printStackTrace();
                    succ = false;
                }
                if (!succ) {
                    // TODO: Handle deck read failure
                    return;
                }
            }
        }
    }
}
