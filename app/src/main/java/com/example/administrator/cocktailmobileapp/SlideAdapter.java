package com.example.administrator.cocktailmobileapp;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import java.util.ArrayList;

public class SlideAdapter extends FragmentPagerAdapter {
    private static final String TAG = "SlideAdapter";

    private ArrayList<Fragment> mFragmentList;

    public SlideAdapter(FragmentManager fm) {
        super(fm);

        mFragmentList = new ArrayList<>();

        mFragmentList.add(
                SlideFragment.createSlideFragment(
                        "음료수 제조기 전원을 켜주세요!",
                        "음료수 제조기가 켜져 있어야 온도를 읽을 수 있어요.",
                        R.drawable.image_cocktail_blue,
                        R.color.colorBlue255,
                        R.color.colorOrange255,
                        "6"
                )
        );

        mFragmentList.add(
                SlideFragment.createSlideFragment(
                        "",
                        "",
                        "",
                        R.drawable.image_jamong,
                        R.color.colorPink255,
                        R.color.colorOrange255,
                        "0")
        );

        mFragmentList.add(
                SlideFragment.createSlideFragment(
                        "",
                        "",
                        "",
                        R.drawable.image_lemon,
                        R.color.colorOrange255,
                        R.color.colorYellow255,
                        "1")
        );

        mFragmentList.add(
                SlideFragment.createSlideFragment(
                        "",
                        "",
                        "",
                        R.drawable.image_orange,
                        R.color.colorYellow255,
                        R.color.colorGreen255,
                        "2")
        );

    }

    /*
    *
    * 칵테일이 무엇으로 만들어졌는지만 간략하게 3줄로 설명해주세요
    *
    * */

    @Override
    public Fragment getItem(int pos) {
        return mFragmentList.get(pos);
    }

    @Override
    public int getCount() {
        return mFragmentList.size();
    }
}
