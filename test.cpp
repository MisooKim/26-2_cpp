#include <iostream>
namespace Student1
{
    int score = 80;
}
namespace Student2
{
    int score = 95;
}

int main()
{
    std::cout << "안녕!" << std::endl;   // Student1의 score 출력
    std::cout << Student2::score << std::endl;   // Student2의 score 출력

    return 0;
}

