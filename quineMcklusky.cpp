/////////////////////////////////////////////////////////////////////////////////////////////////
#define MAXBITS 26

#include"iostream"
#include"boost/logic/tribool.hpp"
#include"boost/logic/tribool_fwd.hpp"
#include"boost/logic/tribool_io.hpp"

using namespace std;
using namespace boost::logic;

///////////////////////////////////////////////////////////////////////
////////////////////    DATA STRUCTURES    ////////////////////////////
///////////////////////////////////////////////////////////////////////
struct terms {                        //Linked list
	int term;
	terms* nextTerm;
	terms() {
		nextTerm = NULL;
	}
};

//NODE
/*
Structure to hold a single entry of table
--Arr  : A pointer to an array storing single implicant of table
Size of array is passed in parameterized constructor
--flag : Is false if the implicant does not compare with any one
present in current table
--Next : Points to next node in linked list
*/
struct Node
{
	bool flag;
	tribool* Arr;
	terms* start_minterm;
	Node* next;
	void print(int bits)
	{
		if (bits > MAXBITS) { return; }
		for (int i = bits - 1, printed = 0; i >= 0; i--, printed++)
		{
			if (Arr[i])
			{
				cout << char('a' + (char)printed);
			}
			else if (!Arr[i])
			{
				cout << char('a' + (char)printed) << '\'';
			}
		}
	}
	Node() {
		flag = 0;
		Arr = NULL;
		next = NULL;
		start_minterm = NULL;
	}
	Node(int bits)
	{
		Arr = new tribool[bits];
		flag = 0;
		next = NULL;
		start_minterm = NULL;
	}
	void addMinterm(int one_minterm) {
		terms* temp = start_minterm;
		if (temp == NULL) {
			temp = new terms;
			temp->term = one_minterm;
			start_minterm = temp;
			temp->nextTerm = NULL;
		}
		else {
			while (temp->nextTerm != NULL) {
				if (temp->term == one_minterm)return;//so thet repetetion of minterms can be avoided
				temp = temp->nextTerm;
			}
			temp->nextTerm = new terms;
			temp->nextTerm->term = one_minterm;
			temp->nextTerm->nextTerm = NULL;
		}
	}
	void addMinterms(int additive_term, int ii) {
		if (ii < 0) {
			addMinterm(additive_term);
			return;
		}
		int term = additive_term;
		for (int i = ii; i >= 0; i--) {
			if (Arr[i]) {
				term += pow(2, i);
				if (i == 0)addMinterm(term);
			}
			else if (!Arr[i]) {
				if (i == 0)
					addMinterm(term);
			}
			else {
				addMinterms(term, i - 1);
				addMinterms(term + pow(2, i), i - 1);
			}
		}
	}
	void set_all_minterms(int bits) {
		if (Arr == NULL)return;
		addMinterms(0, bits - 1);
	}
	bool linearSearch(int key)
	{
		terms* temp = start_minterm;
		while (temp != NULL) {
			if (temp->term == key)return true;
			temp = temp->nextTerm;
		}
		return false;
	}
	void if_present_del(int key) {
		terms* temp = start_minterm;
		terms* temp2 = NULL;
		while (temp != NULL)//Find if key is present in minterm list
		{
			if (temp->term == key)break;
			temp2 = temp;
			temp = temp->nextTerm;
		}

		if (temp == NULL)return;
		else if (temp == start_minterm) {//Delete operation starts in case term is present
			temp2 = start_minterm;//Use tem2 as simple pointer variable
			start_minterm = temp2->nextTerm;
			delete temp2;
		}
		else {
			temp2->nextTerm = temp->nextTerm;//use temp2 as previous of term node to be deleted
			delete temp;
		}
	}
};
bool isEqual(tribool arr1[], tribool arr2[], int size) {//Indetermined state needs to be tackled a little differently
	//int size = sizeof(arr1) / sizeof(arr1[0]);
	for (int i = 0; i < size; i++) {
		if (arr1[i]) {
			if (arr2[i]);
			else return 0;
		}
		else if (!arr1[i]) {
			if (!arr2[i]);
			else return 0;
		}
		else {
			if (arr2[i])return 0;
			else if (!arr2[i])return 0;
		}
	}
	return 1;//This statement will be reached only when arrays will be equal
}

//Linked List
/*
A chain of Nodes such that each node's Next pointer points to the succesive
node in the structur.
--bits : Holds the size of arrays to which the Arr of nodes in list point
--head : Points to the first node in list
--tail : Points to the last node in list

Functions
--AddNode : Adds a node at the end of list
The first argument is a pointer to an array which is copied
in the list. The second argument is the size of array passed to AddNode
--gethead : Returns address of first node
--setbits : Sets the value of bits
--isEmpty : Returns true if List has no node
*/
struct Linked_List
{
	int bits;

