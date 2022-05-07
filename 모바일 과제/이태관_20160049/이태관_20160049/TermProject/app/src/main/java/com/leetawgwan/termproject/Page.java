package com.leetawgwan.termproject;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.GridView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.List;

public class Page extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.matthew_page);
        final Intent inte = getIntent();
        int position = inte.getIntExtra("position",0);
        int max_page[] = {28,16,24,21,28,16,16,13,6,6,4,4,5,3,6,4,3,1,13,5,5,3,5,1,1,1,22};
        List<String> page = new ArrayList<String>();

        for (int i = 1 ; i <= max_page[position] ; i++){
            page.add(Integer.toString(i));
        }
        GridView list = (GridView) findViewById(R.id.matthew_page);

        list.setAdapter(new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, page));
        list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int index, long l) {
                    Intent bible_intent = new Intent(Page.this, content_bible.class);
                    bible_intent.putExtra("page",index);
                    bible_intent.putExtra("position",position);
                    startActivity(bible_intent);
            }
        });
    }
}
