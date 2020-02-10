#include <iostream>
#include <map>
#include "myallocator.h"
#include "mycontainer.h"

int factorial(int value)
{
    if (value==0)
        return 1;

return value * factorial(value-1);
}

int main() {

//std::map со стандартным аллокатором
std::map<int,int> mapStdAlloc;

for (int i=0 ; i<10; i++)
mapStdAlloc.emplace( std::make_pair(i,factorial(i)) );

for (auto& i : mapStdAlloc)
     {
     std::cout << "map Std::Allocator: " << i.first << " " << i.second << std::endl ;

     }

//std::map с моим аллокатором
std::map<int,int,std::less<int>,myAllocator<std::pair<const int,int>,10>> mapMyAlloc;

for (int i=0 ; i<10; i++)
mapMyAlloc.emplace( std::make_pair(i,factorial(i)) );

for (auto& i : mapMyAlloc)
     {
     std::cout << "map MyAllocator: " << i.first << " " << i.second << std::endl ;

     }


//мой контейнер типа СТЕК со стандартным аллокатором
MyStackContainer<int> myContainer;

for (int i=0; i<10; i++)
myContainer.push(std::move(i));


for (auto i = myContainer.begin(); i!=nullptr; i = myContainer.next()) {
    if (i!=nullptr)
    std::cout << "myContainer Std::Allocator: " << i->value << " " << std::endl;
}


//Мой контейнер с моим аллокатором
MyStackContainer<int, listElement<int>, myAllocator<listElement<int>,10>> myContainerMyAlloc;

for (int i=0; i<10; i++)
myContainerMyAlloc.push(std::move(i));

for (auto i = myContainerMyAlloc.begin(); i!=nullptr; i = myContainerMyAlloc.next()) {
    if (i!=nullptr)
    std::cout << "myContainer MyAllocator: " << i->value << " " << std::endl;
}


    return 0;
}
