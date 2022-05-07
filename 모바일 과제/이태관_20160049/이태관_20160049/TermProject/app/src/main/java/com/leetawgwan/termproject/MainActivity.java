package com.leetawgwan.termproject;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;


public class MainActivity extends AppCompatActivity{
    private ListView bookList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bookList = (ListView) findViewById(R.id.booklist);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.bookList,android.R.layout.simple_list_item_1);
        bookList.setAdapter(adapter);
        bookList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                Intent intent = new Intent(MainActivity.this, Page.class);
                intent.putExtra("position",position);
                startActivity(intent);


            }
        });


    }
}

class GetBookContent{
    private String bookName;
    public GetBookContent(){
        this.bookName = "마";
    }
    public GetBookContent(String bookName){
        this.bookName = bookName;
        Log.d("test","start");
    }

    public void getMaxPage(){
        try {
            File file2 = new File("D:\\file.txt");
            try {
                FileOutputStream fileOutputStream = new FileOutputStream(file2, true);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }


            File file = new File("D:/bible.txt");
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String line = "";
            while((line = bufferedReader.readLine())!=null){
                Log.d("test",line);
                line = "";
            }
            bufferedReader.close();
        }
        catch (FileNotFoundException e){
            Log.d("error","notfonnd");
        }
        catch (IOException e){
            Log.d("error","IOExceoption");
        }
    }


}

