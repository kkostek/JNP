#include <stack>
#include <string>
#include <functional>
#include <exception>
#include <cassert>
#include <map>


using Lazy = std::function<int(void)>;
using Lazy_func = std::function<int(Lazy, Lazy)>;
class SyntaxError : std::exception {};
class OperatorAlreadyDefined : std::exception {};
class UnknownOperator : std::exception {};

bool is_literal(char c) {
    return (c == '4' || c == '2' || c == '0');
}

class LazyCalculator {
    std::map<char, Lazy_func> operators;
    std::map<char, Lazy> literals;

public:

    LazyCalculator() {
        operators.insert(std::make_pair('+', [](Lazy a, Lazy b){return a() + b();}));
        operators.insert(std::make_pair('-', [](Lazy a, Lazy b){return a() - b();}));
        operators.insert(std::make_pair('*', [](Lazy a, Lazy b){return a() * b();}));
        operators.insert(std::make_pair('/', [](Lazy a, Lazy b){return a() / b();}));
        operators.insert(std::make_pair('0', [](Lazy, Lazy) { return 0; }));
        operators.insert(std::make_pair('2', [](Lazy, Lazy) { return 2; }));
        operators.insert(std::make_pair('4', [](Lazy, Lazy) { return 4; }));
    }

    Lazy parse(const std::string& s) const {
        std::stack<Lazy> my_stack;

        for (char c : s) {

            if (is_literal(c)) {
                my_stack.push(static_cast<Lazy>([c](){return c - '0';}));
            }
            else if (operators.find(c) != operators.end()) {

                if (my_stack.size() < 2) {
                    throw SyntaxError();
                }
                Lazy a = my_stack.top();
                my_stack.pop();
                Lazy b = my_stack.top();
                my_stack.pop();
                my_stack.push(static_cast<Lazy>(std::bind(operators.find(c)->second, b, a)));
            }
            else {
                throw UnknownOperator();
            }
        }

        if (my_stack.empty() || my_stack.size() > 1) {
            throw SyntaxError();
        }
        auto res = my_stack.top();
        my_stack.pop();
        return res;
    }

    int calculate(const std::string& s) const {
        return parse(s)();
    }

    void define(char c, std::function<int(Lazy, Lazy)> fn) {
        if (operators.find(c) != operators.end()) {
            throw OperatorAlreadyDefined();
        }
        operators.insert(std::make_pair(c, fn));
    }
};

std::function<void(void)> operator*(int n, std::function<void(void)> fn) {
    return [=]() {
        for (int i = 0; i < n; i++)
            fn();
    };
}

int manytimes(Lazy n, Lazy fn) {
    (n() * fn)();  // Did you notice the type cast?
    return 0;
}

int main() {
    LazyCalculator calculator;

    // The only literals...
    assert(calculator.calculate("0") == 0);
    assert(calculator.calculate("2") == 2);
    assert(calculator.calculate("4") == 4);

    // Built-in operators.
    assert(calculator.calculate("42+") == 6);
    assert(calculator.calculate("24-") == -2);
    assert(calculator.calculate("42*") == 8);
    assert(calculator.calculate("42/") == 2);

    assert(calculator.calculate("42-2-") == 0);
    assert(calculator.calculate("242--") == 0);
    assert(calculator.calculate("22+2-2*2/0-") == 2);


    // The fun.
    calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
    assert(calculator.calculate("42!") == 42);
    std::string buffer;
    calculator.define(',', [](Lazy a, Lazy b) { a(); return b(); });
    calculator.define('P', [&buffer](Lazy, Lazy) { buffer += "pomidor"; return 0; });
    assert(calculator.calculate("42P42P42P42P42P42P42P42P42P42P42P42P42P42P42P4"
                                "2P,,,,42P42P42P42P42P,,,42P,42P,42P42P,,,,42P,"
                                ",,42P,42P,42P,,42P,,,42P,42P42P42P42P42P42P42P"
                                "42P,,,42P,42P,42P,,,,,,,,,,,,") == 0);
    assert(buffer.length() == 42 * std::string("pomidor").length());


    std::string buffer2 = std::move(buffer);
    buffer.clear();
    calculator.define('$', manytimes);
    assert(calculator.calculate("42!42P$") == 0);
    // Notice, how std::move worked.
    assert(buffer.length() == 42 * std::string("pomidor").length());

    calculator.define('?', [](Lazy a, Lazy b) { return a() ? b() : 0; });
    assert(calculator.calculate("042P?") == 0);
    assert(buffer == buffer2);

    assert(calculator.calculate("042!42P$?") == 0);
    assert(buffer == buffer2);

    calculator.define('1', [](Lazy, Lazy) { return 1; });
    assert(calculator.calculate("021") == 1);

    for (auto bad: {"", "42", "4+", "424+"}) {
        try {
            calculator.calculate(bad);
            assert(false);
        }
        catch (SyntaxError) {
        }
    }

    try {
        calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
        assert(false);
    }
    catch (OperatorAlreadyDefined) {
    }

    try {
        calculator.define('0', [](Lazy, Lazy) { return 0; });
        assert(false);
    }
    catch (OperatorAlreadyDefined) {
    }

    try {
        calculator.calculate("02&");
        assert(false);
    }
    catch (UnknownOperator) {
    }

    return 0;
}
