package kds.ksrs;

import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.OpenableColumns;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import org.json.JSONException;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;

import kds.ksrs.m.Deck;
import kds.ksrs.m.DeckFileUtility;

public class MainActivity extends AppCompatActivity {

    private static final int JSON_REQ_CODE = 1;
    // no application/json
    private static final String JSON_MIME_TYPE = "application/octet-stream";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onLoadClick(View v) {
        searchForJSONFiles();
    }

    private void searchForJSONFiles() {
        final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType(JSON_MIME_TYPE);
        startActivityForResult(intent, JSON_REQ_CODE);
    }

    private String getFileNameFromUri(Uri uri) {
        String fileName = null;
        final Cursor cursor = getContentResolver().query(uri, null, null, null, null, null);
        if (cursor != null && cursor.moveToFirst()) {
            fileName = cursor.getString(cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME));
        }
        cursor.close();
        if (fileName == null) {
            final String[] tokens = uri.getLastPathSegment().split("\\/");
            fileName = tokens[tokens.length - 1];
        }
        final String[] extTokens = fileName.split("\\.");
        final String ext = extTokens[extTokens.length - 1];
        if (!ext.toLowerCase().equals("json")) {
            fileName = null;
        }
        return fileName;
    }

    private void changeTitleText(String newText) {
        final TextView textView = (TextView)findViewById(R.id.textView);
        textView.setText(newText);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == JSON_REQ_CODE) {
            if (resultCode == RESULT_OK) {
                boolean succ = true;
                try {
                    final String fileName = getFileNameFromUri(data.getData());
                    if (fileName == null) {
                        // TODO: Handle null file
                        Log.d("ERROR", "FILE DID NOT LOAD");
                        return;
                    }
                    final InputStream inStream = getContentResolver().openInputStream(data.getData());
                    Deck deck = DeckFileUtility.ReadDeckFile(fileName, inStream);
                    changeTitleText(getString(R.string.loaded_deck_msg) + " " + deck.getName());
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
