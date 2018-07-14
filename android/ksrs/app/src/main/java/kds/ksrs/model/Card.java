package kds.ksrs.model;

import android.annotation.TargetApi;
import android.os.Build;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Objects;

public class Card {
    private String front;
    private String back;
    private Date dueDate;
    private int intervalSum = 0;

    public Card (String front, String back, int intervalSum, Date dueDate) {
        this.front = front;
        this.back = back;
        this.dueDate = dueDate;
        this.intervalSum = intervalSum;
    }

    public Card (String front, String back, int intervalSum, String dueDate) throws ParseException {
        this.front = front;
        this.back = back;
        this.intervalSum = intervalSum;
        setDueDateString(dueDate);
    }

    public Card (String front, String back) {
        this.front = front;
        this.back = back;
        dueDate = Calendar.getInstance().getTime();
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

    public Date getDueDate() {
        return dueDate;
    }

    public void setDueDate(Date dueDate) {
        this.dueDate = dueDate;
    }

    public int getIntervalSum() {
        return intervalSum;
    }

    public void setIntervalSum(int intervalSum) {
        this.intervalSum = intervalSum;
    }

    public String getDueDateString() {
        if (dueDate == null)
            return null;
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd");
        return simpleDateFormat.format(dueDate);
    }

    public void setDueDateString(String dueDateString) throws ParseException {
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd");
        dueDate = simpleDateFormat.parse(dueDateString);
    }

    @Override
    public String toString() {
        return "Card{" +
                "front='" + front + '\'' +
                ", back='" + back + '\'' +
                ", dueDate=" + getDueDateString() +
                ", intervalSum=" + intervalSum +
                '}';
    }

    @TargetApi(Build.VERSION_CODES.KITKAT)
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Card card = (Card) o;
        return intervalSum == card.intervalSum &&
                Objects.equals(front, card.front) &&
                Objects.equals(back, card.back) &&
                Objects.equals(getDueDateString(), card.getDueDateString());
    }

    @TargetApi(Build.VERSION_CODES.KITKAT)
    @Override
    public int hashCode() {

        return Objects.hash(front, back, getDueDateString(), intervalSum);
    }
}
