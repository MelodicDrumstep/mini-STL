#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#include "utility.hpp"
#include "exceptions.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"
#include <memory>
#include <vector>

const int SIZE = 256;

class Hash 
{
public:
	unsigned int operator () (Integer lhs) const 
	{
		int val = lhs.val;
		return std::hash<int>()(val);
	}
};

class Equal 
{
public:
	bool operator () (const Integer &lhs, const Integer &rhs) const 
	{
		return lhs.val == rhs.val;
	}
};

namespace sjtu 
{
template<class T> class double_list
{
public:
	/**
	 * elements
	 * add whatever you want
	*/
	struct node
	{
		std::shared_ptr<T> val;
		std::shared_ptr<node> pre;
		std::shared_ptr<node> next;
		node(std::shared_ptr<T> v , std::shared_ptr<node> p, std::shared_ptr<node> n) : val(v), pre(p), next(n) {}
	};

	std::shared_ptr<node> head;
	std::shared_ptr<node> tail;


// --------------------------
	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	double_list()
	{
		head = std::make_shared<node>(nullptr, nullptr, nullptr);
		tail = std::make_shared<node>(nullptr, head, nullptr);
		head -> next = tail;
	}
	double_list(const double_list<T> &other)
	{
		head = std::make_shared<node>(nullptr, nullptr, nullptr);
		tail = std::make_shared<node>(nullptr, head, nullptr);
		head -> next = tail;
		std::shared_ptr<node> p = other.head -> next;
		while(p != other.tail)
		{
			insert_tail(*(p -> val));
			p = p -> next;
		}
	}

	class iterator
	{
	public:
		double_list<T>* list;
		std::shared_ptr<node> p;
    	/**
		 * elements
		 * add whatever you want
		*/
	    // --------------------------
        /**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		*/
		iterator(double_list<T> & L) : p(nullptr), list(&L) {}
		iterator(const iterator &t)
		{
			list = t.list;
			p = t.p;
		}
		~iterator() {}
        /**
		 * iter++
		 */
		iterator operator++(int) 
		{
			iterator tmp(*this);
			if(p == list -> tail)
			{
				throw "invalid";
			}
			p = p -> next;
			return tmp;
		}
        /** 
		 * ++iter
		 */
		iterator &operator++() 
		{
			if(p == list -> tail)
			{
				throw "invalid";
			}
			p = p -> next;
			return *this;
		}
        /**
		 * iter--
		 */
		iterator operator--(int) 
		{
			iterator tmp(*this);
			if(p -> pre == list -> head)
			{
				throw "invalid";
			}
			p = p -> pre;
			return tmp;
		}
        /**
		 * --iter
		 */
		iterator &operator--() 
		{
			if(p -> pre == list -> head)
			{
				throw "invalid";
			}
			p = p -> pre;
			return *this;
		}
		/**
		 * if the iter didn't point to a value
		 * throw " invalid"
		*/
		T &operator*() const 
		{
			if(p == list -> head || p == list -> tail)
			{
				throw "invalid";
			}
			return *(p -> val);
		}
        /**
         * other operation
        */
		T *operator->() const noexcept 
		{
			if(p == list -> head || p == list -> tail)
			{
				throw "invalid";
			}
			return (p -> val).get();
		}

		bool operator==(const iterator &rhs) const 
		{
			return list == rhs.list && p == rhs.p;
    	}

		bool operator!=(const iterator &rhs) const 
		{
			return list != rhs.list || p != rhs.p;
		}
	};

	class const_iterator 
	{
	public:
		const double_list<T>* list;
		const std::shared_ptr<node> p;
		const_iterator(const double_list<T> & L) : p(nullptr), list(&L) {}
		const_iterator(const const_iterator &t)
		{
			list = t.list;
			p = t.p;
		}
		~const_iterator() {}
		const T &operator*() const 
		{
			if(p == list -> head || p == list -> tail)
			{
				throw "invalid";
			}
			return *(p -> val);
		}
		const T *operator->() const noexcept 
		{
			if(p == list -> head || p == list -> tail)
			{
				throw "invalid";
			}
			return (p -> val).get();
		}
		const_iterator operator++(int) 
		{
			const_iterator tmp(*this);
			if(p == list -> tail)
			{
				throw "invalid";
			}
			p = p -> next;
			return tmp;
		}
		const_iterator &operator++() 
		{
			if(p == list -> tail)
			{
				throw "invalid";
			}
			p = p -> next;
			return *this;
		}
		const_iterator operator--(int) 
		{
			const_iterator tmp(*this);
			if(p -> pre == list -> head)
			{
				throw "invalid";
			}
			p = p -> pre;
			return tmp;
		}
		const_iterator &operator--() 
		{
			if(p -> pre == list -> head)
			{
				throw "invalid";
			}
			p = p -> pre;
			return *this;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return list == rhs.list && p == rhs.p;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return list != rhs.list || p != rhs.p;
		}
	};
	/**
	 * return an iterator to the beginning
	 */
	iterator begin()
	{
		iterator it(*this);
		it.p = head -> next;
		return it;
	}
	/**
	 * return an iterator to the ending
	 * in fact, it returns the iterator point to nothing,
	 * just after the last element.
	 */
	iterator end()
	{
		iterator it(*this);
		it.p = tail;
		return it;
	}
	/**
	 * if the iter didn't point to anything, do nothing,
	 * otherwise, delete the element pointed by the iter
	 * and return the iterator point at the same "index"
	 * e.g.
	 * 	if the origin iterator point at the 2nd element
	 * 	the returned iterator also point at the
	 *  2nd element of the list after the operation
	 *  or nothing if the list after the operation
	 *  don't contain 2nd elememt.
	*/
	iterator erase(iterator pos)
	{
		if(pos.p == head || pos.p == tail || this != pos.list)
		{
			return pos;
		}
		std::shared_ptr<node> tmp = pos.p;
		pos.p = pos.p -> next;
		tmp -> pre -> next = tmp -> next;
		tmp -> next -> pre = tmp -> pre;
		return pos;
	}

	/**
	 * the following are operations of double list
	*/
	void insert_head(const T &val)
	{
		std::shared_ptr<T> v = std::make_shared<T>(val);
		std::shared_ptr<node> n = std::make_shared<node>(v, head, head -> next);
		head -> next -> pre = n;
		head -> next = n;
	}
	void insert_tail(const T &val)
	{
		std::shared_ptr<T> v = std::make_shared<T>(val);
		std::shared_ptr<node> n = std::make_shared<node>(v, tail -> pre, tail);
		tail -> pre -> next = n;
		tail -> pre = n;
	}
	void delete_head()
	{
		if(head -> next == tail) 
		{
			return;
		}
		head -> next = head -> next -> next;
		head -> next -> pre = head;	
	}
	void delete_tail()
	{
		if(tail -> pre == head) 
		{
			return;
		}
		tail -> pre = tail -> pre -> pre;
		tail -> pre -> next = tail;
	}
	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty()
	{
		return head -> next == tail;
	}
};

template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class hashmap
{
public:
	using value_type = pair<const Key, T>;
	/**
	 * elements
	 * add whatever you want
	*/
	int size;
	int C;
	double f;// C is for capacity and f is for LoadFactor
	std::shared_ptr<std::vector<std::shared_ptr<value_type>>> table;
// --------------------------

	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	hashmap(int SIZE = 256) 
	{
		size = SIZE;
		C = 0;
		f = 0;
		table = std::make_shared<std::vector<std::shared_ptr<value_type>>>(size);
	}
	hashmap(const hashmap &other)
	{
		size = other.size;
		C = other.C;
		f = other.f;
		table = std::make_shared<std::vector<std::shared_ptr<value_type>>>(*(other.table));
	}
	~hashmap() {}
	hashmap & operator=(const hashmap &other)
	{
		size = other.size;
		C = other.C;
		f = other.f;
		table = std::make_shared<std::vector<std::shared_ptr<value_type>>>(*(other.table));
		return *this;
	}


	class iterator
	{
	public:
		const hashmap<Key,T,Hash,Equal> * p2hashmap;
		int index;
    	/**
         * elements
         * add whatever you want
        */
// --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
        */
		iterator(const hashmap<Key,T,Hash,Equal> & hashmapObj, int ID = 0): p2hashmap(&hashmapObj), index(ID) {}
		iterator(const iterator &t)
		{
			p2hashmap = t.p2hashmap;
			index = t.index;
		}
		~iterator(){}

        /**
		 * if point to nothing
		 * throw 
		*/
		value_type &operator*() const 
		{
			if(index > p2hashmap -> size)
			{
				std::cout << "invalid" << std::endl;
			}
			return *((*(p2hashmap -> table)).at(index));
		}

        /**
		 * other operation
		*/
		value_type *operator->() const noexcept 
		{
			if(index >= p2hashmap -> size)
			{
				std::cout << "invalid" << std::endl;
			}
			return 	((*(p2hashmap -> table)).at(index)).get();
		}
		bool operator==(const iterator &rhs) const 
		{
			return p2hashmap == rhs.p2hashmap && index == rhs.index;
    	}
		bool operator!=(const iterator &rhs) const 
		{
			return p2hashmap != rhs.p2hashmap || index != rhs.index;
		}
		iterator &operator++() 
		{
			index++;
			return *this;
		}
		iterator operator++(int) 
		{
			iterator tmp(*this);
			index++;
			return tmp;
		}
		iterator &operator--() 
		{
			index--;
			return *this;
		}
		iterator operator--(int) 
		{
			iterator tmp(*this);
			index--;
			return tmp;
		}
	};

	void clear()
	{
		(*table).clear();
		size = SIZE;
		C = 0;
		f = 0;
		(*table).resize(size);
	}
	/**
	 * you need to expand the hashmap dynamically
	*/
	void expand()
	{
		size *= 2;
		(*table).resize(size);
	}

	iterator begin() const
	{
		iterator it(*this);
		it.index = 0;
		return it;
	}

    /**
     * the iterator point at nothing
    */
	iterator end() const
	{
		iterator it(*this);
		it.index = size;
		return it;
	}
	/**
	 * find, return a pointer point to the value
	 * not find, return the end (point to nothing)
	*/
	iterator find(const Key &key) const
	{
		std::cout << "size : " << (size == (*table).size()) << std::endl;
		iterator it(*this);
		int index = Hash()(key) % size;
		std::cout << "index : " << index << std::endl;
		while(index < size)
		{
			//std::cout << "OKOK" << std::endl;
			if((*table).at(index) && (*table).at(index) -> first == key)
			{
				it.index = index;
				return it;
			}
			index++;
		}
		std::cout << "finish the loop" << std::endl;
		std::cout << "index : " << index << std::endl;
		it.index = size;
		return it;
	}
	/**
	 * already have a value_pair with the same key
	 * -> just update the value, return false
	 * not find a value_pair with the same key
	 * -> insert the value_pair, return true
	*/
	sjtu::pair<iterator,bool> insert(const value_type &value_pair)
	{
		iterator it = find(value_pair.first);
		std::cout << "index:" << it.index << " " << std::endl;
		if(it != end())
		{
			std::cout << "They are different" << std::endl;
			std::cout << "index:" << it.index << " " << end().index << std::endl;
			std::cout << "ptr : " << it.p2hashmap << " " << end().p2hashmap << std::endl; 
			(*table).at(it.index) = std::make_shared<value_type>(value_pair);
			std::cout << "Arrive" << std::endl;
			return sjtu::pair<iterator,bool>(it, false);
		}
		if(f > 0.75)
		{
			expand();
		}
		int index = Hash()(value_pair.first) % size;
		it.index = index;
		std::cout << "index2:" << it.index << " " << std::endl;
		while(index < size)
		{
			if((*table).at(index) == nullptr)
			{
				(*table).at(it.index) = std::make_shared<value_type>(value_pair);
				C++;
				f = (double)C / size;
				it.index = index;
				return sjtu::pair<iterator,bool>(it, true);
			}
			index++;
		}
		return sjtu::pair<iterator,bool>(it, false);
	}
	/**
	 * the value_pair exists, remove and return true
	 * otherwise, return false
	*/
	bool remove(const Key &key)
	{
		iterator it = find(key);
		if(it == end())
		{
			return false;
		}
		(*table).at(it.index) = nullptr;
		C--;
		f = (double)C / size;
		return true;
	}
};

template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap :public hashmap<Key,T,Hash,Equal>
{
public:
	typedef pair<const Key, T> value_type;
	/**
	 * elements
	 * add whatever you want
	*/
	double_list<value_type> list;

// --------------------------
	class const_iterator;
	class iterator
	{
	public:
		typename double_list<value_type>::iterator listIterator;
		linked_hashmap<Key,T,Hash,Equal> * p2linked_hashmap;
    	/**
         * elements
         * add whatever you want
        */
    // --------------------------
		iterator(const linked_hashmap<Key,T,Hash,Equal> & linked_hashmapObj) : p2linked_hashmap(&linked_hashmapObj), listIterator(p2linked_hashmap -> list) {}
		iterator(const iterator &other)
		{
			p2linked_hashmap = other.p2linked_hashmap;
			listIterator = other.listIterator;
		}
		~iterator(){}

		/**
		 * iter++
		 */
		iterator operator++(int) 
		{
			iterator tmp(*this);
			listIterator++;
			return tmp;
		}
		/**
		 * ++iter
		 */
		iterator &operator++() 
		{
			listIterator++;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int) 
		{
			iterator tmp(*this);
			listIterator--;
			return tmp;
		}
		/**
		 * --iter
		 */
		iterator &operator--() 
		{
			listIterator--;
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw "star invalid"
		*/
		value_type &operator*() const 
		{
			return *listIterator;
		}
		value_type *operator->() const noexcept 
		{
			return listIterator.operator->();
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const 
		{
			return listIterator == rhs.listIterator;
		}
		bool operator!=(const iterator &rhs) const 
		{
			return listIterator != rhs.listIterator;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return listIterator == rhs.listIterator;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return listIterator != rhs.listIterator;
		}
	};
 
	class const_iterator 
	{
		public:
		typename double_list<value_type>::const_iterator listIterator;
		const linked_hashmap<Key,T,Hash,Equal> * p2linked_hashmap;
        	/**
             * elements
             * add whatever you want
            */
    // --------------------------   
		const_iterator(const linked_hashmap<Key,T,Hash,Equal> & linked_hashmapObj) : p2linked_hashmap(&linked_hashmapObj), listIterator(p2linked_hashmap -> list) {}
		const_iterator(const iterator &other) 
		{
			p2linked_hashmap = other.p2linked_hashmap;
			listIterator = other.listIterator;
		}

		/**
		 * iter++
		 */
		const_iterator operator++(int) 
		{
			const_iterator tmp(*this);
			listIterator++;
			return tmp;
		}
		/**
		 * ++iter
		 */
		const_iterator &operator++() 
		{
			listIterator++;
			return *this;
		}
		/**
		 * iter--
		 */
		const_iterator operator--(int) 
		{
			const_iterator tmp(*this);
			listIterator--;
			return tmp;
		}
		/**
		 * --iter
		 */
		const_iterator &operator--() 
		{
			listIterator--;
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw 
		*/
		const value_type &operator*() const 
		{
			return *listIterator;
		}
		const value_type *operator->() const noexcept 
		{
			return listIterator.operator->();
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const 
		{
			return listIterator == rhs.listIterator;
		}
		bool operator!=(const iterator &rhs) const 
		{
			return listIterator != rhs.listIterator;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return listIterator == rhs.listIterator;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return listIterator != rhs.listIterator;
		}
	};
 
	linked_hashmap() : hashmap(){}	
	linked_hashmap(const linked_hashmap &other)
	{
		hashmap(other);
		list(other.list);
	}
	~linked_hashmap() {}
	linked_hashmap & operator=(const linked_hashmap &other) 
	{
		hashmap::operator=(other);
		list = other.list;
		return *this;
	}

 	/**
	 * return the value connected with the Key(O(1))
	 * if the key not found, throw 
	*/
	T & at(const Key &key) 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			throw "invalid";
		}
		return (*it).second;
	}
	const T & at(const Key &key) const 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			throw "invalid";
		}
		return (*it).second;
	}
	T & operator[](const Key &key) 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			throw "invalid";
		}
		return (*it).second;
	}

	const T & operator[](const Key &key) const 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			throw "invalid";
		}
		return (*it).second;
	}

	/**
	 * return an iterator point to the first 
	 * inserted and existed element
	 */
	iterator begin() 
	{
		iterator it(*this);
		it.listIterator = list.begin();
		return it;
	}

	const_iterator cbegin() const 
	{
		const_iterator it(*this);
		it.listIterator = list.begin();
		return it;
	}
    /**
	 * return an iterator after the last inserted element
	 */
	iterator end() 
	{
		iterator it(*this);
		it.listIterator = list.end();
		return it;
	}
	const_iterator cend() const 
	{
		const_iterator it(*this);
		it.listIterator = list.end();
		return it;
	}
  	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty() const 
	{
		return list.empty();
	}

    void clear()
	{
		hashmap::clear();
		list.clear();
	}

	size_t size() const 
	{
		return hashmap::size;
	}
 	/**
	 * insert the value_piar
	 * if the key of the value_pair exists in the map
	 * update the value instead of adding a new element，
     * then the order of the element moved from inner of the 
     * list to the head of the list
	 * and return false
	 * if the key of the value_pair doesn't exist in the map
	 * add a new element and return true
	*/
	pair<iterator, bool> insert(const value_type &value) 
	{
		hashmap::iterator it = hashmap::find(value.first);
		if(it != hashmap::end())
		{
			(*it).second = value.second;
			list.erase(it.listIterator);
			list.insert_head(*it);
			return pair<iterator, bool>(it, false);
		}
		hashmap::iterator it2 = hashmap::insert(value).first;
		list.insert_head(*it2);
		return pair<iterator, bool>(it2, true);
	}
 	/**
	 * erase the value_pair pointed by the iterator
	 * if the iterator points to nothing
	 * throw 
	*/
 	/**
	 * erase the value_pair pointed by the iterator
	 * if the iterator points to nothing
	 * throw 
	*/
	void remove(iterator pos) 
	{
		hashmap::remove((*pos).first);
		list.erase(pos.listIterator);
	}
	/**
	 * return how many value_pairs consist of key
	 * this should only return 0 or 1
	*/
	size_t count(const Key &key) const 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			return 0;
		}
		return 1;
	}
	/**
	 * find the iterator points at the value_pair
	 * which consist of key
	 * if not find, return the iterator 
	 * point at nothing
	*/
	iterator find(const Key &key) 
	{
		hashmap::iterator it = hashmap::find(key);
		if(it == hashmap::end())
		{
			iterator it2(*this);
			it2.listIterator = list.end();
			return it2;
		}
		return it;
	}

};

