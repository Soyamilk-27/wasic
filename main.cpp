#include <iostream>
#include <memory>

int main() {
    auto script = R"(
    fn main()
        x = [1 2 3 4 5]
        y = x * 2
        print(y)
    end
    )";

    

    return 0;
}