#ifndef __HASHING__
#define __HASHING__

#include <string>

inline unsigned int stringHashFunction( std::string key, int tablesize ) {
	unsigned int total = 0;
	unsigned int shift = key.length() % 2 == 0 ? 1 : 2;
	int prime = 29;
	for(int i = 0; i < key.length(); i++) {
		total += (key[i] << shift) * prime;
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
			values = new hashContainer<T>*[size];
			for(int i = 0; i < size; i++) {
				values[i] = NULL;
			}
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

			if( cur == NULL ) {
				return NULL;
			}

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
			std::string headers = std::string();
			for( int i = 0; i < size; i++ ) {
				hashContainer<std::string> * current = values[i];
				while( current != NULL ) {
					headers += current->key + ":" + * current->value + "\r\n";
					current = current->next;
				}
			}
			headers += "\r\n";
			return headers;
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
		int size = 100;
		hashContainer<T> ** values;
		
};

#endif