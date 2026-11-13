#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <sstream>

// ============================================================================
// 1. CLASS VECTOR
// ============================================================================
template <typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    Vector() : data_(new T[8]), size_(0), capacity_(8) {}

    explicit Vector(size_t initial_size, const T& value = T())
        : size_(initial_size), capacity_(initial_size > 0 ? initial_size * 2 : 8) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    Vector(const Vector& other)
        : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            T* new_data = new T[other.capacity_];
            for (size_t i = 0; i < other.size_; ++i) {
                new_data[i] = other.data_[i];
            }
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~Vector() {
        delete[] data_;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 8 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector::pop_back(): vector is empty");
        }
        --size_;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Vector::at(): index out of range");
        }
        return data_[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Vector::at(): index out of range");
        }
        return data_[index];
    }

    T& back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector::back(): vector is empty");
        }
        return data_[size_ - 1];
    }

    const T& back() const {
        if (size_ == 0) {
            throw std::out_of_range("Vector::back(): vector is empty");
        }
        return data_[size_ - 1];
    }

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() noexcept {
        size_ = 0;
    }

    void resize(size_t new_size, const T& value = T()) {
        if (new_size > capacity_) {
            reallocate(std::max(new_size, capacity_ * 2));
        }
        for (size_t i = size_; i < new_size; ++i) {
            data_[i] = value;
        }
        size_ = new_size;
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i) {
            if (!(data_[i] == other.data_[i])) return false;
        }
        return true;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
};

// ============================================================================
// 2. CLASS STACK (На базе Vector)
// ============================================================================
template <typename T>
class Stack {
private:
    Vector<T> data_;

public:
    Stack() = default;

    void push(const T& value) {
        data_.push_back(value);
    }

    T pop() {
        if (empty()) {
            throw std::out_of_range("Stack::pop(): stack is empty");
        }
        T val = data_.back();
        data_.pop_back();
        return val;
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack::top(): stack is empty");
        }
        return data_.back();
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack::top(): stack is empty");
        }
        return data_.back();
    }

    bool empty() const noexcept {
        return data_.empty();
    }

    size_t size() const noexcept {
        return data_.size();
    }

    void clear() noexcept {
        data_.clear();
    }

    bool operator==(const Stack& other) const {
        return data_ == other.data_;
    }

    bool operator!=(const Stack& other) const {
        return data_ != other.data_;
    }
};

// ============================================================================
// 3. CLASS QUEUE (На циклическом буфере)
// ============================================================================
template <typename T>
class Queue {
private:
    Vector<T> buffer_;
    size_t head_;
    size_t tail_;
    size_t count_;

    void expand() {
        size_t old_cap = buffer_.size();
        size_t new_cap = (old_cap == 0) ? 8 : old_cap * 2;
        Vector<T> new_buffer(new_cap);

        for (size_t i = 0; i < count_; ++i) {
            new_buffer[i] = buffer_[(head_ + i) % old_cap];
        }

        buffer_ = std::move(new_buffer);
        head_ = 0;
        tail_ = count_;
    }

public:
    explicit Queue(size_t initial_capacity = 8)
        : buffer_(initial_capacity > 0 ? initial_capacity : 8),
          head_(0),
          tail_(0),
          count_(0) {}

    void push(const T& value) {
        if (count_ == buffer_.size()) {
            expand();
        }
        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % buffer_.size();
        ++count_;
    }

    T pop() {
        if (empty()) {
            throw std::out_of_range("Queue::pop(): queue is empty");
        }
        T val = buffer_[head_];
        head_ = (head_ + 1) % buffer_.size();
        --count_;
        return val;
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue::front(): queue is empty");
        }
        return buffer_[head_];
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue::front(): queue is empty");
        }
        return buffer_[head_];
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Queue::back(): queue is empty");
        }
        size_t back_idx = (tail_ == 0) ? (buffer_.size() - 1) : (tail_ - 1);
        return buffer_[back_idx];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue::back(): queue is empty");
        }
        size_t back_idx = (tail_ == 0) ? (buffer_.size() - 1) : (tail_ - 1);
        return buffer_[back_idx];
    }

    bool empty() const noexcept {
        return count_ == 0;
    }

    size_t size() const noexcept {
        return count_;
    }

    size_t capacity() const noexcept {
        return buffer_.size();
    }

    void clear() noexcept {
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }
};