	Node* head;
	Node* tail;

public:

	Linked_List()
	{
		head = tail = NULL;
	}
	void AddNode(tribool* data, int b)
	{
		if (isEmpty())
		{
			head = tail = new Node(b);
			head->flag = false;
			for (int i = 0; i < bits; i++)
				head->Arr[i] = data[i];
			head->next = NULL;
		}

		else
		{
			tail->next = new Node(b);
			tail = tail->next;
			tail->flag = false;
			for (int i = 0; i < bits; i++)
				tail->Arr[i] = data[i];
			tail->next = NULL;
		}
	}
	inline Node* gethead()
	{
		return head;
	}
	inline bool isEmpty() {
		if (head == NULL)
			return true;
		else
			return false;
	}
	void setbits(int b)
	{
		bits = b;
	}
	void show()
	{
		Node* temp;
		temp = gethead();
		while (temp != NULL)
		{
			for (int i = bits - 1; i >= 0; i--)
				cout << temp->Arr[i];
			cout << "  ";
			terms* temp2 = temp->start_minterm;
			while (temp2 != NULL) {
				cout << temp2->term << " ";
				temp2 = temp2->nextTerm;
			}
			cout << "    " << temp->flag;
			cout << endl;
			temp = temp->next;
		}
	}
	int getBits() {
		return bits;
	}
	void printEsential() {

		Node* nodeTemp = head;
		while (nodeTemp != NULL)
		{
			if (nodeTemp->flag == 1) {
				nodeTemp->print(bits);//First print essential minterms
				cout << " + ";
			}
			nodeTemp = nodeTemp->next;
		}
	}
	void eliminate_common() {
		Node* temp1;
		Node* temp2;
		Node* temp;
		temp1 = gethead();//key
		while (temp1 != NULL)
		{
			temp2 = temp1;//on starting temp1 and 2 have same values
			while (temp2->next != NULL)
			{
				if (isEqual(temp1->Arr, temp2->next->Arr, bits))//if temp1 and next of temp2 are equal
				{												//This approach is used because it is a singly linked list
					temp = temp2->next;
					temp2->next = temp->next;
					delete temp;
				}
				else
					temp2 = temp2->next;//temp2++
			}
			temp1 = temp1->next;
		}
	}
	void set_all_minterms() {
		Node* temp = head;
		while (temp != NULL) {
			temp->set_all_minterms(bits);
			temp = temp->next;
		}
	}
	void markEPI() {//mark essential prime implicants and remove corresponding minterms from other imlicants
		Node*temp_node = gethead();
		while (temp_node != NULL) {//make sureall flags are false
			temp_node->flag = false;
			temp_node = temp_node->next;
		}

		temp_node = gethead();
		int current_minterm = 0, num = 0;
		Node* essential = NULL;
		int max_minterm = pow(2, bits);
		while (current_minterm < max_minterm) {
			while (temp_node != NULL) {
				if (temp_node->linearSearch(current_minterm))
				{
					essential = temp_node;
					num++;
				}
				temp_node = temp_node->next;
			}
			if (1 == num)//It is essential iff minterm comes in it and not in others
				essential->flag = true;
			num = 0;
			current_minterm++;
			temp_node = gethead();
		}//Here temp_node is already head

		terms* current_term = NULL;
		while (temp_node != NULL)	//Traverse whole list to find essential implicants i.e flag = 1 terms
		{
			if (temp_node->flag) {//Do operation if flag = 1 i.e Its an essential minterm
				current_term = temp_node->start_minterm;
				while (current_term != NULL)//We need to check for every minterm present in our terms list
				{
					current_minterm = current_term->term;
					Node* temp2 = gethead();
					while (temp2 != 0)
					{
						if (!temp2->flag)//Minterms of essential implicants should not be deleted
							temp2->if_present_del(current_minterm);
						temp2 = temp2->next;
					}
					current_term = current_term->nextTerm;
				}
			}//End if
			temp_node = temp_node->next;
		}
	}//Function end
};


//Table
/*
An Array of Linked lists to model tables formed as the algorithm proceeds
Linked List at ith index has Nodes pointing to arrays containing i ones
--bits   : Holds the size of Arrays to which Nodes of Table point

--Groups : The total number of Linked Lists in present in table
present in current table

--start  : Points to the first node of table

--extract: Adds all entries flagged as false in the linked list passed as argument
*/
struct Table
{
	int bits;
	int Groups;
	Linked_List* start = NULL;

