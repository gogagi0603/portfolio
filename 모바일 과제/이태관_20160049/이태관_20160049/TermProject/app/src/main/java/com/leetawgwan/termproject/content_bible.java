package com.leetawgwan.termproject;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.ListView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class content_bible extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.matthew_bible);
        final Intent intent = getIntent();
        int page = intent.getIntExtra("page",0);
        int position = intent.getIntExtra("position",0);

        List<String> contents = new ArrayList<String>();
        Bible bible = new Bible();
        String[][][] c = new String[0][][];
        if (position == 0) {
            c = bible.getMattew();
        }
        else if (position ==1){
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
        else if(position == 6){
            c = bible.getCorinthians_first();
        }
        else if(position ==7){
            c = bible.getCorinthians_second();
        }
        else if (position == 8) {
            c = bible.getGalatians();
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

        for (int j = 0; j < c[0][page].length ; j++) {
            contents.add(Integer.toString(page+1)+"장 "+Integer.toString(j+1) + "절"+" : "+c[0][page][j]);
        }

        List<String> test = new ArrayList<String>();
        test.add("hello");
        ListView list = (ListView) findViewById(R.id.matthew_bible);

        list.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, contents));
        list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int index, long l) {
                if (Arrays.asList(5,6,7,8,9,10,11,12,13,14,16,17,19,20,21,25).contains(position)) {
                    Intent comment_intent = new Intent(content_bible.this, getComment.class);
                    comment_intent.putExtra("position", position);
                    comment_intent.putExtra("page", page);
                    comment_intent.putExtra("comment_index", index);
                    startActivity(comment_intent);
                }
            }
        });
    }
}
