#pragma once

// Контейнер типа Stack


//Хранилище для элемента стека
template <class T>
struct listElement
{
    listElement(T value): value(value) {};

    T value;
    listElement* prevElement;
};


//По умолчанию для хранения элемента класса Т будет использоваться listElement
//и дефолтный (std::allocator) аллокатор для этого контейнера
template <class T, class itemContainer = listElement<T>, class itemContainerAllocator = std::allocator<itemContainer>>
class MyStackContainer
{
private:

    //аллокатор
    itemContainerAllocator itemAllocator;

    //указатель на вершину стека
    itemContainer* topElement{nullptr};

    //размер стека
    size_t stackSize{0};

    //добавить элемент в стек, предварительно выделив память под него
    auto pushElement(T&& element)
    {
        //попросим место под один элемент
        itemContainer* item = itemAllocator.allocate(1);
        //и сконструируем его
        itemAllocator.construct(item, std::move(element));

        return item;
    }

    //позиция итератора. Начальная позиция - верхний элемент
    itemContainer* currentPosition{topElement} ;

public:

    //дефолтный конструктор
    MyStackContainer() {};

    //конструктор с возможностью вставить первый элемент
    MyStackContainer(T value)
    {

        topElement = pushElement(std::move(value));
        topElement->prevElement = nullptr;

    };

    //деструктор
    ~MyStackContainer()
    {
        while (stackSize>0)
            pop();
    }

    //Добавить элемент
    void push(T&& element)
    {

        auto item = pushElement(std::move(element));
        item->prevElement = topElement;

        topElement = item;

        reset();

        stackSize++;

    }

    //выдать элемент из стека
    T&& pop()
    {
        if (stackSize==0)
            throw(std::exception());

        auto popValue = topElement->value;

        auto nextTop = topElement->prevElement;

        //деструктор элемента и очистка памяти
        itemAllocator.destroy(topElement);
        itemAllocator.deallocate(topElement,1);

        topElement = nextTop;

        reset();

        stackSize--;

        return std::move(popValue);

    }

    size_t size() const
    {
        return stackSize;
    }
    //указатель на вершину стека
    const itemContainer* begin() const
    {
        return topElement;
    }

    //получить следующий элемент от позиции currentPosition
    const itemContainer* next()
    {
        if (currentPosition == nullptr)
            return currentPosition;

        return currentPosition = currentPosition->prevElement;

    }

    // сбросить currentPosition на вершину стека
    void reset()
    {
        currentPosition = topElement;
    }



};
