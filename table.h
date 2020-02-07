/*************************************************************/
/*    V 1.1:                                                 */
/*      - added prototypes for numRecords() and isEmpty()    */
/*      - coded isEmpty() and numRecords() in SimpleTable    */
/*      - created empty functions for isEmpty() and          */
/*        numRecords() in LPTable                            */
/*************************************************************/
/*Modified:
Sort()
copy const & assign operator
destructor*/
#include <string>
#include <utility>
#include <functional>

using namespace std;

template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data){
			key_=key;
			data_=data;
		}

	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);
	void sort();
	void grow();
public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const{return size_==0;}
	virtual int numRecords() const{return size_;}
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
	int rc=-1;
	for(int i=0;i<size_;i++){
		if(records_[i]->key_==key){
			rc=i;
		}
	}
	return rc;
}

//insertion sort function used by the quicksort function
template <class TYPE>
void insertionSort(TYPE arr[], int left, int right) {
	TYPE curr;
	int i, j;
	for (i = left + 1; i <= right; i++) {
		curr = arr[i];
		for (j = i; j>0 && arr[j - 1] > curr; j--) {
			arr[j] = arr[j - 1];
		}
		arr[j] = curr;
	}
}

//quicksort functon sorts the table much faster than the previous function
template <class TYPE>
void QuickSort(TYPE arr[], int left, int right) {
	if (right - left <= 3) {
		insertionSort(arr, left, right);
	}
	else {
		int pivotpt = right;
		int i = left;
		int j = right - 1;
		TYPE pivot = arr[pivotpt];
		while (i<j) {
			while (arr[i]<pivot) i++;
			while (arr[j]>pivot) j--;
			if (i<j)
				swap(arr[i++], arr[j--]);
		}
		swap(arr[i], arr[pivotpt]);
		QuickSort(arr, left, i - 1);
		QuickSort(arr, i + 1, right);
	}
}

//sort the according to key in table
//calls the quicksort function to sort the array using median of three as the pivot
template <class TYPE>
void SimpleTable<TYPE>::sort(){
	if (size_ > 1) {
		
	}
	else if (size_ == 2) {
		insertionSort(records_, 0, 1);
	}
	else {
		Record* medianArray[3] = { records_[0], records_[size_ - 1], records_[size_ - 1 / 2] };
		insertionSort(medianArray, 0, 2);
		records_[0] = medianArray[0];
		records_[size_ - 1] = medianArray[1];// median
		records_[size_ - 1 / 2] = medianArray[2];

		QuickSort(records_, 0, size_ - 1);
	}
}


//grow the array by double
template <class TYPE>
void SimpleTable<TYPE>::grow(){
	Record** newArray=new Record*[max_*2];
	max_=max_*2;
	for(int i=0;i<size_;i++){
		newArray[i]=records_[i];
	}
	delete [] records_;
	records_=newArray;
}

//cunstructor 
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected): Table<TYPE>(){
	records_=new Record*[maxExpected];
	max_=maxExpected;
	size_=0;
}
//copy constructor
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.max_];
	max_=other.max_;
	size_ = other.size_;
	for(int i=0;i<other.size_;i++){
		records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
	}
}
//move constructor
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	size_=other.size_;
	max_=other.max_;
	records_=other.records_;
	other.records_=nullptr;
	other.size_=0;
	other.max_=0;
}
//update function updates value if record exists or add a value if it dose not exist
template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx=search(key);
	if(idx==-1){
		if(size_ == max_){
			grow();
		}
		records_[size_++]=new Record(key,value);
		sort();
	}
	else{
		records_[idx]->data_=value;
	}
	return true;
}
//removes record and moves each value down to fill the empty spot made by removal.
template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	int idx=search(key);
	if(idx!=-1){
		delete records_[idx];
		for(int i=idx;i<size_-1;i++){
			records_[i]=records_[i+1];
		}
		size_--;
		return true;
	}
	else{
		return false;
	}
}
//find function takes in a key to find, and passes back the data in the value pointer passed in
template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	int idx=search(key);
	if(idx==-1)
		return false;
	else{
		value=records_[idx]->data_;
		return true;
	}
}
//copy constructor = operator
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
	if(this!=&other){
		if(records_){
			for (int i = 0; i < size_; i++) {
				delete records_[i];
			}
		}
		records_ = new Record*[other.max_];
		max_ = other.max_;
		size_ = other.size_;
		for (int i = 0; i<other.size_; i++) {
			records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
		}
	}
	return *this;
}
//move constructor =operator
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_);
	swap(size_,other.size_);
	swap(max_,other.max_);
	return *this;
}
//deconstructor
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		for (int i = 0; i < size_; i++) {
			delete records_[i];
		}
	}
	delete[] records_;
}

