#include "FlashCard.h"
#include <vector>
using std::vector;

#ifndef DECK_H
#define DECK_H

class Deck
{
private:
	string deckName;
	vector<FlashCard> flashcards;
	double totalCorrect;
	double totalIncorrect;
	int cardsLeftToDoToday;
public:
	Deck(string newDeckName)
	{
		deckName = newDeckName;
		totalCorrect = 0;
		totalIncorrect = 0;
		cardsLeftToDoToday = 0;
	}

	string getName() { return deckName; }
	string getFront(int i) { return flashcards.at(i).getFront(); }
	void setFront(string oldFront, string newFront) { flashcards.at(findCard(oldFront)).setFront(newFront); }
	string getBack(int i) { return flashcards.at(i).getBack(); }
	void setBack(string oldBack, string newBack) { flashcards.at(findCard(oldBack)).setBack(newBack); }
	string getExtra(int i) { return flashcards.at(i).getExtra(); }
	void setExtra(string oldExtra, string newExtra) { flashcards.at(findCard(oldExtra)).setExtra(newExtra); }
	int size() { return flashcards.size(); }
	bool getDoneForToday(int position) { return flashcards.at(position).getDoneForToday(); } //check if a card has been done during that study session
	bool getDoneForToday(string card) { return flashcards.at(findCard(card)).getDoneForToday(); } //check if a card has been done during that study session
	void setDoneForToday(int position, bool done) { flashcards.at(position).setDoneForToday(done); } //mark that a card has been done during that study session
	int getStreak(int i) { return flashcards.at(i).getStreak(); }
	void setStreak(int i, int num) { flashcards.at(i).setStreak(num); }
	int getTotalCorrect(int i) { return flashcards.at(i).getTotalCorrect(); }
	void setTotalCorrect(int i, int num) { flashcards.at(i).setTotalCorrect(num); }
	int getTotalIncorrect(int i) { return flashcards.at(i).getTotalIncorrect(); }
	void setTotalIncorrect(int i, int num) { flashcards.at(i).setTotalIncorrect(num); }
	void startNewDay() { for (int i = 0; i < flashcards.size(); ++i) flashcards.at(i).subtractDaysTillNextReview(); }//Subtracts a day from each card so that they can be studied sooner
	int getDaysTillNextReview(int i) { return flashcards.at(i).getDaysTillNextReview(); }
	void setDaysTillNextReview(int i, int num) { flashcards.at(i).setDaysTillNextReview(num); }
	void clearDeck() { flashcards.clear(); }
	void answeredCorrectly(int i) { flashcards.at(i).answeredCorrectly(); }
	void answeredIncorrectly(int i) { flashcards.at(i).answeredIncorrectly(); }
	void closeEnough(int i) { flashcards.at(i).restoreOldStats(); }//When they mistype something, but it's close enough, they can use this to restore their stats

	int getCardsLeftToDoToday()
	{
		cardsLeftToDoToday = flashcards.size();
		for (int i = 0; i < flashcards.size(); ++i)
		{
			if (flashcards.at(i).getDoneForToday()) --cardsLeftToDoToday;
		}
		return cardsLeftToDoToday;
	}

	void addFlashCard(string front, string back, string extra)
	{
		FlashCard flashCard = FlashCard(front, back, extra);
		flashcards.push_back(flashCard);
	}

	void addFlashCard(FlashCard& flashCard)
	{
		flashcards.push_back(flashCard);
	}

	int findCard(string front)
	{
		for (int i = 0; i < flashcards.size(); ++i)
		{
			if (flashcards.at(i).getFront() == front) return i;
		}
		return -1;
	}

	bool removeFlashCard(string flashCardToRemove)
	{
		if (findCard(flashCardToRemove) >= 0)
		{
			flashcards.erase(flashcards.begin() + findCard(flashCardToRemove));
			return true;
		}
		else return false;
	}

	string toString() const
	{
		std::stringstream out;
		out << std::endl;
		out << deckName << " Deck:" << std::endl << std::endl;
		for (int i = 0; i < flashcards.size(); ++i)	out << flashcards.at(i).toString() << std::endl;
		return out.str();
	}

	string displayStats()
	{
		double totalCorrectInDeck = 0;
		double totalIncorrectInDeck = 0;
		std::stringstream out;
		out << std::endl;
		out << deckName << " Deck:" << std::endl << std::endl;
		for (int i = 0; i < flashcards.size(); ++i)
		{
			out << flashcards.at(i).getStats() << std::endl;
			totalCorrectInDeck += flashcards.at(i).getTotalCorrect();
			totalIncorrectInDeck += flashcards.at(i).getTotalIncorrect();
		}
		if (totalCorrectInDeck + totalIncorrectInDeck) out <<"Percent of " << deckName << " cards answered correctly: " << (totalCorrectInDeck / (totalCorrectInDeck + totalIncorrectInDeck)) * 100  << "%" << std::endl;
		return out.str();
	}

	friend std::ostream& operator<<(std::ostream& os, Deck& deck)
	{
		os << deck.toString();
		return os;
	}
};

#endif // !DECK_H