	Table(int a, int b)
	{
		Groups = a;
		bits = b;
		start = new Linked_List[Groups];
		for (int i = 0; i < Groups; i++)
			start[i].setbits(b);
	}

	bool isEmpty()
	{
		bool r = true;
		for (int i = 0; i < Groups; i++)
		{
			if (!(start[i].isEmpty()))
			{
				r = false; break;
			}
		}
		return r;
	}

	void extract(Linked_List* L)
	{
		Node* temp;
		for (int i = 0; i < Groups; i++)
		{
			temp = start[i].gethead();
			while (temp != NULL)
			{
				if (temp->flag == false)
					L->AddNode(temp->Arr, bits);
				temp = temp->next;
			}
		}
	}
};

/*
///////////////////////////////////////////////////////////////////////
Binary converter
Returns an array of tribool type. Each index corresponds to weight of digit in binary.
--The first arrgument gets converted to binary and returned as an array
--The second argument specifies the size of tribool array to be returned.
--The fourth argument passes the frequency of ones in variable passed
*/
tribool* BinaryConv(unsigned int num, unsigned int bits, unsigned int* freq)
{
	tribool* ptr = new tribool[bits];
	int i = 0;
	*freq = 0;
	while (num > 0)
	{
		if (ptr[i] = (num % 2))
			(*freq)++;
		num /= 2;
		i++;
	}

	for (int j = i; j < bits; j++)
		ptr[j] = (bool)0;
	return ptr;
}

/*
///////////////////////////////////////////////////////////////////////
State function
returns 1 if tribool is true
returns 0 if tribool is false
returns -1 if tribool is indeterminate
*/
int state(tribool a)
{
	if (a) {
		return 1;
	}
	else if (!a) {
		return 0;
	}
	else {
		return -1;
	}
}

/*
///////////////////////////////////////////////////////////////////////
Compare function
The function compares two tribool arrays passed as pointers.
Returns -1 if they are not fit to compare, else returns index of changing bit.
Third and fourth argument specify the size of implicants and stage of table
*/
int compare(tribool* a, tribool* b, int dashes, int bits)
{
	if (dashes != 0)
	{
		for (int k = 0; k < bits; k++)
		{
			if (state(a[k]) == -1 && state(b[k]) != -1)
				return -1;
		}
	}

	int k;

	int count = 0;
	for (int j = 0; j < bits; j++)
	{
		if (state(a[j]) == -1)
			continue;
		if (a[j] != b[j])
		{
			count++;
			k = j;
		}
		if (count > 1)
			break;
	}

	return (count > 1) ? -1 : k;
}

void print(tribool a[], int size) {
	for (int i = 0; i < size; i++)
		cout << a[i];
}

class Petrick{					//This class simplifies the minterm linked list by Petrick's method
private:

	static int bits;
	struct Petrick_unit{
		Node* nodePtr;
		Petrick_unit* nextUnitPtr;
		Petrick_unit() :nodePtr(NULL), nextUnitPtr(NULL){}	//Constructor
		Petrick_unit(Node* nodeTemp) :nodePtr(NULL), nextUnitPtr(NULL){	//Constructor
			nodePtr = nodeTemp;
		}
	};
	struct Petrick_node{
		unsigned int unitCount;
		Petrick_unit* unitStartPtr;
		Petrick_node* nextPNodePtr;
		Petrick_node() :unitCount(0), unitStartPtr(NULL), nextPNodePtr(NULL){}
		void showUnits(int bits){
			Petrick_unit* unitTemp = unitStartPtr;
			while (unitTemp != NULL)
			{
				unitTemp->nodePtr->print(bits);
				unitTemp = unitTemp->nextUnitPtr;
				cout << " + ";
			}
			cout << "\b \b\b \b";
		}
		Petrick_node(Node*nodeTemp) :unitCount(0), unitStartPtr(NULL), nextPNodePtr(0){
			addPetrickUnit(nodeTemp);
		}
		void addPetrickUnit(Node*nodeTemp)
		{
			if (unitStartPtr == NULL){
				unitStartPtr = new Petrick_unit(nodeTemp);
			}
			else
			{
				Petrick_unit* unitTemp = unitStartPtr;
				while (unitTemp->nextUnitPtr != NULL)
				{
					if (isEqual(nodeTemp->Arr, unitTemp->nodePtr->Arr, bits))
					{
						return;
					}
					unitTemp = unitTemp->nextUnitPtr;
				}
				if (isEqual(nodeTemp->Arr, unitTemp->nodePtr->Arr, bits))
				{
					return;
				}
				unitTemp->nextUnitPtr = new Petrick_unit(nodeTemp);
			}
			unitCount++;
		}
		void duplicateUnitList(Petrick_node* toBeCopied)
		{
			Petrick_unit* cpyPtr = toBeCopied->unitStartPtr;
			while (cpyPtr != NULL)
			{
				this->addPetrickUnit(cpyPtr->nodePtr);
				cpyPtr = cpyPtr->nextUnitPtr;
			}
		}
	};

