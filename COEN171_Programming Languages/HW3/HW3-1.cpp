#include <iostream>
#include <string>
#include <limits>

using namespace std;


class store;
int findPass(string pass);

class book {
	friend class store;
private:
	string name;
	string author;
	int isbn;
	double price;
	int stock;
public:
	book();
	book(string n, string a, int is, double p, int s) {
		name = n;
		author = a;
		isbn = is;
		price = p;
		stock = s;
	}
	string getName() {return name;}
	string getAuthor() {return author;}
	int getISBN() { return isbn;}
	double getPrice() { return price;}
	int getStock() { return stock;}
};

class person {
protected:
	string name;
	string address;
	string credit_card;
public:
	string getName() {return name;}
	string getAddress() {return address;}
	string getCredit() {return credit_card;}
};

class client : public person {
protected:
	string password;
	friend void updateClientList(client *c);
public:
	string getPassword() { return password;}
	void changePassword() {
		cout << "Please input a password:";
		string pass;
		cin >> pass;
		while(findPass(pass) != -1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Someone already has that password. Please input a new one:";
			cin >> pass;
		}
		password = pass;
		cout << "Password changed. Your file won't be in the database until you press 'q' to step back." << "\n";
	}
	void changeName() {
		cout << "What do you want your new name to be:";
		string newName;
		cin >> newName;
		name = newName;
		cout << "Your new name is:" << name << "\n";
		if(!password.length()) {
			cout << "You don't have a password.";
			changePassword();
		}
	}
	void changeAddress() {
		cout << "What do you want your new address to be:";
		string newAddress;
		cin >> newAddress;
		address = newAddress;
		cout << "Your new address is:" << address << "\n";
		if(!password.length()) {
			cout << "You don't have a password.";
			changePassword();
		}
	}
	void changeCredit() {
		cout << "Input your new credit card:";
		string newCredit;
		cin >> newCredit;
		credit_card = newCredit;
		cout << "Your new credit card is:" << credit_card << "\n";
		if(!password.length()) {
			cout << "You don't have a password.";
			changePassword();
		}
	}
	
	void purchaseBook(store *S);
};

class store {
	private:
		void updatePrice(int state,double p) {
			inventory[state]->price = p;
		}
		void updateInventory(int state, int s) {
			inventory[state]->stock = s;
		}
		book *inventory[8];
		friend void client::purchaseBook(store *s);
	public:
		store() {
			
			//Stock inventory
			inventory[0] = new book("Lord of the Rings", "Tolkien", 1212, 30.12, 10);
			inventory[1] = new book("The Hobbit", "Tolkien", 1314, 12.78, 20);
			inventory[2] = new book("Harry Potter and the Sorceror's Stone", "Rowling", 1108, 19.19, 11);
			inventory[3] = new book("Harry Potter and the Chamber of Secrets", "Rowling", 1013, 11.90, 3);
			inventory[4] = new book("Harry Potter and the Order of Phoneix", "Rowling", 1212, 9.99, 2);
			inventory[5] = new book("War and Peace", "Tolstoi",1011,7.77, 25);
			inventory[6] = new book("Anne Karenina", "Tolstoi", 901, 2.23, 12);
			inventory[7] = new book("The Adventures of Huckleberry Finn", "Twain", 999, 123.45, 75);
		}
		
		~store() {
			for(int i = 0 ; i < 8; i++) {
				delete inventory[i];
			}
		}
		int accessBook() {
			for(int i  = 0; i < 8; i ++)
				cout << "[" << i << "]" << inventory[i]->getName() << "\n";
			cout << "Which book's information do you want to look at? :";
			int state;
			while(!(cin>>state) || state < 0 || state > 7) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please input an appropriate catalog number:";
				cin >> state;
			}
			return state;
		}
		friend class owner;
};


void client::purchaseBook(store *S) {
	if(!getName().length() || !getAddress().length() || !getCredit().length()) {
		cout << "We need you to fill out some information:\n";
		if(!getName().length())
			changeName();
		if(!getAddress().length())
			changeAddress();
		if(!getCredit().length())
			changeCredit();
	}
	int state = S->accessBook();
	int newStock = S->inventory[state]->getStock();
	S->updateInventory(state, newStock-1);
	cout << "Purchase successful!" << "\n";
}


class owner : public client {
private:
	store *myStore;
public:
	owner();
	owner(store *S) {
		myStore = S;
		password = "8888888";
	}
	
