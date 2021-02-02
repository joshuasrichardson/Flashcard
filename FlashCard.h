#include <string>
#include <sstream>
#include <cmath>
using std::string;

#ifndef FLASH_CARD_H
#define FLASH_CARD_H

class FlashCard
{
private:
	string front;
	string back;
	string extra;
	int streak;
	double totalCorrect;
	double totalIncorrect;
	int daysTillNextReview;
	int oldStreak;
	double oldTotalCorrect;
	double oldTotalIncorrect;
	int oldDaysTillNextReview;
	bool doneForToday;
public:
	FlashCard(string newFront, string newBack, string newExtra)
	{
		front = newFront;
		back = newBack;
		extra = newExtra;
		streak = 0;
		totalCorrect = 0;
		totalIncorrect = 0;
		daysTillNextReview = 0;
		oldStreak = 0;
		oldTotalCorrect = 0;
		oldTotalIncorrect = 0;
		oldDaysTillNextReview = 0;
		doneForToday = false;
	}
	~FlashCard() {}
	string getFront() { return front; }
	void setFront(string newFront) { front = newFront; }
	string getBack() { return back; }
	void setBack(string newBack) { back = newBack; }
	string getExtra() { return extra; }
	void setExtra(string newExtra) { extra = newExtra; }
	bool getDoneForToday() { return doneForToday; }
	void setDoneForToday(bool done) { doneForToday = done; }//mark that the card is done for that study session so it won't be chosen again at random
	void setStreak(int num) { streak = num; }
	int getStreak() { return streak; }
	void setTotalCorrect(int num) { totalCorrect = num; }
	double getTotalCorrect() { return totalCorrect; }
	void setTotalIncorrect(int num) { totalIncorrect = num; }
	double getTotalIncorrect() { return totalIncorrect; }
	void setDaysTillNextReview(int num) { daysTillNextReview = num; }
	int getDaysTillNextReview() { return daysTillNextReview; }

	void subtractDaysTillNextReview() 
	{ 
		if (daysTillNextReview > 0) --daysTillNextReview; 
		if (daysTillNextReview == 0) setDoneForToday(false);
	}

	void setFlashCard(string newFront, string newBack, string newExtra)
	{
		front = newFront;
		back = newBack;
		extra = newExtra;
	}

	void answeredCorrectly()
	{
		++totalCorrect;
		++streak;
		if (streak < 2) ++daysTillNextReview;
		else daysTillNextReview = (streak - 1) * 2.5;
	}

	void answeredIncorrectly()
	{
		savePreviousStats();
		++totalIncorrect;
		streak = 0;
		daysTillNextReview = 0;
	}

	void savePreviousStats()
	{
		oldStreak = streak;
		oldTotalCorrect = totalCorrect;
		oldTotalIncorrect = totalIncorrect;
		oldDaysTillNextReview = daysTillNextReview;
	}

	void restoreOldStats()
	{
		streak = oldStreak;
		totalCorrect = oldTotalCorrect;
		totalIncorrect = oldTotalIncorrect;
		daysTillNextReview = oldDaysTillNextReview;
		doneForToday = true;
	}

	string toString() const
	{
		std::stringstream out;
		out << "Front: " << front << std::endl;
		out << "Back: " << back << std::endl;
		out << "Extra: " << extra << std::endl;
		return out.str();
	}

	friend std::ostream& operator<<(std::ostream& os, FlashCard& flashcard)
	{
		os << flashcard.toString();
		return os;
	}

	string getStats() const
	{
		std::stringstream out;
		out << "Front: " << front << std::endl;
		out << "Back: " << back << std::endl;
		out << "Extra: " << extra << std::endl;
		out << "Days till next review: " << daysTillNextReview << std::endl;
		out << "Current Streak: " << streak << std::endl;
		out << "Total times answered correctly: " << totalCorrect << std::endl;
		out << "Total times answered incorrectly: " << totalIncorrect << std::endl;
		if (totalCorrect + totalIncorrect) out << "Percent answered correctly: " << ((totalCorrect / (totalCorrect + totalIncorrect)) * 100) << "%" << std::endl;
		return out.str();
	}
	

};

#endif // !FLASH_CARD_H