package kds.ksrs.model;

import java.util.Calendar;

public class Card {
    private String front;
    private String back;
    private Calendar dueDate;
    private int intervalSum = 0;

    public Card (String front, String back, Calendar dueDate, int intervalSum) {
        this.front = front;
        this.back = back;
        this.dueDate = dueDate;
        this.intervalSum = intervalSum;
    }

    public Card (String front, String back) {
        this.front = front;
        this.back = back;
    }

    public String getFront() {
        return front;
    }

    public void setFront(String value) {
        front = value;
    }

    public String getBack() {
        return back;
    }

    public void setBack(String value) {
        back = value;
    }

    public Calendar getDueDate() {
        return dueDate;
    }

    public void setDueDate(Calendar dueDate) {
        this.dueDate = dueDate;
    }

    public int getIntervalSum() {
        return intervalSum;
    }

    public void setIntervalSum(int intervalSum) {
        this.intervalSum = intervalSum;
    }

}