	void printInventory() {
		int state = myStore->accessBook();
		cout << "NAME:" << myStore->inventory[state]->getName();
		cout << " AUTHOR:" << myStore->inventory[state]->getAuthor();
		cout << " ISBN:" << myStore->inventory[state]->getISBN();
		cout << " PRICE($):" << myStore->inventory[state]->getPrice();
		cout << " STOCK:" << myStore->inventory[state]->getStock() << "\n";
		
	}
	
	void changePrice() {
		int state = myStore->accessBook();
		cout << "What do you want to change it's price to? :";
		double newPrice;
		cin >> newPrice;
		if(!cin) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "You inputted an invalid number.";
		}
		myStore->updatePrice(state,newPrice);
	}
};

client* clientList[255];
string password[255];
int length=0;

void list(bool owner) {
	cout << "[0] To change your name.\n";
	cout << "[1] To change your credit card.\n";
	cout << "[2] To change your address.\n";
	cout << "[3] To purchase a book.\n";
	if(owner) {
		cout << "[4] To print inventory about books.\n";
		cout << "[5] To change the price of a book.\n";
	}
	else cout << "[6] To login.\n";
	cout << "[7] All information.\n";
	cout << "[q] To quit.\n";
}

bool findClient(client *c) {
	for(int i = 0 ; i < length;i++) {
		if(clientList[i] == c)
			return true;
	}
	return false;
}

int findPass(string pass) {
	for(int i = 0 ; i < length;i++) {
		if(password[i].compare(pass)==0)
			return i;
	}
	return -1;
}

void updateClientList(client *c){
	if(!findClient(c) && c->password.length() != 0) {
		clientList[length] = c; //cL[0] = c
		password[length++] = c->password; //password[0] = pass
		cout << "Client List Updated. \n";
	}
	
}

int main() {
	store *s = new store();
	owner *o = new owner(s);
	client *buyer;
	for(int INF = 0; INF < 1; INF++) {
		cout << "Hi, welcome to the bookstore!Are you a client or an owner?(c = client, o = owner, x = exit program):";
		char c;
		cin >> c;
		char cmd;
		if(c == 'o') {
			cout << "Please input your password:";
			string pass;
			cin >> pass;
			if(pass.compare("8888888") == 0) {
				cout << "Welcome Owner!\n";
				for(int i = 0; i < 1; i++) {
					cout << "Enter a command(l for list of command):";
					cin >> cmd;
					switch(cmd) {
						case 'l':
							list(true);
							break;
						case '0':
							o->changeName();
							break;
						case '1':
							o->changeCredit();
							break;
						case '2':
							o->changeAddress();
							break;
						case '3':
							o->purchaseBook(s);
							break;
						case '4':
							o->printInventory();
							break;
						case '5':
							o->changePrice();
							break;
						case '7':
							cout << "NAME: " << o->getName() << "ADDRESS: " << o->getAddress() << "CREDIT CARD:" << o->getCredit() << "\n";
							break;
						case 'q':
							i=3;
							break;
					}
					i--;
				}
			} else {
				cout << "Incorrect Password.\n";
				INF--;
			}
		}
		else if(c == 'c') {
			buyer = new client();
			for(int i = 0; i < 1; i++) { 
				cout << "Enter a command(l for list of command):";
				cin >> cmd;
				switch(cmd) {
					case 'l':
						list(false);
						break;
					case '0':
						buyer->changeName();
						break;
					case '1':
						buyer->changeCredit();
						break;
					case '2':
						buyer->changeAddress();
						break;
					case '3':
						buyer->purchaseBook(s);
						break;
					case '6':
					{
						cout << "Type in your password:";
						string clientPass;
						cin >> clientPass;
						
						int idx = findPass(clientPass);
						if(idx != -1) {
							cout << "Login successful!";
							delete buyer;
							buyer = clientList[idx];
						} else {
							cout << "Login failed! \n";
						}
					}
						break;
					case '7':
						cout << "NAME: " << buyer->getName() << " ADDRESS: " << buyer->getAddress() << " CREDIT CARD: " << buyer->getCredit() << "\n";
						break;
					case 'q': 
					{
						i=3;
						updateClientList(buyer);
						buyer = NULL;
					}
						break;
				}
				i--;
			}
		}
		else if(c == 'x') {
			for(int w = 0; w < length; w++) {
				if(clientList[w])
					delete clientList[w];
			}
			delete s;
			delete o;
			if(buyer)
				delete buyer;
			return 0;
		}
		INF--;
	}
}