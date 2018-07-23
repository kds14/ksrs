package kds.ksrs.model;

import android.annotation.TargetApi;
import android.os.Build;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Calendar;
import java.util.Date;
import java.util.Objects;

public class Card {
    private String front;
    private String back;
    private LocalDateTime dueDate;
    private int intervalSum = 0;

    private final static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    public Card (String front, String back, int intervalSum, LocalDateTime dueDate) {
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
        dueDate = LocalDateTime.now().withSecond(0).withMinute(0).withHour(0);
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

    public LocalDateTime getDueDate() {
        return dueDate;
    }

    public void setDueDate(LocalDateTime dueDate) {
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
        return dueDate.format(dateTimeFormatter);
    }

    public void setDueDateString(String dueDateString) throws ParseException {
        dueDate = LocalDateTime.parse(dueDateString, dateTimeFormatter);
    }

    public boolean isDue() {
        LocalDateTime now = LocalDateTime.now();
        return dueDate.isBefore(now) || dueDate.isEqual(now);
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

    @Override
    public int hashCode() {

        return Objects.hash(front, back, getDueDateString(), intervalSum);
    }
}
