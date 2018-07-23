package kds.ksrs.m;

import java.text.ParseException;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Objects;

public class Card {
    private String front;
    private String back;
    private LocalDate dueDate;
    private int intervalSum = 0;

    private final static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    public Card (final String front, final String back, final int intervalSum, final LocalDate dueDate) {
        this.front = front;
        this.back = back;
        this.dueDate = dueDate;
        this.intervalSum = intervalSum;
    }

    public Card (final String front, final String back, final int intervalSum, final String dueDate) throws ParseException {
        this.front = front;
        this.back = back;
        this.intervalSum = intervalSum;
        setDueDateString(dueDate);
    }

    public Card (final String front, final String back) {
        this.front = front;
        this.back = back;
        dueDate = LocalDate.now();
    }

    public String getFront() {
        return front;
    }

    public void setFront(final String value) {
        front = value;
    }

    public String getBack() {
        return back;
    }

    public void setBack(final String value) {
        back = value;
    }

    public LocalDate getDueDate() {
        return dueDate;
    }

    public void setDueDate(final LocalDate dueDate) {
        this.dueDate = dueDate;
    }

    public int getIntervalSum() {
        return intervalSum;
    }

    public void setIntervalSum(final int intervalSum) {
        this.intervalSum = intervalSum;
    }

    public String getDueDateString() {
        if (dueDate == null)
            return null;
        return dueDate.format(dateTimeFormatter);
    }

    public void setDueDateString(final String dueDateString) throws ParseException {
        dueDate = LocalDate.parse(dueDateString, dateTimeFormatter);
    }

    public boolean isDue() {
        final LocalDate now = LocalDate.now();
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
    public boolean equals(final Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        final Card card = (Card) o;
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
