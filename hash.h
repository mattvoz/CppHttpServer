#ifndef __HASHING__
#define __HASHING__

inline int stringHashFunction( std::string key, int tablesize ) {
	int total = 0;
	int shift = key.length() % 2 == 0 ? 1 : 2;
	for(int i = 0; i < key.length(); i++) {
		total += key[i] << shift;
	}

	return total % tablesize;
}

template <typename T>
struct hashContainer {
	std::string key;
	T * value;
	hashContainer * next;
};

template <typename T>
class hashTable {
	public:
		hashTable() {
			values = new hashContainer<T>*[100];
			for(int i = 0; i < size; i++) {
				values[i] = NULL;
			}
			this->size = 100;
		};

		hashTable(int tableSize) {
			this->size = tableSize;
			values = new hashContainer<T>*[tableSize];
			for(int i = 0; i < tableSize; i++) {
				values[i] = NULL;
			}
		};

		void add( std::string key, T value) {
			int pos = stringHashFunction(key, size);
			hashContainer<T> * newVal = new struct hashContainer<T>();
			newVal->key = key.c_str();
			newVal->value = new T();
			*(newVal->value) = value;
			newVal->next = NULL;

			hashContainer<T> * tmp = values[pos];

			if( tmp == NULL ) {
				values[pos] = newVal;
				return;
			}

			if( tmp->key == key ) {
				delete newVal;
				*(tmp->value) = value;
				return;
			}

			while( tmp->next != NULL ) {
				tmp = tmp->next;
				if( tmp->key == key ) {
					delete newVal;
					*(tmp->value) = value;
					return;
				}
			}

			tmp->next = newVal;
			return;
		};

		T * operator[](std::string key) {
			int hashVal = stringHashFunction(key, size);
			hashContainer<T> * cur = values[hashVal];

			while(cur->next != NULL) {
				if(cur->key == key) {
					return cur->value;
				}
				cur = cur->next;
			}

			if(cur->key == key) {
				return cur->value;
			}
			
			return NULL;
		};

		std::string toString() {
			static std::string response = std::string();
			for( int i = 0; i < size; i++ ) {
				hashContainer<T> * current = values[i];
				while( current != NULL ) {
					response += current->key + ":" + * current->value + "\n";
					current = current->next;
				}
			}
			return response;
		}

		~hashTable() {
			for(int i = 0; i < size; i++) {
				struct hashContainer<T> * cur = values[i];
				while( cur->next != NULL ) {
					struct hashContainer<T> * tmp = cur;
					cur = cur->next;
					delete tmp->value;
					delete tmp;
				}
			}
		};
	private:
		int size;
		hashContainer<T> ** values;
		
};

#endif