#include <iostream>
#include "../../common/include/MyMath.h"

void testStack()
{
    MyMath myMath(10.);
    std::cout << "Vrijednost poslije dodavanja: " << myMath.add(5.) << std::endl;
    std::cout << "Vrijednost poslije množenja: " << myMath.mult(2.) << std::endl;
}

int main()
{
    int a = 15, b = 25;
    std::cout << "Zbir brojeva iznosi : " << saberiDvaBroja(a, b) << std::endl;
    
    testStack();
    
    return 0; 
}
