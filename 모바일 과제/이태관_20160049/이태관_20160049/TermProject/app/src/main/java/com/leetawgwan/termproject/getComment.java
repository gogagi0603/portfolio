package com.leetawgwan.termproject;

import android.content.Intent;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.List;

public class getComment extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.comment);
        final Intent intent = getIntent();
        int page = intent.getIntExtra("page",0);
        int position = intent.getIntExtra("position",0);
        int comment_index = intent.getIntExtra("comment_index",0);
        Bible bible = new Bible();
        String[][][] c = new String[1][][];
        if (position ==8) {
            c = bible.getGalatians();
        }
        else if (position == 0){
            c = bible.getMattew();
        }
        else if (position == 1){
            c = bible.getMark();
        }
        else if (position == 2){
            c = bible.getNuke();
        }
        else if (position == 3){
            c = bible.getJohn();
        }
        else if (position == 4){
            c = bible.getActs();
        }
        else if (position == 5){
            c = bible.getRomans();
        }
        else if (position == 6){
            c=bible.getCorinthians_first();
        }
        else if (position == 7){
            c = bible.getCorinthians_second();
        }
        else if (position == 9){
            c = bible.getEphesians();
        }
        else if (position == 10){
            c = bible.getPhilippians();
        }
        else if (position ==11){
            c = bible.getColossians();
        }
        else if (position ==12){
            c = bible.getThessalonians_first();
        }
        else if (position ==13){
            c = bible.getThessalonians_second();
        }
        else if (position == 14){
            c = bible.getTimothy_first();
        }
        else if (position == 15) {
            c = bible.getTimothy_second();
        }
        else if (position == 16){
            c = bible.getTitus();
        }
        else if (position == 17){
            c = bible.getPhilemon();
        }
        else if (position == 18){
            c = bible.getHebrews();
        }
        else if (position == 19){
            c = bible.getJames();
        }
        else if (position == 20){
            c = bible.getPeter_first();
        }
        else if (position == 21){
            c = bible.getPeter_second();
        }
        else if (position == 22){
            c = bible.getJohn_first();
        }
        else if (position == 23){
            c = bible.getJohn_second();
        }
        else if (position == 24){
            c = bible.getJohn_third();
        }
        else if (position == 25){
            c = bible.getJude();
        }
        else if (position == 26){
            c = bible.getApocalypse();
        }

        TextView textView = (TextView) findViewById(R.id.textView);
        textView.setText(c[1][page][comment_index]);
        textView.setMovementMethod(new ScrollingMovementMethod());
    }
}