class lru
{
    using lmap = sjtu::linked_hashmap<Integer,Matrix<int>,Hash,Equal>;
    using value_type = sjtu::pair<const Integer, Matrix<int> >;
public:
    lru(int s) : MAXSIZE(s)
	{
		Map = std::make_shared<lmap>();
	}
	lru(const lru &other)
	{
		Map = other.Map;
		MAXSIZE = other.MAXSIZE;
	}
	lru & operator=(const lru &other)
	{
		Map = other.Map;
		MAXSIZE = other.MAXSIZE;
		return *this;
	}
    ~lru() {}
    /**
     * save the value_pair in the memory
     * delete something in the memory if necessary
    */
    void save(const value_type &v) const
	{
		if(Map -> size() == MAXSIZE)
		{
			Map -> list.delete_tail();
		}
		Map -> insert(v);
    }
    /**
     * return a pointer contain the value
    */
    Matrix<int>* get(const Integer &v) const
	{
		lmap::iterator it = Map -> find(v);
		if(it == Map -> end())
		{
			return nullptr;
		}
		return &((*it).second);
    }
    /**
     * just print everything in the memory
     * to debug or test.
     * this operation follows the order, but don't
     * change the order.
    */
    // void print()
	// {

    // }

private:
	std::shared_ptr<lmap> Map;
	int MAXSIZE;

};
}

#endif