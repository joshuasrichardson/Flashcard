#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include "FlashCard.h"
#include "Deck.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif
const int INVALID = 9999;

using namespace std;

int chooseDeck(string deckName, int numDecks, ifstream& in, ofstream& out, vector<Deck>& deckVector)
{
	int deckPlace = 0;
	for (int i = 0; i < numDecks; ++i)
	{
		out << deckVector[i].getName() << endl;
	}
	getline(in, deckName);
	for (int i = 0; i < numDecks; ++i)
	{
		if (deckVector[i].getName() == deckName)
		{
			deckPlace = i;
			break;
		}
		else if (i == numDecks - 1)
		{
			out << "No existing deck matches that name." << endl;
			out << endl;
			deckPlace = INVALID;
		}
	}
	return deckPlace;
}

int chooseDeck(string deckName, int numDecks, istream& cin, ostream& cout, vector<Deck>& deckVector)
{
	int deckPlace = 0;
	for (int i = 0; i < numDecks; ++i)
	{
		cout << deckVector[i].getName() << "\t(Total Cards: " << deckVector[i].size() << ", To Do Today: " << deckVector[i].getCardsLeftToDoToday() <<  ")" << endl;
	}
	getline(cin, deckName);
	for (int i = 0; i < numDecks; ++i)
	{
		if (deckVector[i].getName() == deckName || deckVector[i].getName().at(0) == deckName.at(0) && deckName.at(1) == '\t')
		{
			deckPlace = i;
			break;
		}
		else if (i == numDecks - 1)
		{
			cout << "No existing deck matches that name." << endl;
			cout << endl;
			deckPlace = INVALID;
		}
	}
	return deckPlace;
}