// ============================================================================
// 4. CLASS TRANSLATOR (Арифметический транслятор на ОПЗ)
// ============================================================================
class Translator {
public:
    enum class TokenType {
        NUMBER,
        OPERATOR,
        LPAREN,
        RPAREN
    };

    struct Token {
        TokenType type;
        double value;
        char op;

        Token() : type(TokenType::NUMBER), value(0.0), op(0) {}
        explicit Token(double val) : type(TokenType::NUMBER), value(val), op(0) {}
        Token(TokenType t, char c) : type(t), value(0.0), op(c) {}
    };

private:
    std::string expression_;

    static int getPriority(char op) {
        switch (op) {
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
            case '~': return 3; // Унарный минус
            case '^': return 4; // Возведение в степень
            default:  return 0;
        }
    }

    static bool isRightAssociative(char op) {
        return op == '~' || op == '^';
    }

    static bool isOperatorChar(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

public:
    Translator() = default;
    explicit Translator(const std::string& expr) : expression_(expr) {}

    void setExpression(const std::string& expr) {
        expression_ = expr;
    }

    const std::string& getExpression() const noexcept {
        return expression_;
    }

    // 1. Лексический анализ и синтаксическая валидация (разбиение на токены в очередь)
    static Queue<Token> tokenize(const std::string& expr) {
        Queue<Token> tokens;
        size_t i = 0;
        const size_t n = expr.length();

        bool expect_operand = true;
        int paren_balance = 0;

        while (i < n) {
            if (std::isspace(static_cast<unsigned char>(expr[i]))) {
                ++i;
                continue;
            }

            // Чтение числа (целого или с плавающей точкой)
            if (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.') {
                if (!expect_operand) {
                    throw std::invalid_argument("Syntax error: unexpected number without operator");
                }
                size_t start = i;
                bool has_dot = false;
                while (i < n && (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.')) {
                    if (expr[i] == '.') {
                        if (has_dot) {
                            throw std::invalid_argument("Lexical error: multiple decimal points in number");
                        }
                        has_dot = true;
                    }
                    ++i;
                }
                std::string num_str = expr.substr(start, i - start);
                if (num_str == ".") {
                    throw std::invalid_argument("Lexical error: standalone decimal point");
                }
                double val = std::stod(num_str);
                tokens.push(Token(val));
                expect_operand = false;
            }
            // Открывающая скобка
            else if (expr[i] == '(') {
                if (!expect_operand) {
                    throw std::invalid_argument("Syntax error: missing operator before '('");
                }
                tokens.push(Token(TokenType::LPAREN, '('));
                ++paren_balance;
                expect_operand = true;
                ++i;
            }
            // Закрывающая скобка
            else if (expr[i] == ')') {
                if (expect_operand) {
                    throw std::invalid_argument("Syntax error: empty parentheses or missing operand before ')'");
                }
                --paren_balance;
                if (paren_balance < 0) {
                    throw std::invalid_argument("Syntax error: unmatched closing parenthesis ')'");
                }
                tokens.push(Token(TokenType::RPAREN, ')'));
                expect_operand = false;
                ++i;
            }
            // Арифметические операторы
            else if (isOperatorChar(expr[i])) {
                char op = expr[i];
                if (expect_operand) {
                    if (op == '-') {
                        tokens.push(Token(TokenType::OPERATOR, '~')); // Унарный минус
                    } else if (op == '+') {
                        // Унарный плюс можно безопасно пропустить
                    } else {
                        throw std::invalid_argument(std::string("Syntax error: unexpected binary operator '") + op + "'");
                    }
                } else {
                    tokens.push(Token(TokenType::OPERATOR, op));
                    expect_operand = true;
                }
                ++i;
            }
            // Неизвестный символ
            else {
                throw std::invalid_argument(std::string("Lexical error: invalid character '") + expr[i] + "'");
            }
        }

        if (paren_balance != 0) {
            throw std::invalid_argument("Syntax error: unclosed parentheses");
        }
        if (expect_operand) {
            throw std::invalid_argument("Syntax error: expression is empty or ends with an operator");
        }

        return tokens;
    }

    // 2. Перевод в обратную польскую запись (Алгоритм сортировочной станции)
    static Queue<Token> toPostfix(Queue<Token> infix_tokens) {
        Queue<Token> output;
        Stack<Token> op_stack;

        while (!infix_tokens.empty()) {
            Token t = infix_tokens.pop();

            if (t.type == TokenType::NUMBER) {
                output.push(t);
            } else if (t.type == TokenType::LPAREN) {
                op_stack.push(t);
            } else if (t.type == TokenType::RPAREN) {
                bool found_lparen = false;
                while (!op_stack.empty()) {
                    if (op_stack.top().type == TokenType::LPAREN) {
                        op_stack.pop();
                        found_lparen = true;
                        break;
                    }
                    output.push(op_stack.pop());
                }
                if (!found_lparen) {
                    throw std::invalid_argument("Syntax error: mismatched parentheses");
                }
            } else if (t.type == TokenType::OPERATOR) {
                while (!op_stack.empty() && op_stack.top().type == TokenType::OPERATOR) {
                    char top_op = op_stack.top().op;
                    int p_top = getPriority(top_op);
                    int p_curr = getPriority(t.op);

                    if (p_top > p_curr || (p_top == p_curr && !isRightAssociative(t.op))) {
                        output.push(op_stack.pop());
                    } else {
                        break;
                    }
                }
                op_stack.push(t);
            }
        }

        while (!op_stack.empty()) {
            Token top_tok = op_stack.pop();
            if (top_tok.type == TokenType::LPAREN || top_tok.type == TokenType::RPAREN) {
                throw std::invalid_argument("Syntax error: mismatched parentheses");
            }
            output.push(top_tok);
        }

        return output;
    }

    // Возвращает строковое представление ОПЗ (удобно для отладки и тестов)
    static std::string toPostfixString(const std::string& expr) {
        Queue<Token> rpn = toPostfix(tokenize(expr));
        std::ostringstream oss;
        bool first = true;
        while (!rpn.empty()) {
            if (!first) oss << " ";
            Token t = rpn.pop();
            if (t.type == TokenType::NUMBER) {
                oss << t.value;
            } else {
                oss << t.op;
            }
            first = false;
        }
        return oss.str();
    }

    // 3. Вычисление выражения по очереди токенов в ОПЗ
    static double evaluatePostfix(Queue<Token> postfix_tokens) {
        Stack<double> calc_stack;

        while (!postfix_tokens.empty()) {
            Token t = postfix_tokens.pop();

            if (t.type == TokenType::NUMBER) {
                calc_stack.push(t.value);
            } else if (t.type == TokenType::OPERATOR) {
                if (t.op == '~') {
                    if (calc_stack.empty()) {
                        throw std::runtime_error("Evaluation error: missing operand for unary minus");
                    }
                    double val = calc_stack.pop();
                    calc_stack.push(-val);
                } else {
                    if (calc_stack.size() < 2) {
                        throw std::runtime_error("Evaluation error: insufficient operands for binary operator");
                    }
                    double right = calc_stack.pop();
                    double left = calc_stack.pop();

                    switch (t.op) {
                        case '+': calc_stack.push(left + right); break;
                        case '-': calc_stack.push(left - right); break;
                        case '*': calc_stack.push(left * right); break;
                        case '/':
                            if (std::abs(right) < 1e-12) {
                                throw std::domain_error("Math error: division by zero");
                            }
                            calc_stack.push(left / right);
                            break;
                        case '^':
                            calc_stack.push(std::pow(left, right));
                            break;
                        default:
                            throw std::runtime_error("Evaluation error: unknown operator");
                    }
                }
            }
        }

        if (calc_stack.size() != 1) {
            throw std::runtime_error("Evaluation error: malformed expression");
        }

        return calc_stack.pop();
    }

    // Главный статический метод для вычисления строки
    static double calculate(const std::string& expr) {
        Queue<Token> tokens = tokenize(expr);
        Queue<Token> postfix = toPostfix(tokens);
        return evaluatePostfix(postfix);
    }

    // Метод экземпляра класса
    double evaluate() const {
        return calculate(expression_);
    }
};

// Алиас с маленькой буквы (так как на доске написано class translator)
using translator = Translator;