template <class TYPE>
class LPTable:public Table<TYPE>{
	struct Record {
		TYPE data_;
		string key_;

		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};
	Record** records_;
	int size;//elements currently in the array
	int maxExpected;
	int capacity;
	double percentOpen;
	
public:
	LPTable(int maxExpected,double percentOpen);
	LPTable(const LPTable& other);
	LPTable(LPTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const LPTable& operator=(const LPTable& other);
	virtual const LPTable& operator=(LPTable&& other);
	virtual ~LPTable();
	int hashIndex(const string& key);
	virtual bool isEmpty() const { return size == 0; }
	virtual int numRecords() const { return size; }
};
//constructor
template <class TYPE>
LPTable<TYPE>::LPTable(int me,double po): Table<TYPE>(){
	maxExpected = me;
	percentOpen = po;
	capacity =(int)((maxExpected*percentOpen) + maxExpected);
	records_ = new Record*[capacity]();
}
//copy constructor
template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other) {
	if (other.records_) {
		records_ = new Record*[other.capacity]();
		capacity = other.capacity;
		maxExpected = other.maxExpected;
		percentOpen = other.percentOpen;
		size = other.size;
		for (int i = 0; i < other.capacity; i++) {
			if (other.records_[i]==nullptr) {

			}
			else {
				records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
			}
		}
	}
}
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other){
	swap(records_, other.records_);
	swap(capacity, other.capacity);
	swap(maxExpected, other.maxExpected);
	swap(percentOpen, other.percentOpen);
	swap(size, other.size);
	
}
//hash function takes in a key string and returns the hash index as an int
template <class TYPE>
int LPTable<TYPE>::hashIndex(const string& key) {
	std::hash<string> hashFunction;
	size_t hash = hashFunction(key);
	int idx = hash % capacity;
	return idx;
}
//update function updates record if it exists or adds a record if it dosent exist 
//records are placed by hash index based on key
//searches only loop up to a nullptr 
template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value){
		Record* temp = new Record(key, value);
		int idx = hashIndex(key);
		int stop = 0;
		while (records_[idx] != nullptr) {
			if (records_[idx]->key_ == key) {
				records_[idx]->data_ = value;
				return true;
			}
			//set index to start of array if idx is at the end of array only once
			if (idx == capacity - 1) {
				if (stop == 0) {
					idx = -1;
					stop = 1;
				}
			}
			idx++;
		}
		if (maxExpected == size) {
			return false;
		}
		records_[idx] = temp;
		size++;
		return true;
}
//removes a record 
//loops through follwing records untill nullptr is reached
//deletes each record it encounters that is not already in the desired hashindex 
//adds them back in using update() to place them where they belong by hashindex
template <class TYPE>
bool LPTable<TYPE>::remove(const string& key){
	int found = 0;
	int stop = 0;
	int idx = hashIndex(key);
	//search loop finds and removes record
	while (records_[idx] != nullptr && found==0) {
		if (records_[idx]->key_ == key) {
			delete records_[idx];
			records_[idx] = nullptr;
			size--;
			found = 1;
		}
		if (idx == capacity - 1) {
			if (stop == 0) {
				idx = -1;
				stop = 1;
			}
			else {
				return false;
			}
		}
		idx++;
	}
	if (found==0) {
		return false;
	}
	else {
		stop = 0;
		//update() loop deletes and re adds records back placing them where they belong according to hash function
		while (records_[idx] != nullptr) {
			int innerIdx = hashIndex(records_[idx]->key_);
			if (innerIdx == idx) {

			}
			else {
				Record *temp = new Record(records_[idx]->key_, records_[idx]->data_);
				delete records_[idx];
				records_[idx] = nullptr;
				size--;
				update(temp->key_, temp->data_);
			}
			if (idx == capacity - 1) {
				if (stop == 0) {
					idx = -1;
					stop = 1;
				}
				else {
					return false;
				}
			}
			idx++;
		}
		return true;
	}
}

//find function finds a record and returns the data through passed in pointer	
template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& data_){
	int stop = 0;
	int idx = hashIndex(key);
	while (records_[idx]!= nullptr) {
		if (records_[idx]->key_ == key) {
			data_ = records_[idx]->data_;
			return true;
		}
		if (idx == capacity - 1) {
			if (stop == 0) {
				idx = -1;
				stop = 1;
			}
		}
		idx++;
	}
	return false;
}
//copy constructor =operator 
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){
	if (this != &other) {
		if (records_) {
			for (int i = 0; i < capacity-1; i++) {
				delete records_[i];
			}
		}
		records_ = new Record*[other.capacity]();
		capacity = other.capacity;
		maxExpected = other.maxExpected;
		percentOpen = other.percentOpen;
		size = other.size;
		for (int i = 0; i < other.capacity; i++) {
			if (other.records_[i] == nullptr) {

			}
			else {
				records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
			}
		}
	}
	return *this;

}
//move constructor =operator
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other){
	swap(records_, other.records_);
	swap(capacity, other.capacity);
	swap(maxExpected, other.maxExpected);
	swap(percentOpen, other.percentOpen);
	swap(size, other.size);
	return *this;

}
//deconstructor
template <class TYPE>
LPTable<TYPE>::~LPTable(){
	if (records_) {
		for (int i = 0; i < capacity; i++) {
			delete records_[i];
		}
	}
	delete[] records_;
}


