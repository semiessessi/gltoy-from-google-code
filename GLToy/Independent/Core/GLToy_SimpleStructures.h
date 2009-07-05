///////////////////////////////////////////////
// C R A P P Y    H E A D E R    F I L E
///////////////////////////////////////////////
// This header inherits C++ template crappyness
// Personally I dislike the .i/.h method and
// prefer it all to be in a .h, but if anyone
// wants feel free to clean this up into some
// "better" C++
///////////////////////////////////////////////

#ifndef __GLTOY_SIMPLESTRUCTURES_H
#define __GLTOY_SIMPLESTRUCTURES_H

static const unsigned int uSIMPLE_DATASTRUCTURE_START_ALLOC = 16;

#include <malloc.h>

template<class T>
class DataStructure
{
public:
	DataStructure<T>() {}
	virtual ~DataStructure<T>() {}
	
	virtual unsigned int GetCount() const
	{
		return 0;
	}
	
	virtual unsigned int GetSizeInMemory() const
	{
		return 0;
	}
};

template<class T>
class SimpleDataStructure : public DataStructure<T>
{
protected:
    T* data;
	unsigned int size;
	
	void CheckAlloc(const unsigned int& count)
	{
        if(count < uSIMPLE_DATASTRUCTURE_START_ALLOC) return; // do nothing if we are smaller than the initial alloc
		// if count goes over size allocate another size entries
		if(count > size)
		{
			while(count > size) size <<= 1;
			data = static_cast<T*>(realloc(data, sizeof(T)*size));
		}
		// else if count goes below half of size then reallocate
		else if(count < (size >> 1))
		{
			while(count < (size >> 1))
			{
				if(size == uSIMPLE_DATASTRUCTURE_START_ALLOC) break;
				size >>= 1;
			}
			data = static_cast<T*>(realloc(data, sizeof(T)*size));
		}
	}
public:
	SimpleDataStructure<T>() : DataStructure<T>()
	{
		// set up initial memory usage
		size = uSIMPLE_DATASTRUCTURE_START_ALLOC;
		data = static_cast<T*>(malloc(sizeof(T)*size));
	}

	virtual ~SimpleDataStructure<T>()
	{
		// clean up any allocated memory
		if(data) free(data);
	}

    SimpleDataStructure<T>(const SimpleDataStructure<T>& d)
    {
        size = d.size;
        data = static_cast<T*>(malloc(sizeof(T)*size));
    }
	
	virtual unsigned int GetSizeInMemory() const
	{
		return sizeof(*this) + sizeof(T)*size;
	}

    virtual T& operator [](const unsigned int& index)
	{
		return data[index];
	}

    virtual const T& operator [](const unsigned int& index) const
	{
		return data[index];
	}
};

class ANSIString : public SimpleDataStructure<char>
{
private:
    unsigned int length; 
public:
    ANSIString() : SimpleDataStructure<char>(), length(0)
    {
        data[0] = 0;
    }

    ANSIString(const char* const& str) : SimpleDataStructure<char>()
    {
        length = 0;
        while(str[length]) ++length;
        CheckAlloc(length + 2);
        for(unsigned int i = 0; i < length; ++i) data[i] = str[i];
        data[length] = 0;
    }

    ANSIString(const ANSIString& a) : SimpleDataStructure<char>(a)
    {
        length = a.length;
        CheckAlloc(length + 2);
        for(unsigned int i = 0; i < (length + 1); ++i) data[i] = a.data[i];
    }

    ANSIString& ANSIString::operator =(const ANSIString& a)
    {
        length = a.length;
        CheckAlloc(length + 2);
        for(unsigned int i = 0; i < (length + 1); ++i) data[i] = a.data[i];

        return *this;
    }

    bool operator ==(const ANSIString& str) const
    {
        if(str.length != length) return false;

        for(unsigned int i = 0; i < length; ++i)
        {
            if(data[i] != str.data[i]) return false;
        }

        return true;
    }

    ANSIString& operator +=(const ANSIString& str)
    {
        length += str.length;
        CheckAlloc(length + 2);
        for(unsigned int i = 0; i < str.length; ++i) data[length - str.length + i] = str.data[i];
        data[length] = 0;

        return *this;
    }

    ANSIString& operator +=(const char* const& str)
    {
        unsigned int l = 0;
        while(str[l]) ++l;
        length += l;
        CheckAlloc(length + 2);
        for(unsigned int i = 0; i < l; ++i) data[length - l + i] = str[i];
        data[length] = 0;

        return *this;
    }

    ANSIString& operator +=(const char c)
    {
        ++length;
        CheckAlloc(length + 2);
        data[length - 1] = c;
        data[length] = 0;

        return *this;
    }