	void addPetrickNode(Node* nodeTemp){
		if (petrickHead == NULL){
			petrickHead = new Petrick_node(nodeTemp);
		}
		else{
			Petrick_node* temp = petrickHead;
			while (temp->nextPNodePtr != NULL){ temp = temp->nextPNodePtr; }
			temp->nextPNodePtr = new Petrick_node(nodeTemp);
		}
	}
	Petrick_node* petrickHead;
	Node*** answerChecker;//This is used in show function to help in not displaying repeted values like a+b and b+a
	//A 2D array of Node type pointer
public:
	Petrick(Linked_List PItable){
		bits = PItable.getBits();

		Node* nodeTemp = PItable.gethead();
		terms* termsTemp = NULL;
		int currentMinterm = 0, previousCount = 1, count = 0;
		const int maxMinterm = pow(2, PItable.getBits()) - 1;
		Petrick_node* additionPtr = petrickHead;
		while (currentMinterm <= maxMinterm)
		{
			for (Node* temporaryNode = PItable.gethead(); temporaryNode != NULL; temporaryNode = temporaryNode->next)//To count frequency/number of occurances of minterm
			{
				if (temporaryNode->linearSearch(currentMinterm) && !temporaryNode->flag){ count++; }
			}

			multiplyList(count);
			while ((nodeTemp != NULL) && count)//And count is not 0 i.e minterm is present atleast 1 time
			{
				if (!nodeTemp->flag && nodeTemp->linearSearch(currentMinterm))
				{
					if (additionPtr == NULL)
					{
						addPetrickNode(nodeTemp);
					}
					else
					{
						for (int i = 1; i <= previousCount; i++)
						{
							additionPtr->addPetrickUnit(nodeTemp);
							additionPtr = additionPtr->nextPNodePtr;
						}
					}
				}
				nodeTemp = nodeTemp->next;
			}
			if (count){//count = 0 will false trigger it
				previousCount = count*previousCount;
			}
			count = 0;
			nodeTemp = PItable.gethead();
			currentMinterm++;
			additionPtr = petrickHead;
		}
	}
	void multiplyList(int multiplicationFactor){
		if (multiplicationFactor == 0 || multiplicationFactor == 1 || petrickHead == NULL){ return; }

		Petrick_node* PNodeTemp = petrickHead;
		int length = 1;
		while (PNodeTemp->nextPNodePtr != 0)
		{
			length++;
			PNodeTemp = PNodeTemp->nextPNodePtr;
		}

		Petrick_node* PNodeTemp2 = petrickHead;//to store the real list's location
		for (int j = 2; j <= multiplicationFactor; j++)
		{
			PNodeTemp2 = petrickHead;//this is taken reference to start list copying
			for (int i = 1; i <= length; i++)
			{
				PNodeTemp->nextPNodePtr = new Petrick_node;
				PNodeTemp = PNodeTemp->nextPNodePtr;
				//duplicate units
				PNodeTemp->duplicateUnitList(PNodeTemp2);
				PNodeTemp2 = PNodeTemp2->nextPNodePtr;
			}
		}

	}
	void show(Linked_List PItable){
		if (petrickHead == NULL){
			cout << "\nMinimum cost sum of products expression is :\n";
			PItable.printEsential();
			cout << "\b \b\b \n";//Two backspace to remove + and space and \n to go to new line
			return;
		}

		Petrick_node*PNodeTemp = petrickHead;
		int minCount = PNodeTemp->unitCount;//just a temporary value to initialize variable
		while (PNodeTemp != 0)
		{
			if (PNodeTemp->unitCount < minCount){ minCount = PNodeTemp->unitCount; }
			PNodeTemp = PNodeTemp->nextPNodePtr;
		}

		int solutionCount = 0;
		PNodeTemp = petrickHead;
		while (PNodeTemp != 0)
		{
			if (PNodeTemp->unitCount == minCount)
			{
				solutionCount++;//This is to count possible number of solutions
			}
			PNodeTemp = PNodeTemp->nextPNodePtr;
		}

		answerChecker = new Node**[solutionCount];
		for (int a = 0; a < solutionCount; ++a)//Our maximum limit of number of solution is solutionCount
		{
			answerChecker[a] = new Node*[minCount];
		}
		for (int i = 0; i < solutionCount; i++){//Assign NULL to every pointer
			for (int j = 0; j < minCount; j++){
				answerChecker[i][j] = NULL;
			}
		}

		PNodeTemp = petrickHead;
		cout << "\nMinimum cost sum of products expression(s):\n";
		int number = 0, row = 0;
		while (PNodeTemp != 0)
		{
			if (PNodeTemp->unitCount == minCount && isNotPrinted(PNodeTemp, row + 1, minCount))//We want minimum cose sum of products solution
			{
				cout << ++number << ". ";//Answer number
				PItable.printEsential();
				Petrick_unit* unitTemp = PNodeTemp->unitStartPtr;
				for (int coloumn = 0; coloumn < minCount; coloumn++)//Assign nodes that are printed, to answer checker
				{				//This common removing test is necessary so that many same solutions may not come e.g. a+b and b+a are same thing
					answerChecker[row][coloumn] = unitTemp->nodePtr;
					unitTemp = unitTemp->nextUnitPtr;
				}
				row++;
				PNodeTemp->showUnits(PItable.getBits());//then print non essential ones
				cout << "\n";
			}//Two backspace to remove + and space and \n to go to new line
			PNodeTemp = PNodeTemp->nextPNodePtr;
		}
	}
private:
	bool isNotPrinted(Petrick_node* PNodeTemp, int current_row, int minCount)
	{
		if (answerChecker[0][0] == NULL)return 1;
		int num_common = 0;
		Petrick_unit* PUnitTemporary = PNodeTemp->unitStartPtr;
		for (int i = 0; i < current_row; i++)
		{
			while (PUnitTemporary != NULL)
			{
				for (int j = 0; j < minCount; j++)
				{
					if (answerChecker[i][j] == PUnitTemporary->nodePtr){ num_common++; }//Comparing pointers of node to see if answer is being repeated or not
				}
				PUnitTemporary = PUnitTemporary->nextUnitPtr;
			}
			if (num_common == minCount){ return 0; }
			PUnitTemporary = PNodeTemp->unitStartPtr;
			num_common = 0;
		}
		return 1;
	}
};

