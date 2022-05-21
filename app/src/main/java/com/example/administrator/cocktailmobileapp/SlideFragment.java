package com.example.administrator.cocktailmobileapp;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.graphics.drawable.RoundedBitmapDrawable;
import android.support.v4.graphics.drawable.RoundedBitmapDrawableFactory;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;

public class SlideFragment extends Fragment {
    private static final String TAG = "SlideView";

    private View mView;

    private TextView mName;
    private TextView mTitle;
    private TextView mDesciption;
    private Button mButton;
    private ImageView mCardImage;
    private RelativeLayout mBackground;
    private String message = "옆으로 슬라이드 해서 음료를 선택 해주세요.";

    // 음료수 선택 화면 생성자
    public static SlideFragment createSlideFragment (String name, String title, String description, int imageID , int gradientStartColor, int gradientEndColor, String message) {
        SlideFragment slideFragment = new SlideFragment();

        Bundle bundle = new Bundle();

        bundle.putString("name", name);
        bundle.putString("title", title);
        bundle.putString("description", description);
        bundle.putInt("imageID", imageID);
        bundle.putInt("gradientStartColor", gradientStartColor);
        bundle.putInt("gradientEndColor", gradientEndColor);
        bundle.putString("message", message);

        slideFragment.setArguments(bundle);

        return slideFragment;
    }

    // 첫 화면, 현재 온도 알려주는 화면 생성자
    public static SlideFragment createSlideFragment(String temp, String recommand , int imageID, int gradientStartColor, int gradientEndColor,String message){
        SlideFragment slideFragment = new SlideFragment();

        Bundle bundle = new Bundle();
        bundle.putString("name", temp);
        bundle.putString("title", recommand);
        bundle.putInt("imageID", imageID);
        bundle.putInt("gradientStartColor", gradientStartColor);
        bundle.putInt("gradientEndColor", gradientEndColor);
        bundle.putString("message", message);

        slideFragment.setArguments(bundle);

        return slideFragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LayoutInflater layoutInflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        mView = layoutInflater.inflate(R.layout.fragment_slideview, null);

        mName = mView.findViewById(R.id.name);
        mTitle = mView.findViewById(R.id.title);
        mDesciption = mView.findViewById(R.id.description);
        mButton = mView.findViewById(R.id.button);
        mCardImage = mView.findViewById(R.id.card_image);
        mBackground = mView.findViewById(R.id.background);

        final Bundle bundle = getArguments();


        mName.setText(bundle.getString("name"));
        mTitle.setText(bundle.getString("title"));
        mDesciption.setText(bundle.getString("description"));
        mCardImage.setImageResource(bundle.getInt("imageID"));

        // 앱을 키고 아두이노 업로딩 하면 현재 온도가 설정 됨
        // Bluetooth 클래스에서 ui를 변경하기 위해 BluetoothSet 인터페이스를 만들어 handler 스레드에서 ui 접근
        if(bundle.getString("message").equals("6")){
            mButton.setVisibility(View.INVISIBLE);  // 버튼만 사라지게 하기
           Bluetooth.getInstance().tempData(new Bluetooth.BluetoothSet() {
               @Override
               public void setView(int temp) {
                   mName.setText("현재 온도:" + temp +"C");
                   if(temp >= 20){  // 더울 떄
                    mTitle.setText("날씨가 덥네요.\n시원한 얼음을 넣어 먹는걸 추천해요!\n\n" + message);
                   }else if(temp <= 20){    // 추울 때
                    mTitle.setText("날씨가 춥네요.\n얼음을 넣어 드시면 감기 걸리실 수도 있겠어요 ~\n\n" + message);
                   }
               }
           });

        }


        GradientDrawable gradientDrawable = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT,
                new int[] {getResources().getColor(bundle.getInt("gradientStartColor")),
                           getResources().getColor(bundle.getInt("gradientEndColor"))});

        gradientDrawable.setCornerRadius(0f);
        mBackground.setBackgroundDrawable(gradientDrawable);


        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CustomDialog.getInstance(getContext()).show(bundle.getString("message"), findName(bundle.getString("message")));
            }
        });
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return mView;
    }

    @Override
    public void onStop() {
        super.onStop();

        recycleBitmap(mCardImage);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        recycleBitmap(mCardImage);
    }

    private void recycleBitmap (ImageView imageView) {
        Drawable drawable = imageView.getDrawable();

        if (drawable instanceof BitmapDrawable) {
            Bitmap bitmap = ((BitmapDrawable)drawable).getBitmap();
            bitmap.recycle();
        }

        drawable.setCallback(null);
    }

    private String findName(String num){
        switch(num){
            case "0": return "자몽";
            case "1": return "레몬";
            case "2": return "한라봉";
        }
        return "";
    }
}