    void AppendHex(unsigned int val)
    {
        char add[] = "0x00000000";
        for(unsigned int i = 0; i < 8; ++i)
        {
            unsigned int tmp = ((val >> (i << 2)) & 0xF);
            add[9 - i] = (tmp < 10) ? '0' + tmp : '7' + tmp;
        }

        *this += add;
    }

    void AppendInt(int val)
    {
        if(val == 0)
        {
            *this += "0";
            return;
        }

        char add[15];
        const int test[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
        int pos = 0;
        if(val < 0)
        {
            add[pos] = '-';
            ++pos;
        }

        unsigned int digits = 0;
        while(val >= test[digits]) ++digits;

        for(unsigned int i = 0; i < digits; ++i)
        {
            add[digits - i - 1 + pos] = '0' + (val % 10);
            val /= 10;
        }
        add[digits + pos] = 0;

        *this += add;
    }

    const char* GetPointer() const { return data; }
};

template<class T>
class Stack : public SimpleDataStructure<T>
{
protected:
	unsigned int sp;
public:
	Stack<T>() : sp(0xFFFFFFFF), SimpleDataStructure<T>()
	{
	}

    Stack<T>(const Stack<T>& s) : SimpleDataStructure<T>(s)
    {
        sp = s.sp;
        for(unsigned int i = 0; i <=sp; ++i)
            data[i] = s.data[i];
    }
	
	T* Push(const T& value)
	{
		++sp;
		CheckAlloc(sp + 1);
		data[sp] = value;
		return &(data[sp]);
	}

    T* Peek()
    {
        return &(data[sp]);
    }
	
	T* Pop()
	{
		return &(data[sp--]);
	}
	
	void Clear()
	{
		sp = 0;
	}
	
	unsigned int GetCount() const
	{
		return sp + 1;
	}

    unsigned int GetSizeInMemory() const
	{
		return sizeof(Stack<T>) + sizeof(T)*size;
	}
};

template<class T>
class Array : public SimpleDataStructure<T>
{
protected:
	unsigned int count;
public:
	Array<T>() : count(0), SimpleDataStructure<T>()
	{
	}
	
    Array<T>(const Array<T>& arr) : SimpleDataStructure<T>(arr)
	{
	    // copy the whole array
        count = arr.count;
		for(unsigned int i = 0; i < count; ++i) data[i] = arr.data[i];
	}
	
	Array<T>& operator =(const Array<T>& arr)
	{
	    // copy the whole array again...
		count = arr.count;
		for(unsigned int i = 0; i < count; ++i) data[i] = arr.data[i];
		return *this;
	}
	
	void Append(const T& value)
	{
	    // increment count then make sure memory is allocated before setting the new entry
		++count;
		CheckAlloc(count);
		data[count - 1] = value;
	}
	
	void Append(const Array<T>& values)
	{
		// store count for later use when copying
		unsigned int oldCount = count;
		// increase count and allocate accordingly
		count += values.count;
		CheckAlloc(count);
		for(unsigned int i = 0; i < values.count; ++i)
		{
			data[oldCount + i] = values.data[i];
		}
	}
	
	void InsertAt(const unsigned int& index, const T& value)
	{
		// increase count and check allocation
		++count;
		CheckAlloc(count);
		// move the existing values along to make room
		for(unsigned int i = count - 1; i >= index; --i)
		{
			data[i] = data[i - 1];
		}
		a[index] = value;
	}
	
	void InsertAt(const unsigned int& index, const Array<T>& values)
	{
		// increase count and check allocation
		count += values.count;
		CheckAlloc(count);
		// move the existing values along to make room
		for(unsigned int i = count - 1; i >= (index + values.count); --i)
		{
			data[i] = data[i - values.count];
		}
		// insert new values
		for(unsigned int i = 0; i < values.count; ++i)
		{
			data[index + i] = values.data[i];
		}
	}
	
	void RemoveAt(const unsigned int& index, unsigned int amount = 1)
	{
		count -= amount;
		for(unsigned int i = index; i < count; ++i)
		{
			data[i] = data[i + amount];
		}
	}
	
	void RemoveFromEnd(unsigned int amount = 1)
	{
		count -= amount;
	}
	
	unsigned int GetCount() const
	{
		return count;
	}

    unsigned int GetSizeInMemory() const
	{
		return sizeof(Array<T>) + sizeof(T)*size;
	}
	
	T& Start()
	{
		return &(data[0]);
	}
	
	T& End()
	{
		return &(data[count - 1]);
	}
	
	void Clear()
	{
		count = 0;
	}
};

#endif