bool checkForDeck(string deckName, vector<Deck>& deckVector, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (deckVector.at(i).getName() == deckName) return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	// enable memory leak check
	VS_MEM_CHECK

	//check and open up input and output files
	if (argc < 3)
	{
		cerr << "Please provide name of input and output files";
		return 1;
	}
	//cout << "Input file: " << argv[1] << endl;
	ifstream in(argv[1]);
	if (!in)
	{
		cerr << "Unable to open " << argv[1] << " for input";
		return 2;
	}
	//cout << "Output file: " << argv[2] << endl;
	ofstream out(argv[2]);
	if (!out)
	{
		in.close();
		cerr << "Unable to open " << argv[2] << " for output";
		return 3;
	}

	out << "\t Welcome to Joshua Richardson's Studying Program!" << endl;
	out << "Please select what you would like to do by typing in one of the numbers on the left." << endl;
	out << "0. Start new day." << endl;
	out << "1. Create a new deck" << endl;
	out << "2. Add a card to a deck" << endl;
	out << "3. Remove a deck" << endl;
	out << "4. Remove a card from a deck" << endl;
	out << "5. Study all decks" << endl;
	out << "6. Study a deck" << endl;
	out << "7. Check all stats" << endl;
	out << "8. Check deck stats" << endl;
	out << "9. Display Deck" << endl;
	out << "10. Duh" << endl;
	out << "11. Save" << endl;
	out << "12. Set stats" << endl;
	out << "13.	Set day" << endl;

	// process input strings from input file
	string line, deckName, front, back, extra;
	int command;
	FlashCard flashCard("Initializion", "Initializion", "Initializion");
	Deck deck("Initializion");
	vector<Deck> deckVector;
	int numDecks = 0;
	int deckInteger = 0;
	int day = 1;
	string edit;
	while (getline(in, line))
	{
		try
		{
			if (line.size() == 0) continue;
			istringstream iss(line);
			iss >> command;
			if (command == 0)
			{
				out << endl << "\t*Start a new day*" << endl;
				for (int i = 0; i < deckVector.size(); ++i)
				{
					deckVector.at(i).startNewDay();
					for (int j = 0; j < deckVector.at(i).size(); ++j)
					{
						if (deckVector.at(i).getDaysTillNextReview(j) == 0) deckVector.at(i).setDoneForToday(j, false);
					}
				}
				++day;
				out << "Starting day " << day << endl;
				out << "All of your cards have become one day closer to being ready to be studied. Gratz!" << endl;
				out << endl;
			}
			else if (command == 1)
			{
				out << endl << "\t*Create a new deck*" << endl;
				out << "Enter the name of the new deck." << endl;
				getline(in, deckName);
				deck = Deck(deckName);
				deckVector.push_back(deck);
				++numDecks;
				out << "Successfully created a new deck called " << deckVector.back().getName() << "." << endl;
				out << endl;
			}
			else if (command == 2)
			{
				out << endl << "\t*Add a card to a deck*" << endl;
				out << "Which deck would you like to add the card to?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
				if (deckInteger == INVALID) break;
				deckName = deckVector.at(deckInteger).getName();
				out << "Adding card to the " << deckName << " deck." << endl;
				out << "Enter the front side of the card:" << endl;
				getline(in, front);
				out << "Enter the back side of the card:" << endl;
				getline(in, back);
				out << "Enter any extra information to know about this card:" << endl;
				getline(in, extra);
				flashCard.setFlashCard(front, back, extra);
				deckVector.at(deckInteger).addFlashCard(flashCard);
				out << endl;
				out << "You created this flashcard in the " << deckVector.at(deckInteger).getName() << " deck:" << endl;
				out << flashCard << endl;
			}
			else if (command == 3)
			{
				out << endl << "\t*Remove a deck*" << endl;
				out << "Which deck would you like to remove?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
				if (deckInteger == INVALID) break;
				deckName = deckVector.at(deckInteger).getName();
				if (deckVector.at(deckInteger).size() != 0) deckVector.at(deckInteger).clearDeck();
				deckVector.erase(deckVector.begin() + deckInteger);
				--numDecks;
				out << "Deleted the " << deckName << " deck." << endl;
				out << endl;
			}
			else if (command == 4)
			{
				out << endl << "\t*Remove a card from a deck*" << endl;
				out << "Which deck would you like to remove a card from?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
				if (deckInteger == INVALID) break;
				out << "Which card would you like to remove from " << deckVector.at(deckInteger).getName() << "?" << endl;
				out << "(Enter the Front of the card.)" << endl;
				out << deckVector.at(deckInteger) << endl;
				getline(in, front);
				deckVector.at(deckInteger).removeFlashCard(front);
				out << "Removed the " << front << " flashcard." << endl;
				out << endl;
			}
			else if (command == 5)
			{
				int randomDeckNumber = 0;
				int randomCardNumber = 0;
				out << endl << "\t*Study all decks*" << endl;

				out << endl;
			}
			else if (command == 6)
			{
				string answer;
				int randomCardNumber = 0;
				out << endl << "\t*Study a Deck*" << endl;
				out << "Which deck would you like to study?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
				if (deckInteger == INVALID) break;
				int cardsToDo = deckVector.at(deckInteger).getCardsLeftToDoToday();
				for (int i = 0; i < cardsToDo; ++i)//go through each card once
				{
					srand(time(NULL));
					randomCardNumber = rand() % deckVector.at(deckInteger).size(); //start with a random card
					while (deckVector.at(deckInteger).getDoneForToday(randomCardNumber)) randomCardNumber = rand() % deckVector.at(deckInteger).size();//make sure the card hasn't been done yet
					out << deckVector.at(deckInteger).getFront(randomCardNumber) << endl;
					getline(in, answer);
					out << deckVector.at(deckInteger).getBack(randomCardNumber) << endl;
					if (deckVector.at(deckInteger).getExtra(randomCardNumber) != "") out << "(" << deckVector.at(deckInteger).getExtra(randomCardNumber) << ")" << endl;
					if (answer == deckVector.at(deckInteger).getBack(randomCardNumber))
					{
						out << "Correct!" << endl;
						deckVector.at(deckInteger).answeredCorrectly(randomCardNumber);//add to the correct stat
						deckVector.at(deckInteger).setDoneForToday(randomCardNumber, true);//make sure the card won't be chosen again durng that study session
					}
					else if (answer == "Quit") break;
					else
					{
						out << "You're wrong." << endl;
						deckVector.at(deckInteger).answeredIncorrectly(randomCardNumber);//add to the incorrect stat
						--i;
					}
					out << endl;
				}
				out << endl;
			}
			else if (command == 7)
			{
				out << endl << "\t*Check all stats*" << endl;
				out << endl;
			}
			else if (command == 8)
			{
				out << endl << "\t*Check deck stats*" << endl;
				out << "Which deck would you like to check?" << endl;
				out << endl;
			}
			else if (command == 9)
			{
				out << endl << "\t*Display Deck*" << endl;
				out << "Which deck would you like to view?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
				if (deckInteger == INVALID) break;
				out << deckVector.at(deckInteger) << endl;
				out << endl;
			}
			else if (command == 10)
			{
				out << endl << "/tDuh" << endl;
			}
			else if (command == 11)
			{
				out << endl << "\t---------------*Saving Progress*---------------" << endl;
				for (int i = 0; i < deckVector.size(); ++i)
				{
					out << "1" << endl;
					out << deckVector.at(i).getName() << endl;
					for (int j = 0; j < deckVector.at(i).size() ; ++j)
					{
						out << "2" << endl;//add card to deck
						out << deckVector.at(i).getName() << endl;
						out << deckVector.at(i).getFront(j) << endl;
						out << deckVector.at(i).getBack(j) << endl;
						out << deckVector.at(i).getExtra(j) << endl;
						out << "12" << endl;//update stats
						out << deckVector.at(i).getName() << endl;
						out << j << endl;
						out << deckVector.at(i).getDoneForToday(j) << endl;
						out << deckVector.at(i).getStreak(j) << endl;
						out << deckVector.at(i).getTotalCorrect(j) << endl;
						out << deckVector.at(i).getTotalIncorrect(j) << endl;
						out << deckVector.at(i).getDaysTillNextReview(j) << endl;
					}
				}
				out << "13" << endl;//set days
				out << day << endl;
				out << "\t---------------*Progress saved*---------------" << endl;
			}

			else if (command == 12)
			{
			int cardToBeUpdated = 0;
			int num = 0;
			out << endl << "\t*Updating stats*" << endl;
			deckInteger = chooseDeck(deckName, numDecks, in, out, deckVector);
			if (deckInteger == INVALID) break;
			in >> cardToBeUpdated;
			in >> num;
			deckVector.at(deckInteger).setDoneForToday(cardToBeUpdated, num);
			in >> num;
			deckVector.at(deckInteger).setStreak(cardToBeUpdated, num);
			in >> num;
			deckVector.at(deckInteger).setTotalCorrect(cardToBeUpdated, num);
			in >> num;
			deckVector.at(deckInteger).setTotalIncorrect(cardToBeUpdated, num);
			in >> num;
			deckVector.at(deckInteger).setDaysTillNextReview(cardToBeUpdated, num);
			out << "\t*Stats updated*" << endl;
			}
			else if (command == 13)
			{
				in >> day;
			}
			else out << "Invalid Selection. Try again" << endl << endl;
		}
		catch (std::exception& e) { out << e.what() << endl; }
	}

	int cardsToStudy = 0;
	for (int i = 0; i < deckVector.size(); ++i)
	{
		cardsToStudy += deckVector.at(i).getCardsLeftToDoToday();
	}
	cout << "\t Welcome to Joshua Richardson's Studying Program!" << endl;
	cout << "Day " << day << ". " << cardsToStudy << " cards to study." << endl;
	cout << "Please select what you would like to do by typing in one of the numbers on the left." << endl;
	cout << "0. Start a New Day." << endl;
	cout << "1. Create a New Deck" << endl;
	cout << "2. Add a Card to a Deck" << endl;
	cout << "3. Edit a Card" << endl;
	cout << "4. Remove a Deck" << endl;
	cout << "5. Remove a Card From a Deck" << endl;
	cout << "6. Study All Decks" << endl;
	cout << "7. Study a Deck" << endl;
	cout << "8. Check All Stats" << endl;
	cout << "9. Check a Deck's Stats" << endl;
	cout << "10. Duh" << endl;
	cout << "11. Display All Decks" << endl;
	cout << "12. Display a Deck" << endl;
	cout << "13. Save" << endl;
	cout << "14. Quit" << endl;

	// process input strings from keyboard
	while (getline(cin, line))
	{
		try
		{
			if (line.size() == 0) continue;
			if (isdigit(line.at(0)))
			{
				istringstream iss(line);
				iss >> command;
			}
			else command = INVALID;
			if (command == 0)
			{
				cout << endl << "\t*Start a new day*" << endl;
				cardsToStudy = 0;
				for (int i = 0; i < deckVector.size(); ++i)
				{
					deckVector.at(i).startNewDay();
					for (int j = 0; j < deckVector.at(i).size(); ++j)
					{
						if (deckVector.at(i).getDaysTillNextReview(j) == 0) deckVector.at(i).setDoneForToday(j, false);
					}
					cardsToStudy += deckVector.at(i).getCardsLeftToDoToday();
				}
				++day;
				cout << "Starting day " << day << endl;
				cout << "All of your cards have become one day closer to being ready to be studied. Gratz!" << endl;
				cout << endl;
			}
			else if (command == 1)
			{
				cout << endl << "\t*Create a new deck*" << endl;
				cout << "Enter the name of the new deck:" << endl;
				getline(cin, deckName);
				if (checkForDeck(deckName, deckVector, deckVector.size()))
				{
					cout << "A deck with that name has already been created." << endl << endl;
					goto options;//The name they typed in was already the name of an existing deck, so they just go back to the main menu
				}
				deck = Deck(deckName);
				deckVector.push_back(deck);
				++numDecks;
				cout << "Successfully created a new deck called " << deckName << "." << endl;
				cout << endl;
			}
			else if (command == 2)
			{
				cout << endl << "\t*Add a card to a deck*" << endl;
				cout << "Which deck would you like to add the card to?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				cout << "Adding card to the " << deckVector.at(deckInteger).getName() << " deck." << endl;
				enterFront:
				cout << "Enter the front side of the card:" << endl;
				getline(cin, front);
				if (deckVector.at(deckInteger).findCard(front) >= 0)
				{
					cout << "A card with this front already exists. Try making a new card instead." << endl << endl;
					goto enterFront;
				}
				cout << "Enter the back side of the card:" << endl;
				getline(cin, back);
				cout << "Enter any extra information to know about this card:" << endl;
				getline(cin, extra);
				flashCard.setFlashCard(front, back, extra);
				deckVector.at(deckInteger).addFlashCard(flashCard);
				cout << endl;
				cout << "You created this flashcard in the " << deckVector.at(deckInteger).getName() << " deck:" << endl;
				++cardsToStudy;
				cout << flashCard << endl;
			}
			else if (command == 3)
			{
				cout << endl << "\t*Edit a card*" << endl;
				cout << "Which deck contains the card that you would like to edit?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				deckName = deckVector.at(deckInteger).getName();
				cout << endl << "Which card would you like to edit?" << endl;
				cout << "(Enter the Front of the card.)" << endl;
				cout << deckVector.at(deckInteger) << endl;
				getline(cin, front);
				if (deckVector.at(deckInteger).findCard(front) < 0)
				{
					cout << "That card does not exist in the " << deckVector.at(deckInteger).getName() << " deck." << endl;
					cout << endl;
					goto options;//The name they typed in wasn't the name of an existing card, so they just go back to the main menu
				}
				editFront:
				cout << "Would you like to edit the front?" << endl;
				cout << "(Type \"yes\" or \"no\")" << endl;
				cin >> edit;
				if (edit == "yes")
				{
					cout << "Enter the new Front:" << endl;
					cin.ignore();
					getline(cin, edit);
					if (deckVector.at(deckInteger).findCard(edit) >= 0)
					{
						cout << "A card with this front already exists. Try keeping it original." << endl << endl;
						goto editFront;
					}
					deckVector.at(deckInteger).setFront(front, edit);
					front = edit;
					cout << "Done!" << endl;
				}
				cout << "Would you like to edit the back?" << endl;
				cout << "(Type \"yes\" or \"no\")" << endl;
				cin >> edit;
				if (edit == "yes")
				{
					cout << "Enter the new Back:" << endl;
					cin.ignore();
					getline(cin, edit);
					deckVector.at(deckInteger).setBack(front, edit);
					cout << "Done!" << endl;
				}
				cout << "Would you like to edit the extra information?" << endl;
				cout << "(Type \"yes\" or \"no\")" << endl;
				cin >> edit;
				if (edit == "yes")
				{
					cout << "Enter the new Extra:" << endl;
					cin.ignore();
					getline(cin, edit);
					deckVector.at(deckInteger).setExtra(front, edit);
					cout << "Done!" << endl;
				}
				cout << endl;
			}
			else if (command == 4)
			{
				cout << endl << "\t*Remove a deck*" << endl;
				cout << "Which deck would you like to remove?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				deckName = deckVector.at(deckInteger).getName();
				cardsToStudy -= deckVector.at(deckInteger).size();
				deckVector.erase(deckVector.begin() + deckInteger);
				--numDecks;
				cout << "Deleted the " << deckName << " deck." << endl;
				cout << endl;
			}
			else if (command == 5)
			{
				cout << endl << "\t*Remove a card from a deck*" << endl;
				cout << "Which deck would you like to remove a card from?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				cout << "Which card would you like to remove from " << deckVector.at(deckInteger).getName() << "?" << endl;
				cout << "(Enter the Front of the card.)" << endl;
				cout << deckVector.at(deckInteger) << endl;
				getline(cin, front);
				if (deckVector.at(deckInteger).findCard(front) < 0)
				{
					cout << "That card does not exist in the " << deckVector.at(deckInteger).getName() << " deck." << endl;
					cout << endl;
					goto options;
				}
				if (!deckVector.at(deckInteger).getDoneForToday(front)) --cardsToStudy;
				if (deckVector.at(deckInteger).removeFlashCard(front)) cout << "Removed the " << front << " flashcard." << endl;
				cout << endl;
			}
			else if (command == 6)
			{
				string answer;
				int randomCardNumber = 0;
				int cardsToDo = cardsToStudy;//We need this variable so that the condition in the for loop stays the same and we can go through it the right number of times.
				cout << endl << "\t*Study all decks*" << endl;
				for (int i = 0; i < cardsToDo; ++i)
				{
					deckInteger = rand() % deckVector.size();//start with a random deck
					while (deckVector.at(deckInteger).getCardsLeftToDoToday() < 1) deckInteger = rand() % deckVector.size();//make sure the deck hasn't been done yet
					randomCardNumber = rand() % deckVector.at(deckInteger).size(); //start with a random card
					while (deckVector.at(deckInteger).getDoneForToday(randomCardNumber)) randomCardNumber = rand() % deckVector.at(deckInteger).size();//make sure the card hasn't been done yet
					cout << deckVector.at(deckInteger).getFront(randomCardNumber) << "\t (" << deckVector.at(deckInteger).getName() << ")" << endl;
					answerAllDecks:
					getline(cin, answer);
					cout << deckVector.at(deckInteger).getBack(randomCardNumber) << endl;
					if (deckVector.at(deckInteger).getExtra(randomCardNumber) != "") cout << "(" << deckVector.at(deckInteger).getExtra(randomCardNumber) << ")" << endl;
					if (answer == deckVector.at(deckInteger).getBack(randomCardNumber))
					{
						cout << "Correct!" << endl;
						deckVector.at(deckInteger).answeredCorrectly(randomCardNumber);//add to the correct stat
						deckVector.at(deckInteger).setDoneForToday(randomCardNumber, true);//make sure the card won't be chosen again durng that study session
						--cardsToStudy;
					}
					else if (answer == "Quit") break;
					else if (answer == "Close enough")
					{
						if (deckVector.at(deckInteger).getStreak(randomCardNumber) == 0)
						{
							cout << "Sounds good. Your stats will be restored to how they were before the card was marked wrong, and then it will be marked off as studied and answered correctly for today." << endl;
							deckVector.at(deckInteger).closeEnough(randomCardNumber);
							deckVector.at(deckInteger).answeredCorrectly(randomCardNumber);//add to the correct stat
							--cardsToStudy;
						}
						else
						{
							cout << "You didn't miss that one. Try answering it." << endl;
							goto answerAllDecks;
						}
					}
					else
					{
						cout << "You're wrong." << endl;
						deckVector.at(deckInteger).answeredIncorrectly(randomCardNumber);//add to the incorrect stat
						--i;
					}
					cout << endl;
				}
				cout << endl;
			}
			else if (command == 7)
			{
				string answer;
				int randomCardNumber = 0;
				cout << endl << "\t*Study a Deck*" << endl;
				cout << "Which deck would you like to study?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				int cardsToDo = deckVector.at(deckInteger).getCardsLeftToDoToday();//We need this variable so that the condition in the for loop stays the same and we can go through it the right number of times.
				for (int i = 0; i < cardsToDo; ++i)//go through each card once
				{
					randomCardNumber = rand() % deckVector.at(deckInteger).size(); //start with a random card
					while (deckVector.at(deckInteger).getDoneForToday(randomCardNumber)) randomCardNumber = rand() % deckVector.at(deckInteger).size();//make sure the card hasn't been done yet
					cout << deckVector.at(deckInteger).getFront(randomCardNumber) << endl;
					answer:
					getline(cin, answer);
					cout << deckVector.at(deckInteger).getBack(randomCardNumber) << endl;
					if (deckVector.at(deckInteger).getExtra(randomCardNumber) != "") cout << "(" << deckVector.at(deckInteger).getExtra(randomCardNumber) << ")" << endl;
					if (answer == deckVector.at(deckInteger).getBack(randomCardNumber))
					{
						cout << "Correct!" << endl;
						deckVector.at(deckInteger).answeredCorrectly(randomCardNumber);//add to the correct stat
						deckVector.at(deckInteger).setDoneForToday(randomCardNumber, true);//make sure the card won't be chosen again durng that study session
						--cardsToStudy;
					}
					else if (answer == "Quit") break;
					else if (answer == "Close enough")
					{
						if (deckVector.at(deckInteger).getStreak(randomCardNumber) == 0)
						{
							cout << "Sounds good. Your stats will be restored to how they were before the card was marked wrong, and then it will be marked off as studied and answered correctly for today." << endl;
							deckVector.at(deckInteger).closeEnough(randomCardNumber);
							deckVector.at(deckInteger).answeredCorrectly(randomCardNumber);//add to the correct stat
							--cardsToStudy;
						}
						else
						{
							cout << "You didn't miss that one. Try answering it." << endl;
							goto answer;
						}
					}
					else
					{
						cout << "You're wrong." << endl;
						deckVector.at(deckInteger).answeredIncorrectly(randomCardNumber);//add to the incorrect stat
						--i;
					}
					cout << endl;
				}
				cout << endl;
			}
			else if (command == 8)
			{
				cout << endl << "\t*Check all stats*" << endl;
				for (int i = 0; i < deckVector.size(); ++i) cout << deckVector.at(i).displayStats() << endl;
				cout << endl;
				cout << endl;
			}
			else if (command == 9)
			{
				cout << endl << "\t*Check deck stats*" << endl;
				cout << "Which deck would you like to check?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				cout << deckVector.at(deckInteger).displayStats() << endl;
				cout << endl;
				cout << endl;
			}
			else if (command == 10)
			{
			cout << endl << "\t*Duh*" << endl;
			cout << endl;
			}
			else if (command == 11)
			{
			cout << endl << "\t*Display All Decks*" << endl;
			for (int i = 0; i < deckVector.size(); ++i) cout << deckVector.at(i) << endl;
			cout << endl;
			}
			else if (command == 12)
			{
				cout << endl << "\t*Display Deck*" << endl;
				cout << "Which deck would you like to see?" << endl;
				deckInteger = chooseDeck(deckName, numDecks, cin, cout, deckVector);
				if (deckInteger == INVALID) goto options;//The name they typed in wasn't the name of an existing deck, so they just go back to the main menu
				cout << deckVector.at(deckInteger) << endl;
				cout << endl;
			}
			else if (command == 13)
			{
				out << endl << "\t---------------*Saving Progress*---------------" << endl;
				for (int i = 0; i < deckVector.size(); ++i)
				{
					out << "1" << endl;
					out << deckVector.at(i).getName() << endl;
					for (int j = 0; j < deckVector.at(i).size(); ++j)
					{
						out << "2" << endl;//add card to deck
						out << deckVector.at(i).getName() << endl;
						out << deckVector.at(i).getFront(j) << endl;
						out << deckVector.at(i).getBack(j) << endl;
						out << deckVector.at(i).getExtra(j) << endl;
						out << "12" << endl;//update stats
						out << deckVector.at(i).getName() << endl;
						out << j << endl;
						out << deckVector.at(i).getDoneForToday(j) << endl;
						out << deckVector.at(i).getStreak(j) << endl;
						out << deckVector.at(i).getTotalCorrect(j) << endl;
						out << deckVector.at(i).getTotalIncorrect(j) << endl;
						out << deckVector.at(i).getDaysTillNextReview(j) << endl;
					}
				}
				out << "13" << endl;//set day
				out << day << endl;
				out << "\t---------------*Progress saved*---------------" << endl;
				cout << endl << "\t*Progress saved*" << endl << endl;
			}
			else if (command == 14)
			{
				cout << endl << "\tNice job today, Feller!" << endl;
				break;
			}
			else cout << "Invalid Selection. Try again" << endl << endl; 
			options:
			cout << "Day " << day << ". " << cardsToStudy << " cards to study." << endl;
			cout << "Please select what you would like to do by typing in one of the numbers on the left." << endl;
			cout << "0. Start a New Day." << endl;
			cout << "1. Create a New Deck" << endl;
			cout << "2. Add a Card to a Deck" << endl;
			cout << "3. Edit a Card" << endl;
			cout << "4. Remove a Deck" << endl;
			cout << "5. Remove a Card From a Deck" << endl;
			cout << "6. Study All Decks" << endl;
			cout << "7. Study a Deck" << endl;
			cout << "8. Check All Stats" << endl;
			cout << "9. Check a Deck's Stats" << endl;
			cout << "10. Duh" << endl;
			cout << "11. Display All Decks" << endl;
			cout << "12. Display a Deck" << endl;
			cout << "13. Save" << endl;
			cout << "14. Quit" << endl;
		}
		catch (std::exception& e) { cout << e.what() << endl; }
	}

	return 0;
}