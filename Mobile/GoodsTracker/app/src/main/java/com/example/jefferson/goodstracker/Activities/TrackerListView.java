package com.example.jefferson.goodstracker.Activities;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Jefferson on 09/07/2017.
 */

public class TrackerListView {

    List<String> groupList;
    List<String> childList;
    Map<String, List<String>> collection;


    TrackerListView(){

        createGroupList();
        createCollection();
    }

    public void createGroupList() {
        groupList = new ArrayList<String>();
        groupList.add("HP");
        groupList.add("Dell");
        groupList.add("Lenovo");
        groupList.add("Sony");
        groupList.add("HCL");
        groupList.add("Samsung");
    }

    public void createCollection() {
        // preparing laptops collection(child)
        String[] hpModels = {"HP Pavilion G6-2014TX", "ProBook HP 4540",
                "HP Envy 4-1025TX"};
        String[] hclModels = {"HCL S2101", "HCL L2102", "HCL V2002"};
        String[] lenovoModels = {"IdeaPad Z Series", "Essential G Series",
                "ThinkPad X Series", "Ideapad Z Series"};
        String[] sonyModels = {"VAIO E Series", "VAIO Z Series",
                "VAIO S Series", "VAIO YB Series"};
        String[] dellModels = {"Inspiron", "Vostro", "XPS"};
        String[] samsungModels = {"NP Series", "Series 5", "SF Series"};

        collection = new LinkedHashMap<String, List<String>>();

        for (String laptop : groupList) {
            if (laptop.equals("HP")) {
                loadChild(hpModels);
            } else if (laptop.equals("Dell"))
                loadChild(dellModels);
            else if (laptop.equals("Sony"))
                loadChild(sonyModels);
            else if (laptop.equals("HCL"))
                loadChild(hclModels);
            else if (laptop.equals("Samsung"))
                loadChild(samsungModels);
            else
                loadChild(lenovoModels);

            collection.put(laptop, childList);
        }
    }

    public void loadChild(String[] laptopModels) {

        childList = new ArrayList<String>();

        for (String model : laptopModels) {
            childList.add(model);
        }
    }

    public List<String> getGroupList() {
        return groupList;
    }

    public void setGroupList(List<String> groupList) {
        this.groupList = groupList;
    }

    public List<String> getChildList() {
        return childList;
    }

    public void setChildList(List<String> childList) {
        this.childList = childList;
    }

    public Map<String, List<String>> getLaptopCollection() {
        return collection;
    }

    public void setLaptopCollection(Map<String, List<String>> collection) {
        this.collection = collection;
    }
}
