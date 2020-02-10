#pragma once

template<typename T, size_t reserve = 0>
struct myAllocator
{

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;


    template<typename U>
    struct rebind {
        using other = myAllocator<U,reserve>;
    };

    //будет ли использоваться резерв
    bool isReserveToUse{false};

    //сколько элементов уже использует резерв
    size_t inUseElements{0};

    //Указатель на резервированный участок памяти
    T* reservedMemory {nullptr};

    //указатель на последний элемент
    T* lastReservedElement{nullptr};


    myAllocator()
    {
        //если нужен резерв, аллоцируем память под запрашиваемое кол-во элементов
        if (reserve > 0) {

            reservedMemory = allocate(reserve);
            lastReservedElement = reservedMemory + reserve * sizeof(T);
            isReserveToUse = true;
        }



    }

        ~myAllocator()
        {

        //Если была задействована резервированная память, освободим её
        if (isReserveToUse) {

            isReserveToUse = false;
            deallocate(reservedMemory,reserve*sizeof(T) );

        }
        }


    template<typename U>
    myAllocator(const myAllocator<U,reserve>&)
     {

    }

    T *allocate(std::size_t n) {


        void* mallocPtr;

        //Если был резерв и кол-во запрашиваемой памяти не превышает резерв
        if (isReserveToUse && n <= reserve - inUseElements)
        {

            //отдадим память из резерва
        mallocPtr = reservedMemory + inUseElements;


            //увеличим счетчик элементов
            inUseElements +=  n;

        }
        else {

            //Выделим память из кучи
            mallocPtr = std::malloc(n * sizeof(T));


            if (!mallocPtr)
                throw std::bad_alloc();

        }


        return reinterpret_cast<T*>(mallocPtr);
    }



    void deallocate(T *p, std::size_t n) {


        //Если элемент из резервированной памяти, освобождать не будем.
        //Освободим потом всё сразу в деструкторе
        if (isReserveToUse && (p >=reservedMemory  && p <=lastReservedElement))
        return;

        std::free(p);


    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {

        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {

        p->~T();
    }


};

