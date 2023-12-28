#include "numbersgetter.hpp"

std::string NumbersGetter::convert(std::string message)
{
    std::string current_number = "";
    
    std::vector<LNL::LongInt> numbers;

    //numbers.reserve(20);
    
    LNL::LongInt sum {0};
    LNL::LongInt number {0};
    for(auto it = message.begin(); it < message.end(); ++it)
    {
        if((char)(*it) >= 48 && (char)(*it) <= 57)
        {
            current_number += *it;
            if(it + 1 == message.end() || (char)*(it + 1) < 48 || (char)*(it + 1) > 57)
            {
                //out += current_number + " ";
                number = LNL::LongInt(current_number);
                numbers.push_back(number);
                sum += number;
                current_number = "";
            }
        }
    }
    std::sort(numbers.begin(), numbers.end());
    std::string out = "";
    for(auto num : numbers)
    {
        out += num.toString() + " ";
    }
    out += "\n" + sum.toString() + '\n';
    return out;
}