int Petrick::bits;
int main(void)
{
	int bits; unsigned int freq = 0;
	cout << "Enter number of variables/bits: " << endl; cin >> bits;
	while (bits > MAXBITS || bits < 1){
		cout << "\nYour range is 0 to " << MAXBITS << endl;
		cout << "Enter Literals: " << endl; cin >> bits;
	}

	Table* A = NULL;
	Table* B = NULL;

	A = new Table(bits + 1, bits);

	//Creation of first table
	cout << "Enter Minterms, -1 when done" << endl;
	unsigned int input; cin >> input;
	while (input != -1)
	{
		if (input<0 || input>(pow(2, bits) - 1)){
			cout << "You entered an invalid minterm. Enter again\n";
			cin >> input;
			continue;
		}
		A->start[freq].AddNode(BinaryConv(input, bits, &freq), bits * sizeof(tribool));
		cin >> input;
	}

	//Linked List to hold prime implicants
	Linked_List PI; PI.setbits(bits);

	Node* k = NULL;
	Node* l = NULL;

	int q = 0;
	int counter = 0;
	int x = bits;

	//Next table is created after each iteration of while loop
	while (x >= 1)
	{
		//Creation of successive table
		B = new Table(x, bits);

		//For loop to compare each groups elements with next group's elements
		for (int i = 0; i < x; i++)
		{
			//k points to preceding groups elements
			k = A->start[i].gethead();
			while (k != NULL)
			{
				//l points to successive groups elements
				l = A->start[i + 1].gethead();
				while (l != NULL)
				{
					q = compare(k->Arr, l->Arr, counter, bits);
					if (q >= 0)
					{
						B->start[i].AddNode(k->Arr, bits);

						//Changing bit marked as indeterminate
						B->start[i].tail->Arr[q] = indeterminate;

						//Entries flagged as true because they have compared
						l->flag = true;
						k->flag = true;
					}
					l = l->next;
				}
				k = k->next;
			}
		}
		//Collects all crossed terms in table
		A->extract(&PI);

		//Breaks the loop if successive table comes out to be empty
		//Indicating no further comparisons are possible
		if (B->isEmpty())
			break;

		A = B;
		x--; counter++;
	}
	PI.eliminate_common();
	PI.set_all_minterms();
	PI.markEPI();
	//PI.show();		//This can be uncommented to see the Prime Implicant Table
	Petrick solver(PI);
	solver.show(PI);
	cout << endl;
	system("pause");
}


